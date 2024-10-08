#pragma once

#include <set>
#include "dllmain.h"
#include "resource.h"
//#include "FancyMenu.h"

void _naked_InitDirectXHooks();
void dualInputDisplay();
void cursorDraw();

extern bool lClick;
extern bool rClick;
extern bool lHeld;
extern bool rHeld;

// my inconsistent use of D3DXVECTOR2 vs point is bad. i should use point

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

typedef struct DragInfo {
	float* dragPointX;
	float* dragPointY;

	union {
		struct {
			union {
				struct {
					float topLeftX;
					float topLeftY;
				};
				Point topLeft;
			};

			union {
				struct {
					float bottomRightX;
					float bottomRightY;
				};
				Point bottomRight;
			};
		};
		Rect bounds;
	};
	
	bool enable = false;

	DragInfo() {}
	DragInfo(float* dragX, float* dragY) {
		dragPointX = dragX;
		dragPointY = dragY;
	}
} DragInfo;

extern unsigned directxFrameCount;

extern Point mousePos; // no use getting this multiple times a frame

class DragManager { // there will only ever be one of this object. it should probs be something else other than a class?
public:

	DragManager() {

	}

	void add(DragInfo* info);

	void remove(DragInfo* info);

	void handleDragId(DragInfo* info);

	void handleDrag();

	DragInfo* hasDrag = NULL; // object currently in possesion of drag
	bool hasHover = false;
	std::set<DragInfo*> dragInfoData;

};

extern DragManager dragManager;

void printDirectXError(HRESULT hr);

void logMatrix(const D3DMATRIX& matrix);

void writeClipboard(const std::string& text);

// -----

// why have i not abstracted this yet omfg
// also, tbh, i think im reserving these for use only with MeltyTestVert. maybe. might make drawing easier? or i could just pass this to draw func. i dont want to have different vers for scaled and unscaled tho
template <typename T>
class Tri {
public:

	Tri() {}

	Tri(const T& v1_, const T& v2_, const T& v3_) {
		v1 = v1_;
		v2 = v2_;
		v3 = v3_;
	}

	Tri(const T& v1_, const T& v2_, const T& v3_, DWORD col) {
		v1 = v1_;
		v2 = v2_;
		v3 = v3_;

		v1.color = col;
		v2.color = col;
		v3.color = col;
	}

	union {
		T verts[3];
		struct {
			T v1;
			T v2;
			T v3;
		};
	};
};

template <typename T>
class Quad { 
public:
	// really should have made this class more complex to allow for easier texture usage, and also i need to make my point class better. but i also need to actually use that class 
	Quad() {}

	Quad(const T& v1_, const T& v2_, const T& v3_, const T& v4_) {
		v1 = v1_;
		v2 = v2_;
		v3 = v3_;
		v4 = v4_;
	}

	Quad(const T& v1_, const T& v2_, const T& v3_, const T& v4_, DWORD col) {
		v1 = v1_;
		v2 = v2_;
		v3 = v3_;
		v4 = v4_;

		v1.color = col;
		v2.color = col;
		v3.color = col;
		v4.color = col;
	}

	Quad(const Rect& pos, const Rect& texPos = Rect({ 0.0f, 0.0f }, { 0.0f, 0.0f }), DWORD col = 0xFF42E5F4) {
		v1 = T(pos.x1, pos.y1, 0.0f, 1.0f, texPos.x1, texPos.y1, col); 
		v2 = T(pos.x2, pos.y1, 0.0f, 1.0f, texPos.x2, texPos.y1, col);
		v3 = T(pos.x1, pos.y2, 0.0f, 1.0f, texPos.x1, texPos.y2, col);
		v4 = T(pos.x2, pos.y2, 0.0f, 1.0f, texPos.x2, texPos.y2, col);
	}

	union { // this use of unions here is one of my fave things in c++
		T verts[4];
		struct {
			T v1;
			T v2;
			T v3;
			T v4;
		};
	};
};

extern unsigned _vertexBytesTotal;
extern unsigned _vertexBytesTransferedThisFrame;
// this could have been done without a class. i hope the overhead isnt stupid
template <typename T, size_t vertexCount, D3DPRIMITIVETYPE primType = D3DPT_TRIANGLELIST>
class VertexData {
public:

	VertexData(DWORD vertexFormat_, IDirect3DTexture9** texture_ = NULL) {
		vertexFormat = vertexFormat_;
		texture = texture_;
	}

	void alloc() {
		if (vertexBuffer == NULL) {
			if (FAILED(device->CreateVertexBuffer(vertexCount * sizeof(T), 0, vertexFormat, D3DPOOL_MANAGED, &vertexBuffer, NULL))) {
				log("failed to alloc a vertex buffer!");
				vertexBuffer = NULL;
			}
			_vertexBytesTotal += vertexCount * sizeof(T);
		}
	}

	~VertexData() {
		_vertexBytesTotal -= vertexCount * sizeof(T);

		if (vertexBuffer != NULL) {
			vertexBuffer->Release();
		}
	}

	void add(const T& v1, const T& v2) {

		if (vertexIndex >= vertexCount) {
			//log("a vertex buffer overflowed. this is critical. increase the buffer size! current: %d fmt: %08X", vertexCount, vertexFormat);
			// this log call was getting called to often, and would fuck things up
			return;
		}

		vertexData[vertexIndex++] = v1;
		vertexData[vertexIndex++] = v2;

	}

	void add(const T& v1, const T& v2, const T& v3) {

		if (vertexIndex >= vertexCount) {
			//log("a vertex buffer overflowed. this is critical. increase the buffer size! current: %d fmt: %08X", vertexCount, vertexFormat);
			// this log call was getting called to often, and would fuck things up
			return;
		}

		vertexData[vertexIndex++] = v1;
		vertexData[vertexIndex++] = v2;
		vertexData[vertexIndex++] = v3;

	}

	void addScale(const T& v1, const T& v2) {

		if (vertexIndex >= vertexCount) {
			//log("a vertex buffer overflowed. this is critical. increase the buffer size! current: %d fmt: %08X", vertexCount, vertexFormat);
			// this log call was getting called to often, and would fuck things up
			return;
		}

		vertexData[vertexIndex++] = v1;
		vertexData[vertexIndex++] = v2;

		scaleVertex(vertexData[vertexIndex - 2]);
		scaleVertex(vertexData[vertexIndex - 1]);
	}

	void addScale(const T& v1, const T& v2, const T& v3) {

		if (vertexIndex >= vertexCount) {
			//log("a vertex buffer overflowed. this is critical. increase the buffer size! current: %d fmt: %08X", vertexCount, vertexFormat);
			// this log call was getting called to often, and would fuck things up
			return;
		}
		
		vertexData[vertexIndex++] = v1;
		vertexData[vertexIndex++] = v2;
		vertexData[vertexIndex++] = v3;

		scaleVertex(vertexData[vertexIndex - 3]);
		scaleVertex(vertexData[vertexIndex - 2]);
		scaleVertex(vertexData[vertexIndex - 1]);
	}

	void add(const Tri<T>& tri) {
		addScale(tri.v1, tri.v2, tri.v3);
	}

	void add(const Quad<T>& quad) {
		addScale(quad.v1, quad.v2, quad.v3);
		addScale(quad.v2, quad.v3, quad.v4);
	}

	void draw() {

		if (vertexBuffer == NULL) {
			log("a vertex data buffer was null? how.");
			exit(1);
		}

		if (vertexIndex == 0) {
			return;
		}

		if (texture != NULL && *texture != NULL) {
			device->SetTexture(0, *texture);
		}

		// ideally i should be scaling the vertices in here, but is it worth it?

		VOID* pVoid;

		_vertexBytesTransferedThisFrame += vertexIndex * sizeof(T);

		vertexBuffer->Lock(0, vertexIndex * sizeof(T), (void**)&pVoid, 0);
		memcpy(pVoid, &vertexData[0], vertexIndex * sizeof(T));
		vertexBuffer->Unlock();

		DWORD primCount = vertexIndex;

		if constexpr (primType == D3DPT_TRIANGLELIST) {
			primCount /= 3;
		} else if constexpr (primType == D3DPT_LINELIST) {
			primCount /= 2;
		}

		device->SetStreamSource(0, vertexBuffer, 0, sizeof(T));
		device->SetFVF(vertexFormat);
		device->DrawPrimitive(primType, 0, primCount);

		// i could use DrawIndexedPrimitive here? check if faster.
		// would be super nice, esp for text drawing at the minimum
		// wait no, they, they still share tex stuff??

		vertexIndex = 0;

		if (texture != NULL && *texture != NULL) {
			device->SetTexture(0, NULL);
		}
	}

	DWORD vertexFormat = 0;
	IDirect3DVertexBuffer9* vertexBuffer = NULL;
	IDirect3DTexture9** texture = NULL;
	T vertexData[vertexCount]; // i distrust vectors
	unsigned vertexIndex = 0; // number of verts. i,, ugh. i should have written a const size vec class.

};

// -----

typedef struct PosVert {
	D3DVECTOR position;
} PosVert;

typedef struct PosColVert {
	D3DVECTOR position;
	D3DCOLOR color;
} PosColVert;

typedef struct PosTexVert {
	D3DVECTOR position;
	D3DXVECTOR2 texCoord;
} PosTexVert;

typedef struct PosColTexVert {
	D3DVECTOR position;
	D3DCOLOR color;
	D3DXVECTOR2 texCoord;
} PosColTexVert;

typedef struct MeltyVert { // if having all these initializers causes slowdown, ill cry
	
	union {
		D3DVECTOR position = D3DVECTOR(0.0f, 0.0f, 0.0f);
		struct {
			float x;
			float y;
			float z;
		};
	};
	float rhw = 1.0f;
	D3DCOLOR color = 0xFFFFFFFF;
	union {
		D3DXVECTOR2 uv = D3DXVECTOR2(0.0f, 0.0f); // might not be the smartest idea, but it works
		struct {
			float u;
			float v;
		};
	};
	
	MeltyVert() {}

	MeltyVert(float x, float y, D3DXVECTOR2 uv_, DWORD col = 0xFFFFFFFF) {
		position.x = x;
		position.y = y;
		position.z = 0.0f;
		rhw = 1.0f;
		color = col;
		uv.x = uv_.x;
		uv.y = uv_.y;
	}

	MeltyVert(float x, float y, DWORD col = 0xFFFFFFFF) {
		position.x = x;
		position.y = y;
		position.z = 0.0f;
		rhw = 1.0f;
		color = col;
	}

} MeltyVert;

extern size_t fontBufferSize;
extern BYTE* fontBuffer; // this is purposefully not freed on evict btw
extern IDirect3DTexture9* fontTexture;

extern size_t fontBufferSizeWithOutline;
extern BYTE* fontBufferWithOutline;
extern IDirect3DTexture9* fontTextureWithOutline;

extern BYTE* fontBufferMelty;
extern size_t fontBufferMeltySize;
extern IDirect3DTexture9* fontTextureMelty;

extern VertexData<PosColVert, 3 * 2048> posColVertData;//(D3DFVF_XYZ | D3DFVF_DIFFUSE);
extern VertexData<PosTexVert, 3 * 2048> posTexVertData;//(D3DFVF_XYZ | D3DFVF_TEX1, &fontTexture);
// need to rework font rendering, 4096 is just horrid
extern VertexData<PosColTexVert, 3 * 4096 * 2> posColTexVertData;// (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1, &fontTextureMelty);

extern VertexData<MeltyVert, 3 * 4096 * 2> meltyVertData;// (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1, &fontTextureMelty);
extern VertexData<MeltyVert, 2 * 16384, D3DPT_LINELIST> meltyLineData;// (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1, &fontTextureMelty); // 8192 is overkill

// ----

//const int fontTexWidth = 256;
//const int fontTexHeight = 256;
//int fontSize = 32;
extern const int fontTexWidth;
extern const int fontTexHeight;
extern int fontSize;
extern int fontHeight;
extern int fontWidth;
extern float fontRatio;

IDirect3DPixelShader9* createPixelShader(const char* pixelShaderCode);

IDirect3DVertexShader9* createVertexShader(const char* shaderCode);

inline unsigned scaleNextPow2(unsigned v) {
	v--;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	v++;
	return v;
}

bool loadResource(int id, BYTE*& buffer, unsigned& bufferSize);

void _initDefaultFont(IDirect3DTexture9*& resTexture);

IDirect3DPixelShader9* getFontOutlinePixelShader();

IDirect3DVertexShader9* getFontOutlineVertexShader();

void _initDefaultFontOutline(IDirect3DTexture9*& fontTex);

void _initMeltyFont();

void _initFontFirstLoad();

void initFont();

// -----

enum class BoxType {
	//None,
	Origin, // 0xFF42E5F4
	Collision, // grey
	Hitbox, // red
	Hurtbox, // green
	Clash, // yellow
	Blue, // what is this
	Shield, // Purple, also like,,, vaki??
	Reflect, // 

	_Count, // dont use
};

typedef struct BoxData {
	//BoxType type = BoxType::None;
	float x = 0.0f;
	float y = 0.0f;
	float w = 0.0f;
	float h = 0.0f;
	//int player = 0; // what player this is being drawn for. should i,,, have each projectile be a seperate color as well?
} BoxData;

typedef std::vector<BoxData> BoxList;

//typedef std::vector<BoxData> BoxObjects;
typedef std::array<BoxList, static_cast<int>(BoxType::_Count)> BoxObjects; // 2d array, index is what type of box

extern std::vector<BoxObjects*> boxObjectList;

extern bool _hasStateToRestore;
extern IDirect3DPixelShader9* _pixelShaderBackup;
extern IDirect3DVertexShader9* _vertexShaderBackup;
extern IDirect3DBaseTexture9* _textureBackup;
extern DWORD _D3DRS_BLENDOP;
extern DWORD _D3DRS_ALPHABLENDENABLE;
extern DWORD _D3DRS_SRCBLEND;
extern DWORD _D3DRS_DESTBLEND;
extern DWORD _D3DRS_SEPARATEALPHABLENDENABLE;
extern DWORD _D3DRS_SRCBLENDALPHA;
extern DWORD _D3DRS_DESTBLENDALPHA;
extern DWORD _D3DRS_MULTISAMPLEANTIALIAS;
extern DWORD _D3DRS_ALPHATESTENABLE;
extern DWORD _D3DRS_ALPHAREF;
extern DWORD _D3DRS_ALPHAFUNC;
extern D3DMATRIX _D3DTS_VIEW;

extern float vWidth;
extern float vHeight;
extern float wWidth;
extern float wHeight;
extern bool isWide;
extern D3DVECTOR factor;
extern D3DVECTOR topLeftPos; // top left of the screen, in pixel coords. maybe should be in directx space but idk
extern D3DVECTOR renderModificationFactor;
extern D3DXVECTOR2 mouseTopLeft;
extern D3DXVECTOR2 mouseBottomRight;
extern D3DXVECTOR2 mouseFactor;

// -----

constexpr BYTE ARROW_0(0x80 + 0x00);
constexpr BYTE ARROW_1(0x80 + 0x01);
constexpr BYTE ARROW_2(0x80 + 0x02);
constexpr BYTE ARROW_3(0x80 + 0x03);
constexpr BYTE ARROW_4(0x80 + 0x04);
constexpr BYTE ARROW_5(0x80 + 0x05);
constexpr BYTE ARROW_6(0x80 + 0x06);
constexpr BYTE ARROW_7(0x80 + 0x07);
constexpr BYTE ARROW_8(0x80 + 0x08);
constexpr BYTE ARROW_9(0x80 + 0x09);

constexpr BYTE BUTTON_A(0x90 + 0x00);
constexpr BYTE BUTTON_B(0x90 + 0x01);
constexpr BYTE BUTTON_C(0x90 + 0x02);
constexpr BYTE BUTTON_D(0x90 + 0x03);
constexpr BYTE BUTTON_E(0x90 + 0x04);
constexpr BYTE BUTTON_DASH(0x90 + 0x05);

constexpr BYTE BUTTON_A_GRAY(0xA0 + 0x00);
constexpr BYTE BUTTON_B_GRAY(0xA0 + 0x01);
constexpr BYTE BUTTON_C_GRAY(0xA0 + 0x02);
constexpr BYTE BUTTON_D_GRAY(0xA0 + 0x03);
constexpr BYTE BUTTON_E_GRAY(0xA0 + 0x04);
constexpr BYTE BUTTON_DASH_GRAY(0xA0 + 0x05);

constexpr BYTE JOYSTICK(0x90 + 0x07); // double size

constexpr BYTE CURSOR(0x80 + 0x0C); 
constexpr BYTE CURSOR_LOADING(0x00 + 0x00);

inline void scaleVertex(D3DVECTOR& v) {
	/*
	if (isWide) {
		v.x /= factor;
	} else {
		v.y /= factor;
	}*/
	// branchless, and with multiplication is faster
	v.x *= factor.x;
	v.y *= factor.y;
	/*
	mov		eax,	[esp + 4];
    movaps	xmm0,	dword ptr [eax];
    movaps	xmm1,	dword ptr [factor];
    mulps	xmm0,	xmm1;
    movaps	[eax],	xmm0;

    ret;

	might be faster, 
	but would require the struct to be alignas(16) 
	at the very least, it is not slower, despite doing 
	2x mults.
	very weird
	
	*/
}

inline void scaleVertex(MeltyVert& v) {
	v.position.x += topLeftPos.x;
	v.position.y += topLeftPos.y;
	
	v.position.x *= renderModificationFactor.x;
	v.position.y *= renderModificationFactor.y;
}

void __stdcall backupRenderState();

void __stdcall restoreRenderState();

// -----

void LineDraw(float x1, float y1, float x2, float y2, DWORD ARGB = 0x8042e5f4, bool side = false);;

void RectDraw(float x, float y, float w, float h, DWORD ARGB = 0x8042e5f4);

void RectDraw(const Rect& rect, DWORD ARGB = 0x8042e5f4);

void BorderDraw(float x, float y, float w, float h, DWORD ARGB = 0x8042e5f4);

void BorderDraw(const Rect& rect, DWORD ARGB = 0x8042e5f4);

void BorderRectDraw(float x, float y, float w, float h, DWORD ARGB = 0x8042e5f4);

// -----

void LineDrawBlend(float x1, float y1, float x2, float y2, DWORD ARGB = 0x8042e5f4, bool side = false);

void RectDrawBlend(float x, float y, float w, float h, DWORD ARGB = 0x8042e5f4);

void BorderDrawBlend(float x, float y, float w, float h, DWORD ARGB = 0x8042e5f4);

void BorderRectDrawBlend(float x, float y, float w, float h, DWORD ARGB = 0x8042e5f4);

// -----

Rect TextDraw(float x, float y, float size, DWORD ARGB, const char* format, ...);

Rect TextDraw(const Point& p, float size, DWORD ARGB, const char* format, ...);

void TextDrawSimple(float x, float y, float size, DWORD ARGB, const char* format, ...);

void joystickDraw(float x, float y, float size, DWORD ARGB);

void cursorDraw();

IDirect3DPixelShader9* getOutlinePixelShader();

IDirect3DVertexShader9* getOutlineVertexShader();

IDirect3DPixelShader9* getColorPixelShader();

IDirect3DVertexShader9* getColorVertexShader();

extern IDirect3DTexture9* renderTargetTex;

void drawSingleHitbox(const BoxData& box, DWORD ARGB, bool shade = true);

void drawSingleHitboxBlend(const BoxData& box, DWORD ARGB, bool shade = true);

void drawBatchHitboxes(const BoxList& boxList, DWORD ARGB);

constexpr DWORD arrNormalColors[] = {
	0xFF42E5F4, // origin
	0xA0D0D0D0, // collision
	0xFFFF0000, // hitbox
	0xFF00FF00, // hurtbox
	0xFFFFFF00, // clash
	0xFF0000FF, // projectile
	0xFFF54298 // shield
};

constexpr DWORD arrColorBlindColors[] = {
	0xFF42E5F4, // origin
	0xA0D0D0D0, // collision
	0xFFFF0000, // hitbox
	0xFF1403ff, // hurtbox
	0xFFFFFF00, // clash
	0xFF00ffcf, // projectile
	0xFFffffff // shield
};

void HitboxBatchDrawNoBlend(const BoxObjects* b);

void HitboxBatchDrawBlend(const BoxObjects* b);

// ----- horrid Profiler, as a treat 

extern std::vector<std::function<void(void)>> drawCalls;
typedef struct ProfileInfo {
	unsigned callCount = 0; // amount called this frame
	long long currentFrameTime = 0; // time spent here this frame
	unsigned index = 0; // having a index for each value might be overkill
	std::array<long long, 64> times; // past log of times, to make number smoother
} ProfileInfo;
extern std::map<const char*, ProfileInfo > profilerData;

class Profiler {
public:

	const char* name;
	long long startTime;

	Profiler(const char* name_) {
		name = name_;
		startTime = getMicroSec();
	}

	~Profiler() {

		auto it = profilerData.find(name);

		if (it == profilerData.end()) [[unlikely]] {
			profilerData[name] = ProfileInfo();
		}

		profilerData[name].callCount++;
		profilerData[name].currentFrameTime += getMicroSec() - startTime;
	}

};

#define profileFunction() \
    volatile Profiler profiler(__func__);

// -----

void DrawHitboxes(BoxObjects* b);

constexpr int logHistorySize = 32;
extern char* logHistory[logHistorySize];
extern int logHistoryIndex;
void DrawLog(char* s);

// -----

#include "FancyInputDisplay.h" // really need to switch the program over to proper .cpp files

// -----

void _drawHitboxes();

void _drawProfiler();

void _drawLog();

extern bool debugMode;
extern bool verboseMode;
extern bool overkillVerboseMode;
void _drawMiscInfo();

// -----

void allocVertexBuffers();

void _drawGeneralCalls();

// -----

void _drawDebugMenu();

void __stdcall _doDrawCalls();

// -----

void _naked_PresentHook();

void cleanForDirectXReset();

void reInitAfterDirectXReset();

extern DWORD _RESET_HOOKS;
extern DWORD _DirectX_Reset_Func_ret;
extern DWORD _DirectX_Reset_Func_Addr;
void _DirectX_Reset_Func();

extern DWORD _DirectX_BeginStateBlock_Func_Addr;
void _DirectX_BeginStateBlock_Func();

extern DWORD _DirectX_EvictManagedResources_Func_Addr;
void _DirectX_EvictManagedResources_Func();

extern DWORD _DirectX_Present_Func_ret;
extern DWORD _DirectX_Present_Func_Addr;
void _DirectX_Present_Func();

void _naked_InitDirectXHooks();

void _naked_RehookDirectX();

bool HookDirectX();