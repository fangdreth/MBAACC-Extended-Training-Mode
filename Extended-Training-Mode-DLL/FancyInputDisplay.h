#pragma once
#include <set>


typedef struct InputData {
	DWORD rawInput = 0;
	BYTE direction = ' ';
	BYTE buttons = 0;
	BYTE buttonString[6] = {' ', ' ', ' ', ' ', ' ', '\0' };
	int length = 0;
	int maxLengthPreRollback = 0;
} InputData;

class InputColumn {
public:

	InputColumn(unsigned addr_, float xVal_, float yVal_, int inputMaxLen_);

	void update();

	void rollforward();

	void rollback();

	void draw();

	void reset();

	unsigned addr;
	float xVal;
	float yVal;
	int inputMaxLen;

	CircularBuffer<InputData, 120> inputs;
	//InputData inputs[inputMaxLen];
	//std::vector<InputData> inputs;
	//int inputIndex = 0;


	
	//DWORD rawInputData[60]; // store the last second of inputs for the input display. should it be more than this? maybe? 
	//int rawInputIndex = 0;
	//CircularBuffer<DWORD, 60> joyLog;

	DragInfo dragInfo;
};

extern InputColumn P1InputBar; // (0x00555134 + 0x02E7, 10.0f, 124.0f, 24);
extern InputColumn P2InputBar; // (0x00555C30 + 0x02E7, 545.0f, 124.0f, 24);

void dualInputDisplay();

void dualInputDisplayReset();

class InputDisplay {
public:

	typedef struct JoyLog {
		int dir;
		unsigned frame;
	} JoyLog;

	InputDisplay(float xPos_, float yPos_, float scale_, float cornerScale_, InputColumn* inputColumn_, int player_) : xPos(xPos_), yPos(yPos_), scale(scale_), cornerScale(cornerScale_), inputColumn(inputColumn_), player(player_) { 
		dragManager.add(&dragInfo);
		dragInfo.dragPointX = &xPos;
		dragInfo.dragPointY = &yPos;
	}

	void drawPoint(float x, float y, DWORD col = 0xFFD0D0D0, float size = 4.0f);

	void numpadToCoords(int v, D3DXVECTOR2& out);

	void drawLine(int a, int b, DWORD colA, DWORD colB, float lineBaseWidth = 6.0f, float size = 8.0f);

	void drawBounds();

	void drawBase();

	void drawLines();

	void drawJoystick();

	void drawButtons();

	void draw();

	float xPos = 300;
	float yPos = 300;
	const float scale = 50.0f;
	const float cornerScale = 60.0f;

	DragInfo dragInfo;

	int sizeA = 16;
	int sizeB = 16;
	int sizeC = 16;
	int sizeD = 16;
	int sizeE = 16;
	int sizeZ = 16;

	float prevJoyX = xPos - 8;
	float prevJoyY = yPos - 8;
	int prevDir = 0;

	CircularBuffer<JoyLog, 60> joyLog;
	unsigned frame = 0;

	InputColumn* inputColumn = NULL;
	int player;

	static int hasDrag;
	DWORD prevInput = 0;

};

//int InputDisplay::hasDrag = 0;

extern InputDisplay P1InputDisplay; //(200.0f - 0.0f, 112.0f, 25.0f, 25.0f * 1.2f, &P1InputBar, 1);
extern InputDisplay P2InputDisplay; //(378.0f + 0.0f, 112.0f, 25.0f, 25.0f * 1.2f, &P2InputBar, 2);

void drawFancyInputDisplay();

void rollFancyInputDisplay(int n);
