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

struct Player
{
	char cPlayerNumber = 0;

	PlayerData* PlayerData = 0x0;
	DWORD adInaction = 0x0;

	int nLastInactionableFrames = 0;

	FrameBarCell cells[BAR_MEMORY_SIZE];

	int nActiveCounter = 0;
	int nInactionableMemory = 0;
	int nAdvantageCounter = 0;
	int nFirstActiveCounter = 0;
	int nLastFrameCount = 0;
	bool bLastOnRight = false;
	DWORD dwLastActivePointer = 0x0;
	char cLastHitstop = 0;
	int nFirstActive = 0;
	bool bAlreadyGotFirstActive = false;
	bool bProjectileActive = false;
	bool bLastProjectileActive = false;
};

extern Player P1;
extern Player P2;
extern Player P3;
extern Player P4;

extern Player* paPlayerArray[4];

extern Player* Main1;
extern Player* Main2;
extern Player* Assist1;
extern Player* Assist2;

void UpdatePlayers(); //Called after bar handling

void CheckProjectiles();

void CalculateAdvantage(Player& P1, Player& P2);

void ResetBars();

void UpdateBars(Player& P, Player& Assist);

void IncrementActive(Player& P);

void IncrementFirstActive(Player& P1, Player& P2);

void HandleInactive(Player& P);

void BarHandling(Player& P1, Player& P2, Player& P1Assist, Player& P2Assist);

void FrameBar(Player& P1, Player& P2, Player& P3, Player& P4);

void drawFrameBar(int frameBarY);