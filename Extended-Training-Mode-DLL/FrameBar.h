#pragma once
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include "..\Common\Common.h"

char cGameState = 0; // 1:In-Game 2:Title 3:Logos 8:Loading 9:Arcade Cutscene 10:Next Stage 12:Options 20:CSS 25:Main Menu
char cGlobalEXFlash = 0; //Used for EXFlashes where neither char moves
char cP1EXFlash = 0; //Used for EXFlashes where initiator still moves (ex. Satsuki 214C winds up during flash)
char cP2EXFlash = 0;
int nFrameCount = 0; //Counts slower during slowdown
int nLastFrameCount = 0;
int nTrueFrameCount = 0; //Counts all frames during slowdown
int nLastTrueFrameCount = 0;
char cTrainingControlPlayer = 0;
char cFN1Input = 0;
char cFN2Input = 0;
char cDummyState = 0; // Same as Common.h "Enemy Status" except -1 for recording

int nBarCounter = 0;
int nBarScrolling = 0;
int nBarIntervalCounter = 0;
int nBarIntervalMax = 0;
int nBarDisplayRange = 75;
bool bIsBarReset = false;
bool bDoBarReset = false;
bool bUpdateBar = false;
bool bDoAdvantage = false;

bool bLockInput = false;

static bool bHideFreeze = true; //Whether to hide global ex flashes and frames where both chars are in hitstop

static int nPlayerAdvantage;
int nSharedHitstop;

struct Player
{
	char Port = 0;

	DWORD dwBaseAddress = 0x0;
	DWORD dwInactionOffset = 0x0;

	int nLastInactionableFrames = 0;

	DWORD dwColorBar1[400][2];
	DWORD dwColorBar2[400][2];

	int nActiveCounter = 0;
	int nInactiveMemory = 0;
	int nAdvantageCounter = 0;
	int nLastFrameCount = 0;
	int nActiveProjectileCount = 0;
	bool bLastOnRight = false;
};

void UpdatePlayer(Player& P) {
	P.nLastInactionableFrames = *(int*)(P.dwInactionOffset);
	P.nLastFrameCount = *(int*)(P.dwBaseAddress + 0xF0);
	P.bLastOnRight = *(int*)(P.dwBaseAddress + 0x315);
}

Player P1{ 0, 0x555130, 0x557FC0 };
Player P2{ 1, 0x555C2C, 0x5581CC };
Player P3{ 2, 0x556728, 0x557FC0 };
Player P4{ 3, 0x557224, 0x5581CC };

Player* Player1 = &P1;
Player* Player2 = &P2;
Player* Player3 = &P3;
Player* Player4 = &P4;

void CalculateAdvantage(Player& P1, Player& P2)
{
	if (*(int*)(P1.dwInactionOffset) == 0 && *(int*)(P2.dwInactionOffset) == 0)
	{
		bDoAdvantage = false;
	}
	else if (*(int*)(P1.dwInactionOffset) != 0 && *(int*)(P2.dwInactionOffset) != 0)
	{
		bDoAdvantage = true;
		P1.nAdvantageCounter = 0;
		P2.nAdvantageCounter = 0;
	}

	if (bDoAdvantage && nFrameCount != nLastFrameCount && cGlobalEXFlash == 0)
	{
		if (*(int*)(P1.dwInactionOffset) == 0 && *(int*)(P2.dwInactionOffset) != 0)
		{
			P1.nAdvantageCounter++;
		}
		else if (*(int*)(P2.dwInactionOffset) == 0 && *(int*)(P1.dwInactionOffset) != 0)
		{
			P2.nAdvantageCounter++;
		}
	}
}

void ResetBars(Player& P)
{
	bIsBarReset = true;
	nBarCounter = 0;
	nBarIntervalCounter = 0;
	nBarScrolling = 0;
	bDoBarReset = false;
	nBarIntervalMax = nBarDisplayRange;
	for (int i = 0; i < BAR_MEMORY_SIZE; i++)
	{
		P.dwColorBar1[i][0] = 0x00000000;
		P.dwColorBar1[i][1] = 0x00000000;
		P.dwColorBar2[i][0] = 0x00000000;
		P.dwColorBar2[i][1] = 0x00000000;
	}
}

void UpdateBars(Player& P, Player& Assist)
{
	// Foreground color -> \x1b[38;2;R;G;Bm
	// Background color -> \x1b[48;2;R;G;Bm
	DWORD dwColor = 0x00000000;
	DWORD dwColor2 = 0x00000000;
	bool bIsButtonPressed = *(char*)(P.dwBaseAddress + 0x2ED) != 0 || *(char*)(P.dwBaseAddress + 0x2EE) != 0;

	//Bar 1 - General action information
	if (*(int*)(P.dwInactionOffset) != 0) //Doing something with limited actionability
	{
		dwColor = 0xFF41C800;
		//sBarValue = std::format("{:2}", P.nInactionableFrames % 100);
		if (*(int*)(P.dwBaseAddress + 0x10) >= 35 && *(int*)(P.dwBaseAddress + 0x10) <= 37) //Jump Startup
		{
			dwColor = 0xFFF1E084;
		}
		else if (*(int*)(P.dwBaseAddress + 0x1AC) != 0 && *(char*)(P.dwBaseAddress + 0x17B) == 0) //Hitstun
		{
			dwColor = 0xFF8C8C8C;
			if (*(char*)(*(DWORD*)(*(DWORD*)(P.dwBaseAddress + 0x320) + 0x38) + 0xC) == 1) //Airborne
			{
				if (*(short*)(P.dwBaseAddress + 0x190) < *(short*)(P.dwBaseAddress + 0x18E)) //Still has untech remaining
				{
					//sBarValue = std::format("{:2}", (P.sUntechTotal - P.sUntechCounter) % 100);
				}
			}
			else //Grounded
			{
				if (*(int*)(P.dwBaseAddress + 0x1AC) > 1) //Still has hitstun remaining
				{
					//sBarValue = std::format("{:2}", P.nHitstunRemaining - 1 % 100);
				}
			}
		}
		else if (*(char*)(P.dwBaseAddress + 0x17B)) //Blockstun
		{
			dwColor = 0xFFB4B4B4;
		}
		else if (*(DWORD*)(P.dwBaseAddress + 0x324) != 0) //Attacking
		{
			dwColor = 0xFFFF0000;
		}
	}
	else //Fully actionable
	{
		dwColor = 0xFF202020;
		//sBarValue = std::format("{:2}", P.nPattern % 100);

		if (bDoAdvantage) //Has advantage
		{
			//sBarValue = std::format("{:2}", P.nAdvantageCounter % 100);
			if (P.nAdvantageCounter != 0)
			{
				dwColor = 0xFF101010;
			}
		}

		if (P.nLastInactionableFrames != 0) //Neutral frame
		{
			dwColor = 0xFF205A00;
		}
	}

	if (*(char*)(P.dwBaseAddress + 0x176) != 0) //Being thrown
	{
		dwColor = 0xFF6E6E6E;
		//sBarValue = " t";
	}
	else if (*(char*)(*(DWORD*)(P.dwBaseAddress + 0x320) + 0x42) == 12) //Clash
	{
		dwColor = 0xFFE1B800;
	}
	else if (*(char*)(*(DWORD*)(P.dwBaseAddress + 0x320) + 0x42) == 10) //Shield
	{
		dwColor = 0xFF91C2FF;
	}

	if (*(char*)(*(DWORD*)(P.dwBaseAddress + 0x320) + 0x42) <= 1 || *(char*)(P.dwBaseAddress + 0x185) != 0 || *(char*)(*(DWORD*)(*(DWORD*)(P.dwBaseAddress + 0x320) + 0x38) + 0xD) == 3) //Various forms of invuln
	{
		dwColor2 = 0xFFFFFFFF;
	}

	if (*(char*)0x562A48 != 0 || *(int*)0x558908 != 0 || *(int*)0x558C14 != 0) //Screen is frozen
	{
		dwColor = 0xFF3C3C3C;
	}
	else if (*(char*)(P.dwBaseAddress + 0x172) != 0) //in hitstop
	{
		dwColor = 0xFF3C5080;
	}

	if (*(char*)(*(DWORD*)(*(DWORD*)(P.dwBaseAddress + 0x320) + 0x38) + 0xC) == 1) //Airborne
	{
		P.dwColorBar2[nBarCounter % BAR_MEMORY_SIZE][0] = 0xFFF1E084;
	}

	for (int i = 0; i < 1000; i++)
	{
		bool validProj = true;
		if (*(char*)(0x67BDE8 + 0x33C * i) != 0 && *(DWORD*)(0x67BDE8 + 0x324 + 0x33C * i) != 0 && *(char*)(0x67BDE8 + 0x8 + 0x33C * i) >= 0 && *(char*)(0x67BDE8 + 0x2F4 + 0x33C * i) == P.Port && *(int*)(0x67BDE8 + 0x10 + 0x33C * i) >= 60)
		{
			int nCharacterID = 0;
			if (P.Port % 2 == 0)
			{
				nCharacterID = *(char*)(0x400000 + dwP1CharNumber) * 10 + *(char*)(0x400000 + dwP1CharMoon);
			}
			else
			{
				nCharacterID = *(char*)(0x400000 + dwP2CharNumber) * 10 + *(char*)(0x400000 + dwP2CharMoon);
			}
			std::map<std::string, int> CharacterMap = MBAACC_Map[nCharacterID];
			for (auto const& [key, val] : CharacterMap)
			{
				if (*(int*)(0x67BDE8 + 0x10 + 0x33C * i) == val)
				{
					validProj = false;
					break;
				}
			}
			if (validProj)
			{
				P.dwColorBar2[nBarCounter % BAR_MEMORY_SIZE][1] = 0xFFFF0000;
			}
		}
	}

	if (*(DWORD*)(Assist.dwBaseAddress + 0x324) != 0)
	{
		P.dwColorBar2[nBarCounter % BAR_MEMORY_SIZE][1] = 0xFFFF8000;
	}

	P.dwColorBar1[nBarCounter % BAR_MEMORY_SIZE][0] = dwColor;
	if (dwColor2 != 0x0)
	{
		P.dwColorBar1[nBarCounter % BAR_MEMORY_SIZE][1] = dwColor2;
	}
	else
	{
		P.dwColorBar1[nBarCounter % BAR_MEMORY_SIZE][1] = dwColor;
	}

}

void IncrementActive(Player& P)
{
	if (*(DWORD*)(P.dwBaseAddress + 0x324) != 0 && *(DWORD*)(P.dwBaseAddress + 0x172) == 0)
	{
		P.nActiveCounter++;
	}
	else if (*(DWORD*)(P.dwBaseAddress + 0x324) == 0)
	{
		P.nActiveCounter = 0;
	}
}

void HandleInactive(Player& P)
{
	if (*(int*)(P.dwInactionOffset) != 0)
	{
		P.nInactiveMemory = *(int*)(P.dwInactionOffset);
	}
}

void BarHandling(Player& P1, Player& P2, Player& P1Assist, Player& P2Assist)
{
	CalculateAdvantage(P1, P2);

	bool IsInput = (
		*(DWORD*)(P1.dwBaseAddress + 0x2EB) != 0 || *(DWORD*)(P2.dwBaseAddress + 0x2EB) != 0
		); //True if player or dummy has any input
	bool DoBarUpdate = (
		*(int*)(P1.dwInactionOffset) != 0 || *(int*)(P2.dwInactionOffset) ||
		*(DWORD*)(*(DWORD*)(*(DWORD*)(P1.dwBaseAddress + 0x320) + 0x38) + 0x18) != 0 || *(DWORD*)(*(DWORD*)(*(DWORD*)(P2.dwBaseAddress + 0x320) + 0x38) + 0x18) != 0 ||
		P1.nActiveProjectileCount != 0 || P2.nActiveProjectileCount ||
		*(DWORD*)(P1Assist.dwBaseAddress + 0x324) != 0 || *(DWORD*)(P2Assist.dwBaseAddress + 0x324) ||
		P1Assist.nActiveProjectileCount != 0 || P2Assist.nActiveProjectileCount != 0
		); //True if either char is inactionable, can't block, has an active projectile, has an active assist, or has an active assist projectile

	if (DoBarUpdate)
	{
		bIsBarReset = false;
		bUpdateBar = true;
		nBarIntervalCounter = 0;
	}
	else
	{
		bUpdateBar = false;
		nBarIntervalCounter++;
	}

	if (nBarCounter >= nBarDisplayRange)
	{
		nBarIntervalMax = BAR_INTERVAL;
	}

	if (nBarIntervalCounter > nBarIntervalMax)
	{
		bDoBarReset = true;
	}

	if (bDoBarReset && bUpdateBar)
	{
		ResetBars(P1);
		ResetBars(P2);
		ResetBars(P1Assist);
		ResetBars(P2Assist);
	}

	if (nBarIntervalCounter < nBarIntervalMax && !bIsBarReset)
	{
		bUpdateBar = true;
	}

	if (bUpdateBar)
	{
		if (*(char*)(P1.dwBaseAddress + 0x172) != 0 && *(char*)(P2.dwBaseAddress + 0x172) != 0) //Player hitstop values count down but we need it to count up
		{
			nSharedHitstop++;
		}
		else
		{
			nSharedHitstop = 0;
		}

		bool bIsFreeze = (
			*(char*)0x562A48 != 0 ||
			nSharedHitstop > 1
			);

		if (!(bHideFreeze && bIsFreeze))
		{
			IncrementActive(P1);
			IncrementActive(P2);
			HandleInactive(P1);
			HandleInactive(P2);
			UpdateBars(P1, P1Assist);
			UpdateBars(P2, P2Assist);
			if (*(char*)(P1Assist.dwBaseAddress))
			{
				IncrementActive(P1Assist);
				UpdateBars(P1Assist, P1);

			}
			if (*(char*)(P2Assist.dwBaseAddress))
			{
				IncrementActive(P2Assist);
				UpdateBars(P2Assist, P2);

			}
			nBarCounter++;
		}
	}
}

void FrameBar(Player& P1, Player& P2, Player& P3, Player& P4)
{
	Player1 = &P1;
	Player2 = &P2;
	Player3 = &P3;
	Player4 = &P4;
	if (*(char*)(P1.dwBaseAddress + 0x178))
	{
		Player1 = &P3;
		Player3 = &P1;
	}
	if (*(char*)(P2.dwBaseAddress + 0x178))
	{
		Player2 = &P4;
		Player4 = &P2;
	}

	BarHandling(*Player1, *Player2, *Player3, *Player4);

	if (*(int*)0x562A40 == 1)
	{
		ResetBars(P1);
		ResetBars(P2);
		ResetBars(P3);
		ResetBars(P4);
	}

	nPlayerAdvantage = (P1.nAdvantageCounter - P2.nAdvantageCounter) % 100;

	UpdatePlayer(P1);
	UpdatePlayer(P2);
	UpdatePlayer(P3);
	UpdatePlayer(P4);
}
