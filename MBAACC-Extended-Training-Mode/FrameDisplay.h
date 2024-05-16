#pragma once
#include <cmath>
#include <iostream>
#include <string>
#include "Constants.h"

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
char cDummyState = 0; // Same as Constants.h "Enemy Status" except -1 for recording

int nBarCounter = 0;
int nBarScrolling = 0;
int nBarIntervalCounter = 0;
int nBarIntervalMax = 0;
int nBarDisplayRange = 0;
bool bIsBarReset = false;
bool bDoBarReset = false;
bool bUpdateBar = false;
bool bDoAdvantage = false;

static bool bHideFreeze = true; //Whether to hide global ex flashes and frames where both chars are in hitstop

static bool bEnableFN1Save = false;
static bool bEnableFN2Load = false;

static bool bShowInfo1 = false; //Position, pattern + state, speed, acceleration, health, circuit
bool bShowInfo2 = true; //Distance, advantage
static bool bShowInfo3 = false; //EX flash, counter hit, guard gauge, scaling, partner, red health

bool bShowBar1 = true; //General action info
bool bShowBar2 = true; //Active Frames
static bool bShowBar3 = false; //Projectile and assist active frames
static bool bShowBar4 = false; //A, B, E inputs
static bool bShowBar5 = false; //C, D, directional inputs

static bool bIsStateSaved = false;
static int nSaveSlot = 0;

struct Save {
	bool bIsThisStateSaved = false;

	DWORD dwSaveEffects[74576 / 4] = {}; //Effect and projectile data
	DWORD dwSaveStopSituation[1632 / 4] = {};
	char cSaveGlobalEXFlash = 0;
	DWORD dwSaveAttackDisplayInfo[52 / 4] = {}; //Ends just before Max Damage
	DWORD dwSaveAttackDisplayInfo2[1004 / 4] = {}; //Starts after Max Damage, maybe also has current combo info
	DWORD dwSaveDestinationCamX = {};
	DWORD dwSaveCurrentCamX = {};
	DWORD dwSaveDestinationCamY = {};
	DWORD dwSaveCurrentCamY = {};
	DWORD dwSaveDestinationCamZoom = {};
	DWORD dwSaveCurrentCamZoom = {};
	DWORD dwSaveContlFlag = {}; //No idea
	DWORD dwSaveContlFlag2 = {}; //No idea

	DWORD dwSaveP1[972 / 4] = {}; //Player data from pattern to just before input buffers
	DWORD dwSaveP2[972 / 4] = {};
	DWORD dwSaveP3[972 / 4] = {};
	DWORD dwSaveP4[972 / 4] = {};
};

Save Saves[MAX_SAVES];

/*
DWORD dwSaveEffects[74576 / 4]; //Effect and projectile data
DWORD dwSaveStopSituation[1632 / 4];
char cSaveGlobalEXFlash;
DWORD dwSaveAttackDisplayInfo[52 / 4]; //Ends just before Max Damage
DWORD dwSaveAttackDisplayInfo2[1004 / 4]; //Starts after Max Damage, maybe also has current combo info
DWORD dwSaveDestinationCamX;
DWORD dwSaveCurrentCamX;
DWORD dwSaveDestinationCamY;
DWORD dwSaveCurrentCamY;
DWORD dwSaveDestinationCamZoom;
DWORD dwSaveCurrentCamZoom;
DWORD dwSaveContlFlag; //No idea
DWORD dwSaveContlFlag2; //No idea

DWORD dwSaveP1[972 / 4]; //Player data from pattern to just before input buffers
DWORD dwSaveP2[972 / 4];
DWORD dwSaveP3[972 / 4];
DWORD dwSaveP4[972 / 4];
*/

bool bLockInput = false;

void CheckGameState(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x14EEE8), &cGameState, 1, 0);
}

void UpdateGlobals(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
	nLastFrameCount = nFrameCount;
	nLastTrueFrameCount = nTrueFrameCount;
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x158908), &cP1EXFlash, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x158C14), &cP2EXFlash, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x15D1CC), &nFrameCount, 4, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x162A40), &nTrueFrameCount, 4, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x162A48), &cGlobalEXFlash, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x37144C), &cFN1Input, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x37144D), &cFN2Input, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x34D7F8), &cDummyState, 1, 0);
}

struct Player
{
	int nPlayerNumber = 0;
	DWORD dwCharacterBaseAddress = 0x0;
	char cExists = 0;
	int nPattern = 0;
	int nState = 0;
	int nHealth = 0;
	int nRedHealth = 0;
	float fGuardGauge = 0.0f;
	float fGuardQuality = 0.0f;
	int nMagicCircuit = 0;
	int nFrameCount = 0;
	int nXPosition = 0;
	int nYPosition = 0;
	int nXSpeed = 0;
	int nYSpeed = 0;
	short sXAcceleration = 0;
	short sYAcceleration = 0;
	int nMomentum = 0;
	char cHitstop = 0;
	bool bThrowFlag = false;
	bool bTagFlag = false;
	bool bBlockstunFlag = false;
	char sGrantedStrikeInvuln = 0;
	char sGrantedThrowInvuln = 0;
	short sUntechTotal = 0;
	short sUntechCounter = 0;
	int nHitstunRemaining = 0;
	char cCounterHit = 0;
	float fGravity = 0.0f;
	short sUntechPenalty = 0;
	char cAirDirectionalInput = 0; //I think this is actually used to determine blocking but its accurate for inputs in the air
	char cRawDirectionalInput = 0;
	char cButtonInput = 0;
	char cMacroInput = 0;
	bool bIsOnRight = false;
	DWORD dwAnimationDataPointer = 0x0;
	DWORD dwStateDataPointer = 0x0;
	DWORD dwAttackDataPointer = 0x0;
	char cStance = 0;
	char cStateInvuln = 0;
	char cNormalCancels = 0;
	char cSpecialCancels = 0;
	int nStateFlagset2 = 0;
	char cBoxIndex = 0;

	int nInactionableFrames = 0;

	std::string sBar1[BAR_MEMORY_SIZE];
	std::string sBar2[BAR_MEMORY_SIZE];
	std::string sBar3[BAR_MEMORY_SIZE];
	std::string sBar4[BAR_MEMORY_SIZE];
	std::string sBar5[BAR_MEMORY_SIZE];
	std::string sBarString1;
	std::string sBarString2;
	std::string sBarString3;
	std::string sBarString4;
	std::string sBarString5;

	int nActiveCounter = 0;
	int nAdvantageCounter = 0;
	int nLastFrameCount = 0;
	int nActiveProjectileCount = 0;
	bool bLastOnRight = false;
};

void CheckProjectiles(HANDLE hMBAAHandle, DWORD dwBaseAddress, Player& P)
{
	bool bProjectileExists = 0;
	char cProjectileSource = 0;
	int nProjectilePattern = 0;
	DWORD dwProjectileAttackDataPointer = 0;
	int nCount = 0;
	for (int i = 0; i < 100; i++)
	{
		ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adProjectileBase + dwProjectileStructSize * i), &bProjectileExists, 1, 0);
		if (bProjectileExists)
		{
			ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adProjectileBase + dwProjectileStructSize * i + 0x8), &cProjectileSource, 1, 0);
			if (cProjectileSource + 1 == P.nPlayerNumber)
			{
				ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adProjectileBase + dwProjectileStructSize * i + 0x10), &nProjectilePattern, 4, 0);
				ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adProjectileBase + dwProjectileStructSize * i + 0x324), &dwProjectileAttackDataPointer, 4, 0);
				if (dwProjectileAttackDataPointer != 0 && nProjectilePattern >= 100)
				{
					nCount++;
				}
			}
		}
	}
	P.nActiveProjectileCount = nCount;
}

void UpdatePlayer(HANDLE hMBAAHandle, DWORD dwBaseAddress, Player &P) {
	P.nLastFrameCount = P.nFrameCount;
	P.bLastOnRight = P.bIsOnRight;
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress), &P.cExists, 1, 0);
	if (!P.cExists)
	{
		return;
	}
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + 0x010), &P.nPattern, 4, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + 0x014), &P.nState, 4, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + 0x0BC), &P.nHealth, 4, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + 0x0C0), &P.nRedHealth, 4, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + 0x0C4), &P.fGuardGauge, 4, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + 0x0D8), &P.fGuardQuality, 4, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + 0x0E0), &P.nMagicCircuit, 4, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + 0x0F0), &P.nFrameCount, 4, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + 0x108), &P.nXPosition, 4, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + 0x10C), &P.nYPosition, 4, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + 0x11C), &P.nXSpeed, 4, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + 0x120), &P.nYSpeed, 4, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + 0x124), &P.sXAcceleration, 2, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + 0x126), &P.sYAcceleration, 2, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + 0x138), &P.nMomentum, 4, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + 0x172), &P.cHitstop, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + 0x176), &P.bThrowFlag, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + 0x178), &P.bTagFlag, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + 0x17B), &P.bBlockstunFlag, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + 0x185), &P.sGrantedStrikeInvuln, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + 0x186), &P.sGrantedThrowInvuln, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + 0x18E), &P.sUntechTotal, 2, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + 0x190), &P.sUntechCounter, 2, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + 0x1AC), &P.nHitstunRemaining, 4, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + 0x1FA), &P.cCounterHit, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + 0x2E4), &P.fGravity, 4, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + 0x2E8), &P.sUntechPenalty, 2, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + 0x2EA), &P.cAirDirectionalInput, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + 0x2EB), &P.cRawDirectionalInput, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + 0x2ED), &P.cButtonInput, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + 0x2EE), &P.cMacroInput, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + 0x315), &P.bIsOnRight, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + 0x320), &P.dwAnimationDataPointer, 4, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + 0x324), &P.dwAttackDataPointer, 4, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(P.dwAnimationDataPointer + 0x38), &P.dwStateDataPointer, 4, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(P.dwAnimationDataPointer + 0x42), &P.cBoxIndex, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(P.dwStateDataPointer + 0xC), &P.cStance, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(P.dwStateDataPointer + 0xD), &P.cStateInvuln, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(P.dwStateDataPointer + 0xE), &P.cNormalCancels, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(P.dwStateDataPointer + 0xF), &P.cSpecialCancels, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(P.dwStateDataPointer + 0x18), &P.nStateFlagset2, 4, 0);
	if (P.nPlayerNumber % 2 == 1)
	{
		ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x157FC0), &P.nInactionableFrames, 4, 0);
	}
	else if (P.nPlayerNumber % 2 == 0)
	{
		ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x1581CC), &P.nInactionableFrames, 4, 0);
	}
	CheckProjectiles(hMBAAHandle, dwBaseAddress, P);
}

void SaveState(HANDLE hMBAAHandle, DWORD dwBaseAddress, Save &S)
{
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x27BD70), &S.dwSaveEffects, 74576, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x158600), &S.dwSaveStopSituation, 1632, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x162A48), &S.cSaveGlobalEXFlash, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x157DD8), &S.dwSaveAttackDisplayInfo, 52, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x157E10), &S.dwSaveAttackDisplayInfo2, 1004, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x164B14), &S.dwSaveDestinationCamX, 4, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x164B18), &S.dwSaveCurrentCamX, 4, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x15DEC4), &S.dwSaveDestinationCamY, 4, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x15DEC8), &S.dwSaveCurrentCamY, 4, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x14EB70), &S.dwSaveCurrentCamZoom, 4, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x14EB74), &S.dwSaveDestinationCamZoom, 4, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x157DB8), &S.dwSaveContlFlag, 4, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x157DBC), &S.dwSaveContlFlag2, 4, 0);

	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x155140), &S.dwSaveP1, 972, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x155C3C), &S.dwSaveP2, 972, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x156738), &S.dwSaveP3, 972, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x157234), &S.dwSaveP4, 972, 0);

	S.bIsThisStateSaved = true;
}

void LoadState(HANDLE hMBAAHandle, DWORD dwBaseAddress, Save &S)
{
	WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x27BD70), &S.dwSaveEffects, 74576, 0);
	WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x158600), &S.dwSaveStopSituation, 1632, 0);
	WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x162A48), &S.cSaveGlobalEXFlash, 1, 0);
	WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x157DD8), &S.dwSaveAttackDisplayInfo, 52, 0);
	WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x157E10), &S.dwSaveAttackDisplayInfo2, 1004, 0);
	WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x164B14), &S.dwSaveDestinationCamX, 4, 0);
	WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x164B18), &S.dwSaveCurrentCamX, 4, 0);
	WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x15DEC4), &S.dwSaveDestinationCamY, 4, 0);
	WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x15DEC8), &S.dwSaveCurrentCamY, 4, 0);
	WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x14EB70), &S.dwSaveCurrentCamZoom, 4, 0);
	WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x14EB74), &S.dwSaveDestinationCamZoom, 4, 0);
	WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x157DB8), &S.dwSaveContlFlag, 4, 0);
	WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x157DBC), &S.dwSaveContlFlag2, 4, 0);

	WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x155140), &S.dwSaveP1, 972, 0);
	WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x155C3C), &S.dwSaveP2, 972, 0);
	WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x156738), &S.dwSaveP3, 972, 0);
	WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x157234), &S.dwSaveP4, 972, 0);
}

void SetStringLength(std::string &sString, int nDesiredLength, std::string sPadding = " ")
{
	int nStringLength = sString.length();
	if (nStringLength > nDesiredLength)
	{
		sString = sString.substr(nStringLength-2, nStringLength);
	}
	else if (nStringLength < nDesiredLength)
	{
		for (int i = 0; i < nDesiredLength - nStringLength; i++)
		{
			sString = sPadding + sString;
		}
	}
}

void CalculateAdvantage(Player& P1, Player& P2)
{
	if (P1.nInactionableFrames == 0 && P2.nInactionableFrames == 0)
	{
		bDoAdvantage = false;
	}
	else if (P1.nInactionableFrames != 0 && P2.nInactionableFrames != 0)
	{
		bDoAdvantage = true;
		P1.nAdvantageCounter = 0;
		P2.nAdvantageCounter = 0;
	}

	if (bDoAdvantage && nFrameCount != nLastFrameCount && cGlobalEXFlash == 0)
	{
		if (P1.nInactionableFrames == 0 && P2.nInactionableFrames != 0)
		{
			P1.nAdvantageCounter++;
		}
		else if (P2.nInactionableFrames == 0 && P1.nInactionableFrames != 0)
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
		P.sBar1[i] = "";
		P.sBar2[i] = "";
		P.sBar3[i] = "";
		P.sBar4[i] = "";
		P.sBar5[i] = "";
	}
}

void UpdateBars(Player& P, Player& Assist)
{
	// Foreground color -> \x1b[38;2;R;G;Bm
	// Background color -> \x1b[48;2;R;G;Bm
	std::string sFont = "\x1b[0m";
	std::string sBarValue = "  ";
	std::string sLeftFont = "\x1b[0m";
	std::string sRightFont = "\x1b[0m";
	std::string sLeftValue = " ";
	std::string sRightValue = " ";
	bool bIsButtonPressed = P.cButtonInput != 0 || P.cMacroInput != 0;

	if (bShowBar1)
	{
		//Bar 1 - General action information
		if (P.nInactionableFrames != 0)
		{
			sFont = "\x1b[38;2;255;255;255m\x1b[48;2;65;200;0m";
			sBarValue = std::to_string(P.nInactionableFrames);
			if (P.nPattern >= 35 && P.nPattern <= 37) //Jump Startup
			{
				sFont = "\x1b[38;2;177;177;255m\x1b[48;2;241;224;132m";
			}
			else if (P.nHitstunRemaining != 0 && P.bBlockstunFlag == 0)
			{
				sFont = "\x1b[38;2;255;255;255m\x1b[48;2;140;140;140m";
				if (P.cStance == 1)
				{
					if (P.sUntechCounter < P.sUntechTotal)
					{
						sBarValue = std::to_string(P.sUntechTotal - P.sUntechCounter);
					}
				}
				else
				{
					if (P.nHitstunRemaining > 1)
						sBarValue = std::to_string(P.nHitstunRemaining - 1);
				}
			}
			else if (P.bBlockstunFlag)
			{
				sFont = "\x1b[38;2;255;255;255m\x1b[48;2;180;180;180m";
			}
		}
		else
		{
			sFont = "\x1b[38;2;92;92;92m\x1b[48;2;0;0;0m";
			sBarValue = std::to_string(P.nPattern);

			if (bDoAdvantage)
			{
				sBarValue = std::to_string(P.nAdvantageCounter);
				if (P.nAdvantageCounter != 0)
				{
					sFont = "\x1b[38;2;255;255;255m\x1b[48;2;0;0;0m";
				}
			}
		}

		if (P.bThrowFlag != 0)
		{
			sFont = "\x1b[38;2;255;255;255m\x1b[48;2;110;110;110m";
			sBarValue = " t";
		}
		else if (P.cBoxIndex == 12)
		{
			sFont = "\x1b[38;2;255;255;255m\x1b[48;2;225;184;0m";
		}
		else if (P.cBoxIndex == 10)
		{
			sFont = "\x1b[38;2;255;255;255m\x1b[48;2;145;194;255m";
		}
		else if (P.cBoxIndex <= 1 || P.sGrantedStrikeInvuln != 0 || P.cStateInvuln == 3)
		{
			if (P.nInactionableFrames != 0)
			{
				sFont = "\x1b[38;2;160;160;160m\x1b[48;2;255;255;255m";
			}
			else
			{
				sFont = "\x1b[38;2;100;100;100m\x1b[48;2;255;255;255m";
			}

		}

		if (cGlobalEXFlash != 0 || cP1EXFlash != 0 || cP2EXFlash != 0)
		{
			sFont = "\x1b[38;2;255;255;255m\x1b[48;2;60;60;60m";
		}
		else if (P.cHitstop != 0)
		{
			sFont = "\x1b[38;2;255;255;255m\x1b[48;2;60;80;128m";
		}

		SetStringLength(sBarValue, 2);
		P.sBar1[nBarCounter % BAR_MEMORY_SIZE] = sFont + sBarValue + "\x1b[0m";

		sFont = "\x1b[0m";
		sBarValue = "  ";

	}
	if (bShowBar2)
	{
		//Bar 2 - Active frames
		if (P.cStance == 1)
		{
			sBarValue = " ^";
		}

		if (P.dwAttackDataPointer != 0)
		{
			sBarValue = std::to_string(P.nActiveCounter);
			if (cGlobalEXFlash != 0 || cP1EXFlash != 0 || cP2EXFlash != 0)
			{
				sFont = "\x1b[38;2;255;255;255m\x1b[48;2;60;60;60m";
			}
			else if (P.cHitstop != 0)
			{
				sFont = "\x1b[38;2;255;255;255m\x1b[48;2;60;80;128m";
			}
			else
			{
				sFont = "\x1b[38;2;255;255;255m\x1b[48;2;255;0;0m";
			}

			if (P.cStance == 1)
			{
				sFont += "\x1b[4m";
			}
		}

		SetStringLength(sBarValue, 2);
		P.sBar2[nBarCounter % BAR_MEMORY_SIZE] = sFont + sBarValue + "\x1b[0m";

		sFont = "";
		sBarValue = "  ";
	}
	if (bShowBar3)
	{
		//Bar 3 - Projectile and assist active frames
		if (P.nActiveProjectileCount != 0 || Assist.nActiveProjectileCount != 0)
		{
			sFont = "\x1b[38;2;255;255;255m\x1b[48;2;255;0;0m";
			sBarValue = std::to_string(P.nActiveProjectileCount + Assist.nActiveProjectileCount);
		}

		if (Assist.dwAttackDataPointer != 0)
		{
			sBarValue = std::to_string(Assist.nActiveCounter);
			if (cGlobalEXFlash != 0 || cP1EXFlash != 0 || cP2EXFlash != 0)
			{
				sFont = "\x1b[38;2;255;255;255m\x1b[48;2;60;60;60m";
			}
			else if (Assist.cHitstop != 0)
			{
				sFont = "\x1b[38;2;255;255;255m\x1b[48;2;60;80;128m";
			}
			else
			{
				sFont = "\x1b[38;2;255;255;255m\x1b[48;2;255;128;0m";
			}

			if (Assist.cStance == 1)
			{
				sFont += "\x1b[4m";
			}
		}

		SetStringLength(sBarValue, 2);
		P.sBar3[nBarCounter % BAR_MEMORY_SIZE] = sFont + sBarValue + "\x1b[0m";

		sFont = "";
		sBarValue = "  ";
	}
	if (bShowBar4)
	{
		//Bar 4
		if (P.cMacroInput != 0)
		{
			sRightValue = "e";
		}
		else
		{
			sRightValue = ".";
		}

		if (P.cButtonInput & 0x10)
		{
			sLeftFont = "\x1b[38;2;255;143;169m\x1b[48;2;170;27;58m";
		}
		else if (bIsButtonPressed)
		{
			sLeftFont = "\x1b[38;2;255;255;255m\x1b[48;2;128;128;128m";
		}

		if (P.cButtonInput & 0x20)
		{
			sRightFont = "\x1b[38;2;255;255;137m\x1b[48;2;169;91;7m";
		}
		else if (bIsButtonPressed)
		{
			sRightFont = "\x1b[38;2;255;255;255m\x1b[48;2;128;128;128m";
		}

		if (P.bLastOnRight != P.bIsOnRight)
		{
			sLeftFont += "\x1b[4m";
			sRightFont += "\x1b[4m";
		}

		P.sBar4[nBarCounter % BAR_MEMORY_SIZE] = sLeftFont + sLeftValue + sRightFont + sRightValue + "\x1b[0m";

		sLeftFont = "\x1b[0m";
		sRightFont = "\x1b[0m";
		sLeftValue = " ";
		sRightValue = " ";
	}
	if (bShowBar5)
	{
		//Bar 5
		if (P.cRawDirectionalInput == 0)
		{
			sRightValue = ".";
		}
		else if (P.cStance == 1)
		{
			sRightValue = std::to_string(P.cAirDirectionalInput);
		}
		else if (P.bIsOnRight)
		{
			sRightValue = std::to_string(REVERSE_INPUT_MAP[P.cRawDirectionalInput]);
		}
		else
		{
			sRightValue = std::to_string(P.cRawDirectionalInput);
		}

		if (P.cButtonInput & 0x40)
		{
			sLeftFont = "\x1b[38;2;143;255;195m\x1b[48;2;18;132;62m";
		}
		else if (bIsButtonPressed)
		{
			sLeftFont = "\x1b[38;2;255;255;255m\x1b[48;2;128;128;128m";
		}

		if (P.cButtonInput & 0x80)
		{
			sRightFont = "\x1b[38;2;137;255;255m\x1b[48;2;21;66;161m";
		}
		else if (bIsButtonPressed)
		{
			sRightFont = "\x1b[38;2;255;255;255m\x1b[48;2;128;128;128m";
		}

		if (P.bLastOnRight != P.bIsOnRight)
		{
			sLeftFont += "\x1b[4m";
			sRightFont += "\x1b[4m";
		}

		P.sBar5[nBarCounter % BAR_MEMORY_SIZE] = sLeftFont + sLeftValue + sRightFont + sRightValue + "\x1b[0m";
	}
}

void IncrementActive(Player& P)
{
	if (P.dwAttackDataPointer != 0 && P.cHitstop == 0 && P.nLastFrameCount != P.nFrameCount)
	{
		P.nActiveCounter++;
	}
	else if (P.dwAttackDataPointer == 0)
	{
		P.nActiveCounter = 0;
	}
}

void BarHandling(Player &P1, Player &P2, Player& P1Assist, Player& P2Assist)
{
	CalculateAdvantage(P1, P2);

	bool IsInput = (
		P1.cRawDirectionalInput != 0 || P2.cRawDirectionalInput != 0 ||
		P1.cButtonInput != 0 || P2.cButtonInput ||
		P1.cMacroInput != 0 || P2.cMacroInput != 0
		); //True if player or dummy has any input
	bool DoBarUpdate = (
		P1.nInactionableFrames != 0 || P2.nInactionableFrames ||
		P1.nStateFlagset2 != 0 || P2.nStateFlagset2 != 0 ||
		P1.nActiveProjectileCount != 0 || P2.nActiveProjectileCount ||
		P1Assist.dwAttackDataPointer != 0 || P2Assist.dwAttackDataPointer ||
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
		bool bIsFreeze = (
			cGlobalEXFlash != 0 ||
			(P1.cHitstop > 1 && P2.cHitstop > 1)
			);

		if (!(bHideFreeze && bIsFreeze))
		{
			IncrementActive(P1);
			IncrementActive(P2);
			UpdateBars(P1, P1Assist);
			UpdateBars(P2, P2Assist);
			if (P1Assist.cExists)
			{
				IncrementActive(P1Assist);
				UpdateBars(P1Assist, P1);

			}
			if (P2Assist.cExists)
			{
				IncrementActive(P2Assist);
				UpdateBars(P2Assist, P2);

			}
			nBarCounter++;
		}
	}

	if (nTrueFrameCount == 0)
	{
		ResetBars(P1);
		ResetBars(P2);
		ResetBars(P1Assist);
		ResetBars(P2Assist);
	}
}

void PrintFrameDisplay(Player &P1, Player &P2, Player &P3, Player &P4)
{
	std::string sColumnHeader = "\x1b[0;4m";
	for (int i = 1; i <= nBarDisplayRange; i++)
	{
		if (i % 10 != 0)
		{
			sColumnHeader += std::format("{:2}", i % 10);
		}
		else
		{
			sColumnHeader += std::format("\x1b[7;4m{:2}\x1b[0;4m", i % 100);
		}
	}
	sColumnHeader += "\x1b[0m\x1b[K\n";

	P1.sBarString1 = "";
	P1.sBarString2 = "";
	P1.sBarString3 = "";
	P1.sBarString4 = "";
	P1.sBarString5 = "";
	for (int i = (nBarCounter % BAR_MEMORY_SIZE) - nBarDisplayRange - nBarScrolling; i < (nBarCounter % BAR_MEMORY_SIZE) - nBarScrolling; i++)
	{
		if (i < 0)
		{
			P1.sBarString1 += P1.sBar1[i + BAR_MEMORY_SIZE];
			P1.sBarString2 += P1.sBar2[i + BAR_MEMORY_SIZE];
			P1.sBarString3 += P1.sBar3[i + BAR_MEMORY_SIZE];
			P1.sBarString4 += P1.sBar4[i + BAR_MEMORY_SIZE];
			P1.sBarString5 += P1.sBar5[i + BAR_MEMORY_SIZE];
		}
		else
		{
			P1.sBarString1 += P1.sBar1[i];
			P1.sBarString2 += P1.sBar2[i];
			P1.sBarString3 += P1.sBar3[i];
			P1.sBarString4 += P1.sBar4[i];
			P1.sBarString5 += P1.sBar5[i];
		}
	}
	P1.sBarString1 += "\x1b[0m\x1b[K\n";
	P1.sBarString2 += "\x1b[0m\x1b[K\n";
	P1.sBarString3 += "\x1b[0m\x1b[K\n";
	P1.sBarString4 += "\x1b[0m\x1b[K\n";
	P1.sBarString5 += "\x1b[0m\x1b[K\n";

	P2.sBarString1 = "";
	P2.sBarString2 = "";
	P2.sBarString3 = "";
	P2.sBarString4 = "";
	P2.sBarString5 = "";
	for (int i = (nBarCounter % BAR_MEMORY_SIZE) - nBarDisplayRange - nBarScrolling; i < (nBarCounter % BAR_MEMORY_SIZE) - nBarScrolling; i++)
	{
		if (i < 0)
		{
			P2.sBarString1 += P2.sBar1[i + BAR_MEMORY_SIZE];
			P2.sBarString2 += P2.sBar2[i + BAR_MEMORY_SIZE];
			P2.sBarString3 += P2.sBar3[i + BAR_MEMORY_SIZE];
			P2.sBarString4 += P2.sBar4[i + BAR_MEMORY_SIZE];
			P2.sBarString5 += P2.sBar5[i + BAR_MEMORY_SIZE];
		}
		else
		{
			P2.sBarString1 += P2.sBar1[i];
			P2.sBarString2 += P2.sBar2[i];
			P2.sBarString3 += P2.sBar3[i];
			P2.sBarString4 += P2.sBar4[i];
			P2.sBarString5 += P2.sBar5[i];
		}
	}
	P2.sBarString1 += "\x1b[0m\x1b[K\n";
	P2.sBarString2 += "\x1b[0m\x1b[K\n";
	P2.sBarString3 += "\x1b[0m\x1b[K\n";
	P2.sBarString4 += "\x1b[0m\x1b[K\n";
	P2.sBarString5 += "\x1b[0m\x1b[K\n";

	int nP1XPixelPosition = (int)floor(P1.nXPosition / 128.0);
	int nP1YPixelPosition = (int)floor(P1.nYPosition / 128.0);
	char cP1CounterHit = CH_MAP[P1.cCounterHit];
	int nP1GuardGauge = (int)P1.fGuardGauge;
	int nP1Gravity = (int)max(0, round((P1.fGravity - 0.072) / 0.008));
	nP1Gravity -= (int)floor(nP1Gravity / 60.0);
	nP1Gravity = (int)ceil(nP1Gravity / 6.0);
	int nP1GravityHits = (int)round(P1.fGravity / 0.008);

	int nP2XPixelPosition = (int)floor(P2.nXPosition / 128.0);
	int nP2YPixelPosition = (int)floor(P2.nYPosition / 128.0);
	char cP2CounterHit = CH_MAP[P2.cCounterHit];
	int nP2GuardGauge = (int)P2.fGuardGauge;
	int nP2Gravity = (int)max(0, round((P2.fGravity - 0.072) / 0.008));
	nP2Gravity -= (int)floor(nP2Gravity / 60.0);
	nP2Gravity = (int)ceil(nP2Gravity / 6.0);
	int nP2GravityHits = (int)round(P2.fGravity / 0.008);

	int nXDistance = (int)abs(P1.nXPosition - P2.nXPosition);
	int nYDistance = (int)abs(P1.nYPosition - P2.nYPosition);
	int nXPixelDistance = (int)abs(floor(P1.nXPosition / 128.0) - floor(P2.nXPosition / 128.0));
	int nYPixelDistance = (int)abs(floor(P1.nYPosition / 128.0) - floor(P2.nYPosition / 128.0));

	if (bShowInfo1)
	{
		printf("\x1b[0m" "(%6i, %6i)" "\x1b[7m" "(%4i, %4i)" "\x1b[0m" "pat %3i [%2i]" "\x1b[7m"
			"x-spd %5i" "\x1b[0m" "x-acc %5i" "\x1b[7m" "y-spd %5i" "\x1b[0m" "y-acc %5i" "\x1b[7m" "hp %5i" "\x1b[0m" "mc %5i" "\x1b[0m\x1b[K\n",
			P1.nXPosition, P1.nYPosition, nP1XPixelPosition, nP1YPixelPosition, P1.nPattern, P1.nState,
			P1.nXSpeed, P1.sXAcceleration, P1.nYSpeed, P1.sYAcceleration, P1.nHealth, P1.nMagicCircuit);

		printf("\x1b[0m" "(%6i, %6i)" "\x1b[7m" "(%4i, %4i)" "\x1b[0m" "pat %3i [%2i]" "\x1b[7m"
			"x-spd %5i" "\x1b[0m" "x-acc %5i" "\x1b[7m" "y-spd %5i" "\x1b[0m" "y-acc %5i" "\x1b[7m" "hp %5i" "\x1b[0m" "mc %5i" "\x1b[0m\x1b[K\n",
			P2.nXPosition, P2.nYPosition, nP2XPixelPosition, nP2YPixelPosition, P2.nPattern, P2.nState,
			P2.nXSpeed, P2.sXAcceleration, P2.nYSpeed, P2.sYAcceleration, P2.nHealth, P2.nMagicCircuit);
	}
	
	if (bShowInfo2)
	{
		printf("\x1b[0m" "(%6i, %6i)" "\x1b[7m" "(%4i, %4i)" "\x1b[0m" "adv %3i" "\x1b[K\n",
			nXDistance, nYDistance, nXPixelDistance, nYPixelDistance, (P1.nAdvantageCounter - P2.nAdvantageCounter) % 100);
	}

	if (bShowBar1 || bShowBar2 || bShowBar3)
	{
		std::cout << sColumnHeader;
	}

	if (bShowBar1)
	{
		std::cout << P1.sBarString1;
	}
	if (bShowBar2)
	{
		std::cout << P1.sBarString2;
	}
	if (bShowBar3)
	{
		std::cout << P1.sBarString3;
	}

	if (bShowBar1)
	{
		std::cout << P2.sBarString1;
	}
	if (bShowBar2)
	{
		std::cout << P2.sBarString2;
	}
	if (bShowBar3)
	{
		std::cout << P2.sBarString3;
	}

	if (bShowInfo3)
	{
		printf("\x1b[0m" "ex %2i" "\x1b[7m" "ch %c" "\x1b[0m" "partner %3i [%2i]" "\x1b[7m" "scaling %2i [%i+%i]" "\x1b[0m" "rhp %5i" "\x1b[7m" "gg %4i [%.3f]" "\x1b[0m\x1b[K\n",
			cP1EXFlash, cP1CounterHit, P3.nPattern, P3.nState, nP1GravityHits, nP1Gravity % 10, P1.sUntechPenalty % 10, P1.nRedHealth, nP1GuardGauge, P1.fGuardQuality);
		printf("\x1b[0m" "ex %2i" "\x1b[7m" "ch %c" "\x1b[0m" "partner %3i [%2i]" "\x1b[7m" "scaling %2i [%i+%i]" "\x1b[0m" "rhp %5i" "\x1b[7m" "gg %4i [%.3f]" "\x1b[0m\x1b[K\n",
			cP2EXFlash, cP2CounterHit, P4.nPattern, P4.nState, nP2GravityHits, nP2Gravity % 10, P2.sUntechPenalty % 10, P2.nRedHealth, nP2GuardGauge, P2.fGuardQuality);
	}

	if (bShowBar4 || bShowBar5)
	{
		std::cout << sColumnHeader;
	}

	if (bShowBar4)
	{
		std::cout << P1.sBarString4;
	}
	if (bShowBar5)
	{
		std::cout << P1.sBarString5;
	}

	if (bShowBar4)
	{
		std::cout << P2.sBarString4;
	}
	if (bShowBar5)
	{
		std::cout << P2.sBarString5;
	}

	std::cout << "\x1b[J";
}

void FrameDisplay(HANDLE hMBAAHandle, DWORD dwBaseAddress, Player& P1, Player& P2, Player& P3, Player& P4)
{
	CheckGameState(hMBAAHandle, dwBaseAddress);

	if (cGameState != 1) //If not in game (any gamemode)
	{
		bIsStateSaved = false;
		for (int i = 0; i < MAX_SAVES; i++)
		{
			Saves[i].bIsThisStateSaved = false;
		}
		std::cout << "\x1b[J";
		return;
	}

	CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &screenBufferInfo);
	nBarDisplayRange = (screenBufferInfo.srWindow.Right - screenBufferInfo.srWindow.Left) / 2;

	if (cFN1Input > 0 && bEnableFN1Save)
	{
		if (!bLockInput)
		{
			bLockInput = true;
			SaveState(hMBAAHandle, dwBaseAddress, Saves[nSaveSlot - 1]);
			bIsStateSaved = true;
		}
		char c5 = 5;
		WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x162A48), &c5, 1, 0);
	}
	else if (cFN2Input > 0 && bEnableFN2Load)
	{
		if (!bLockInput)
		{
			bLockInput = true;
			if (cDummyState == 5 || cDummyState == -1)
			{
				char cFF = 0xFF;
				WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + 0x15DEC3), &cFF, 1, 0);
			}
		}
	}
	else if (bLockInput)
	{
		bLockInput = false;
	}

	UpdateGlobals(hMBAAHandle, dwBaseAddress);
	Player* Player1 = &P1;
	Player* Player2 = &P2;
	Player* Player3 = &P3;
	Player* Player4 = &P4;
	if (P1.bTagFlag)
	{
		Player1 = &P3;
		Player3 = &P1;
	}
	if (P2.bTagFlag)
	{
		Player2 = &P4;
		Player4 = &P2;
	}

	if (nLastTrueFrameCount != nTrueFrameCount)
	{
		UpdatePlayer(hMBAAHandle, dwBaseAddress, P1);
		UpdatePlayer(hMBAAHandle, dwBaseAddress, P2);
		UpdatePlayer(hMBAAHandle, dwBaseAddress, P3);
		UpdatePlayer(hMBAAHandle, dwBaseAddress, P4);
		
		BarHandling(*Player1, *Player2, *Player3, *Player4);

		if (nTrueFrameCount == 1 && bIsStateSaved  && Saves[nSaveSlot - 1].bIsThisStateSaved && bEnableFN2Load)
		{
			ResetBars(P1);
			ResetBars(P2);
			ResetBars(P3);
			ResetBars(P4);
			LoadState(hMBAAHandle, dwBaseAddress, Saves[nSaveSlot - 1]);
		}
	}

	PrintFrameDisplay(*Player1, *Player2, *Player3, *Player4);
}
