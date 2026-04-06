#pragma once
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include "DebugInfo.h"
#include "TrainingMenu.h"
#include "DirectX.h"

const ADDRESS adBaseAddress = (0x00400000);

extern int nLastFrameCount; //Counts slower during slowdown
extern int nLastTrueFrameCount; //Counts all frames during slowdown

extern int nBarCounter;
extern int nBarScrolling;
extern int nBarIntervalCounter;
extern int nBarIntervalMax;

extern bool bIsBarReset;
extern bool bDoBarReset;
extern bool bUpdateBar;
extern bool bDoAdvantage;

extern bool bDisplayFreeze; //Whether to show global ex flashes and frames where both chars are in hitstop
extern bool bDisplayInputs;
extern int displayNumbers;

extern int nPlayerAdvantage;
extern int nSharedHitstop;

extern float fFrameBarW;
extern float fFrameBarH;

extern int nFrameBarY;
extern int nFrameBarDisplayRange;

extern char pcTextToDisplay[256];
extern char pcTextPattern[256];
extern int nDrawTextTimer;
extern int nClearSaveTimer;

extern int nAdjustNumbersForFreeze;

class FrameBar
{
public:
	FrameBar(float x_, float y_, float w_, float h_, int numCells_);

	void draw();

	float x;
	float y;
	float w;
	float h;
	int numCells;

	DragInfo dragInfo;
};

extern FrameBar frameBar;

struct FrameBarCell
{
	DWORD mainColor = 0x0;
	DWORD subColor = 0x0;
	DWORD subActiveColor = 0x0;
	DWORD airborneColor = 0x0;

	int number = -1;
	int numFlag = 0;

	void reset();

	void draw(float x, float y, float w, float h);
};

struct FrameBarPlayerData
{
	char cPlayerNumber = 0;

	PlayerData* PlayerData = 0x0;
	DWORD adInaction = 0x0;

	int nLastInactionableFrames = 0;

	FrameBarCell cells[BAR_MEMORY_SIZE];

	int nActiveCounter = 0;
	int nInactionableMemory = 0;
	int nInactionCounter = 0;
	int nInactionCounterMemory = 0;
	int nAdvantageCounter = 0;
	int nFirstActiveCounter = 0;
	DWORD nLastFrameCount = 0;
	byte bLastOnRight = 0;
	DWORD dwLastActivePointer = 0x0;
	char cLastHitstop = 0;
	int nFirstActive = 0;
	bool bAlreadyGotFirstActive = false;
	bool bProjectileActive = false;
	bool bLastProjectileActive = false;
};

extern FrameBarPlayerData FB_P1;
extern FrameBarPlayerData FB_P2;
extern FrameBarPlayerData FB_P3;
extern FrameBarPlayerData FB_P4;

extern FrameBarPlayerData* FB_PlayerArray[4];

extern FrameBarPlayerData* FB_Main1;
extern FrameBarPlayerData* FB_Main2;
extern FrameBarPlayerData* FB_Assist1;
extern FrameBarPlayerData* FB_Assist2;

void UpdatePlayers(); //Called after bar handling

void CheckProjectiles();

void CalculateAdvantage(FrameBarPlayerData& P1, FrameBarPlayerData& P2);

void ResetBars();

void UpdateBars(FrameBarPlayerData& P, FrameBarPlayerData& Assist);

void IncrementActive(FrameBarPlayerData& P);

void IncrementFirstActive(FrameBarPlayerData& P1, FrameBarPlayerData& P2);

void HandleInactive(FrameBarPlayerData& P);

void BarHandling(FrameBarPlayerData& P1, FrameBarPlayerData& P2, FrameBarPlayerData& P1Assist, FrameBarPlayerData& P2Assist);

void UpdateFrameBar();

void drawFrameBar(bool doDraw);

void drawColorGuide();

void drawSimpleMeter();