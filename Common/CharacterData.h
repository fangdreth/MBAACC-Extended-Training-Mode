#pragma once
#include "..\Common\Common.h"
#include "..\Common\RawData.h"

// Character ID is created as a concatination of char number and moon.  Ex. Sats is 29 and C is 0, so CSats is 290
enum eCharacterValues
{
	UNIVERSAL = 0,
	CAOKO = 20, FAOKO = 21, HAOKO = 22,
	CTOHNO = 30, FTOHNO = 31, HTOHNO = 32,
	CHIME = 40, FHIME = 41, HHIME = 43,
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

static std::map<std::string, int> CharacterValues_Map =
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

static std::map<int, std::map<std::string, int>> MBAACC_Map = { {} };

static std::vector<std::string> vEveryPatternName =
{
	"5A", "2A", "6A", "5B", "2B", "4B", "6B", "3B", "5C", "2C", "4C", "6C", "3C",
	"j.6A", "j.6B", "j.6C", "j.2C",
	"A DIVEKICK", "B DIVEKICK", "C DIVEKICK",
	"BUNKER", "5D", "2D", "j.D", "DODGE", "AIR DODGE", "HEAT", "THROW", "AIR THROW", "FLOAT", "AIR FLOAT",
	"BACK DASH", "BACK AIRDASH", "FORWARD DASH", "FORWARD AIRDASH", "FASTFALL", "JUMP", "DBL JUMP", "FORWARD JUMP", "FORWARD DBL JUMP", "BACKWARD JUMP", "BACKWARD DBL JUMP", "FORWARD SUPER JUMP", "SUPER JUMP", "SUPER DBL JUMP",
	"623A", "623B", "623C",
	"j.623A", "j.623B", "j.623C",
	"22A", "22B", "22C", "22D", "2222C", "22222C",
	"j.22A", "j.22B", "j.22C",
	//"NEKO TECH", // don't work right :|
	"214A", "214B", "214C", "214214C",
	"j.214A", "j.214B", "j.214C",
	"236A", "236B", "236C",
	"j.236A", "j.236B", "j.236C",
	"421A", "421B", "421C",
	"j.421A", "j.421B", "j.421C",
	"624A", "624B", "624C",
	"j.624A", "j.624B", "j.624C",
	"ARC DRIVE",
	"DRUG INSTALL"
};

static void InitializeCharacterMaps()
{
	MBAACC_Map[eCharacterValues::UNIVERSAL] = UniversalPatterns_Map;
	MBAACC_Map[eCharacterValues::CAOKO] = CAOKO_Map;
	MBAACC_Map[eCharacterValues::FAOKO] = FAOKO_Map;
	MBAACC_Map[eCharacterValues::HAOKO] = HAOKO_Map;
	MBAACC_Map[eCharacterValues::CTOHNO] = CTOHNO_Map;
	MBAACC_Map[eCharacterValues::FTOHNO] = FTOHNO_Map;
	MBAACC_Map[eCharacterValues::HTOHNO] = HTOHNO_Map;
	MBAACC_Map[eCharacterValues::CHIME] = CHIME_Map;
	MBAACC_Map[eCharacterValues::HHIME] = HHIME_Map;
	MBAACC_Map[eCharacterValues::FHIME] = FHIME_Map;
	MBAACC_Map[eCharacterValues::CNANAYA] = CNANAYA_Map;
	MBAACC_Map[eCharacterValues::HNANAYA] = HNANAYA_Map;
	MBAACC_Map[eCharacterValues::FNANAYA] = FNANAYA_Map;
	MBAACC_Map[eCharacterValues::CKOUMA] = CKOUMA_Map;
	MBAACC_Map[eCharacterValues::HKOUMA] = HKOUMA_Map;
	MBAACC_Map[eCharacterValues::FKOUMA] = FKOUMA_Map;
	MBAACC_Map[eCharacterValues::CMIYAKO] = CMIYAKO_Map;
	MBAACC_Map[eCharacterValues::HMIYAKO] = HMIYAKO_Map;
	MBAACC_Map[eCharacterValues::FMIYAKO] = FMIYAKO_Map;
	MBAACC_Map[eCharacterValues::CCIEL] = CCIEL_Map;
	MBAACC_Map[eCharacterValues::HCIEL] = HCIEL_Map;
	MBAACC_Map[eCharacterValues::FCIEL] = FCIEL_Map;
	MBAACC_Map[eCharacterValues::CSION] = CSION_Map;
	MBAACC_Map[eCharacterValues::HSION] = HSION_Map;
	MBAACC_Map[eCharacterValues::FSION] = FSION_Map;
	MBAACC_Map[eCharacterValues::CRIES] = CRIES_Map;
	MBAACC_Map[eCharacterValues::HRIES] = HRIES_Map;
	MBAACC_Map[eCharacterValues::FRIES] = FRIES_Map;
	MBAACC_Map[eCharacterValues::CVSION] = CVSION_Map;
	MBAACC_Map[eCharacterValues::HVSION] = HVSION_Map;
	MBAACC_Map[eCharacterValues::FVSION] = FVSION_Map;
	MBAACC_Map[eCharacterValues::CWARA] = CWARA_Map;
	MBAACC_Map[eCharacterValues::HWARA] = HWARA_Map;
	MBAACC_Map[eCharacterValues::FWARA] = FWARA_Map;
	MBAACC_Map[eCharacterValues::CROA] = CROA_Map;
	MBAACC_Map[eCharacterValues::HROA] = HROA_Map;
	MBAACC_Map[eCharacterValues::FROA] = FROA_Map;
	MBAACC_Map[eCharacterValues::CMAIDS] = CMAIDS_Map;
	MBAACC_Map[eCharacterValues::HMAIDS] = HMAIDS_Map;
	MBAACC_Map[eCharacterValues::FMAIDS] = FMAIDS_Map;
	MBAACC_Map[eCharacterValues::CAKIHA] = CAKIHA_Map;
	MBAACC_Map[eCharacterValues::HAKIHA] = HAKIHA_Map;
	MBAACC_Map[eCharacterValues::FAKIHA] = FAKIHA_Map;
	MBAACC_Map[eCharacterValues::CARC] = CARC_Map;
	MBAACC_Map[eCharacterValues::HARC] = HARC_Map;
	MBAACC_Map[eCharacterValues::FARC] = FARC_Map;
	MBAACC_Map[eCharacterValues::CPCIEL] = CPCIEL_Map;
	MBAACC_Map[eCharacterValues::HPCIEL] = HPCIEL_Map;
	MBAACC_Map[eCharacterValues::FPCIEL] = FPCIEL_Map;
	MBAACC_Map[eCharacterValues::CWARC] = CWARC_Map;
	MBAACC_Map[eCharacterValues::HWARC] = HWARC_Map;
	MBAACC_Map[eCharacterValues::FWARC] = FWARC_Map;
	MBAACC_Map[eCharacterValues::CVAKIHA] = CVAKIHA_Map;
	MBAACC_Map[eCharacterValues::HVAKIHA] = HVAKIHA_Map;
	MBAACC_Map[eCharacterValues::FVAKIHA] = FVAKIHA_Map;
	MBAACC_Map[eCharacterValues::CMECH] = CMECH_Map;
	MBAACC_Map[eCharacterValues::HMECH] = HMECH_Map;
	MBAACC_Map[eCharacterValues::FMECH] = FMECH_Map;
	MBAACC_Map[eCharacterValues::CSEIFUKU] = CSEIFUKU_Map;
	MBAACC_Map[eCharacterValues::HSEIFUKU] = HSEIFUKU_Map;
	MBAACC_Map[eCharacterValues::FSEIFUKU] = FSEIFUKU_Map;
	MBAACC_Map[eCharacterValues::CSATSUKI] = CSATSUKI_Map;
	MBAACC_Map[eCharacterValues::HSATSUKI] = HSATSUKI_Map;
	MBAACC_Map[eCharacterValues::FSATSUKI] = FSATSUKI_Map;
	MBAACC_Map[eCharacterValues::CLEN] = CLEN_Map;
	MBAACC_Map[eCharacterValues::HLEN] = HLEN_Map;
	MBAACC_Map[eCharacterValues::FLEN] = FLEN_Map;
	MBAACC_Map[eCharacterValues::CRYOUGI] = CRYOUGI_Map;
	MBAACC_Map[eCharacterValues::HRYOUGI] = HRYOUGI_Map;
	MBAACC_Map[eCharacterValues::FRYOUGI] = FRYOUGI_Map;
	MBAACC_Map[eCharacterValues::CWLEN] = CWLEN_Map;
	MBAACC_Map[eCharacterValues::HWLEN] = HWLEN_Map;
	MBAACC_Map[eCharacterValues::FWLEN] = FWLEN_Map;
	MBAACC_Map[eCharacterValues::CNERO] = CNERO_Map;
	MBAACC_Map[eCharacterValues::HNERO] = HNERO_Map;
	MBAACC_Map[eCharacterValues::FNERO] = FNERO_Map;
	MBAACC_Map[eCharacterValues::CNAC] = CNAC_Map;
	MBAACC_Map[eCharacterValues::HNAC] = HNAC_Map;
	MBAACC_Map[eCharacterValues::FNAC] = FNAC_Map;
	MBAACC_Map[eCharacterValues::CKOHAMECH] = CKOHAMECH_Map;
	MBAACC_Map[eCharacterValues::HKOHAMECH] = HKOHAMECH_Map;
	MBAACC_Map[eCharacterValues::FKOHAMECH] = FKOHAMECH_Map;
	MBAACC_Map[eCharacterValues::CHISUI] = CHISUI_Map;
	MBAACC_Map[eCharacterValues::HHISUI] = HHISUI_Map;
	MBAACC_Map[eCharacterValues::FHISUI] = FHISUI_Map;
	MBAACC_Map[eCharacterValues::CKOHAKU] = CKOHAKU_Map;
	MBAACC_Map[eCharacterValues::HKOHAKU] = HKOHAKU_Map;
	MBAACC_Map[eCharacterValues::FKOHAKU] = FKOHAKU_Map;
	MBAACC_Map[eCharacterValues::CNECOMECH] = CNECOMECH_Map;
	MBAACC_Map[eCharacterValues::HNECOMECH] = HNECOMECH_Map;
	MBAACC_Map[eCharacterValues::FNECOMECH] = FNECOMECH_Map;
	MBAACC_Map[eCharacterValues::CNECO] = CNECO_Map;
	MBAACC_Map[eCharacterValues::HNECO] = HNECO_Map;
	MBAACC_Map[eCharacterValues::FNECO] = FNECO_Map;
}

static int GetPattern(int nCharacterID, std::string sPatternName)
{
	// All patterns are positive integers.  If a pattern is not in a map, it is a 0
	return max(MBAACC_Map[eCharacterValues::UNIVERSAL][sPatternName], MBAACC_Map[nCharacterID][sPatternName]);
}

static std::string GetCharacterName(int nCharacterID)
{
	for (std::pair<const std::string, int>& x : CharacterValues_Map)
	{
		if (x.second == nCharacterID)
			return x.first;
	}
	return "UNKNOWN";
}

static std::vector<std::string> GetPatternList(int nCharacterID)
{
	std::vector<std::string> vReturnList = { "None" };

	for (std::string& sPatternName : vEveryPatternName)
	{
		if (MBAACC_Map[eCharacterValues::UNIVERSAL][sPatternName] != 0 || MBAACC_Map[nCharacterID][sPatternName] != 0)
			vReturnList.push_back(sPatternName);
	}

	return vReturnList;
}

static std::vector<std::string> GetEmptyPatternList()
{
	return { "OFF" };
}

static bool IsAir(std::string sMove)
{
	return sMove.substr(0, 1) == "j" || sMove.find("DIVEKICK") != std::string::npos || sMove.find("AIR") != std::string::npos || sMove.find("DBL") != std::string::npos || sMove == "FASTFALL";
}

static void PopulateAirAndGroundReversals(std::vector<int>* vAirReversals, std::vector<int>* vGroundReversals, int nP2CharacterID, std::vector<std::string>* vPatternNames, int nReversalIndex1, int nReversalIndex2, int nReversalIndex3, int nReversalIndex4)
{
	(*vAirReversals) = {};
	(*vGroundReversals) = {};

	//if (vPatternNames->size() == 1)
		//return;

	if (GetPattern(nP2CharacterID, (*vPatternNames)[nReversalIndex1]) != 0)
	{
		if (IsAir((*vPatternNames)[nReversalIndex1]))
			vAirReversals->push_back(GetPattern(nP2CharacterID, (*vPatternNames)[nReversalIndex1]));
		else
			vGroundReversals->push_back(GetPattern(nP2CharacterID, (*vPatternNames)[nReversalIndex1]));
	}
	if (GetPattern(nP2CharacterID, (*vPatternNames)[nReversalIndex2]) != 0)
	{
		if (IsAir((*vPatternNames)[nReversalIndex2]))
			vAirReversals->push_back(GetPattern(nP2CharacterID, (*vPatternNames)[nReversalIndex2]));
		else
			vGroundReversals->push_back(GetPattern(nP2CharacterID, (*vPatternNames)[nReversalIndex2]));
	}
	if (GetPattern(nP2CharacterID, (*vPatternNames)[nReversalIndex3]) != 0)
	{
		if (IsAir((*vPatternNames)[nReversalIndex3]))
			vAirReversals->push_back(GetPattern(nP2CharacterID, (*vPatternNames)[nReversalIndex3]));
		else
			vGroundReversals->push_back(GetPattern(nP2CharacterID, (*vPatternNames)[nReversalIndex3]));
	}
	if (GetPattern(nP2CharacterID, (*vPatternNames)[nReversalIndex4]) != 0)
	{
		if (IsAir((*vPatternNames)[nReversalIndex4]))
			vAirReversals->push_back(GetPattern(nP2CharacterID, (*vPatternNames)[nReversalIndex4]));
		else
			vGroundReversals->push_back(GetPattern(nP2CharacterID, (*vPatternNames)[nReversalIndex4]));
	}
}