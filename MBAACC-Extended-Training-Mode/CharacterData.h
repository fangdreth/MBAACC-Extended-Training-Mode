#pragma once
#include "Constants.h"
#include "RawData.h"

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
	"j.A", "j.6A", "j.B", "j.6B", "j.C", "j.6C",
	"BUNKER", "5D", "2D", "j.D", "DODGE", "AIR DODGE", "HEAT", "THROW", "AIR THROW", "FLOAT", "AIR FLOAT",
	"BACK DASH", "BACK AIRDASH", "FORWARD DASH", "FORWARD AIRDASH", "FASTFALL", "JUMP", "DBL JUMP", "FORWARD JUMP", "FORWARD DBL JUMP", "BACKWARD JUMP", "BACKWARD DBL JUMP", "SUPER JUMP", "FORWARD SUPER JUMP", "SUPER DBL JUMP",
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
	MBAACC_Map[eCharacterValues::UNIVERSAL] = vUniversalPatterns_Map;
	MBAACC_Map[eCharacterValues::CAOKO] = vCAOKO_Map;
	MBAACC_Map[eCharacterValues::FAOKO] = vFAOKO_Map;
	MBAACC_Map[eCharacterValues::HAOKO] = vHAOKO_Map;
	MBAACC_Map[eCharacterValues::CTOHNO] = vCTOHNO_Map;
	MBAACC_Map[eCharacterValues::FTOHNO] = vFTOHNO_Map;
	MBAACC_Map[eCharacterValues::HTOHNO] = vHTOHNO_Map;
	MBAACC_Map[eCharacterValues::CHIME] = vCHIME_Map;
	MBAACC_Map[eCharacterValues::HHIME] = vHHIME_Map;
	MBAACC_Map[eCharacterValues::FHIME] = vFHIME_Map;
	MBAACC_Map[eCharacterValues::CNANAYA] = vCNANAYA_Map;
	MBAACC_Map[eCharacterValues::HNANAYA] = vHNANAYA_Map;
	MBAACC_Map[eCharacterValues::FNANAYA] = vFNANAYA_Map;
	MBAACC_Map[eCharacterValues::CKOUMA] = vCKOUMA_Map;
	MBAACC_Map[eCharacterValues::HKOUMA] = vHKOUMA_Map;
	MBAACC_Map[eCharacterValues::FKOUMA] = vFKOUMA_Map;
	MBAACC_Map[eCharacterValues::CMIYAKO] = vCMIYAKO_Map;
	MBAACC_Map[eCharacterValues::HMIYAKO] = vHMIYAKO_Map;
	MBAACC_Map[eCharacterValues::FMIYAKO] = vFMIYAKO_Map;
	MBAACC_Map[eCharacterValues::CCIEL] = vCCIEL_Map;
	MBAACC_Map[eCharacterValues::HCIEL] = vHCIEL_Map;
	MBAACC_Map[eCharacterValues::FCIEL] = vFCIEL_Map;
	MBAACC_Map[eCharacterValues::CSION] = vCSION_Map;
	MBAACC_Map[eCharacterValues::HSION] = vHSION_Map;
	MBAACC_Map[eCharacterValues::FSION] = vFSION_Map;
	MBAACC_Map[eCharacterValues::CRIES] = vCRIES_Map;
	MBAACC_Map[eCharacterValues::HRIES] = vHRIES_Map;
	MBAACC_Map[eCharacterValues::FRIES] = vFRIES_Map;
	MBAACC_Map[eCharacterValues::CVSION] = vCVSION_Map;
	MBAACC_Map[eCharacterValues::HVSION] = vHVSION_Map;
	MBAACC_Map[eCharacterValues::FVSION] = vFVSION_Map;
	MBAACC_Map[eCharacterValues::CWARA] = vCWARA_Map;
	MBAACC_Map[eCharacterValues::HWARA] = vHWARA_Map;
	MBAACC_Map[eCharacterValues::FWARA] = vFWARA_Map;
	MBAACC_Map[eCharacterValues::CROA] = vCROA_Map;
	MBAACC_Map[eCharacterValues::HROA] = vHROA_Map;
	MBAACC_Map[eCharacterValues::FROA] = vFROA_Map;
	MBAACC_Map[eCharacterValues::CMAIDS] = vCMAIDS_Map;
	MBAACC_Map[eCharacterValues::HMAIDS] = vHMAIDS_Map;
	MBAACC_Map[eCharacterValues::FMAIDS] = vFMAIDS_Map;
	MBAACC_Map[eCharacterValues::CAKIHA] = vCAKIHA_Map;
	MBAACC_Map[eCharacterValues::HAKIHA] = vHAKIHA_Map;
	MBAACC_Map[eCharacterValues::FAKIHA] = vFAKIHA_Map;
	MBAACC_Map[eCharacterValues::CARC] = vCARC_Map;
	MBAACC_Map[eCharacterValues::HARC] = vHARC_Map;
	MBAACC_Map[eCharacterValues::FARC] = vFARC_Map;
	MBAACC_Map[eCharacterValues::CPCIEL] = vCPCIEL_Map;
	MBAACC_Map[eCharacterValues::HPCIEL] = vHPCIEL_Map;
	MBAACC_Map[eCharacterValues::FPCIEL] = vFPCIEL_Map;
	MBAACC_Map[eCharacterValues::CWARC] = vCWARC_Map;
	MBAACC_Map[eCharacterValues::HWARC] = vHWARC_Map;
	MBAACC_Map[eCharacterValues::FWARC] = vFWARC_Map;
	MBAACC_Map[eCharacterValues::CVAKIHA] = vCVAKIHA_Map;
	MBAACC_Map[eCharacterValues::HVAKIHA] = vHVAKIHA_Map;
	MBAACC_Map[eCharacterValues::FVAKIHA] = vFVAKIHA_Map;
	MBAACC_Map[eCharacterValues::CMECH] = vCMECH_Map;
	MBAACC_Map[eCharacterValues::HMECH] = vHMECH_Map;
	MBAACC_Map[eCharacterValues::FMECH] = vFMECH_Map;
	MBAACC_Map[eCharacterValues::CSEIFUKU] = vCSEIFUKU_Map;
	MBAACC_Map[eCharacterValues::HSEIFUKU] = vHSEIFUKU_Map;
	MBAACC_Map[eCharacterValues::FSEIFUKU] = vFSEIFUKU_Map;
	MBAACC_Map[eCharacterValues::CSATSUKI] = vCSATSUKI_Map;
	MBAACC_Map[eCharacterValues::HSATSUKI] = vHSATSUKI_Map;
	MBAACC_Map[eCharacterValues::FSATSUKI] = vFSATSUKI_Map;
	MBAACC_Map[eCharacterValues::CLEN] = vCLEN_Map;
	MBAACC_Map[eCharacterValues::HLEN] = vHLEN_Map;
	MBAACC_Map[eCharacterValues::FLEN] = vFLEN_Map;
	MBAACC_Map[eCharacterValues::CRYOUGI] = vCRYOUGI_Map;
	MBAACC_Map[eCharacterValues::HRYOUGI] = vHRYOUGI_Map;
	MBAACC_Map[eCharacterValues::FRYOUGI] = vFRYOUGI_Map;
	MBAACC_Map[eCharacterValues::CWLEN] = vCWLEN_Map;
	MBAACC_Map[eCharacterValues::HWLEN] = vHWLEN_Map;
	MBAACC_Map[eCharacterValues::FWLEN] = vFWLEN_Map;
	MBAACC_Map[eCharacterValues::CNERO] = vCNERO_Map;
	MBAACC_Map[eCharacterValues::HNERO] = vHNERO_Map;
	MBAACC_Map[eCharacterValues::FNERO] = vFNERO_Map;
	MBAACC_Map[eCharacterValues::CNAC] = vCNAC_Map;
	MBAACC_Map[eCharacterValues::HNAC] = vHNAC_Map;
	MBAACC_Map[eCharacterValues::FNAC] = vFNAC_Map;
	MBAACC_Map[eCharacterValues::CKOHAMECH] = vCKOHAMECH_Map;
	MBAACC_Map[eCharacterValues::HKOHAMECH] = vHKOHAMECH_Map;
	MBAACC_Map[eCharacterValues::FKOHAMECH] = vFKOHAMECH_Map;
	MBAACC_Map[eCharacterValues::CHISUI] = vCHISUI_Map;
	MBAACC_Map[eCharacterValues::HHISUI] = vHHISUI_Map;
	MBAACC_Map[eCharacterValues::FHISUI] = vFHISUI_Map;
	MBAACC_Map[eCharacterValues::CKOHAKU] = vCKOHAKU_Map;
	MBAACC_Map[eCharacterValues::HKOHAKU] = vHKOHAKU_Map;
	MBAACC_Map[eCharacterValues::FKOHAKU] = vFKOHAKU_Map;
	MBAACC_Map[eCharacterValues::CNECOMECH] = vCNECOMECH_Map;
	MBAACC_Map[eCharacterValues::HNECOMECH] = vHNECOMECH_Map;
	MBAACC_Map[eCharacterValues::FNECOMECH] = vFNECOMECH_Map;
}

static int GetPattern(int nCharacterID, std::string sPatternName)
{
	// All patterns are positive integers.  If a pattern is not in a map, it is a 0
	return max(MBAACC_Map[eCharacterValues::UNIVERSAL][sPatternName], MBAACC_Map[nCharacterID][sPatternName]);
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
	return sMove.substr(0, 1) == "j" || sMove.find("AIR") != std::string::npos || sMove.find("DBL") != std::string::npos || sMove == "NEKO TECH" || sMove == "FASTFALL";
}