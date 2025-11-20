#pragma once

//#include <ws2tcpip.h>
//#include <winsock2.h>
#include <Windows.h>

//#include "../MBAACC-Extended-Training-Mode/Logger.h"

#include <string>

#if !defined(DWORD)
//typedef unsigned DWORD;
//static_assert(sizeof(DWORD) == 4, "what are you doing maddy");
#endif 


//void LogInfo(const std::string& sInfo);
//void LogError(const std::string& sError);

#pragma comment(lib, "ws2_32.lib") 
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dxerr.lib")
#pragma comment(lib, "legacy_stdio_definitions.lib")
#pragma comment(lib, "XInput.lib")

extern DWORD __frameDoneCount;

typedef DWORD ADDRESS;

enum eMenu { MAIN = 2, BATTLE_SETTINGS = 6, ENEMY_SETTINGS = 7, VIEW_SCREEN = 12, COMMAND_LIST = 13, TRAINING_DISPLAY = 8 };
enum eEnemyStatus { STAND, JUMP, CROUCH, CPU, MANUAL, DUMMY };
enum eMagicCircuit { UNLIMITED = 3 };
enum eRecover {};
enum eEnemyOffOnRandom { OFF, ON, RANDOM };
enum eEnemyDefense { NOGUARD, ALLGUARD, STATUSGUARD, ALLSHIELD, STATUSSHIELD, DODGE };
enum eEnemyStance { STANDING = 0, STANDGUARDING = 17, CROUCHING = 13 };
enum ePresetSettings { DEFAULT, FUZZY, BLOCKSTRING, HEATOS, FUZZYMASH, FUZZYJUMP, CUSTOM };
enum eEnemyGuardLevelSettings { INF, ONEHUNDRED, SEVENTYFIVE, FIFTY, TWENTYFIVE, ZERO };
enum eSettingsPages { REVERSALS_PAGE = 1, STATS_PAGE, HIGHLIGHT_PAGE, POSITIONS_PAGE, CHARACTER_SPECIFICS, HITBOXES_PAGE, SAVE_STATE_PAGE, FRAME_TOOL, RNG_PAGE, UI_PAGE, SYSTEM_PAGE };
enum eHotkeyPages { GENERIC_HOTKEYS_PAGE = 1, FRAME_TOOL_HOTKEYS_PAGE, RNG_HOTKEYS_PAGE, REVERSALS_HOTKEY_PAGE };
enum eReversalType { REVERSAL_OFF, REVERSAL_NORMAL, REVERSAL_RANDOM, REVERSAL_SHIELD, /*REVERSAL_SEQUENCE,*/ REVERSAL_REPEAT };
enum eFrameDataDisplay { FRAMEDISPLAY_NORMAL, FRAMEDISPLAY_ADVANCED };
enum eHighlightSettings { NO_HIGHLIGHT, RED_HIGHLIGHT, YELLOW_HIGHLIGHT, GREEN_HIGHLIGHT, BLUE_HIGHLIGHT, PURPLE_HIGHLIGHT, BLACK_HIGHLIGHT };
enum eRNGMode { RNG_OFF, RNG_SEED, RNG_RN };
enum eRNGRate { RNG_EVERY_FRAME, RNG_EVERY_RESET };
enum eHitboxStyle { HITBOX_DRAW_ALL = 0, HITBOX_BLEND = 1 };
enum eBackground { BG_NORMAL, BG_WHITE, BG_GRAY, BG_BLACK, BG_RED, BG_YELLOW, BG_GREEN, BG_BLUE, BG_PURPLE };
enum eSlow { SLOW_THREE_FOURTHS = 3, SLOW_ONE_HALF = 2, SLOW_ONE_FOURTH = 1 };
enum eInputDisplay { INPUT_OFF = 0, INPUT_LIST = 1, INPUT_ARCADE = 2, INPUT_BOTH = 3 };
enum class eCharID {
	SION,
	ARC,
	CIEL,
	AKIHA,
	MAIDS,
	HISUI,
	KOHAKU,
	TOHNO,
	MIYAKO,
	WARA,
	NERO,
	VSION,
	WARC,
	VAKIHA,
	MECH,
	NANAYA,
	GAKIHA,
	SATSUKI,
	LEN,
	PCIEL,
	NECO,
	AOKO = 22,
	WLEN,
	NAC = 25,
	GCHAOS = 27,
	KOUMA,
	SEI,
	RIES,
	ROA,
	HERMES,
	RYOUGI,
	NECOMECH,
	KOHAMECH,
	HIME = 51
};

const std::string VERSION = "v2.1";
const std::string GITHUB_LATEST = "https://api.github.com/repos/fangdreth/MBAACC-Extended-Training-Mode/releases/latest";
const std::string GITHUB_DOWNLOAD = "https://github.com/fangdreth/MBAACC-Extended-Training-Mode/releases/download/";
const std::string GITHUB_RELEASE = "https://github.com/fangdreth/MBAACC-Extended-Training-Mode/releases";
const std::string EXE_NAME = "MBAACC-Extended-Training-Mode.exe";
const std::string DLL_NAME = "Extended-Training-Mode-DLL.dll";
const std::string UPDATER_NAME = "MBAACC-Extended-Training-Mode-Updater.exe";

const DWORD dwP1WillBlock = 0x1552AC;
const DWORD dwP2WillBlock = 0x1552AC + 0xAFC;
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
const DWORD dwP1ExGuard = 0x1551E0; //10:ExGuard
const DWORD dwP2ExGuard = 0x1551E0 + 0xAFC; //10:ExGuard
const DWORD dwP1Meter = 0x155210;   // abc.xy truncated Ex. 12345 = 123.4%
const DWORD dwP1Health = 0x1551EC;
const DWORD dwP1RedHealth = 0x1551F0;
const DWORD dwP2Meter = 0x155210 + 0xAFC;
const DWORD dwP2Health = 0x1551EC + 0xAFC;
const DWORD dwP2RedHealth = 0x1551F0 + 0xAFC;
const DWORD dwP1SionBullets = 0x1552F6; // counts shots used i.e. 0=13 in mag, 4=9 in mag, etc
const DWORD dwP2SionBullets = 0x1552F6 + 0xAFC; // counts shots used i.e. 0=13 in mag, 4=9 in mag, etc
const DWORD dwP1RoaVisibleCharge = 0x155302;    // 0-9
const DWORD dwP2RoaVisibleCharge = 0x155302 + 0xAFC;    // 0-9
const DWORD dwP1RoaHiddenCharge = 0x155300; // 0-9
const DWORD dwP2RoaHiddenCharge = 0x155300 + 0xAFC; // 0-9
const DWORD dwP1Y = 0x155248;
const DWORD dwP2Y = 0x155248 + 0xAFC;
const DWORD dwP1X = 0x155238;
const DWORD dwP2X = 0x155238 + 0xAFC;
const DWORD dwP3X = 0x155238 + 0xAFC * 2;
const DWORD dwP4X = 0x155238 + 0xAFC * 3;
const DWORD dwP1Exists = 0x155130;
const DWORD dwP2Exists = 0x155130 + 0xAFC;
const DWORD dwP3Exists = 0x155130 + 0xAFC * 2;
const DWORD dwP4Exists = 0x155130 + 0xAFC * 3;
const DWORD dwP2PatternSet = 0x155F38;
const DWORD dwP1PatternRead = 0x155140;
const DWORD dwP4PatternSet = 0x157234;
const DWORD dwP2PatternRead = dwP1PatternRead + 0xAFC;
const DWORD dwCSSFlag = 0x155130;
const DWORD dwP2CharNumber = 0x34D91C;
const DWORD dwP2CharMoon = 0x34D924;
const DWORD dwP1CharMoon = 0x34D900;
const DWORD dwP1CharNumber = 0x34D8F8;
const DWORD dwP1ButtonPress = 0x154654;
const DWORD dwP1APressed = 0x371399;
const DWORD dwP1BPressed = 0x37139A;
const DWORD dwP1F1Pressed = 0x37144C;
const DWORD dwP1F2Pressed = 0x37144D;
const DWORD dwP1DPressed = 0x37139C;
const DWORD dwP1DirectionInput = 0x371398;
const DWORD dwPausedFlag = 0x162A64;
const DWORD dwEnemyAction = 0x37C1EC;
const DWORD dwLifeRecover = 0x37C1F8;
const DWORD dwP1InputEvent = 0x15543C;
const DWORD dwP2InputEvent = dwP1InputEvent + 0xAFC;
const DWORD dwP1ShieldType = 0x1552A0;
const DWORD dwP2ShieldType = dwP1ShieldType + 0xAFC;
const DWORD dwP1HitstunRemaining = 0x1552DC;
const DWORD dwP2HitstunRemaining = 0x1552DC + 0xAFC;
const DWORD dwP1NotInCombo = 0x155194; //1:InCombo 0:NotInCombo
const DWORD dwP2NotInCombo = 0x155194 + 0xAFC; //1:InCombo 0:NotInCombo
const DWORD dwP1HitstopRemaining = 0x1552D4; //0:not in hitstop
const DWORD dwP2HitstopRemaining = 0x1552D4 + 0xAFC; //0:not in hitstop
const DWORD dwComboCount = 0x157E00; // there's another address that tracks this
const DWORD dwP2E = 0x3713C9;
const DWORD dwMagicCircuitSetting = 0x37C1FC;
const DWORD dwBasePointer = 0x34D7FC;
const DWORD dwBurstCooldown = 0x155DBE;
const DWORD dwP1ArmorTimer = 0x1552C8; // 0:no armor >0:armor
const DWORD dwP2ArmorTimer = 0x1552C8 + 0xAFC;
const DWORD dwP2RecievedHitstop = 0x155DD0;
const DWORD dwGameMode = 0x162A74; // 16:training
const DWORD dwVersusCheck = 0x37BF2C; //1 = versus mode (0 is training, 2 is replay/training dummy playback)
const DWORD dwDrawFPS = 0x154128; // 0:off 1:on
const DWORD dwFPS = 0x374A70;
const DWORD dwP1Color = 0x34D83C;
const DWORD dwP2Color = 0x34D868;
const DWORD dwP1Struct = 0x155130;
const DWORD dwP2Struct = dwP1Struct + 0xAFC;
const DWORD dwP1RyougiKnife = 0x1552E8; //0=knife 1=no knife
const DWORD dwP2RyougiKnife = 0x1552E8 + 0xAFC; //0=knife 1=no knife
const DWORD dwP1FMaidsHisuiLeadHearts = 0x1552FC; //0=full 5=empty
const DWORD dwP1FMaidsKohaLeadHearts = 0x1568F6; //0=full 5=empty
const DWORD dwP2FMaidsHisuiLeadHearts = 0x1552FC + 0xAFC; //0=full 5=empty
const DWORD dwP2FMaidsKohaLeadHearts = 0x1568F6 + 0xAFC; //0=full 5=empty
const DWORD dwP1Blocking = 0x1552AB;
const DWORD dwP2Blocking = dwP1Blocking + 0xAFC;
const DWORD dwGlobalExFlash = 0x162A48;
const DWORD dwP1ExFlash = 0x158908;
const DWORD dwP2ExFlash = 0x158C14;
const DWORD dwFrameTimer = 0x15D1CC;
const DWORD dwGameState = 0x14EEE8; //1=training
const DWORD dwP1ThrowInvuln = 0x1552B6; //0=no >0=invuln
const DWORD dwP2ThrowInvuln = 0x1552B6 + 0xAFC; //0=no >0=invuln
const DWORD dwAttackInfo = 0x1595B8;
const DWORD dwInputDisplay = 0x1585F8;
const DWORD dwP1ButtonHeld = 0x15541D; // 16:A 32:B 64:C 112:ABC
const DWORD dwSomeDummyPlaybackFlag = 0x37BF28;
const DWORD dwSomeDummyPlaybackFlag2 = 0x37BF2C;
const DWORD dwTrainingMenu = 0x34D7FC;

// DLL Constants
const DWORD dwCameraX = 0x0055dec4;
const DWORD dwCameraY = 0x0055dec8;
const DWORD dwCameraZoom = 0x0054eb70;
const DWORD dwWindowWidth = 0x0054d048;
const DWORD dwWindowHeight = 0x0054d04c;
const DWORD dwP1AnimationPtr = dwP1Struct + 0x320;
const DWORD dwP2AnimationPtr = dwP2Struct + 0x320;

//FrameDisplay Constants

const int BAR_MEMORY_SIZE = 400; //Number of frames stored before overriding (FIFO). Only used in determining how far back you can scroll.
const int BAR_INTERVAL = 20; //Number of frames of only blanks before the bar stops and resets on the next non-blank frame
const int MAX_SAVES = 3;
const int SAVE_RESET_TIME = 60; //Frames of holding Save State Hotkey before clearing that save
const char TEXT_TIMER = 40; //How many frames Save State popup text stays on screen

const char REVERSE_INPUT_MAP[10] = { 0, 3, 2, 1, 6, 0, 4, 9, 8, 7 };
const char CH_MAP[3] = { ' ', 'H', 'L' };

// Foreground color -> \x1b[38;2;R;G;Bm
// Background color -> \x1b[48;2;R;G;Bm
const std::string FD_CLEAR = "\x1b[0m";
const std::string FD_ITALICS = "\x1b[3m";
const std::string FD_UNDERLINE = "\x1b[4m";
const std::string FD_INACTIONABLE = "\x1b[38;2;255;255;255m\x1b[48;2;65;200;0m";
const std::string FD_JUMP = "\x1b[38;2;177;177;255m\x1b[48;2;241;224;132m";
const std::string FD_HITSTUN = "\x1b[38;2;255;255;255m\x1b[48;2;140;140;140m";
const std::string FD_BLOCKSTUN = "\x1b[38;2;255;255;255m\x1b[48;2;180;180;180m";
const std::string FD_ACTIONABLE = "\x1b[38;2;92;92;92m\x1b[48;2;0;0;0m";
const std::string FD_ADVANTAGE = "\x1b[38;2;255;255;255m\x1b[48;2;0;0;0m";
const std::string FD_NEUTRAL = "\x1b[38;2;255;255;255m\x1b[48;2;32;90;0m";
const std::string FD_THROW_ACTIVE = "\x1b[38;2;255;255;255m\x1b[48;2;192;0;128m";
const std::string FD_THROWN = "\x1b[38;2;255;255;255m\x1b[48;2;110;110;110m";
const std::string FD_CLASH = "\x1b[38;2;255;255;255m\x1b[48;2;225;184;0m";
const std::string FD_SHIELD = "\x1b[38;2;255;255;255m\x1b[48;2;145;194;255m";
const std::string FD_INACTIONABLE_INVULN = "\x1b[38;2;160;160;160m\x1b[48;2;255;255;255m";
const std::string FD_ACTIONABLE_INVULN = "\x1b[38;2;100;100;100m\x1b[48;2;255;255;255m";
const std::string FD_FREEZE = "\x1b[38;2;255;255;255m\x1b[48;2;60;60;60m";
const std::string FD_HITSTOP = "\x1b[38;2;255;255;255m\x1b[48;2;60;80;128m";
const std::string FD_ACTIVE = "\x1b[38;2;255;255;255m\x1b[48;2;255;0;0m";
const std::string FD_ASSIST_ACTIVE = "\x1b[38;2;255;255;255m\x1b[48;2;255;128;0m";
const std::string FD_BUTTON_PRESSED = "\x1b[38;2;255;255;255m\x1b[48;2;128;128;128m";
const std::string FD_A_PRESSED = "\x1b[38;2;255;143;169m\x1b[48;2;170;27;58m";
const std::string FD_B_PRESSED = "\x1b[38;2;255;255;137m\x1b[48;2;169;91;7m";
const std::string FD_C_PRESSED = "\x1b[38;2;143;255;195m\x1b[48;2;18;132;62m";
const std::string FD_D_PRESSED = "\x1b[38;2;137;255;255m\x1b[48;2;21;66;161m";

const std::string FD_SPECIAL_GUIDE_BG = "\x1b[48;2;65;200;0m"; 
const std::string SPECIAL_CANCEL_FONTS[4] = { "\x1b[38;2;255;255;255m", "\x1b[38;2;128;80;128m", "\x1b[38;2;0;0;0m", "\x1b[38;2;128;80;128m" };
const std::string FD_NORMAL_GUIDE_FG = "\x1b[38;2;255;255;255m";
const std::string NORMAL_CANCEL_FONTS[4] = { "\x1b[48;2;65;200;0m", "\x1b[48;2;65;150;30m", "\x1b[48;2;65;80;65m", "\x1b[48;2;65;150;30m" };

const DWORD METER_COLOR_MAP[3] = { 0xFFC80000, 0xFFC8C800, 0xFF00C800 };
const DWORD HEAT_COLOR = 0xFF5A5AE6;
const DWORD HEATFONT_COLOR = 0xFFFA5300;
const DWORD MAX_COLOR = 0xFFFAA000;
const DWORD MAXFONT_COLOR = 0xFF2796FD;
const DWORD BLOODHEAT_COLOR = 0xFFB4B4B4;
const DWORD BLOODHEATFONT_COLOR = 0xFF8C0000;
const DWORD UNLIMITED_COLOR = 0xFF3296FF;
const DWORD UNLIMITEDFONT_COLOR = 0xFFFEABFF;
const DWORD CIRCUITBREAK_COLOR = 0xFFBE64C8;
const DWORD CIRCUITBREAKFONT_COLOR = 0xFFB06ED7;

const DWORD FB_INACTIONABLE = 0xFF41C800;
const DWORD FB_JUMP = 0xFFF1E084;
const DWORD FB_HITSTUN = 0xFF8C8C8C;
const DWORD FB_BLOCKSTUN = 0xFFB4B4B4;
const DWORD FB_ACTIVE = 0xFFFF0000;
const DWORD FB_ACTIONABLE = 0xFF202020;
const DWORD FB_ADVANTAGE = 0xFF101010;
const DWORD FB_NEUTRAL = 0xFF205A00;
const DWORD FB_FREEZE = 0xFF3C3C3C;
const DWORD FB_FREEZE_ACTIVE = 0xFFFF8080;
const DWORD FB_THROWN = 0xFF6E6E6E;
const DWORD FB_HITSTOP = 0xFF3C5080;
const DWORD FB_SHIELD = 0xFF91C2FF;
const DWORD FB_THROW_ACTIVE = 0xFFC00080;
const DWORD FB_CLASH = 0xFFE1B800;
const DWORD FB_INVULN = 0xFFFFFFFF;
const DWORD FB_ASSIST_ACTIVE = 0xFFFF8000;
const DWORD FB_COUNTER = 0xFFC485EA;

const ADDRESS adMBAABase = 0x400000;

const ADDRESS adSaveCurrentCamZoom = 0x14EB70;
const ADDRESS adSaveDestinationCamZoom = 0x14EB74;
const ADDRESS adGameState = 0x14EEE8;
const ADDRESS adSaveDestinationCamX = 0x155124;
const ADDRESS adSaveDestinationCamY = 0x155128;

const ADDRESS adRNGArray = 0x16406C;
const ADDRESS adRNGIndex = 0x164068;

const DWORD dwPlayerStructSize = 0xAFC;
const ADDRESS adP1Base = 0x155130; //0x155130
const ADDRESS adP2Base = 0x155130 + 1 * dwPlayerStructSize; //0x155C2C
const ADDRESS adP3Base = 0x155130 + 2 * dwPlayerStructSize; //0x156728
const ADDRESS adP4Base = 0x155130 + 3 * dwPlayerStructSize; //0x157224

const ADDRESS adP1SubBase = 0x155134;
const ADDRESS adP2SubBase = 0x155134 + 1 * dwPlayerStructSize; //0x155C30
const ADDRESS adP3SubBase = 0x155134 + 2 * dwPlayerStructSize; //0x15672C
const ADDRESS adP4SubBase = 0x155134 + 3 * dwPlayerStructSize; //0x157228

//Sub Base Addresses
const ADDRESS adDoTrainingAction = 0x3;
const ADDRESS adPattern = 0xC;
const ADDRESS adSave1Offset = 0xC;
const ADDRESS adState = 0x10;
const ADDRESS adIDKFlags = 0x84;
const ADDRESS adHealth = 0xB8;
const ADDRESS adRedHealth = 0xBC;
const ADDRESS adGuardGauge = 0xC0;
const ADDRESS adOnBlockCount = 0xD0;
const ADDRESS adGuardQuality = 0xD4;
const ADDRESS adMagicCircuit = 0xDC;
const ADDRESS adMagicCircuitTime = 0xE0;
const ADDRESS adMagicCircuitMode = 0xE4;
const ADDRESS adMagicCircuitPause = 0xEA;
const ADDRESS adMagicCircuitModeCounter = 0xEC;
const ADDRESS adCircuitBreakTimer = 0xFC;
const ADDRESS adCircuitBreakTotal = 0xFE;
const ADDRESS adBreakOrPenalty = 0x100;
const ADDRESS adXPosition = 0x104;
const ADDRESS adYPosition = 0x108;
const ADDRESS adXSpeed = 0x118;
const ADDRESS adYSpeed = 0x11C;
const ADDRESS adXAcceleration = 0x120;
const ADDRESS adYAcceleration = 0x122;
const ADDRESS adMomentum = 0x134;
const ADDRESS adHitstop = 0x16E;
const ADDRESS adThrowFlag = 0x172;
const ADDRESS adTagFlag = 0x174;
const ADDRESS adBlockstunFlag = 0x177;
const ADDRESS adWillBlock = 0x178;
const ADDRESS adBurstLock = 0x17E;
const ADDRESS adEFStrikeInvuln = 0x181;
const ADDRESS adEFThrowInvuln = 0x182;
const ADDRESS adUntechTotal = 0x18A;
const ADDRESS adUntechCounter = 0x18C;
const ADDRESS adHitstunRemaining = 0x1A8;
const ADDRESS adOnHitCount = 0x1EA;
const ADDRESS adCounterHit = 0x1F6;
const ADDRESS adSave2Offset = 0x1FC;
const ADDRESS adGravity = 0x2E0;
const ADDRESS adUntechPenalty = 0x2E4;
const ADDRESS adAirDirectionalInput = 0x2E6;
const ADDRESS adRawDirectionalInput = 0x2E7;
const ADDRESS adButtonInput = 0x2E9;
const ADDRESS adMacroInput = 0x2EA;
const ADDRESS adInputPattern = 0x308;
const ADDRESS adOnRightFlag = 0x311;
const ADDRESS adStanceDelayed = 0x317;
const ADDRESS adAnimationDataPointer = 0x31C;
const ADDRESS adAttackDataPointer = 0x320;
const ADDRESS adAnimationData_StateDataPointer = 0x38;
const ADDRESS adAnimationData_ConditionCount = 0x40;
const ADDRESS adAnimationData_BoxIndex = 0x42;
const ADDRESS adAnimationData_ConditionsPointer = 0x44;
const ADDRESS adStateData_Stance = 0xC;
const ADDRESS adStateData_Invuln = 0xD;
const ADDRESS adStateData_NormalCancel = 0xE;
const ADDRESS adStateData_SpecialCancel = 0xF;
const ADDRESS adStateData_Flagset2 = 0x18;
const ADDRESS adConditions_Condition1Pointer = 0x0;
const ADDRESS adConditions_Condition2Pointer = 0x4;
const ADDRESS adCondition_Type = 0x0;
const ADDRESS adCondition_Param1 = 0x4;

//Base Addresses
const ADDRESS adDirInputBuffer = 0x3E8;
const ADDRESS adAInputBuffer = 0x4EA;
const ADDRESS adBInputBuffer = 0x5EC;
const ADDRESS adCInputBuffer = 0x6EE;
const ADDRESS adDInputBuffer = 0x7F0;
const ADDRESS adEInputBuffer = 0x8F2;

const ADDRESS adP1TagFlag = 0x155134 + 0x174; //0x1552A8
const ADDRESS adP2TagFlag = 0x155134 + 0x174 + 1 * dwPlayerStructSize; //0x155DA4
const ADDRESS adP3TagFlag = 0x155134 + 0x174 + 2 * dwPlayerStructSize; //0x1568A0
const ADDRESS adP4TagFlag = 0x155134 + 0x174 + 3 * dwPlayerStructSize; //0x15739C

const DWORD dwPlayerDataStructSize = 0x20C;
const ADDRESS adP1DataBase = 0x157DB8; //0x157DB8
const ADDRESS adP2DataBase = 0x157DB8 + 1 * dwPlayerDataStructSize; //0x157FC4
const ADDRESS adP3DataBase = 0x157DB8 + 2 * dwPlayerDataStructSize; //0x1571D0
const ADDRESS adP4DataBase = 0x157DB8 + 3 * dwPlayerDataStructSize; //0x1583DC

const ADDRESS adP1ControlledCharacter = 0x157DB8;
const ADDRESS adP1NextControlledCharacter = 0x157DBC;
const ADDRESS adSaveAttackDisplayInfo = 0x157DD8;
const ADDRESS adSaveAttackDisplayInfo2 = 0x157E10;
const ADDRESS adP1Inaction = 0x157FC0;
const ADDRESS adP2ControlledCharacter = 0x157FC4;
const ADDRESS adP2NextControlledCharacter = 0x157FC8;
const ADDRESS adP2Inaction = 0x1581CC;

const ADDRESS adSaveStopSituation = 0x158600;

const ADDRESS adP1Freeze = 0x158908;
const ADDRESS adP2Freeze = 0x158C14;
const ADDRESS adP3Freeze = 0x158F20;
const ADDRESS adP4Freeze = 0x15922C;
const ADDRESS adFrameCount = 0x15D1CC;

const ADDRESS adSaveCurrentCamX = 0x15DEC4;
const ADDRESS adSaveCurrentCamY = 0x15DEC8;

const ADDRESS adTrainingResetScroll = 0x15DEF0;

const ADDRESS adTrueFrameCount = 0x162A40;
const ADDRESS adGlobalFreeze = 0x162A48;

const ADDRESS adSaveRNG = 0x164068;

const ADDRESS adSaveCurrentCamXCopy = 0x164B14;
const ADDRESS adSaveCurrentCamYCopy = 0x164B18;

const DWORD dwEffectStructSize = 0x33C;
const ADDRESS adSaveEffects = 0x27BDE8;
const ADDRESS adEffectBase = 0x27BDE8;
const ADDRESS adEffectSubBase = 0x27BDEC;
const ADDRESS adEffectSource = 0x4;
const ADDRESS adEffectStatus = 0x5;

const ADDRESS adNewSceneFlag = 0x15DEC3;
const ADDRESS adDummyState = 0x34D7F8;
const ADDRESS adTrainingMenu = 0x34D7FC;
const ADDRESS adP1AInput = 0x371399;
const ADDRESS adP1DInput = 0x37139C;
const ADDRESS adP1MenuDirInput = 0x371448;
const ADDRESS adP1FN1Input = 0x37144C;
const ADDRESS adP1FN2Input = 0x37144D;

//Vanilla training menu
const ADDRESS adBS_MAGIC_CIRCUIT = 0x37C1FC;
const ADDRESS adBS_GUARD_GAUGE = 0x37C200;

//FrameBar Constants
const int DISPLAY_RANGE = 75;

const ADDRESS adNotInCombo = 0x60;
const ADDRESS adEffectOwner = 0x2F0;

// addresses for fonts
const ADDRESS adFont0 = 0x55D680;
const ADDRESS adFont1 = 0x55D260;
const ADDRESS adFont2 = 0x55DAA0;

// addresses for memory sharing
// let's include comments here so we know how big everything is
// to make it easier to add more.
const ADDRESS adShareBase = 0x381000;
const ADDRESS adSharedFreezeOverride =				adShareBase + 0x8;	// 1 byte - this one still used during text boxes it looks like, @fang

const ADDRESS adSharedTimer =						adShareBase + 0x28; // 4 bytes - can maybe be removed, idr why it was needed

const ADDRESS adXS_frameData =							adShareBase + 0x503; //1 byte - remaining are used to communicate with console framebar
const ADDRESS adXS_showHitstopAndFreeze =				adShareBase + 0x504; //1 byte
const ADDRESS adXS_showInputs =							adShareBase + 0x505; //1 byte
const ADDRESS adXS_showCancel =							adShareBase + 0x506; //1 byte
const ADDRESS adXS_frameScroll =						adShareBase + 0x507; //2 bytes
const ADDRESS adXS_colorGuide =							adShareBase + 0x509; //1 byte

const char pcLatestVersion_19[19] = "LATEST VERSION";
const char pcOffline_8[8] = "OFFLINE";

//registry keys
typedef LPCTSTR REGKEY;

//extended menu registry keys
const REGKEY sFREEZE_KEY_REG = L"HOTKEY_Freeze";
const REGKEY sNEXT_FRAME_KEY_REG = L"HOTKEY_NextFrame";
const REGKEY sPREV_FRAME_KEY_REG = L"HOTKEY_PrevFrame";
const REGKEY sTOGGLE_HITBOXES_KEY_REG = L"HOTKEY_ToggleHitboxes";
const REGKEY sTOGGLE_FRAME_BAR_KEY_REG = L"HOTKEY_ToggleFrameBar";
const REGKEY sTOGGLE_HIGHLIGHTS_KEY_REG = L"HOTKEY_ToggleHighlights";
const REGKEY sQUEUE_REVERSAL_KEY_REG = L"HOTKEY_QueueReversal";
const REGKEY sINCREMENT_RNG_KEY_REG = L"HOTKEY_IncrementRNG";
const REGKEY sDECREMENT_RNG_KEY_REG = L"HOTKEY_DecrementRNG";

const REGKEY sSAVE_STATE_KEY_REG = L"HOTKEY_SaveState";
const REGKEY sPREV_SAVE_SLOT_KEY_REG = L"HOTKEY_PrevSaveSlot";
const REGKEY sNEXT_SAVE_SLOT_KEY_REG = L"HOTKEY_NextSaveSlot";
const REGKEY sFRAME_BAR_LEFT_KEY_REG = L"HOTKEY_FrameBarLeft";
const REGKEY sFRAME_BAR_RIGHT_KEY_REG = L"HOTKEY_FrameBarRight";

const REGKEY sHIGHLIGHTS = L"Highlights";
const REGKEY sBLOCKING_HIGHLIGHT = L"BlockingHighlight";
const REGKEY sHIT_HIGHLIGHT = L"HitHighlight";
const REGKEY sARMOR_HIGHLIGHT = L"ArmorHighlight";
const REGKEY sTHROW_PROTECTION_HIGHLIGHT = L"ThrowProtectionHighlight";
const REGKEY sIDLE_HIGHLIGHT = L"IdleHighlight";

const REGKEY sHITBOX_STYLE = L"HitboxStyle";
const REGKEY sCOLOR_BLIND_MODE = L"ColorBlindMode";

const REGKEY sFRAME_DATA = L"FrameData";
const REGKEY sFRAME_DISPLAY = L"FrameDisplay";
const REGKEY sDISPLAY_FREEZE = L"DisplayFreeze";
const REGKEY sDISPLAY_INPUTS = L"DisplayInputs";
const REGKEY sDISPLAY_CANCELS = L"DisplayCancels";

const REGKEY sFRAME_BAR_Y = L"FrameBarY";
const REGKEY sP1_INPUT_DISPLAY = L"P1InputDisplay";
const REGKEY sP2_INPUT_DISPLAY = L"P2InputDisplay";

const REGKEY sP1_ARCADE_INPUT_X = L"P1ArcadeInputX";
const REGKEY sP1_ARCADE_INPUT_Y = L"P1ArcadeInputY";
const REGKEY sP2_ARCADE_INPUT_X = L"P2ArcadeInputX";
const REGKEY sP2_ARCADE_INPUT_Y = L"P2ArcadeInputY";
const REGKEY sP1_LIST_INPUT_X = L"P1ListInputX";
const REGKEY sP1_LIST_INPUT_Y = L"P1ListInputY";
const REGKEY sP2_LIST_INPUT_X = L"P2ListInputX";
const REGKEY sP2_LIST_INPUT_Y = L"P2ListInputY";

#define VK_KEY_UNSET 0x0;

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

void __stdcall ___log(const char* msg);

void __stdcall log(const char* format, ...);

void __stdcall ___log(const wchar_t* msg);

// -----

bool GetOpenSAVFileName(HANDLE hMBAAHandle, DWORD dwBaseAddress, std::wstring* pwsFileName);

bool GetSaveSAVFileName(HANDLE hMBAAHandle, DWORD dwBaseAddress, std::wstring* pwsFileName);

void CreateRegistryKey();

LONG ReadFromRegistry(std::wstring sKey, uint8_t* nValue);

LONG ReadFromRegistry(std::wstring sKey, int* pnValue);

LONG SetRegistryValue(std::wstring sKey, int nValue);

LONG SetRegistryValue(std::wstring sKey, float fValue);

LONG SetRegistryValue(std::wstring sKey, bool bValue);

LONG SetRegistryValue(std::wstring sKey, std::string sValue);

LONG ReadFromRegistry(std::wstring sKey, bool* pbValue);

LONG ReadFromRegistry(std::wstring sKey, std::string* psValue);

LONG ReadFromRegistry(std::wstring sKey, float* pfValue);

LONG DeleteRegistry();

void printDirectXError(HRESULT hr);

//void printDIJOYSTATE2(const DIJOYSTATE2& state);
