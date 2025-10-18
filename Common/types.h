#pragma once 

// i should probs have a types.h,,,
// i now have a types.h

#include <ws2tcpip.h>
#include <winsock2.h>
#include <Windows.h>
#include <Xinput.h>
#include <cmath>

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define CLAMP(value, min_val, max_val) MAX(MIN((value), (max_val)), (min_val))
#define SAFEMOD(a, b) (((b) + ((a) % (b))) % (b))

extern DWORD __frameDoneCount;

long long getMicroSec();

long long getNanoSec();

class KeyState
{
public:

	// polling input multiple times a frame is bad, we really should switch over to something which polls the whole keyboard/controllers once a frame

	KeyState();

	KeyState(int k);

	void setKey(int vKey_);

	bool isFocused();

	bool keyHeld();

	template<int freq, int startup = 45> // freq is the frame modulo for when this should trig, startup is how long to wait for that
	bool keyHeldFreq() {

		bool res = keyHeld();

		if (freqHeldCounter < startup) {
			return false;
		}

		if (__frameDoneCount % freq != 0) {
			return false;
		}

		return res;
	}

	template<int freq, int startup = 45> // same as above func, but also will return immediately on keydown
	bool keyDownHeldFreq() {

		bool res = keyHeld();

		if (keyDown()) {
			return true;
		}

		if (freqHeldCounter < startup) {
			return false;
		}

		if (__frameDoneCount % freq != 0) {
			return false;
		}

		return res;
	}

	bool keyDown();

	static void updateControllers();

	static void showControllerState();

	static short pressedButtons();

	static short releasedButtons();

public:
	int nHeldKeyCounter;
	int freqHeldCounter = 0;
private:
	// lets say that -1 will be a value for when we are using a dinput device instead
	// or, a value above 0xFF could be used, vkeys are restricted to a byte.
	int nKey = -1;
	bool prevState = false;
	bool tempState = false;
	static XINPUT_STATE* xState;
	static XINPUT_STATE* prevxState;
};

// -----


template <typename T, int size>
class CircularBuffer {
public:

	CircularBuffer() {}

	void pushHead(const T& v) {
		index++;
		if (index >= size) {
			index = 0;
		}
		data[index] = v;
	}

	void pushTail(const T& v) {
		index--;
		if (index < 0) {
			index = size - 1;
		}
		data[index] = v;
	}

	void rollHead() {
		index--;
		if (index >= size) {
			index = 0;
		}
	}

	void rollTail() {
		index++;
		if (index < 0) {
			index = size - 1;
		}
	}

	T& front() {
		return data[index];
	}

	int totalMemory() {
		return sizeof(T) * size;
	}

	void clear() {
		for (size_t i = 0; i < size; i++) {
			data[i] = T();
		}
		index = 0;
	}

	T& operator [](int i) {
		while (i < 0) {
			i += size;
		}
		i += index;
		return data[i % size];
	}

	const T& operator [](int i) const {
		while (i < 0) {
			i += size;
		}
		i += index;
		return data[i % size];
	}

	T data[size];
	int index = 0; // index will be the head, index-1 will be the tail
};

typedef struct FreqTimerData {
	float min;
	float mean;
	float max;
	float stdev;
} FreqTimerData;

// meant to,,, assist in timing certain things/seeing how frequently something is called per frame
extern float _freqTimerYVal; // template classes dont share statics
template<int size>
class FreqTimer {
public:

	void tick() {
		long long time = getNanoSec();
		float temp = (float)1000000000.0 / ((float)time - prevTime);
		buffer.pushHead(temp);
		prevTime = time;
	}

	FreqTimerData getData() {
		FreqTimerData res;

		res.min = buffer.data[0];
		res.max = buffer.data[0];
		res.mean = 0.0;
		int tempSize = size;
		for (int i = 0; i < size; i++) {
			if (std::isnan(buffer.data[i]) || std::isinf(buffer.data[i])) {
				tempSize--;
				continue;
			}
			res.mean += buffer.data[i];
			res.min = MIN(res.min, buffer.data[i]);
			res.max = MAX(res.max, buffer.data[i]);
		}

		if (tempSize != 0) {
			res.mean /= ((float)tempSize);
		} else {
			res.mean = NAN;
		}

		res.stdev = 0.0f;
		for (int i = 0; i < size; i++) {
			if (std::isnan(buffer.data[i]) || std::isinf(buffer.data[i])) {
				continue;
			}
			res.stdev += (buffer.data[i] - res.mean) * (buffer.data[i] - res.mean);
		}

		if (tempSize != 0) {
			res.stdev /= ((float)tempSize - 1);
			res.stdev = sqrtf(res.stdev);
		} else {
			res.stdev = NAN;
		}

		return res;
	}

	void log() {

		FreqTimerData res = getData();

		TextDraw(50.0f, 10.0 + (_freqTimerYVal * 4), 4, 0xFFFFFFFF, "%5.2lf %5.2lf %5.2lf", res.mean, res.min, res.max);
		_freqTimerYVal += 4;
	}

	long long prevTime = 0;
	CircularBuffer<float, size> buffer;

};

template <typename T>
class Vec {
public:

	Vec(int maxSize_ = 16) {
		maxSize = maxSize_;
		if (maxSize != 0) {
			data = (T*)malloc(maxSize * sizeof(T));
		}
	}

	~Vec() {
		if (data != NULL) {
			free(data);
			data = NULL;
		}
	}

	Vec(const Vec& other) = delete;
	Vec& operator=(const Vec& other) = delete;

	int totalMemory() {
		return sizeof(T) * maxSize;
	}

	void resize() {
		maxSize *= 2;
		T* temp = (T*)realloc(data, maxSize * sizeof(T));

		if (temp == NULL) {
			//log("vec resize failed??!");
			return;
		}

		data = temp;
	}

	void addCapacity(int n) {
		maxSize += n;

		T* temp = (T*)realloc(data, maxSize * sizeof(T));

		if (temp == NULL) {
			// THESE TWO CASES SHOULD NOT FAIL SILENTLY!!!! THIS IS HORRIBLE
			//log("Vec realloc failed??!");
			return;
		}

		data = temp;
	}

	void push_back(const T& newItem) {

		if (size == maxSize) {
			resize();
		}

		data[size] = newItem;
		size++;
	}

	void emplace_back(const T&& newItem) {

		if (size == maxSize) {
			resize();
		}

		data[size] = std::forward<T>(newItem);
		size++;
	}

	T operator [](int i) const { return data[i]; }
	T& operator [](int i) { return data[i]; }

	T* data = NULL;
	int size = 0;
	int maxSize = 0;

};


struct Rect;
typedef struct Rect Rect;

typedef struct Point {
	float x = 0.0;
	float y = 0.0;
	Point() {}
	Point(float x_, float y_) : x(x_), y(y_) {}
	bool operator==(const Point const& rhs) { return x == rhs.x && y == rhs.y; }
	bool operator!=(const Point const& rhs) { return x != rhs.x || y != rhs.y; }
	Point operator+(const Point const& rhs) { return Point(x + rhs.x, y + rhs.y); }
	Point operator-(const Point const& rhs) { return Point(x - rhs.x, y - rhs.y); }
	Point& operator+=(const Point const& rhs) { x += rhs.x; y += rhs.y; return *this; }
	Point& operator-=(const Point const& rhs) { x -= rhs.x; y -= rhs.y; return *this; }
	Point& operator=(const Point const& rhs) { if (this != &rhs) { x = rhs.x; y = rhs.y; } return *this; }

	bool inside(const Rect& rect) const;

	bool outside(const Rect& rect) const;

} Point;

typedef struct Rect {

	// there is specifically not a 4 float constructor due to ambiguity between if its 2 points, or 1 point, and width, height
	Rect() {}

	Rect(const Point& a, const Point& b) {
		x1 = a.x;
		y1 = a.y;
		x2 = b.x;
		y2 = b.y;
	}

	Rect(const Point& a, float w, float h) {
		x1 = a.x;
		y1 = a.y;
		x2 = a.x + w;
		y2 = a.y + h;
	}

	union {
		struct {
			float x1;
			float y1;
		};
		Point p1;
	};

	union {
		struct {
			float x2;
			float y2;
		};
		Point p2;
	};

	bool inside(const Point& p) const {
		return (p.x >= x1 && p.x <= x2 && p.y >= y1 && p.y <= y2);
	}

	bool outside(const Point& p) const {
		return !inside(p);
	}

	Rect& operator=(const Rect const& rhs) { if (this != &rhs) { p1 = rhs.p1; p2 = rhs.p2; } return *this; }

} Rect;
