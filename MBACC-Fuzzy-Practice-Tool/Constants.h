#pragma once
#include <vector>
#include <string>
#include <Windows.h>
#include <map>

enum eEnemyStatus { STAND, JUMP, CROUCH, CPU, MANUAL, DUMMY };
enum eEnemyOffOnRandom { OFF, ON, RANDOM };
enum eEnemyDefense { NOGUARD, ALLGUARD, STATUSGUARD, ALLSHIELD, STATUSSHIELD, DODGE };
enum eEnemyStance { STANDING = 0, STANDGUARDING = 17, CROUCHING = 13 };
enum ePresetSettings { DEFAULT, FUZZY, BLOCKSTRING, HEATOS, FUZZYMASH, FUZZYJUMP, CUSTOM };
enum eEnemyGuardLevelSettings { INF, ONEHUNDRED, SEVENTYFIVE, FIFTY, TWENTYFIVE, ZERO };

const std::string GITHUB_LATEST = "https://api.github.com/repos/fangdreth/MBAACC-Extended-Training-Mode/releases/latest";
const std::string GITHUB_RELEASE = "https://github.com/fangdreth/MBAACC-Extended-Training-Mode/releases/tag/";
const std::string GITHUB_README = "https://github.com/fangdreth/MBAACC-Extended-Training-Mode/blob/main/README.md";

const DWORD dwP2Offset = 0xAFC;
const DWORD dwRoundTime = 0x162A40; //0-inf
const DWORD dwMot = 0x1581CC;   // this one is mysterious.  I think it's an animation counter
const DWORD dwPlayerState = 0x155140;  //0:STAND 13:CROUCH 17:STANDGUARDING 12:STAND->CROUCH ETC
const DWORD dwEnemyStatus = 0x37C1E8; //0:STAND 1:JUMP 2:CROUCH 3:CPU 4:MANUAL 5:DUMMY
const DWORD dwEnemyDefense = 0x37C1F0; //0:OFF 1:ALLGUARD 2:STATUSGUARD 3:ALLSHIELD 4:STATUSSHIELD 5:DODGE
const DWORD dwGuardAmount2 = 0x1551F4; //0-1174011904 aka 0.0f-8000.0f
const DWORD dwGuardAmount = 0x155CF0; //0-1174011904 aka 0.0f-8000.0f
const DWORD dwGuardSetting = 0x37C200; //0:100 1:75 2:50 3:25 4:0
const DWORD dwExGuard = 0x1551E0; //10:ExGuard
const DWORD dwY = 0x155248;
const DWORD dwP2PatternSet = 0x155F38;
const DWORD dwP2PatternRead = 0x155C3C;
const DWORD dwCSSFlag = 0x155130;
const DWORD dwP2CharNumber = 0x34D91C;
const DWORD dwP2CharMoon = 0x34D924;
const DWORD dwP1ButtonPress = 0x154654;
const DWORD dwP1DirectionPress = 0x371448;
const DWORD dwPausedFlag = 0x162A64;
const DWORD dwEnemyAction = 0x37C1EC;

const std::vector<std::string> vPresetSettings = { "Default", "Fuzzy Overhead", "Blockstring", "Heat OS", "Defensive Fuzzy Mash", "Defensive Fuzzy Jump", "Custom" };
const std::vector<std::string> vEnemyDefenseSettings = { "No Guard", "All Guard", "Status Guard", "All Shield", "Status Shield" };
const std::vector<std::string> vEnemyDefenseBiasSettings = { "Unlikely", "Even", "Likely", "Off" };
const std::vector<std::string> vEnemyStatusSettings = { "Stand", "Jump", "Crouch" };
const std::vector<std::string> vEnemyGuardLevelSettings = { "Infinite", "100%", "75%", "50%", "25%", "0%" };