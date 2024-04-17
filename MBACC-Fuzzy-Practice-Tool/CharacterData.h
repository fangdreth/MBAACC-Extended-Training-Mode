#pragma once
#include <map>
#include <string>
#include <Windows.h>
#include <vector>
#include "RawData.h"

// Character ID is created as a concatination of char number and moon.  Ex. Sats is 29 and C is 0, so CSats is 290
enum eCharacterValues
{
	UNIVERSAL = 0,
	CAOKO = 20, FAOKO = 21, HAOKO = 22,
	CTOHNO = 30, FTOHNO = 31, HTOHNO = 32,
	CHIME = 40, HHIME = 41, FHIME = 43,
	CNANAYA = 50, HNANAYA = 51, FNANAYA = 52,
	CKOUMA = 60, HKOUMA = 61, FKOUMA = 62,
	CMIYAKO = 100, HMIYAKO = 101, FMIYAKO = 102,
	CCIEL = 110, HCIEL = 111, FCIEL = 112,
	CSION = 120, HSION = 121, FSION = 122,
	CRIES = 130, HRIES = 131, FRIES = 132,
	CVSION = 140, HVSION = 141, FVSION = 142,
	CWARA = 150, HWARA = 151, FWARA = 152,
	CROA = 160, HROA = 161, FROA = 162,
	CMAIDS = 190, HMAIDS = 191, FMAIDS = 192,
	CAKIHA = 200, HAKIHA = 201, FAKIHA = 202,
	CARC = 210, HARC = 211, FARC = 212,
	CPCIEL = 220, HPCIEL = 221, FPCIEL = 222,
	CWARC = 230, HWARC = 231, FWARC = 232,
	CVAKIHA = 240, HVAKIHA = 241, FVAKIHA = 242,
	CMECH = 250, HMECH = 251, FMECH = 252,
	CSEIFUKU = 280, HSEIFUKU = 281, FSEIFUKU = 282,
	CSATSUKI = 290, HSATSUKI = 291, FSATSUKI = 292,
	CLEN = 300, HLEN = 301, FLEN = 302,
	CRYOUGI = 310, HRYOUGI = 311, FRYOUGI = 312,
	CWLEN = 320, HWLEN = 321, FWLEN = 322,
	CNERO = 330, HNERO = 331, FNERO = 332,
	CNAC = 340, HNAC = 341, FNAC = 342,
	CKOHAMECH = 380, HKOHAMECH = 381, FKOHAMECH = 382,
	CHISUI = 390, HHISUI = 391, FHISUI = 392,
	CNECO = 400, HNECO = 401, FNECO = 402,
	CKOHAKU = 410, HKOHAKU = 411, FKOHAKU = 412,
	CNECOMECH = 420, HNECOMECH = 421, FNECOMECH = 422
};

std::map<std::string, int> CharacterValues_Map =
{
	{"CAOKO", 20}, {"FAOKO", 20}, {"HAOKO", 22},
	{"CTOHNO", 30}, {"FTOHNO", 30}, {"HTOHNO", 32},
	{"CHIME", 40}, {"FHIME", 40}, {"HHIME", 43},
	{"CNANAYA", 50}, {"FNANAYA", 50}, {"HNANAYA", 52},
	{"CKOUMA", 60}, {"FKOUMA", 60}, {"HKOUMA", 62},
	{"CMIYAKO", 100}, {"FMIYAKO", 100}, {"HMIYAKO", 102},
	{"CCIEL", 110}, {"FCIEL", 110}, {"HCIEL", 112},
	{"CSION", 120}, {"FSION", 120}, {"HSION", 122},
	{"CRIES", 130}, {"FRIES", 130}, {"HRIES", 132},
	{"CVSION", 140}, {"FVSION", 140}, {"HVSION", 142},
	{"CWARA", 150}, {"FWARA", 150}, {"HWARA", 152},
	{"CROA", 160}, {"FROA", 160}, {"HROA", 162},
	{"CMAIDS", 190}, {"FMAIDS", 190}, {"HMAIDS", 192},
	{"CAKIHA", 200}, {"FAKIHA", 200}, {"HAKIHA", 202},
	{"CARC", 210}, {"FARC", 210}, {"HARC", 212},
	{"CPCIEL", 220}, {"FPCIEL", 220}, {"HPCIEL", 222},
	{"CWARC", 230}, {"FWARC", 230}, {"HWARC", 232},
	{"CVAKIHA", 240}, {"FVAKIHA", 240}, {"HVAKIHA", 242},
	{"CMECH", 250}, {"FMECH", 250}, {"HMECH", 252},
	{"CSEIFUKU", 280}, {"FSEIFUKU", 280}, {"HSEIFUKU", 282},
	{"CSATSUKI", 290}, {"FSATSUKI", 290}, {"HSATSUKI", 292},
	{"CLEN", 300}, {"FLEN", 300}, {"HLEN", 302},
	{"CRYOUGI", 310}, {"FRYOUGI", 310}, {"HRYOUGI", 312},
	{"CWLEN", 320}, {"FWLEN", 320}, {"HWLEN", 322},
	{"CNERO", 330}, {"FNERO", 330}, {"HNERO", 332},
	{"CNAC", 340}, {"FNAC", 340}, {"HNAC", 342},
	{"CKOHAMECH", 380}, {"FKOHAMECH", 380}, {"HKOHAMECH", 382},
	{"CHISUI", 390}, {"FHISUI", 390}, {"HHISUI", 392},
	{"CNECO", 400}, {"FNECO", 400}, {"HNECO", 402},
	{"CKOHAKU", 410}, {"FKOHAKU", 410}, {"HKOHAKU", 412},
	{"CNECOMECH", 420}, {"FNECOMECH", 420}, {"HNECOMECH", 422}
};

std::map<int, std::map<std::string, int>> MBAACC_Map = { {} };

std::vector<std::string> vEveryPatternName =
{
	"5A", "2A", "6A", "5B", "2B", "4B", "6B", "3B", "5C", "2C", "4C", "6C", "3C",
	"Bunker", "5D", "2D", "Dodge", "Heat", "Throw",
	"Back Dash", "Forward Dash", "Jump", "Forward Jump", "Backward Jump", "Super Jump", "Forward Super Jump",
	"623A", "623B", "623C",
	"22A", "22B", "22[B]", "22C", "22D", "2222C", "22222C",
	"214A", "214B", "214C", "214214C",
	"236A", "236B", "236C",
	"421A", "421B", "421C",
	"624A", "624B", "624C",
	"Arc Drive", "Another Arc Drive",
	"Drug Install"
};

const std::map<std::string, int> vUniversalPatterns_Map =
{
	{ "5A", 1 }, { "5B", 2 }, { "5C", 3 }, { "2A", 4 }, { "2B", 5 }, { "2C", 6 },
	{ "Heat", 260 },
	{ "Jump", 36 }, { "Forward Jump", 35 }, { "Backward Jump", 37 }, { "Super Jump", 360 }
};

void InitializeCharacterMaps()
{
	MBAACC_Map[eCharacterValues::UNIVERSAL] = vUniversalPatterns_Map;
	MBAACC_Map[eCharacterValues::CLEN] = vCLEN_Map;
}

int GetPattern(int nCharacterID, std::string sPatternName)
{
	// All patterns are positive integers.  If a pattern is not in a map, it is a 0
	return max(MBAACC_Map[eCharacterValues::UNIVERSAL][sPatternName], MBAACC_Map[nCharacterID][sPatternName]);
}

std::vector<std::string> GetPatternList(int nCharacterID)
{
	std::vector<std::string> vReturnList = { "None" };

	for (std::string& sPatternName : vEveryPatternName)
	{
		if (MBAACC_Map[eCharacterValues::UNIVERSAL][sPatternName] != 0 || MBAACC_Map[nCharacterID][sPatternName] != 0)
			vReturnList.push_back(sPatternName);
	}

	return vReturnList;
}

std::vector<std::string> GetEmptyPatternList()
{
	return { "None" };
}