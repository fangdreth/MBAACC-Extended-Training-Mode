#pragma once
#include <vector>
#include <string>
#include <Windows.h>
#include <map>
#include <unordered_map>
#include <iostream>

enum eMenu { MAIN = 2, BATTLE_SETTINGS = 6, ENEMY_SETTINGS = 7, VIEW_SCREEN = 12, COMMAND_LIST = 13 };
enum eEnemyStatus { STAND, JUMP, CROUCH, CPU, MANUAL, DUMMY };
enum eMagicCircuit { UNLIMITED = 3 };
enum eRecover {};
enum eEnemyOffOnRandom { OFF, ON, RANDOM };
enum eEnemyDefense { NOGUARD, ALLGUARD, STATUSGUARD, ALLSHIELD, STATUSSHIELD, DODGE };
enum eEnemyStance { STANDING = 0, STANDGUARDING = 17, CROUCHING = 13 };
enum ePresetSettings { DEFAULT, FUZZY, BLOCKSTRING, HEATOS, FUZZYMASH, FUZZYJUMP, CUSTOM };
enum eEnemyGuardLevelSettings { INF, ONEHUNDRED, SEVENTYFIVE, FIFTY, TWENTYFIVE, ZERO };
enum ePages { REVERSALS_PAGE = 1, STATS_PAGE = 2, POSITIONS_PAGE = 3, FRAME_TOOL = 4, CHARACTER_SPECIFICS = 5, HIGHLIGHT_PAGE = 6, HOTKEYS_PAGE = 7 };
enum eReversalType { REVERSAL_NORMAL, REVERSAL_RANDOM, /*REVERSAL_SEQUENCE,*/ REVERSAL_REPEAT };
enum eFrameDataDisplay { FRAMEDISPLAY_NORMAL, FRAMEDISPLAY_ADVANCED };
enum eHighlightSettings { NO_HIGHLIGHT, RED_HIGHLIGHT, YELLOW_HIGHLIGHT, GREEN_HIGHLIGHT, BLUE_HIGHLIGHT, PURPLE_HIGHLIGHT, BLACK_HIGHLIGHT };

const std::string GITHUB_LATEST = "https://api.github.com/repos/fangdreth/MBAACC-Extended-Training-Mode/releases/latest";
const std::string GITHUB_RELEASE = "https://github.com/fangdreth/MBAACC-Extended-Training-Mode/releases";
const std::string GITHUB_README = "https://github.com/fangdreth/MBAACC-Extended-Training-Mode/blob/main/README.md";
const std::string VERSION = "v2.0";

const DWORD dwP1WillBlock = 0x1552AC;
const DWORD dwP2WillBlock = 0x1552AC + 0xAFC;
const DWORD dwP2Offset = 0xAFC;
const DWORD dwRoundTime = 0x162A40; //0-inf
const DWORD dwMot = 0x1581CC;   // this one is mysterious.  I think it's an animation counter
const DWORD dwEnemyStatus = 0x37C1E8; //0:STAND 1:JUMP 2:CROUCH 3:CPU 4:MANUAL 5:DUMMY
const DWORD dwEnemyStatusCopy = 0x34D7F8;
const DWORD dwEnemyDefense = 0x37C1F0; //0:OFF 1:ALLGUARD 2:STATUSGUARD 3:ALLSHIELD 4:STATUSSHIELD 5:DODGE
const DWORD dwEnemyDefenseCorrected = 0x155DA8; // set this after setting dwEnemyDefense
const DWORD dwEnemyDefenseType = 0x37C1F4; //0:OFF 1:COMBO 2:RANDOM
const DWORD dwP1GuardAmount = 0x1551F4; //0-1174011904 aka 0.0f-8000.0f
const DWORD dwP2GuardAmount = 0x1551F4 + 0xAFC; //0-1174011904 aka 0.0f-8000.0f
const DWORD dwP1GuardQuality = 0x155208;
const DWORD dwP2GuardQuality = 0x155208 + 0xAFC;
const DWORD dwGuardSetting = 0x37C200; //0:100 1:75 2:50 3:25 4:0
const DWORD dwAirRecovery = 0x37C208;
const DWORD dwDownRecovery = 0x37C20C;
const DWORD dwThrowRecovery = 0x37C214;
const DWORD dwReduceRecovery = 0x37C218;
const DWORD dwExGuard = 0x1551E0; //10:ExGuard
const DWORD dwP1Meter = 0x155210;   // abc.xy truncated Ex. 12345 = 123.4%
const DWORD dwP1Health = 0x1551EC;
const DWORD dwP1RedHealth = 0x1551F0;
const DWORD dwP2Health = 0x1551EC + 0xAFC;
const DWORD dwP2RedHealth = 0x1551F0 + 0xAFC;
const DWORD dwP1SionBullets = 0x1552F6; // counts shots used i.e. 0=13 in mag, 4=9 in mag, etc
const DWORD dwP1RoaVisibleCharge = 0x155302;    // 0-9
const DWORD dwP1RoaHiddenCharge = 0x155300; // 0-9
const DWORD dwP1Y = 0x155248;
const DWORD dwP2Y = 0x155248 + 0xAFC;
const DWORD dwP1X = 0x155238;
const DWORD dwP1Exists = 0x155130;
const DWORD dwP2PatternSet = 0x155F38;
const DWORD dwP1PatternRead = 0x155140;
const DWORD dwP2PatternRead = dwP1PatternRead + 0xAFC;
const DWORD dwCSSFlag = 0x155130;
const DWORD dwP2CharNumber = 0x34D91C;
const DWORD dwP2CharMoon = 0x34D924;
const DWORD dwP1CharMoon = 0x34D900;
const DWORD dwP1CharNumber = 0x34D8F8;
const DWORD dwP1ButtonPress = 0x154654;
const DWORD dwP1APressed = 0x371399;
const DWORD dwP1F1Pressed = 0x37144C;
const DWORD dwP1F2Pressed = 0x37144D;
const DWORD dwP1DPressed = 0x37139C;
const DWORD dwP1DirectionPress = 0x371448;
const DWORD dwPausedFlag = 0x162A64;
const DWORD dwEnemyAction = 0x37C1EC;
const DWORD dwLifeRecover = 0x37C1F8;
const DWORD dwP1HitstunRemaining = 0x1552DC;
const DWORD dwP1NotInCombo = 0x155194; //1:InCombo 0:NotInCombo
const DWORD dwP2NotInCombo = 0x155194 + 0xAFC; //1:InCombo 0:NotInCombo
const DWORD dwP1HitstopRemaining = 0x1552D4; //0:not in hitstop
const DWORD dwComboCount = 0x157E00; // there's another address that tracks this
const DWORD dwP2E = 0x3713C9;
const DWORD dwMagicCircuitSetting = 0x37C1FC;
const DWORD dwBasePointer = 0x34D7FC;
const DWORD dwBurstCooldown = 0x155DBE;
const DWORD dwP1ArmorTimer = 0x1552C8; // 0:no armor >0:armor
const DWORD dwP2ArmorTimer = 0x1552C8 + 0xAFC;
const DWORD dwP2RecievedHitstop = 0x155DD0;
const DWORD dwGameMode = 0x162A74; // 16:training
const DWORD dwFPS = 0x374A70;
const DWORD dwP1Color = 0x34D83C;
const DWORD dwP2Color = 0x34D868;
const DWORD dwP1Struct = 0x155130;
const DWORD dwP2Struct = dwP1Struct + 0xAFC;
const DWORD dwP1RyougiKnife = 0x1552E8; //0=knife 1=no knife
const DWORD dwP1FMaidsHearts = 0x1552FC; //0=full 5=empty
const DWORD dwP1Blocking = 0x1552AB;
const DWORD dwP2Blocking = dwP1Blocking + 0xAFC;
const DWORD dwGlobalExFlash = 0x162A48;
const DWORD dwP1ExFlash = 0x158908;
const DWORD dwP2ExFlash = 0x158C14;
const DWORD dwFrameTimer = 0x15D1CC;
const DWORD dwGameState = 0x14EEE8; //1=training
const DWORD dwP1ThrowInvuln = 0x1552B6; //0=no >0=invuln
const DWORD dwP2ThrowInvuln = 0x1552B6 + 0xAFC; //0=no >0=invuln

// DLL Constants
const DWORD dwCameraX = 0x0055dec4;
const DWORD dwCameraY = 0x0055dec8;
const DWORD dwCameraZoom = 0x0054eb70;
const DWORD dwWindowWidth = 0x0054d048;
const DWORD dwWindowHeight = 0x0054d04c;
const DWORD dwP1AnimationPtr = dwP1Struct + 0x320;
const DWORD dwP2AnimationPtr = dwP2Struct + 0x320;

//FrameDisplay Constants
typedef DWORD ADDRESS; //I think doing this + adXxxxYyyy looks nice

const int BAR_MEMORY_SIZE = 400; //Number of frames stored before overriding (FIFO). Only used in determining how far back you can scroll.
const int BAR_INTERVAL = 20; //Number of frames of only blanks before the bar stops and resets on the next non-blank frame
const int MAX_SAVES = 3;

const char REVERSE_INPUT_MAP[10] = { 0, 3, 2, 1, 6, 0, 4, 9, 8, 7 };
const char CH_MAP[3] = { ' ', 'H', 'L' };

const DWORD dwPlayerStructSize = 0xAFC;
const ADDRESS adP1Base = 0x155130; //0x155130
const ADDRESS adP2Base = 0x155130 + dwPlayerStructSize; //0x155C2C
const ADDRESS adP3Base = 0x155130 + 2 * dwPlayerStructSize; //0x156728
const ADDRESS adP4Base = 0x155130 + 3 * dwPlayerStructSize; //0x157224

const ADDRESS adP1ControlledCharacter = 0x157DB8;
const ADDRESS adP2ControlledCharacter = 0x157FC4;
const ADDRESS adP1TagFlag = 0x155130 + 0x178; //0x1552A8
const ADDRESS adP2TagFlag = 0x155130 + 0x178 + dwPlayerStructSize; //0x155DA4
const ADDRESS adP3TagFlag = 0x155130 + 0x178 + 2 * dwPlayerStructSize; //0x1568A0
const ADDRESS adP4TagFlag = 0x155130 + 0x178 + 3 * dwPlayerStructSize; //0x15739C

const DWORD dwProjectileStructSize = 0x33C;
const ADDRESS adProjectileBase = 0x27BDE8;

//FrameBar Constants
const ADDRESS adMBAABase = 0x400000;

const ADDRESS adPattern = 0x10;
const ADDRESS adNotInCombo = 0x64;
const ADDRESS adPlayerFrameCount = 0xF0;
const ADDRESS adHitstop = 0x172;
const ADDRESS adBlockstunFlag = 0x17B;
const ADDRESS adThrowFlag = 0x176;
const ADDRESS adTagFlag = 0x178;
const ADDRESS adStrikeInvuln = 0x185;
const ADDRESS adUntechTotal = 0x18E;
const ADDRESS adUntechElapsed = 0x190;
const ADDRESS adHitstunRemaining = 0x1AC;
const ADDRESS adRawInput = 0x2EB;
const ADDRESS adButtonInput = 0x2ED;
const ADDRESS adMacroInput = 0x2EE;
const ADDRESS adOnRightFlag = 0x315;
const ADDRESS adAnimationDataPointer = 0x320;
const ADDRESS adAnimStateDataPointer = 0x38;
const ADDRESS adStateStance = 0xC;
const ADDRESS adStateInvuln = 0xD;
const ADDRESS adStateFlagset2 = 0x18;
const ADDRESS adAnimBoxIndex = 0x42;
const ADDRESS adAttackDataPointer = 0x324;

const ADDRESS adP1Inaction = 0x157FC0;
const ADDRESS adP2Inaction = 0x1581CC;

const ADDRESS adP1Freeze = 0x158908;
const ADDRESS adP2Freeze = 0x158C14;

const ADDRESS adFrameCount = 0x15D1CC;
const ADDRESS adTrueFrameCount = 0x162A40;

const ADDRESS adGlobalFreeze = 0x162A48;

const ADDRESS adProjSource = 0x8;
const ADDRESS adProjOwner = 0x2F4;

// addresses for fonts
const ADDRESS adFont0 = 0x55D680;
const ADDRESS adFont1 = 0x55D260;
const ADDRESS adFont2 = 0x55DAA0;

// integer representations of raw float values
// not interested in messing with converting them when a table is good enough
const std::vector<int> vGuardLevelLookupTable =
{
    1174011904/*C 100*/, 1169915904/*C 75*/, 1165623296/*C 50*/, 1157234688/*C 25*/, 0/*C 0*/,
    1171963904/*F 100*/, 1168379904/*F 75*/, 1163575296/*F 50*/, 1155186688/*F 25*/, 0/*F 0*/,
    1176768512/*H 100*/, 1173755904/*H 75*/, 1168379904/*H 50*/, 1159991296/*H 25*/, 0/*H 0*/
};

const int MAX_REVERSAL_DELAY = 99;
const int MAX_HEALTH = 11400;
const int MAX_METER = 30000;
const int MAX_PAGES = 7;
const int MAX_BULLETS = 13; //14:normal 15:infinite
const int MAX_CHARGE = 9;
const int MAX_HEARTS = 5; //6:normal 7:infinite
const int MIN_X = -65536;
const int MAX_X = 65536;
const int P1_DEFAULT_X = -16384;
const int P2_DEFAULT_X = 16384;
const int P3_DEFAULT_X = -29184;
const int P4_DEFAULT_X = 29184;
const int OFFSCREEN_LOCATION = 0x10000000;
const int ONSCREEN_LOCATION = 0x000000A0;
const int TOO_HIGH_TO_BURST = 10000;
const int MAX_BURST = 99;

const char pcSionBullets_13[13] = "SION BULLETS";
const char pcRoaVisibleCharge_19[19] = "ROA VISIBLE CHARGE";
const char pcRoaHiddenCharge_19[19] = "ROA HIDDEN CHARGE";
const char pcFMaidsHearts_15[15] = "F-MAIDS HEARTS";
const char pcRyougiKnife_13[13] = "RYOUGI KNIFE";
const char pcP1ControlledChar_19[19] = "P1 CONTROLLED CHAR";
const char pcP2ControlledChar_19[19] = "P2 CONTROLLED CHAR";
const char pcHitsUntilBurst_17[17] = "HITS UNTIL BURST";
const char pcNormal_7[7] = "NORMAL";
const char pcRepeat_7[7] = "REPEAT";
const char pcRandom_7[7] = "RANDOM";
const char pcInfinite_10[10] = "INFINITE";
const char pcOne_2[2] = "1";
const char pcMain_5[5] = "MAIN";
const char pcAssist_7[7] = "ASSIST";
const char pcHisui_6[6] = "HISUI";
const char pcKohaku_8[8] = "KOHAKU";

const char pcASlow_12[12] = "[A]->SLOWER";
const char pcP1XLoc_11[11] = "P1 X-LOC";
const char pcP2XLoc_11[11] = "P2 X-LOC";
const char pcAssistLoc_13[13] = "ASSIST X-LOC";
const char pcInvert_7[7] = "INVERT";
const char pcUnstable_11[11] = "(UNSTABLE)";

const char pcReversalSlot1_16[16] = "REVERSAL SLOT 1";
const char pcReversalSlot2_16[16] = "REVERSAL SLOT 2";
const char pcReversalSlot3_16[16] = "REVERSAL SLOT 3";
const char pcReversalSlot4_16[16] = "REVERSAL SLOT 4";
const char pcReversalType_14[14] = "REVERSAL TYPE";
const char pcExGuard_9[9] = "EX GUARD";
const char pcReversalDelay_15[15] = "REVERSAL DELAY";
const char pcMeter_18[18] = "METER [A]->SLOWER";
const char pcHealth_19[19] = "HEALTH [A]->SLOWER";
const char pcGuardBar_10[10] = "GUARD BAR";
const char pcBlank_1[1] = "";

const std::vector<const char*> vHighlightNames = {    "OFF",
                                                "RED",
                                                "YELLOW",
                                                "GREEN",
                                                "BLUE",
                                                "PURPLE",
                                                "BLACK"};

const char pcEnemyReversal_15[15] = "ENEMY REVERSAL";

const char pcRecover_8[8] = "RECOVER";
const char pcRecover_11[11] = "NO RECOVER";

const char pcTrainingPreset_17[17] = "training preset.";
const char pcExtendedSettings_18[18] = "EXTENDED SETTINGS";

const char pcLatestVersion_19[19] = "LATEST VERSION";
const char pcOffline_8[8] = "OFFLINE";

const char pcFrameData_11[11] = "FRAME DATA";
const char pcSaveStateSlot_16[16] = "SAVE STATE SLOT";
const char pcSaveState_11[11] = "SAVE STATE";
const char pcDisplayFreeze_16[16] = "DISPLAY FREEZE";
const char pcDisplayInputs_15[15] = "DISPLAY INPUTS";
const char pcScrollDisplay_15[15] = "SCROLL DISPLAY";
const char pcAdvanced_9[9] = "ADVANCED";
const char pcOff_4[4] = "OFF";
const char pcOn_3[3] = "ON";
const char pcNone_5[5] = "NONE";

const char pcIdle_5[5] = "IDLE";
const char pcBlock_6[6] = "BLOCK";
const char pcHit_4[4] = "HIT";
const char pcArmor_6[6] = "ARMOR";
const char pcThrowProtection_17[17] = "THROW PROTECTION";
const char pcRed_4[4] = "RED";
const char pcGreen_6[6] = "GREEN";
const char pcBlue_5[5] = "BLUE";

const char pcFreeze_11[11] = "FREEZE [A]";
const char pcNextStep_14[14] = "NEXT STEP [A]";
const char pcHitboxes_13[13] = "HITBOXES [A]";
const char pcFrameDisplay_18[18] = "FRAME DISPLAY [A]";
const char pcHighlights_15[15] = "HIGHLIGHTS [A]";
const char pcSaveState_15[15] = "SAVE STATE [A]";

#define VK_KEY_0 0x30
#define VK_KEY_1 0x31
#define VK_KEY_2 0x32
#define VK_KEY_3 0x33
#define VK_KEY_4 0x34
#define VK_KEY_5 0x35
#define VK_KEY_6 0x36
#define VK_KEY_7 0x37
#define VK_KEY_8 0x38
#define VK_KEY_9 0x39

#define VK_KEY_A 0x41
#define VK_KEY_B 0x42
#define VK_KEY_C 0x43
#define VK_KEY_D 0x44
#define VK_KEY_E 0x45
#define VK_KEY_F 0x46
#define VK_KEY_G 0x47
#define VK_KEY_H 0x48
#define VK_KEY_I 0x49
#define VK_KEY_J 0x4A
#define VK_KEY_K 0x4B
#define VK_KEY_L 0x4C
#define VK_KEY_M 0x4D
#define VK_KEY_N 0x4E
#define VK_KEY_O 0x4F
#define VK_KEY_P 0x50
#define VK_KEY_Q 0x51
#define VK_KEY_R 0x52
#define VK_KEY_S 0x53
#define VK_KEY_T 0x54
#define VK_KEY_U 0x55
#define VK_KEY_V 0x56
#define VK_KEY_W 0x57
#define VK_KEY_X 0x58
#define VK_KEY_Y 0x59
#define VK_KEY_Z 0x5A

const int nDefaultFreezeKey = VK_KEY_1;
const int nDefaultFrameStepKey = VK_KEY_2;
const int nDefaultHitboxDisplayKey = VK_KEY_3;
const int nDefaultFrameDataDisplayKey = VK_KEY_4;
const int nDefaultHighlightsOnKey = VK_KEY_5;
const int nDefaultSaveStateKey = VK_KEY_6;

class KeyState
{
public:
	// please tell me if my use of classes here is overkill

	KeyState()
	{
		vKey = 0x00;
	}

	void setKey(int vKey_)
	{
		if (vKey != vKey_)
		{
			// reset the key state here to prevent it from firing
			// when it gets changed
			prevState = true;
			vKey = vKey_;
		}
	}

	bool isFocused() {
		HWND hActiveWindow = GetActiveWindow();
		HWND hForegroundWindow = GetForegroundWindow();
		return hActiveWindow == hForegroundWindow;
	}

	bool keyHeld() {
		return (GetAsyncKeyState(vKey) & 0x8000) ? true : false;
	}

	bool keyDown() {
		bool tempState = (GetAsyncKeyState(vKey) & 0x8000) ? true : false;
		bool res = false;
		if (prevState != tempState && tempState)
		{
			res = true;
		}
		prevState = tempState;

		//if (!isFocused()) {
			//return false;
		//}

		return res;
	}

private:
	int vKey;
	bool prevState = false;
};