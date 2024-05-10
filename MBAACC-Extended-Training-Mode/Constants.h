#pragma once
#include <vector>
#include <string>
#include <Windows.h>
#include <map>

enum eMenu { MAIN = 2, BATTLE_SETTINGS = 6, ENEMY_SETTINGS = 7, VIEW_SCREEN = 12 };
enum eEnemyStatus { STAND, JUMP, CROUCH, CPU, MANUAL, DUMMY };
enum eMagicCircuit { UNLIMITED = 3 };
enum eRecover {};
enum eEnemyOffOnRandom { OFF, ON, RANDOM };
enum eEnemyDefense { NOGUARD, ALLGUARD, STATUSGUARD, ALLSHIELD, STATUSSHIELD, DODGE };
enum eEnemyStance { STANDING = 0, STANDGUARDING = 17, CROUCHING = 13 };
enum ePresetSettings { DEFAULT, FUZZY, BLOCKSTRING, HEATOS, FUZZYMASH, FUZZYJUMP, CUSTOM };
enum eEnemyGuardLevelSettings { INF, ONEHUNDRED, SEVENTYFIVE, FIFTY, TWENTYFIVE, ZERO };
enum ePages { REVERSALS_PAGE = 1, STATS_PAGE = 2, POSITIONS_PAGE = 3, CHARACTER_SPECIFICS = 4 };
enum eReversalType { REVERSAL_NORMAL, REVERSAL_RANDOM, REVERSAL_REPEAT };

const std::string GITHUB_LATEST = "https://api.github.com/repos/fangdreth/MBAACC-Extended-Training-Mode/releases/latest";
const std::string GITHUB_RELEASE = "https://github.com/fangdreth/MBAACC-Extended-Training-Mode/releases";
const std::string GITHUB_README = "https://github.com/fangdreth/MBAACC-Extended-Training-Mode/blob/main/README.md";
const std::string VERSION = "v1.3b";

const DWORD dwP2Offset = 0xAFC;
const DWORD dwRoundTime = 0x162A40; //0-inf
const DWORD dwMot = 0x1581CC;   // this one is mysterious.  I think it's an animation counter
const DWORD dwPlayerState = 0x155140;  //0:STAND 13:CROUCH 17:STANDGUARDING 12:STAND->CROUCH ETC
const DWORD dwEnemyStatus = 0x37C1E8; //0:STAND 1:JUMP 2:CROUCH 3:CPU 4:MANUAL 5:DUMMY
const DWORD dwEnemyDefense = 0x37C1F0; //0:OFF 1:ALLGUARD 2:STATUSGUARD 3:ALLSHIELD 4:STATUSSHIELD 5:DODGE
const DWORD dwEnemyDefenseType = 0x37C1F4; //0:OFF 1:COMBO 2:RANDOM
const DWORD dwP1GuardAmount = 0x1551F4; //0-1174011904 aka 0.0f-8000.0f
const DWORD dwGuardSetting = 0x37C200; //0:100 1:75 2:50 3:25 4:0
const DWORD dwAirRecovery = 0x37C208;
const DWORD dwDownRecovery = 0x37C20C;
const DWORD dwThrowRecovery = 0x37C214;
const DWORD dwReduceRecovery = 0x37C218;
const DWORD dwExGuard = 0x1551E0; //10:ExGuard
const DWORD dwP1Meter = 0x155210;   // abc.xy truncated Ex. 12345 = 123.4%
const DWORD dwP1Health = 0x1551EC;
const DWORD dwP1RedHealth = 0x1551F0;
const DWORD dwP1SionBullets = 0x1552F6; // counts shots used i.e. 0=13 in mag, 4=9 in mag, etc
const DWORD dwP1RoaVisibleCharge = 0x155302;    // 0-9
const DWORD dwP1RoaHiddenCharge = 0x155300; // 0-9
const DWORD dwP2Y = 0x155248;
const DWORD dwP1X = 0x155238;
const DWORD dwP1Exists = 0x155130;
const DWORD dwP2PatternSet = 0x155F38;
const DWORD dwP2PatternRead = 0x155C3C;
const DWORD dwCSSFlag = 0x155130;
const DWORD dwP2CharNumber = 0x34D91C;
const DWORD dwP2CharMoon = 0x34D924;
const DWORD dwP1CharMoon = 0x34D900;
const DWORD dwP1CharNumber = 0x34D8F8;
const DWORD dwP1ButtonPress = 0x154654;
const DWORD dwP1APressed = 0x371399;
const DWORD dwP1DPressed = 0x37139C;
const DWORD dwP1DirectionPress = 0x371448;
const DWORD dwPausedFlag = 0x162A64;
const DWORD dwEnemyAction = 0x37C1EC;
const DWORD dwLifeRecover = 0x37C1F8;
const DWORD dwP1HitstunRemaining = 0x1552DC;
const DWORD dwP1NotInCombo = 0x155194; //1:InCombo 0:NotInCombo
const DWORD dwP1HitstopRemaining = 0x1552D4; //0:not in hitstop
const DWORD dwComboCount = 0x157E00; // there's another address that tracks this
const DWORD dwP2E = 0x3713C9;
const DWORD dwMagicCircuitSetting = 0x37C1FC;
const DWORD dwBasePointer = 0x34D7FC;
const DWORD dwBurstCooldown = 0x155DBE;
const DWORD dwP2RecievedHitstop = 0x155DD0;
const DWORD dwGameMode = 0x37BF2C; // 0:training

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
const int MAX_PAGES = 4;
const int MAX_BULLETS = 13; //14:normal 15:infinite
const int MAX_CHARGE = 9;
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
const char pcHitsUntilBurst_17[17] = "HITS UNTIL BURST";
const char pcNormal_7[7] = "NORMAL";
const char pcInfinite_10[10] = "INFINITE";
const char pcOne_2[2] = "1";

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

const char pcEnemyReversal_15[15] = "ENEMY REVERSAL";

const char pcRecover_8[8] = "RECOVER";
const char pcRecover_11[11] = "NO RECOVER";

const char pcTrainingPreset_17[17] = "training preset.";
const char pcExtendedSettings_18[18] = "EXTENDED SETTINGS";

const char pcLatestVersion_19[19] = "LATEST VERSION";
const char pcOffline_8[8] = "OFFLINE";