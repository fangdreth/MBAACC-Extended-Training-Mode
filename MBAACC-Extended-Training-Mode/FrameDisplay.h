#pragma once
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include "..\Common\Common.h"

char cGameState = 0; // 1:In-Game 2:Title 3:Logos 8:Loading 9:Arcade Cutscene 10:Next Stage 12:Options 20:CSS 25:Main Menu
char cP1Freeze = 0; //Used for EXFlashes where initiator still moves (ex. Satsuki 214C winds up during flash)
char cP2Freeze = 0;
int nFrameCount = 0; //Counts slower during slowdown
int nLastFrameCount = 0;
int nTrueFrameCount = 0; //Counts all frames during slowdown
int nLastTrueFrameCount = 0;
char cGlobalFreeze = 0; //Used for EXFlashes where neither char moves
char cP1FN2Input = 0;
char cDummyState = 0; // Same as Common.h "Enemy Status" except -1 for recording

int nBarCounter = 0;
short sBarScrolling = 0;
int nBarIntervalCounter = 0;
int nBarIntervalMax = 0;
int nBarDisplayRange = 0;
bool bIsBarReset = false;
bool bDoBarReset = false;
bool bUpdateBar = false;
bool bDoAdvantage = false;

bool bLockInput = false;

static bool bEnableFN2Load = false;
static bool bInExtendedSettings = false;

static bool bSimpleFrameInfo = true;
static bool bDisplayFreeze = false; //Whether to show global ex flashes and frames where both chars are in hitstop
static bool bDisplayInputs = false;

static uint8_t nSaveSlot = 0;

static int nPlayerAdvantage;
int nSharedHitstop;

struct Save {
	bool bSaved = false;

	DWORD dwaSaveEffects[ADJ_SAVE_EFFECTS_SIZE] = {}; //Effect and projectile data
	DWORD dwaSaveStopSituation[ADJ_SAVE_STOP_SITUATION_SIZE] = {};
	DWORD dwSaveGlobalFreeze = 0;
	DWORD dwaSaveAttackDisplayInfo[ADJ_SAVE_ATTACK_DISPLAY_INFO_SIZE] = {}; //Ends just before Max Damage
	DWORD dwaSaveAttackDisplayInfo2[ADJ_SAVE_ATTACK_DISPLAY_INFO_2_SIZE] = {}; //Starts after Max Damage, maybe also has current combo info
	DWORD dwSaveDestinationCamX = 0;
	DWORD dwSaveCurrentCamX = 0;
	DWORD dwSaveCurrentCamXCopy = 0;
	DWORD dwSaveDestinationCamY = 0;
	DWORD dwSaveCurrentCamY = 0;
	DWORD dwSaveCurrentCamYCopy = 0;
	DWORD dwSaveDestinationCamZoom = 0;
	DWORD dwSaveCurrentCamZoom = 0;
	DWORD dwSaveP1ControlledCharacter = 0;
	DWORD dwSaveP1NextControlledCharacter = 0;
	DWORD dwSaveP2ControlledCharacter = 0;
	DWORD dwSaveP2NextControlledCharacter = 0;

	DWORD dwaSave1P1[ADJ_SAVE_PLAYER_1_SIZE] = {}; //Player data from pattern up to (not including) recieving attack data pointer
	DWORD dwaSave2P1[ADJ_SAVE_PLAYER_2_SIZE] = {}; //Player data from (not including) recieving attack data pointer to (not including) pattern data pointer
	DWORD dwaSave1P2[ADJ_SAVE_PLAYER_1_SIZE] = {};
	DWORD dwaSave2P2[ADJ_SAVE_PLAYER_2_SIZE] = {};
	DWORD dwaSave1P3[ADJ_SAVE_PLAYER_1_SIZE] = {};
	DWORD dwaSave2P3[ADJ_SAVE_PLAYER_2_SIZE] = {};
	DWORD dwaSave1P4[ADJ_SAVE_PLAYER_1_SIZE] = {};
	DWORD dwaSave2P4[ADJ_SAVE_PLAYER_2_SIZE] = {};
};

Save Saves[MAX_SAVES];

void CheckGameState(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adGameState), &cGameState, 1, 0);
}

void UpdateGlobals(HANDLE hMBAAHandle, DWORD dwBaseAddress)
{
	nLastFrameCount = nFrameCount;
	nLastTrueFrameCount = nTrueFrameCount;
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adP1Freeze), &cP1Freeze, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adP2Freeze), &cP2Freeze, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adFrameCount), &nFrameCount, 4, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adTrueFrameCount), &nTrueFrameCount, 4, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adGlobalFreeze), &cGlobalFreeze, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adDummyState), &cDummyState, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adP1FN2Input), &cP1FN2Input, 1, 0);
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
	char cAnimation_BoxIndex = 0;
	DWORD dwAttackDataPointer = 0x0;
	char cState_Stance = 0;
	char cState_Invuln = 0;
	char cState_NormalCancel = 0;
	char cState_SpecialCancel = 0;
	int nState_Flagset2 = 0;

	int nInactionableFrames = 0;
	int nLastInactionableFrames = 0;

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
	int nInactiveMemory = 0;
	int nAdvantageCounter = 0;
	int nLastFrameCount = 0;
	int nActiveProjectileCount = 0;
	bool bLastOnRight = false;
};

void CheckProjectiles(HANDLE hMBAAHandle, DWORD dwBaseAddress, Player& P)
{
	char cCharacterNumber = 0;
	char cMoon = 0;
	int nCharacterID = 0;
	if (P.nPlayerNumber % 2 == 1)
	{
		ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1CharNumber), &cCharacterNumber, 1, 0);
		ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP1CharMoon), &cMoon, 1, 0);
	}
	else
	{
		ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP2CharNumber), &cCharacterNumber, 1, 0);
		ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + dwP2CharMoon), &cMoon, 1, 0);
	}
	nCharacterID = 10 * cCharacterNumber + cMoon;
	std::map<std::string, int> CharacterMap = MBAACC_Map[nCharacterID];

	bool bProjectileExists = 0;
	char cProjectileSource = 0;
	int nProjectilePattern = 0;
	DWORD dwProjectileAttackDataPointer = 0;
	int nCount = 0;
	for (int i = 0; i < 100; i++)
	{
		ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adEffectBase + dwEffectStructSize * i), &bProjectileExists, 1, 0);
		if (bProjectileExists)
		{
			ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adEffectBase + dwEffectStructSize * i + adEffectSource), &cProjectileSource, 1, 0);
			if (cProjectileSource + 1 == P.nPlayerNumber)
			{
				ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adEffectBase + dwEffectStructSize * i + adPattern), &nProjectilePattern, 4, 0);
				ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adEffectBase + dwEffectStructSize * i + adAttackDataPointer), &dwProjectileAttackDataPointer, 4, 0);
				if (dwProjectileAttackDataPointer != 0 && nProjectilePattern >= 60)
				{
					bool bIncrement = true;
					for (auto const& [key, val] : CharacterMap)
					{
						if (nProjectilePattern == val)
						{
							bIncrement = false;
							break;
						}
					}
					if (bIncrement)
					{
						nCount++;
					}
				}
			}
		}
	}
	P.nActiveProjectileCount = nCount;
}

void UpdatePlayer(HANDLE hMBAAHandle, DWORD dwBaseAddress, Player &P) {
	P.nLastInactionableFrames = P.nInactionableFrames;
	P.nLastFrameCount = P.nFrameCount;
	P.bLastOnRight = P.bIsOnRight;
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress), &P.cExists, 1, 0);
	if (!P.cExists)
	{
		return;
	}
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + adPattern), &P.nPattern, 4, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + adState), &P.nState, 4, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + adHealth), &P.nHealth, 4, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + adRedHealth), &P.nRedHealth, 4, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + adGuardGauge), &P.fGuardGauge, 4, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + adGuardQuality), &P.fGuardQuality, 4, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + adMagicCircuit), &P.nMagicCircuit, 4, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + adPlayerFrameCount), &P.nFrameCount, 4, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + adXPosition), &P.nXPosition, 4, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + adYPosition), &P.nYPosition, 4, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + adXSpeed), &P.nXSpeed, 4, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + adYSpeed), &P.nYSpeed, 4, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + adXAcceleration), &P.sXAcceleration, 2, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + adYAcceleration), &P.sYAcceleration, 2, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + adMomentum), &P.nMomentum, 4, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + adHitstop), &P.cHitstop, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + adThrowFlag), &P.bThrowFlag, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + adTagFlag), &P.bTagFlag, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + adBlockstunFlag), &P.bBlockstunFlag, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + adEFStrikeInvuln), &P.sGrantedStrikeInvuln, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + adEFThrowInvuln), &P.sGrantedThrowInvuln, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + adUntechTotal), &P.sUntechTotal, 2, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + adUntechCounter), &P.sUntechCounter, 2, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + adHitstunRemaining), &P.nHitstunRemaining, 4, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + adCounterHit), &P.cCounterHit, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + adGravity), &P.fGravity, 4, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + adUntechPenalty), &P.sUntechPenalty, 2, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + adAirDirectionalInput), &P.cAirDirectionalInput, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + adRawDirectionalInput), &P.cRawDirectionalInput, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + adButtonInput), &P.cButtonInput, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + adMacroInput), &P.cMacroInput, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + adOnRightFlag), &P.bIsOnRight, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + adAnimationDataPointer), &P.dwAnimationDataPointer, 4, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + P.dwCharacterBaseAddress + adAttackDataPointer), &P.dwAttackDataPointer, 4, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(P.dwAnimationDataPointer + adAnimationData_StateDataPointer), &P.dwStateDataPointer, 4, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(P.dwAnimationDataPointer + adAnimationData_BoxIndex), &P.cAnimation_BoxIndex, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(P.dwStateDataPointer + adStateData_Stance), &P.cState_Stance, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(P.dwStateDataPointer + adStateData_Invuln), &P.cState_Invuln, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(P.dwStateDataPointer + adStateData_NormalCancel), &P.cState_NormalCancel, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(P.dwStateDataPointer + adStateData_SpecialCancel), &P.cState_SpecialCancel, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(P.dwStateDataPointer + adStateData_Flagset2), &P.nState_Flagset2, 4, 0);
	if (P.nPlayerNumber % 2 == 1)
	{
		ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adP1Inaction), &P.nInactionableFrames, 4, 0);
	}
	else if (P.nPlayerNumber % 2 == 0)
	{
		ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adP2Inaction), &P.nInactionableFrames, 4, 0);
	}
	CheckProjectiles(hMBAAHandle, dwBaseAddress, P);
}

void SaveState(HANDLE hMBAAHandle, DWORD dwBaseAddress, int nSaveSlot)
{
	if (nSaveSlot > 0 && nSaveSlot < MAX_SAVES)
	{
		Save &S = Saves[nSaveSlot - 1];
		ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSaveEffects), &S.dwaSaveEffects, SAVE_EFFECTS_SIZE, 0);
		ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSaveStopSituation), &S.dwaSaveStopSituation, SAVE_STOP_SITUATION_SIZE, 0);
		ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adGlobalFreeze), &S.dwSaveGlobalFreeze, 1, 0);
		ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSaveAttackDisplayInfo), &S.dwaSaveAttackDisplayInfo, SAVE_ATTACK_DISPLAY_INFO_SIZE, 0);
		ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSaveAttackDisplayInfo2), &S.dwaSaveAttackDisplayInfo2, SAVE_ATTACK_DISPLAY_INFO_2_SIZE, 0);
		ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSaveDestinationCamX), &S.dwSaveDestinationCamX, 4, 0);
		ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSaveCurrentCamX), &S.dwSaveCurrentCamX, 4, 0);
		ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSaveCurrentCamXCopy), &S.dwSaveCurrentCamXCopy, 4, 0);
		ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSaveDestinationCamY), &S.dwSaveDestinationCamY, 4, 0);
		ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSaveCurrentCamY), &S.dwSaveCurrentCamY, 4, 0);
		ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSaveCurrentCamYCopy), &S.dwSaveCurrentCamYCopy, 4, 0);
		ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSaveCurrentCamZoom), &S.dwSaveCurrentCamZoom, 4, 0);
		ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSaveDestinationCamZoom), &S.dwSaveDestinationCamZoom, 4, 0);
		ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adP1ControlledCharacter), &S.dwSaveP1ControlledCharacter, 4, 0);
		ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adP1NextControlledCharacter), &S.dwSaveP1NextControlledCharacter, 4, 0);
		ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adP2ControlledCharacter), &S.dwSaveP2ControlledCharacter, 4, 0);
		ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adP2NextControlledCharacter), &S.dwSaveP2NextControlledCharacter, 4, 0);

		ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adP1Base + adSave1Offset), &S.dwaSave1P1, SAVE_PLAYER_1_SIZE, 0);
		ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adP1Base + adSave2Offset), &S.dwaSave2P1, SAVE_PLAYER_2_SIZE, 0);
		ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adP2Base + adSave1Offset), &S.dwaSave1P2, SAVE_PLAYER_1_SIZE, 0);
		ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adP2Base + adSave2Offset), &S.dwaSave2P2, SAVE_PLAYER_2_SIZE, 0);
		ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adP3Base + adSave1Offset), &S.dwaSave1P3, SAVE_PLAYER_1_SIZE, 0);
		ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adP3Base + adSave2Offset), &S.dwaSave2P3, SAVE_PLAYER_2_SIZE, 0);
		ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adP4Base + adSave1Offset), &S.dwaSave1P4, SAVE_PLAYER_1_SIZE, 0);
		ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adP4Base + adSave2Offset), &S.dwaSave2P4, SAVE_PLAYER_2_SIZE, 0);

		S.bSaved = true;
	}
}

void LoadState(HANDLE hMBAAHandle, DWORD dwBaseAddress, int nSaveSlot)
{
	if (nSaveSlot > 0 && nSaveSlot < MAX_SAVES)
	{
		Save& S = Saves[nSaveSlot - 1];
		WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSaveEffects), &S.dwaSaveEffects, SAVE_EFFECTS_SIZE, 0);
		WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSaveStopSituation), &S.dwaSaveStopSituation, SAVE_STOP_SITUATION_SIZE, 0);
		WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adGlobalFreeze), &S.dwSaveGlobalFreeze, 1, 0);
		WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSaveAttackDisplayInfo), &S.dwaSaveAttackDisplayInfo, SAVE_ATTACK_DISPLAY_INFO_SIZE, 0);
		WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSaveAttackDisplayInfo2), &S.dwaSaveAttackDisplayInfo2, SAVE_ATTACK_DISPLAY_INFO_2_SIZE, 0);
		WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSaveDestinationCamX), &S.dwSaveDestinationCamX, 4, 0);
		WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSaveCurrentCamX), &S.dwSaveCurrentCamX, 4, 0);
		WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSaveCurrentCamXCopy), &S.dwSaveCurrentCamXCopy, 4, 0);
		WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSaveDestinationCamY), &S.dwSaveDestinationCamY, 4, 0);
		WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSaveCurrentCamY), &S.dwSaveCurrentCamY, 4, 0);
		WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSaveCurrentCamYCopy), &S.dwSaveCurrentCamYCopy, 4, 0);
		WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSaveCurrentCamZoom), &S.dwSaveCurrentCamZoom, 4, 0);
		WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSaveDestinationCamZoom), &S.dwSaveDestinationCamZoom, 4, 0);
		WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adP1ControlledCharacter), &S.dwSaveP1ControlledCharacter, 4, 0);
		WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adP1NextControlledCharacter), &S.dwSaveP1NextControlledCharacter, 4, 0);
		WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adP2ControlledCharacter), &S.dwSaveP2ControlledCharacter, 4, 0);
		WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adP2NextControlledCharacter), &S.dwSaveP2NextControlledCharacter, 4, 0);

		WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adP1Base + adSave1Offset), &S.dwaSave1P1, SAVE_PLAYER_1_SIZE, 0);
		WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adP1Base + adSave2Offset), &S.dwaSave2P1, SAVE_PLAYER_2_SIZE, 0);
		WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adP2Base + adSave1Offset), &S.dwaSave1P2, SAVE_PLAYER_1_SIZE, 0);
		WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adP2Base + adSave2Offset), &S.dwaSave2P2, SAVE_PLAYER_2_SIZE, 0);
		WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adP3Base + adSave1Offset), &S.dwaSave1P3, SAVE_PLAYER_1_SIZE, 0);
		WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adP3Base + adSave2Offset), &S.dwaSave2P3, SAVE_PLAYER_2_SIZE, 0);
		WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adP4Base + adSave1Offset), &S.dwaSave1P4, SAVE_PLAYER_1_SIZE, 0);
		WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adP4Base + adSave2Offset), &S.dwaSave2P4, SAVE_PLAYER_2_SIZE, 0);
	}
}

void SaveStateToFile(int nSaveSlot)
{
	if (nSaveSlot > 0 && nSaveSlot < MAX_SAVES)
	{
		Save& S = Saves[nSaveSlot - 1];
		std::ofstream SaveOutFile;
		SaveOutFile.open("MBAA.save");
		for (int i = 0; i < ADJ_SAVE_EFFECTS_SIZE; i++)
		{
			SaveOutFile << S.dwaSaveEffects[i] << std::endl;
		}
		for (int i = 0; i < ADJ_SAVE_STOP_SITUATION_SIZE; i++)
		{
			SaveOutFile << S.dwaSaveStopSituation[i] << std::endl;
		}
		SaveOutFile << S.dwSaveGlobalFreeze << std::endl;
		for (int i = 0; i < ADJ_SAVE_ATTACK_DISPLAY_INFO_SIZE; i++)
		{
			SaveOutFile << S.dwaSaveAttackDisplayInfo[i] << std::endl;
		}
		for (int i = 0; i < ADJ_SAVE_ATTACK_DISPLAY_INFO_2_SIZE; i++)
		{
			SaveOutFile << S.dwaSaveAttackDisplayInfo2[i] << std::endl;
		}
		SaveOutFile << S.dwSaveDestinationCamX << std::endl;
		SaveOutFile << S.dwSaveCurrentCamX << std::endl;
		SaveOutFile << S.dwSaveCurrentCamXCopy << std::endl;
		SaveOutFile << S.dwSaveDestinationCamY << std::endl;
		SaveOutFile << S.dwSaveCurrentCamY << std::endl;
		SaveOutFile << S.dwSaveCurrentCamYCopy << std::endl;
		SaveOutFile << S.dwSaveCurrentCamZoom << std::endl;
		SaveOutFile << S.dwSaveDestinationCamZoom << std::endl;
		SaveOutFile << S.dwSaveP1ControlledCharacter << std::endl;
		SaveOutFile << S.dwSaveP1NextControlledCharacter << std::endl;
		SaveOutFile << S.dwSaveP2ControlledCharacter << std::endl;
		SaveOutFile << S.dwSaveP2NextControlledCharacter << std::endl;
		for (int i = 0; i < ADJ_SAVE_PLAYER_1_SIZE; i++)
		{
			SaveOutFile << S.dwaSave1P1[i] << std::endl;
		}
		for (int i = 0; i < ADJ_SAVE_PLAYER_2_SIZE; i++)
		{
			SaveOutFile << S.dwaSave2P1[i] << std::endl;
		}
		for (int i = 0; i < ADJ_SAVE_PLAYER_1_SIZE; i++)
		{
			SaveOutFile << S.dwaSave1P2[i] << std::endl;
		}
		for (int i = 0; i < ADJ_SAVE_PLAYER_2_SIZE; i++)
		{
			SaveOutFile << S.dwaSave2P2[i] << std::endl;
		}
		for (int i = 0; i < ADJ_SAVE_PLAYER_1_SIZE; i++)
		{
			SaveOutFile << S.dwaSave1P3[i] << std::endl;
		}
		for (int i = 0; i < ADJ_SAVE_PLAYER_2_SIZE; i++)
		{
			SaveOutFile << S.dwaSave2P3[i] << std::endl;
		}
		for (int i = 0; i < ADJ_SAVE_PLAYER_1_SIZE; i++)
		{
			SaveOutFile << S.dwaSave1P4[i] << std::endl;
		}
		for (int i = 0; i < ADJ_SAVE_PLAYER_2_SIZE; i++)
		{
			SaveOutFile << S.dwaSave2P4[i] << std::endl;
		}
		SaveOutFile.close();
	}
}

void LoadStateFromFile(int nSaveSlot)
{
	if (nSaveSlot > 0 && nSaveSlot < MAX_SAVES)
	{
		Save& S = Saves[nSaveSlot - 1];
		std::ifstream SaveInFile;
		SaveInFile.open("MBAA.save");
		for (int i = 0; i < ADJ_SAVE_EFFECTS_SIZE; i++)
		{
			SaveInFile >> S.dwaSaveEffects[i];
		}
		for (int i = 0; i < ADJ_SAVE_STOP_SITUATION_SIZE; i++)
		{
			SaveInFile >> S.dwaSaveStopSituation[i];
		}
		SaveInFile >> S.dwSaveGlobalFreeze;
		for (int i = 0; i < ADJ_SAVE_ATTACK_DISPLAY_INFO_SIZE; i++)
		{
			SaveInFile >> S.dwaSaveAttackDisplayInfo[i];
		}
		for (int i = 0; i < ADJ_SAVE_ATTACK_DISPLAY_INFO_2_SIZE; i++)
		{
			SaveInFile >> S.dwaSaveAttackDisplayInfo2[i];
		}
		SaveInFile >> S.dwSaveDestinationCamX;
		SaveInFile >> S.dwSaveCurrentCamX;
		SaveInFile >> S.dwSaveCurrentCamXCopy;
		SaveInFile >> S.dwSaveDestinationCamY;
		SaveInFile >> S.dwSaveCurrentCamY;
		SaveInFile >> S.dwSaveCurrentCamYCopy;
		SaveInFile >> S.dwSaveCurrentCamZoom;
		SaveInFile >> S.dwSaveDestinationCamZoom;
		SaveInFile >> S.dwSaveP1ControlledCharacter;
		SaveInFile >> S.dwSaveP1NextControlledCharacter;
		SaveInFile >> S.dwSaveP2ControlledCharacter;
		SaveInFile >> S.dwSaveP2NextControlledCharacter;
		for (int i = 0; i < ADJ_SAVE_PLAYER_1_SIZE; i++)
		{
			SaveInFile >> S.dwaSave1P1[i];
		}
		for (int i = 0; i < ADJ_SAVE_PLAYER_2_SIZE; i++)
		{
			SaveInFile >> S.dwaSave2P1[i];
		}
		for (int i = 0; i < ADJ_SAVE_PLAYER_1_SIZE; i++)
		{
			SaveInFile >> S.dwaSave1P2[i];
		}
		for (int i = 0; i < ADJ_SAVE_PLAYER_2_SIZE; i++)
		{
			SaveInFile >> S.dwaSave2P2[i];
		}
		for (int i = 0; i < ADJ_SAVE_PLAYER_1_SIZE; i++)
		{
			SaveInFile >> S.dwaSave1P3[i];
		}
		for (int i = 0; i < ADJ_SAVE_PLAYER_2_SIZE; i++)
		{
			SaveInFile >> S.dwaSave2P3[i];
		}
		for (int i = 0; i < ADJ_SAVE_PLAYER_1_SIZE; i++)
		{
			SaveInFile >> S.dwaSave1P4[i];
		}
		for (int i = 0; i < ADJ_SAVE_PLAYER_2_SIZE; i++)
		{
			SaveInFile >> S.dwaSave2P4[i];
		}
		S.bSaved = true;
		SaveInFile.close();
	}
}

bool CheckSave(int nSaveSlot)
{
	if (nSaveSlot > 0 && nSaveSlot < MAX_SAVES)
	{
		return Saves[nSaveSlot - 1].bSaved;
	}
	return false;
}

void ClearSave(int nSaveSlot)
{
	if (nSaveSlot > 0 && nSaveSlot < MAX_SAVES)
	{
		Saves[nSaveSlot - 1].bSaved = false;
	}
}

void ClearAllSaves()
{
	for (int i = 0; i < MAX_SAVES; i++)
	{
		Saves[i].bSaved = false;
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

	if (bDoAdvantage && nFrameCount != nLastFrameCount && cGlobalFreeze == 0)
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
	sBarScrolling = 0;
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

	//Bar 1 - General action information
	if (P.nInactionableFrames != 0) //Doing something with limited actionability
	{
		sFont = "\x1b[38;2;255;255;255m\x1b[48;2;65;200;0m";
		sBarValue = std::format("{:2}", P.nInactionableFrames % 100);
		if (P.nPattern >= 35 && P.nPattern <= 37) //Jump Startup
		{
			sFont = "\x1b[38;2;177;177;255m\x1b[48;2;241;224;132m";
		}
		else if (P.nHitstunRemaining != 0 && P.bBlockstunFlag == 0) //Hitstun
		{
			sFont = "\x1b[38;2;255;255;255m\x1b[48;2;140;140;140m";
			if (P.cState_Stance == 1) //Airborne
			{
				if (P.sUntechCounter < P.sUntechTotal) //Still has untech remaining
				{
					sBarValue = std::format("{:2}", (P.sUntechTotal - P.sUntechCounter) % 100);
				}
			}
			else //Grounded
			{
				if (P.nHitstunRemaining > 1) //Still has hitstun remaining
					sBarValue = std::format("{:2}", P.nHitstunRemaining - 1 % 100);
			}
		}
		else if (P.bBlockstunFlag) //Blockstun
		{
			sFont = "\x1b[38;2;255;255;255m\x1b[48;2;180;180;180m";
		}
	}
	else //Fully actionable
	{
		sFont = "\x1b[38;2;92;92;92m\x1b[48;2;0;0;0m";
		sBarValue = std::format("{:2}", P.nPattern % 100);

		if (bDoAdvantage) //Has advantage
		{
			sBarValue = std::format("{:2}", P.nAdvantageCounter % 100);
			if (P.nAdvantageCounter != 0)
			{
				sFont = "\x1b[38;2;255;255;255m\x1b[48;2;0;0;0m";
			}
		}
			
		if (P.nLastInactionableFrames != 0) //Neutral frame
		{
			sFont = "\x1b[38;2;255;255;255m\x1b[48;2;32;90;0m";
		}
	}

	if (P.bThrowFlag != 0) //Being thrown
	{
		sFont = "\x1b[38;2;255;255;255m\x1b[48;2;110;110;110m";
		sBarValue = " t";
	}
	else if (P.cAnimation_BoxIndex == 12) //Clash
	{
		sFont = "\x1b[38;2;255;255;255m\x1b[48;2;225;184;0m";
	}
	else if (P.cAnimation_BoxIndex == 10) //Shield
	{
		sFont = "\x1b[38;2;255;255;255m\x1b[48;2;145;194;255m";
	}
	else if (P.cAnimation_BoxIndex <= 1 || P.sGrantedStrikeInvuln != 0 || P.cState_Invuln == 3) //Various forms of invuln
	{
		if (P.nInactionableFrames != 0) //Doing something with limited actionability
		{
			sFont = "\x1b[38;2;160;160;160m\x1b[48;2;255;255;255m";
		}
		else //Fully actionable
		{
			sFont = "\x1b[38;2;100;100;100m\x1b[48;2;255;255;255m";
		}

	}

	if (cGlobalFreeze != 0 || cP1Freeze != 0 || cP2Freeze != 0) //Screen is frozen
	{
		sFont = "\x1b[38;2;255;255;255m\x1b[48;2;60;60;60m";
	}
	else if (P.cHitstop != 0) //in hitstop
	{
		sFont = "\x1b[38;2;255;255;255m\x1b[48;2;60;80;128m";
	}

	P.sBar1[nBarCounter % BAR_MEMORY_SIZE] = sFont + sBarValue + "\x1b[0m";

	sFont = "\x1b[0m";
	sBarValue = "  ";

	//Bar 2 - Active frames
	if (P.cState_Stance == 1)
	{
		sBarValue = " ^";
	}

	if (P.dwAttackDataPointer != 0)
	{
		sBarValue = std::format("{:2}", P.nActiveCounter % 100);
		if (cGlobalFreeze != 0 || cP1Freeze != 0 || cP2Freeze != 0)
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

		if (P.cState_Stance == 1)
		{
			sFont += "\x1b[4m";
		}
	}

	P.sBar2[nBarCounter % BAR_MEMORY_SIZE] = sFont + sBarValue + "\x1b[0m";

	sFont = "";
	sBarValue = "  ";

	//Bar 3 - Projectile and assist active frames
	if (P.nActiveProjectileCount != 0 || Assist.nActiveProjectileCount != 0)
	{
		sFont = "\x1b[38;2;255;255;255m\x1b[48;2;255;0;0m";
		sBarValue = std::format("{:2}", (P.nActiveProjectileCount + Assist.nActiveProjectileCount) % 100);
	}

	if (Assist.dwAttackDataPointer != 0)
	{
		sBarValue = std::format("{:2}", Assist.nActiveCounter  % 100);
		if (cGlobalFreeze != 0 || cP1Freeze != 0 || cP2Freeze != 0)
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

		if (Assist.cState_Stance == 1)
		{
			sFont += "\x1b[4m";
		}
	}

	P.sBar3[nBarCounter % BAR_MEMORY_SIZE] = sFont + sBarValue + "\x1b[0m";

	sFont = "";
	sBarValue = "  ";

	if (bDisplayInputs)
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

		//Bar 5
		if (P.cRawDirectionalInput == 0)
		{
			sRightValue = ".";
		}
		else if (P.cState_Stance == 1)
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

void HandleInactive(Player& P)
{
	if (P.nInactionableFrames != 0)
	{
		P.nInactiveMemory = P.nInactionableFrames;
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
		P1.nState_Flagset2 != 0 || P2.nState_Flagset2 != 0 ||
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
		if (P1.cHitstop != 0 && P2.cHitstop != 0) //Player hitstop values count down but we need it to count up
		{
			nSharedHitstop++;
		}
		else
		{
			nSharedHitstop = 0;
		}
		
		bool bIsFreeze = (
			cGlobalFreeze != 0 ||
			nSharedHitstop > 1
			);

		if (bDisplayFreeze || !bIsFreeze)
		{
			IncrementActive(P1);
			IncrementActive(P2);
			HandleInactive(P1);
			HandleInactive(P2);
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

	short sAdjustedScroll = min(min(nBarCounter - nBarDisplayRange, BAR_MEMORY_SIZE - nBarDisplayRange), sBarScrolling);

	P1.sBarString1 = "";
	P1.sBarString2 = "";
	P1.sBarString3 = "";
	P1.sBarString4 = "";
	P1.sBarString5 = "";
	for (int i = (nBarCounter % BAR_MEMORY_SIZE) - nBarDisplayRange - sAdjustedScroll; i < (nBarCounter % BAR_MEMORY_SIZE) - sAdjustedScroll; i++)
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
	for (int i = (nBarCounter % BAR_MEMORY_SIZE) - nBarDisplayRange - sAdjustedScroll; i < (nBarCounter % BAR_MEMORY_SIZE) - sAdjustedScroll; i++)
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

	if (bSimpleFrameInfo)
	{
		printf("\x1b[0m" "Total %3i / Advantage %3i / Distance %3i" "\n",
			P1.nInactiveMemory, nPlayerAdvantage, nXPixelDistance);
	}

	if (!bSimpleFrameInfo)
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
	
	if (!bSimpleFrameInfo)
	{
		printf("\x1b[0m" "(%6i, %6i)" "\x1b[7m" "(%4i, %4i)" "\x1b[0m" "adv %3i" "\x1b[K\n",
			nXDistance, nYDistance, nXPixelDistance, nYPixelDistance, nPlayerAdvantage);
	}

	std::cout << sColumnHeader;


	std::cout << P1.sBarString1;
	std::cout << P1.sBarString2;
	std::cout << P1.sBarString3;

	std::cout << P2.sBarString1;
	std::cout << P2.sBarString2;
	std::cout << P2.sBarString3;

	if (!bSimpleFrameInfo)
	{
		printf("\x1b[0m" "ex %2i" "\x1b[7m" "ch %c" "\x1b[0m" "partner %3i [%2i]" "\x1b[7m" "scaling %2i [%i+%i]" "\x1b[0m" "rhp %5i" "\x1b[7m" "gg %5i [%.3f]" "\x1b[0m" "total %3i" "\x1b[0m\x1b[K\n",
			cP1Freeze, cP1CounterHit, P3.nPattern, P3.nState, nP1GravityHits, nP1Gravity % 10, P1.sUntechPenalty % 10, P1.nRedHealth, nP1GuardGauge, P1.fGuardQuality, P1.nInactiveMemory % 1000);
		printf("\x1b[0m" "ex %2i" "\x1b[7m" "ch %c" "\x1b[0m" "partner %3i [%2i]" "\x1b[7m" "scaling %2i [%i+%i]" "\x1b[0m" "rhp %5i" "\x1b[7m" "gg %5i [%.3f]" "\x1b[0m" "total %3i"  "\x1b[0m\x1b[K\n",
			cP2Freeze, cP2CounterHit, P4.nPattern, P4.nState, nP2GravityHits, nP2Gravity % 10, P2.sUntechPenalty % 10, P2.nRedHealth, nP2GuardGauge, P2.fGuardQuality, P2.nInactiveMemory % 1000);
	}

	if (bDisplayInputs)
	{
		std::cout << sColumnHeader;
	}

	if (bDisplayInputs)
	{
		std::cout << P1.sBarString4;
		std::cout << P1.sBarString5;
	}

	if (bDisplayInputs)
	{
		std::cout << P2.sBarString4;
		std::cout << P2.sBarString5;
	}

	std::cout << "\x1b[J";
}

void FrameDisplay(HANDLE hMBAAHandle, DWORD dwBaseAddress, Player& P1, Player& P2, Player& P3, Player& P4)
{
	CheckGameState(hMBAAHandle, dwBaseAddress);

	if (cGameState != 1) //If not in game (any gamemode)
	{
		ClearAllSaves();
		std::cout << "\x1b[J";
		return;
	}

	CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &screenBufferInfo);
	nBarDisplayRange = (screenBufferInfo.srWindow.Right - screenBufferInfo.srWindow.Left) / 2;

	UpdateGlobals(hMBAAHandle, dwBaseAddress);

	bool bDoSave = false;
	bool bDoClearSave = false;
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedDoSave), &bDoSave, 1, 0);
	ReadProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedDoClearSave), &bDoClearSave, 1, 0);
	if (bDoSave)
	{
		SaveState(hMBAAHandle, dwBaseAddress, nSaveSlot);
		char c5 = 5;
		WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adGlobalFreeze), &c5, 1, 0);

		char c0 = 0;
		WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedDoSave), &c0, 1, 0);
	}
	else if (bDoClearSave)
	{
		ClearSave(nSaveSlot);

		char c0 = 0;
		WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedDoClearSave), &c0, 1, 0);
	}
	else if (cP1FN2Input > 0 && bEnableFN2Load && !bInExtendedSettings)
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

		if (nTrueFrameCount == 1 && CheckSave(nSaveSlot) && bEnableFN2Load)
		{
			ResetBars(P1);
			ResetBars(P2);
			ResetBars(P3);
			ResetBars(P4);
			LoadState(hMBAAHandle, dwBaseAddress, nSaveSlot);
		}
	}

	nPlayerAdvantage = (P1.nAdvantageCounter - P2.nAdvantageCounter) % 100;

	PrintFrameDisplay(*Player1, *Player2, *Player3, *Player4);
}
