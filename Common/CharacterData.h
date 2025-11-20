#pragma once


#include <map>
#include <vector>
#include "..\Common\RawData.h"

// Character ID is created as a concatination of char number and moon.  Ex. Sats is 29 and C is 0, so CSats is 290
enum eCharacterValues {
	UNIVERSAL = 0,
	CAOKO = 20, FAOKO = 21, HAOKO = 22,
	CTOHNO = 30, FTOHNO = 31, HTOHNO = 32,
	CHIME = 40, FHIME = 41, HHIME = 42,
	CNANAYA = 50, FNANAYA = 51, HNANAYA = 52,
	CKOUMA = 60, FKOUMA = 61, HKOUMA = 62,
	CMIYAKO = 100, FMIYAKO = 101, HMIYAKO = 102,
	CCIEL = 110, FCIEL = 111, HCIEL = 112,
	CSION = 120, FSION = 121, HSION = 122,
	CRIES = 130, FRIES = 131, HRIES = 132,
	CVSION = 140, FVSION = 141, HVSION = 142,
	CWARA = 150, FWARA = 151, HWARA = 152,
	CROA = 160, FROA = 161, HROA = 162,
	CMAIDS = 190, FMAIDS = 191, HMAIDS = 192,
	CAKIHA = 200, FAKIHA = 201, HAKIHA = 202,
	CARC = 210, FARC = 211, HARC = 212,
	CPCIEL = 220, FPCIEL = 221, HPCIEL = 222,
	CWARC = 230, FWARC = 231, HWARC = 232,
	CVAKIHA = 240, FVAKIHA = 241, HVAKIHA = 242,
	CMECH = 250, FMECH = 251, HMECH = 252,
	CSEIFUKU = 280, FSEIFUKU = 281, HSEIFUKU = 282,
	CSATSUKI = 290, FSATSUKI = 291, HSATSUKI = 292,
	CLEN = 300, FLEN = 301, HLEN = 302,
	CRYOUGI = 310, FRYOUGI = 311, HRYOUGI = 312,
	CWLEN = 320, FWLEN = 321, HWLEN = 322,
	CNERO = 330, FNERO = 331, HNERO = 332,
	CNAC = 340, FNAC = 341, HNAC = 342,
	CKOHAMECH = 380, FKOHAMECH = 381, HKOHAMECH = 382,
	CHISUI = 390, FHISUI = 391, HHISUI = 392,
	CNECO = 400, FNECO = 401, HNECO = 402,
	CKOHAKU = 410, FKOHAKU = 411, HKOHAKU = 412,
	CNECOMECH = 420, FNECOMECH = 421, HNECOMECH = 422
};

extern std::map<std::string, int> CharacterValues_Map;

extern std::map<int, std::map<std::string, int>> MBAACC_Map;

extern std::map<short, std::map<short, bool>> MBAACC_PatternMap;

extern std::vector<std::string> vEveryPatternName;

void InitializeCharacterMaps();

int GetPattern(int nCharacterID, std::string sPatternName);

std::string GetCharacterNameFromID(int nCharacterID);

std::vector<std::string> GetPatternList(int nCharacterID);

std::string GetPatternNameFromID(int nPattern, int nCharacterID);

std::vector<std::string> GetEmptyPatternList();

bool IsAir(std::string sMove);

void PopulateAirAndGroundReversals(std::vector<int>* vAirReversals, std::vector<int>* vGroundReversals, int nP2CharacterID, std::vector<std::string>* vPatternNames, int nReversalIndex1, int nReversalIndex2, int nReversalIndex3, int nReversalIndex4);

void RemoveShieldReversals(std::vector<int>* vReversals, int nCharacterID);

void RemoveNonShieldReversals(std::vector<int>* vReversals, int nCharacterID);
