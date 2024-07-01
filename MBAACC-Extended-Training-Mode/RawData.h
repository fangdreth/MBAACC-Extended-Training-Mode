#pragma once
#include <map>
#include <string>

const std::map<std::string, int> vUniversalPatterns_Map =
{
	{ "5A", 1 }, { "5B", 2 }, { "5C", 3 }, { "2A", 4 }, { "2B", 5 }, { "2C", 6 },
	{"j.A", 7}, {"j.B", 8}, {"j.C", 9},
	{ "JUMP", 36 }, {"DBL JUMP", 39}, { "FORWARD JUMP", 35 }, {"FORWARD DBL JUMP", 38}, {"BACKWARD JUMP", 37}, {"BACKWARD DBL JUMP", 40}, {"SUPER JUMP", 360}, {"SUPER DBL JUMP", 38}
};

const std::map<std::string, int> vCVAKIHA_Map =
{

	{ "4C", 80 },
	{ "6C", 60 },
	{ "BUNKER", 91 },
	{ "5D", 70 },
	{ "2D", 71 },
	{ "j.D", 72 },
	{ "DODGE", 54 },
	{ "AIR DODGE", 270 },
	{ "HEAT", 260 },
	{ "THROW", 62 },
	{ "AIR THROW", 271 },
	{ "FORWARD DASH", 43 },
	{ "FORWARD AIRDASH", 46 },
	{ "FORWARD SUPER JUMP", 95 },
	{ "623A", 205 },
	{ "623B", 206 },
	{ "623C", 207 },
	{ "22A", 160 },
	{ "22B", 161 },
	{ "22C", 162 },
	{ "j.22A", 173 },
	{ "j.22B", 174 },
	{ "j.22C", 175 },
	{ "214A", 180 },
	{ "214B", 181 },
	{ "214C", 182 },
	{ "236A", 114 },
	{ "236B", 115 },
	{ "236C", 116 },
	{ "j.236A", 117 },
	{ "j.236B", 118 },
	{ "j.236C", 118 },
	{ "ARC DRIVE", 540 }
};

const std::map<std::string, int> vCAOKO_Map =
{

	{ "6A", 180 },
	{ "4C", 46 },
	{ "3C", 170 },
	{ "BUNKER", 96 },
	{ "5D", 54 },
	{ "2D", 55 },
	{ "j.D", 56 },
	{ "DODGE", 45 },
	{ "AIR DODGE", 270 },
	{ "HEAT", 260 },
	{ "THROW", 63 },
	{ "AIR THROW", 271 },
	{ "BACK DASH", 61 },
	{ "BACK AIRDASH", 44 },
	{ "FORWARD DASH", 60 },
	{ "FORWARD AIRDASH", 43 },
	{ "FORWARD SUPER JUMP", 58 },
	{ "623A", 68 },
	{ "623B", 69 },
	{ "623C", 70 },
	{ "j.623A", 535 },
	{ "j.623B", 536 },
	{ "j.623C", 537 },
	{ "22A", 147 },
	{ "22B", 148 },
	{ "22C", 149 },
	{ "22D", 495 },
	{ "j.22A", 172 },
	{ "j.22B", 173 },
	{ "j.22C", 174 },
	{ "214A", 72 },
	{ "214B", 73 },
	{ "214C", 74 },
	{ "j.214A", 75 },
	{ "j.214B", 76 },
	{ "j.214C", 77 },
	{ "236A", 108 },
	{ "236B", 100 },
	{ "236C", 468 },
	{ "j.236A", 172 },
	{ "j.236B", 173 },
	{ "j.236C", 174 },
	{ "421A", 174 },
	{ "421B", 208 },
	{ "421C", 562 },
	{ "624A", 234 },
	{ "624B", 231 },
	{ "624C", 235 },
	{ "ARC DRIVE", 115 }
};

const std::map<std::string, int> vCARC_Map =
{

	{ "4C", 104 },
	{ "3C", 120 },
	{ "j.2B", 107 },
	{ "j.2C", 103 },
	{ "BUNKER", 96 },
	{ "5D", 72 },
	{ "2D", 73 },
	{ "j.D", 74 },
	{ "DODGE", 64 },
	{ "AIR DODGE", 63 },
	{ "HEAT", 260 },
	{ "THROW", 55 },
	{ "AIR THROW", 271 },
	{ "BACK DASH", 43 },
	{ "BACK AIRDASH", 69 },
	{ "FORWARD DASH", 46 },
	{ "FORWARD AIRDASH", 68 },
	{ "FORWARD SUPER JUMP", 95 },
	{ "623A", 175 },
	{ "623B", 425 },
	{ "623C", 177 },
	{ "22A", 150 },
	{ "22B", 151 },
	{ "22C", 152 },
	{ "j.22A", 165 },
	{ "j.22B", 166 },
	{ "j.22C", 167 },
	{ "214A", 122 },
	{ "214B", 123 },
	{ "214C", 124 },
	{ "236A", 180 },
	{ "236B", 184 },
	{ "236C", 188 },
	{ "624A", 390 },
	{ "624B", 391 },
	{ "624C", 411 },
	{ "j.624A", 445 },
	{ "j.624B", 446 },
	{ "j.624C", 447 },
	{ "ARC DRIVE", 199 }
};

const std::map<std::string, int> vCCIEL_Map =
{

	{ "4B", 90 },
	{ "3B", 390 },
	{ "4C", 61 },
	{ "BUNKER", 78 },
	{ "5D", 72 },
	{ "2D", 73 },
	{ "j.D", 74 },
	{ "DODGE", 84 },
	{ "AIR DODGE", 270 },
	{ "HEAT", 260 },
	{ "THROW", 65 },
	{ "AIR THROW", 271 },
	{ "BACK DASH", 46 },
	{ "BACK AIRDASH", 69 },
	{ "FORWARD DASH", 45 },
	{ "FORWARD AIRDASH", 68 },
	{ "FORWARD SUPER JUMP", 80 },
	{ "623A", 80 },
	{ "623B", 486 },
	{ "623C", 487 },
	{ "j.623A", 508 },
	{ "j.623B", 509 },
	{ "j.623C", 510 },
	{ "22A", 150 },
	{ "22B", 151 },
	{ "22C", 152 },
	{ "j.22A", 156 },
	{ "j.22B", 157 },
	{ "j.22C", 158 },
	{ "214A", 415 },
	{ "214B", 121 },
	{ "214C", 122 },
	{ "j.214A", 160 },
	{ "j.214B", 161 },
	{ "j.214C", 162 },
	{ "236A", 170 },
	{ "236B", 171 },
	{ "236C", 400 },
	{ "j.236A", 95 },
	{ "j.236B", 96 },
	{ "j.236C", 97 },
	{ "ARC DRIVE", 197 }
};

const std::map<std::string, int> vCHIME_Map =
{

	{ "6C", 124 },
	{ "j.6B", 290 },
	{ "BUNKER", 72 },
	{ "5D", 56 },
	{ "2D", 57 },
	{ "j.D", 58 },
	{ "DODGE", 66 },
	{ "AIR DODGE", 67 },
	{ "HEAT", 260 },
	{ "THROW", 275 },
	{ "AIR THROW", 270 },
	{ "FLOAT", 89 },
	{ "AIR FLOAT", 90 },
	{ "BACK DASH", 61 },
	{ "BACK AIRDASH", 64 },
	{ "FORWARD DASH", 60 },
	{ "FORWARD AIRDASH", 63 },
	{ "FORWARD SUPER JUMP", 41 },
	{ "623A", 230 },
	{ "623B", 231 },
	{ "623C", 232 },
	{ "22A", 170 },
	{ "22B", 171 },
	{ "22C", 172 },
	{ "22D", 89 },
	{ "214A", 150 },
	{ "214B", 151 },
	{ "214C", 152 },
	{ "j.214A", 240 },
	{ "j.214B", 241 },
	{ "j.214C", 242 },
	{ "236A", 160 },
	{ "236B", 161 },
	{ "236C", 162 },
	{ "j.236A", 195 },
	{ "j.236B", 196 },
	{ "j.236C", 45 },
	{ "ARC DRIVE", 299 }
};

const std::map<std::string, int> vCHISUI_Map =
{

	{ "4B", 73 },
	{ "6B", 186 },
	{ "6C", 71 },
	{ "j.6B", 495 },
	{ "j.2C", 70 },
	{ "BUNKER", 181 },
	{ "5D", 81 },
	{ "2D", 82 },
	{ "j.D", 83 },
	{ "DODGE", 104 },
	{ "AIR DODGE", 290 },
	{ "HEAT", 260 },
	{ "THROW", 55 },
	{ "AIR THROW", 271 },
	{ "BACK DASH", 67 },
	{ "BACK AIRDASH", 78 },
	{ "FORWARD DASH", 65 },
	{ "FORWARD AIRDASH", 77 },
	{ "FORWARD SUPER JUMP", 68 },
	{ "623A", 160 },
	{ "623B", 161 },
	{ "623C", 162 },
	{ "22A", 410 },
	{ "22B", 411 },
	{ "22C", 412 },
	{ "214A", 500 },
	{ "214B", 501 },
	{ "214C", 502 },
	{ "236A", 370 },
	{ "236B", 371 },
	{ "236C", 372 },
	{ "j.236A", 377 },
	{ "j.236B", 378 },
	{ "j.236C", 379 },
	{ "ARC DRIVE", 199 }
};

const std::map<std::string, int> vCKOHAKU_Map =
{

	{ "4B", 381 },
	{ "6C", 466 },
	{ "BUNKER", 235 },
	{ "5D", 70 },
	{ "2D", 71 },
	{ "j.D", 72 },
	{ "DODGE", 65 },
	{ "AIR DODGE", 270 },
	{ "HEAT", 260 },
	{ "THROW", 64 },
	{ "AIR THROW", 271 },
	{ "BACK DASH", 61 },
	{ "BACK AIRDASH", 57 },
	{ "FORWARD DASH", 60 },
	{ "FORWARD AIRDASH", 56 },
	{ "FORWARD SUPER JUMP", 385 },
	{ "22A", 125 },
	{ "22B", 126 },
	{ "22C", 127 },
	{ "2222C", 128 },
	{ "22222C", 459 },
	{ "214A", 96 },
	{ "214B", 97 },
	{ "214C", 98 },
	{ "j.214A", 100 },
	{ "j.214B", 425 },
	{ "j.214C", 102 },
	{ "236A", 385 },
	{ "236B", 385 },
	{ "236C", 385 },
	{ "624C", 385 },
	{ "ARC DRIVE", 385 },
	{ "DRUG INSTALL", 385 }
};

const std::map<std::string, int> vCKOHAMECH_Map =
{

	{ "BUNKER", 235 },
	{ "5D", 70 },
	{ "2D", 71 },
	{ "j.D", 72 },
	{ "DODGE", 65 },
	{ "AIR DODGE", 270 },
	{ "HEAT", 260 },
	{ "THROW", 64 },
	{ "AIR THROW", 271 },
	{ "BACK DASH", 61 },
	{ "BACK AIRDASH", 57 },
	{ "FORWARD DASH", 60 },
	{ "FORWARD AIRDASH", 56 },
	{ "FORWARD SUPER JUMP", 63 },
	{ "22A", 685 },
	{ "22B", 686 },
	{ "22C", 687 },
	{ "214A", 600 },
	{ "214B", 601 },
	{ "214C", 602 },
	{ "214214C", 137 },
	{ "j.214A", 605 },
	{ "j.214B", 606 },
	{ "j.214C", 607 },
	{ "236A", 170 },
	{ "236B", 171 },
	{ "236C", 172 },
	{ "624C", 680 },
	{ "421A", 620 },
	{ "421B", 621 },
	{ "421C", 622 },
	{ "ARC DRIVE", 172 }
};

const std::map<std::string, int> vCKOUMA_Map =
{

	{ "6C", 206 },
	{ "BUNKER", 180 },
	{ "5D", 65 },
	{ "2D", 66 },
	{ "j.D", 67 },
	{ "DODGE", 78 },
	{ "AIR DODGE", 79 },
	{ "HEAT", 260 },
	{ "THROW", 70 },
	{ "AIR THROW", 271 },
	{ "BACK DASH", 61 },
	{ "BACK AIRDASH", 63 },
	{ "FORWARD DASH", 60 },
	{ "FORWARD AIRDASH", 62 },
	{ "FORWARD SUPER JUMP", 57 },
	{ "623A", 417 },
	{ "623B", 186 },
	{ "623C", 187 },
	{ "22A", 100 },
	{ "22B", 101 },
	{ "22C", 102 },
	{ "j.22A", 443 },
	{ "j.22B", 444 },
	{ "j.22C", 445 },
	{ "214A", 110 },
	{ "214B", 115 },
	{ "214C", 117 },
	{ "j.214A", 392 },
	{ "j.214B", 393 },
	{ "j.214C", 471 },
	{ "236A", 194 },
	{ "236B", 193 },
	{ "236C", 400 },
	{ "j.236A", 90 },
	{ "j.236B", 91 },
	{ "j.236C", 92 },
	{ "ARC DRIVE", 149 }
};

const std::map<std::string, int> vCLEN_Map =
{

	{ "4B", 210 },
	{ "3C", 66 },
	{ "j.2C", 65 },
	{ "BUNKER", 101 },
	{ "5D", 134 },
	{ "2D", 135 },
	{ "j.D", 136 },
	{ "DODGE", 68 },
	{ "AIR DODGE", 270 },
	{ "HEAT", 260 },
	{ "THROW", 105 },
	{ "AIR THROW", 271 },
	{ "BACK DASH", 61 },
	{ "BACK AIRDASH", 63 },
	{ "FORWARD DASH", 60 },
	{ "FORWARD AIRDASH", 62 },
	{ "FORWARD SUPER JUMP", 59 },
	{ "623A", 59 },
	{ "623B", 59 },
	{ "623C", 59 },
	{ "214A", 85 },
	{ "214B", 86 },
	{ "214C", 82 },
	{ "236A", 59 },
	{ "236B", 59 },
	{ "236C", 59 },
	{ "j.236A", 119 },
	{ "j.236B", 120 },
	{ "j.236C", 121 },
	{ "421A", 200 },
	{ "421B", 201 },
	{ "421C", 202 },
	{ "421D", 467 },
	{ "624C", 59 },
	{ "ARC DRIVE", 59 }
};

const std::map<std::string, int> vCMAIDS_Map;

const std::map<std::string, int> vCMECH_Map =
{

	{ "4B", 172 },
	{ "6C", 71 },
	{ "j.2C", 73 },
	{ "BUNKER", 206 },
	{ "5D", 81 },
	{ "2D", 82 },
	{ "j.D", 82 },
	{ "DODGE", 104 },
	{ "AIR DODGE", 270 },
	{ "HEAT", 260 },
	{ "THROW", 55 },
	{ "AIR THROW", 271 },
	{ "BACK DASH", 67 },
	{ "BACK AIRDASH", 78 },
	{ "FORWARD DASH", 65 },
	{ "FORWARD AIRDASH", 77 },
	{ "FORWARD SUPER JUMP", 43 },
	{ "623A", 143 },
	{ "623B", 144 },
	{ "623C", 146 },
	{ "22A", 120 },
	{ "22B", 121 },
	{ "22C", 121 },
	{ "214A", 150 },
	{ "214B", 151 },
	{ "214C", 152 },
	{ "j.214A", 174 },
	{ "j.214B", 175 },
	{ "j.214C", 176 },
	{ "236A", 410 },
	{ "236B", 411 },
	{ "236C", 411 },
	{ "j.236A", 75 },
	{ "j.236B", 76 },
	{ "j.236C", 213 },
	{ "421A", 416 },
	{ "421B", 417 },
	{ "421C", 129 },
	{ "624C", 531 },
	{ "j.624C", 532 },
	{ "ARC DRIVE", 200 }
};

const std::map<std::string, int> vCMIYAKO_Map =
{

	{ "6B", 62 },
	{ "6C", 60 },
	{ "BUNKER", 120 },
	{ "5D", 67 },
	{ "2D", 68 },
	{ "j.D", 69 },
	{ "DODGE", 57 },
	{ "AIR DODGE", 270 },
	{ "HEAT", 260 },
	{ "THROW", 74 },
	{ "AIR THROW", 271 },
	{ "BACK DASH", 55 },
	{ "BACK AIRDASH", 64 },
	{ "FORWARD DASH", 54 },
	{ "FORWARD AIRDASH", 63 },
	{ "FORWARD SUPER JUMP", 85 },
	{ "623A", 155 },
	{ "623B", 156 },
	{ "623C", 157 },
	{ "j.623A", 170 },
	{ "j.623B", 171 },
	{ "j.623C", 172 },
	{ "22A", 160 },
	{ "22B", 161 },
	{ "22C", 162 },
	{ "j.22A", 445 },
	{ "j.22B", 446 },
	{ "j.22C", 447 },
	{ "214A", 185 },
	{ "214B", 426 },
	{ "214C", 435 },
	{ "236A", 150 },
	{ "236B", 151 },
	{ "236C", 152 },
	{ "ARC DRIVE", 199 }
};

const std::map<std::string, int> vCNAC_Map =
{

	{ "6C", 79 },
	{ "3C", 78 },
	{ "BUNKER", 61 },
	{ "5D", 83 },
	{ "2D", 84 },
	{ "j.D", 85 },
	{ "DODGE", 56 },
	{ "AIR DODGE", 270 },
	{ "HEAT", 260 },
	{ "THROW", 100 },
	{ "AIR THROW", 271 },
	{ "BACK DASH", 64 },
	{ "BACK AIRDASH", 59 },
	{ "FORWARD DASH", 63 },
	{ "FORWARD AIRDASH", 58 },
	{ "FORWARD SUPER JUMP", 60 },
	{ "623A", 60 },
	{ "623B", 60 },
	{ "623C", 60 },
	{ "22A", 150 },
	{ "22B", 151 },
	{ "22C", 152 },
	{ "214A", 61 },
	{ "214B", 61 },
	{ "214C", 61 },
	{ "j.214A", 70 },
	{ "j.214B", 71 },
	{ "j.214C", 72 },
	{ "236A", 60 },
	{ "236B", 60 },
	{ "236C", 60 },
	{ "j.236A", 206 },
	{ "j.236B", 207 },
	{ "j.236C", 208 },
	{ "421A", 61 },
	{ "421B", 61 },
	{ "421C", 61 },
	{ "624C", 60 },
	{ "ARC DRIVE", 61 }
};

const std::map<std::string, int> vCNANAYA_Map =
{

	{ "BUNKER", 96 },
	{ "5D", 63 },
	{ "2D", 64 },
	{ "j.D", 65 },
	{ "DODGE", 78 },
	{ "AIR DODGE", 274 },
	{ "HEAT", 260 },
	{ "THROW", 59 },
	{ "AIR THROW", 271 },
	{ "BACK DASH", 55 },
	{ "BACK AIRDASH", 81 },
	{ "FORWARD DASH", 56 },
	{ "FORWARD AIRDASH", 80 },
	{ "FASTFALL", 82 },
	{ "FORWARD SUPER JUMP", 90 },
	{ "623A", 460 },
	{ "623B", 461 },
	{ "623C", 462 },
	{ "22A", 114 },
	{ "22B", 451 },
	{ "22C", 116 },
	{ "214A", 110 },
	{ "214B", 111 },
	{ "214C", 112 },
	{ "236A", 160 },
	{ "236B", 161 },
	{ "236C", 162 },
	{ "ARC DRIVE", 198 }
};

const std::map<std::string, int> vCNECO_Map =
{

	{ "6A", 191 },
	{ "j.2B", 190 },
	{ "j.2C", 68 },
	{ "BUNKER", 61 },
	{ "5D", 83 },
	{ "2D", 84 },
	{ "j.D", 85 },
	{ "DODGE", 56 },
	{ "AIR DODGE", 270 },
	{ "HEAT", 260 },
	{ "THROW", 100 },
	{ "AIR THROW", 271 },
	{ "BACK DASH", 64 },
	{ "BACK AIRDASH", 59 },
	{ "FORWARD DASH", 63 },
	{ "FORWARD AIRDASH", 58 },
	{ "FORWARD SUPER JUMP", 60 },
	{ "623A", 60 },
	{ "623B", 60 },
	{ "623C", 60 },
	{ "22A", 150 },
	{ "22B", 151 },
	{ "22C", 152 },
	{ "214A", 61 },
	{ "214B", 61 },
	{ "214C", 61 },
	{ "j.214A", 70 },
	{ "j.214B", 71 },
	{ "j.214C", 72 },
	{ "236A", 60 },
	{ "236B", 60 },
	{ "236C", 60 },
	{ "j.236A", 181 },
	{ "j.236B", 182 },
	{ "j.236C", 183 },
	{ "421A", 410 },
	{ "421B", 411 },
	{ "421C", 412 },
	{ "624C", 60 },
	{ "ARC DRIVE", 61 }
};

const std::map<std::string, int> vCNECOMECH_Map =
{

	{ "6C", 71 },
	{ "BUNKER", 206 },
	{ "5D", 81 },
	{ "2D", 82 },
	{ "j.D", 82 },
	{ "DODGE", 104 },
	{ "AIR DODGE", 270 },
	{ "HEAT", 260 },
	{ "THROW", 55 },
	{ "AIR THROW", 271 },
	{ "BACK DASH", 67 },
	{ "BACK AIRDASH", 78 },
	{ "FORWARD DASH", 65 },
	{ "FORWARD AIRDASH", 77 },
	{ "FORWARD SUPER JUMP", 43 },
	{ "623A", 410 },
	{ "623B", 411 },
	{ "623C", 412 },
	{ "22A", 430 },
	{ "22B", 431 },
	{ "22C", 431 },
	{ "214A", 420 },
	{ "214B", 421 },
	{ "214C", 422 },
	{ "236A", 400 },
	{ "236B", 401 },
	{ "236C", 402 },
	{ "ARC DRIVE", 200 }
};

const std::map<std::string, int> vCNERO_Map =
{

	{ "4C", 60 },
	{ "BUNKER", 90 },
	{ "5D", 73 },
	{ "2D", 74 },
	{ "j.D", 75 },
	{ "DODGE", 83 },
	{ "AIR DODGE", 270 },
	{ "HEAT", 260 },
	{ "THROW", 65 },
	{ "AIR THROW", 271 },
	{ "BACK DASH", 70 },
	{ "BACK AIRDASH", 81 },
	{ "FORWARD DASH", 71 },
	{ "FORWARD AIRDASH", 80 },
	{ "FASTFALL", 15 },
	{ "FORWARD SUPER JUMP", 95 },
	{ "623A", 406 },
	{ "623B", 405 },
	{ "623C", 407 },
	{ "214A", 130 },
	{ "214B", 131 },
	{ "214C", 133 },
	{ "236A", 120 },
	{ "236B", 121 },
	{ "236C", 122 },
	{ "421A", 105 },
	{ "421B", 106 },
	{ "421C", 107 },
	{ "624C", 150 },
	{ "ARC DRIVE", 160 }
};

const std::map<std::string, int> vCPCIEL_Map =
{

	{ "6B", 151 },
	{ "4C", 153 },
	{ "6C", 152 },
	{ "3C", 158 },
	{ "BUNKER", 75 },
	{ "5D", 70 },
	{ "2D", 71 },
	{ "j.D", 72 },
	{ "DODGE", 66 },
	{ "AIR DODGE", 67 },
	{ "HEAT", 260 },
	{ "THROW", 275 },
	{ "AIR THROW", 271 },
	{ "BACK DASH", 61 },
	{ "BACK AIRDASH", 64 },
	{ "FORWARD DASH", 60 },
	{ "FORWARD AIRDASH", 63 },
	{ "FORWARD SUPER JUMP", 80 },
	{ "623A", 105 },
	{ "623B", 106 },
	{ "623C", 107 },
	{ "214A", 130 },
	{ "214B", 131 },
	{ "214C", 132 },
	{ "j.214A", 135 },
	{ "j.214B", 136 },
	{ "j.214C", 137 },
	{ "236A", 85 },
	{ "236B", 86 },
	{ "236C", 87 },
	{ "j.236A", 115 },
	{ "j.236B", 116 },
	{ "j.236C", 117 },
	{ "ARC DRIVE", 200 }
};

const std::map<std::string, int> vCRIES_Map =
{

	{ "6B", 374 },
	{ "6C", 490 },
	{ "j.6C", 445 },
	{ "BUNKER", 102 },
	{ "5D", 56 },
	{ "2D", 57 },
	{ "j.D", 58 },
	{ "DODGE", 66 },
	{ "AIR DODGE", 271 },
	{ "HEAT", 260 },
	{ "THROW", 85 },
	{ "AIR THROW", 190 },
	{ "BACK DASH", 71 },
	{ "BACK AIRDASH", 75 },
	{ "FORWARD DASH", 70 },
	{ "FORWARD AIRDASH", 73 },
	{ "FORWARD SUPER JUMP", 90 },
	{ "623A", 425 },
	{ "623B", 426 },
	{ "623C", 427 },
	{ "j.623A", 430 },
	{ "j.623B", 431 },
	{ "j.623C", 432 },
	{ "214A", 110 },
	{ "214B", 111 },
	{ "214C", 112 },
	{ "236A", 125 },
	{ "236B", 126 },
	{ "236C", 127 },
	{ "ARC DRIVE", 399 }
};

const std::map<std::string, int> vCROA_Map =
{

	{ "6C", 386 },
	{ "BUNKER", 85 },
	{ "5D", 80 },
	{ "2D", 81 },
	{ "j.D", 82 },
	{ "DODGE", 64 },
	{ "AIR DODGE", 270 },
	{ "HEAT", 260 },
	{ "THROW", 269 },
	{ "AIR THROW", 271 },
	{ "BACK DASH", 47 },
	{ "BACK AIRDASH", 69 },
	{ "FORWARD DASH", 46 },
	{ "FORWARD AIRDASH", 68 },
	{ "FORWARD SUPER JUMP", 95 },
	{ "623A", 490 },
	{ "623B", 491 },
	{ "623C", 492 },
	{ "j.623A", 496 },
	{ "j.623B", 497 },
	{ "j.623C", 498 },
	{ "22A", 105 },
	{ "22B", 101 },
	{ "22C", 102 },
	{ "214A", 580 },
	{ "214B", 581 },
	{ "214C", 582 },
	{ "j.214A", 595 },
	{ "j.214B", 596 },
	{ "j.214C", 596 },
	{ "236A", 400 },
	{ "236B", 401 },
	{ "236C", 402 },
	{ "ARC DRIVE", 149 }
};

const std::map<std::string, int> vCRYOUGI_Map =
{

	{ "6C", 430 },
	{ "BUNKER", 85 },
	{ "5D", 75 },
	{ "2D", 76 },
	{ "j.D", 77 },
	{ "DODGE", 269 },
	{ "AIR DODGE", 270 },
	{ "HEAT", 260 },
	{ "THROW", 70 },
	{ "AIR THROW", 271 },
	{ "BACK DASH", 61 },
	{ "BACK AIRDASH", 66 },
	{ "FORWARD DASH", 60 },
	{ "FORWARD AIRDASH", 65 },
	{ "FORWARD SUPER JUMP", 95 },
	{ "623A", 310 },
	{ "623B", 311 },
	{ "623C", 312 },
	{ "22A", 720 },
	{ "22B", 721 },
	{ "22C", 732 },
	{ "j.22A", 535 },
	{ "j.22B", 536 },
	{ "j.22C", 578 },
	{ "214A", 515 },
	{ "214B", 516 },
	{ "214C", 517 },
	{ "236A", 371 },
	{ "236B", 372 },
	{ "236C", 420 },
	{ "j.236A", 385 },
	{ "j.236B", 386 },
	{ "j.236C", 387 },
	{ "421A", 340 },
	{ "421B", 330 },
	{ "421C", 336 },
	{ "624C", 705 },
	{ "NEKO TECH", 480 },
	{ "ARC DRIVE", 645 }
};

const std::map<std::string, int> vCSATSUKI_Map =
{

	{ "3C", 490 },
	{ "BUNKER", 175 },
	{ "5D", 90 },
	{ "2D", 91 },
	{ "j.D", 92 },
	{ "DODGE", 78 },
	{ "AIR DODGE", 270 },
	{ "HEAT", 260 },
	{ "THROW", 85 },
	{ "AIR THROW", 271 },
	{ "BACK DASH", 81 },
	{ "BACK AIRDASH", 56 },
	{ "FORWARD DASH", 80 },
	{ "FORWARD AIRDASH", 55 },
	{ "FORWARD SUPER JUMP", 75 },
	{ "623A", 462 },
	{ "623B", 461 },
	{ "623C", 142 },
	{ "22A", 100 },
	{ "22B", 101 },
	{ "22C", 102 },
	{ "214A", 475 },
	{ "214B", 478 },
	{ "214C", 112 },
	{ "236A", 136 },
	{ "236B", 137 },
	{ "236C", 138 },
	{ "421A", 450 },
	{ "421B", 451 },
	{ "421C", 452 },
	{ "624C", 180 },
	{ "j.624C", 420 },
	{ "ARC DRIVE", 150 }
};

const std::map<std::string, int> vCSEIFUKU_Map =
{

	{ "6B", 245 },
	{ "6C", 154 },
	{ "BUNKER", 75 },
	{ "5D", 70 },
	{ "2D", 71 },
	{ "j.D", 72 },
	{ "DODGE", 66 },
	{ "AIR DODGE", 67 },
	{ "HEAT", 260 },
	{ "THROW", 275 },
	{ "AIR THROW", 270 },
	{ "BACK DASH", 61 },
	{ "BACK AIRDASH", 64 },
	{ "FORWARD DASH", 60 },
	{ "FORWARD AIRDASH", 63 },
	{ "FORWARD SUPER JUMP", 80 },
	{ "623A", 230 },
	{ "623B", 231 },
	{ "623C", 232 },
	{ "22A", 390 },
	{ "22B", 391 },
	{ "22C", 392 },
	{ "j.22A", 385 },
	{ "j.22B", 386 },
	{ "j.22C", 387 },
	{ "214A", 180 },
	{ "214B", 181 },
	{ "214C", 182 },
	{ "j.214A", 335 },
	{ "j.214B", 336 },
	{ "j.214C", 337 },
	{ "236A", 310 },
	{ "236B", 311 },
	{ "236C", 312 },
	{ "j.236A", 313 },
	{ "j.236B", 314 },
	{ "j.236C", 315 },
	{ "624C", 200 },
	{ "ARC DRIVE", 370 }
};

const std::map<std::string, int> vCSION_Map =
{

	{ "6B", 58 },
	{ "6C", 59 },
	{ "3C", 61 },
	{ "BUNKER", 85 },
	{ "5D", 65 },
	{ "2D", 66 },
	{ "j.D", 67 },
	{ "DODGE", 132 },
	{ "AIR DODGE", 270 },
	{ "HEAT", 260 },
	{ "THROW", 62 },
	{ "AIR THROW", 271 },
	{ "BACK DASH", 56 },
	{ "BACK AIRDASH", 44 },
	{ "FORWARD DASH", 55 },
	{ "FORWARD AIRDASH", 43 },
	{ "FORWARD SUPER JUMP", 54 },
	{ "623A", 110 },
	{ "623B", 111 },
	{ "623C", 112 },
	{ "j.623A", 485 },
	{ "j.623B", 486 },
	{ "j.623C", 487 },
	{ "22D", 107 },
	{ "214A", 115 },
	{ "214B", 116 },
	{ "214C", 117 },
	{ "j.214A", 120 },
	{ "j.214B", 121 },
	{ "j.214C", 122 },
	{ "236A", 100 },
	{ "236B", 102 },
	{ "236C", 435 },
	{ "421A", 81 },
	{ "421B", 82 },
	{ "421C", 83 },
	{ "ARC DRIVE", 145 }
};

const std::map<std::string, int> vCTOHNO_Map =
{

	{ "6B", 130 },
	{ "BUNKER", 122 },
	{ "5D", 63 },
	{ "2D", 64 },
	{ "j.D", 65 },
	{ "DODGE", 78 },
	{ "AIR DODGE", 274 },
	{ "HEAT", 260 },
	{ "THROW", 59 },
	{ "AIR THROW", 271 },
	{ "BACK DASH", 55 },
	{ "BACK AIRDASH", 81 },
	{ "FORWARD DASH", 56 },
	{ "FORWARD AIRDASH", 80 },
	{ "FORWARD SUPER JUMP", 90 },
	{ "623A", 455 },
	{ "623B", 456 },
	{ "623C", 457 },
	{ "j.623A", 466 },
	{ "j.623B", 467 },
	{ "j.623C", 468 },
	{ "22A", 244 },
	{ "22B", 246 },
	{ "22C", 234 },
	{ "22D", 247 },
	{ "j.22A", 451 },
	{ "j.22B", 452 },
	{ "j.22C", 450 },
	{ "214A", 110 },
	{ "214B", 111 },
	{ "214C", 112 },
	{ "236A", 105 },
	{ "236B", 106 },
	{ "236C", 107 },
	{ "ARC DRIVE", 199 }
};

const std::map<std::string, int> vCAKIHA_Map =
{

	{ "4C", 80 },
	{ "6C", 60 },
	{ "j.2C", 58 },
	{ "BUNKER", 91 },
	{ "5D", 70 },
	{ "2D", 71 },
	{ "j.D", 72 },
	{ "DODGE", 54 },
	{ "AIR DODGE", 270 },
	{ "HEAT", 260 },
	{ "THROW", 62 },
	{ "AIR THROW", 271 },
	{ "BACK DASH", 45 },
	{ "BACK AIRDASH", 47 },
	{ "FORWARD DASH", 43 },
	{ "FORWARD AIRDASH", 46 },
	{ "FORWARD SUPER JUMP", 85 },
	{ "623A", 205 },
	{ "623B", 206 },
	{ "623C", 207 },
	{ "22A", 160 },
	{ "22B", 161 },
	{ "22C", 162 },
	{ "j.22A", 173 },
	{ "j.22B", 174 },
	{ "j.22C", 175 },
	{ "214A", 180 },
	{ "214B", 181 },
	{ "214C", 182 },
	{ "236A", 114 },
	{ "236B", 115 },
	{ "236C", 116 },
	{ "j.236A", 117 },
	{ "j.236B", 118 },
	{ "j.236C", 119 },
	{ "624A", 373 },
	{ "624B", 374 },
	{ "624C", 375 },
	{ "ARC DRIVE", 219 }
};

const std::map<std::string, int> vCVSION_Map =
{

	{ "4B", 525 },
	{ "6C", 225 },
	{ "3C", 61 },
	{ "j.6A", 20 },
	{ "j.2B", 80 },
	{ "BUNKER", 85 },
	{ "5D", 65 },
	{ "2D", 66 },
	{ "j.D", 67 },
	{ "DODGE", 132 },
	{ "AIR DODGE", 270 },
	{ "HEAT", 260 },
	{ "THROW", 77 },
	{ "AIR THROW", 271 },
	{ "BACK DASH", 56 },
	{ "BACK AIRDASH", 44 },
	{ "FORWARD DASH", 57 },
	{ "FORWARD AIRDASH", 43 },
	{ "FORWARD SUPER JUMP", 90 },
	{ "623A", 125 },
	{ "623B", 126 },
	{ "623C", 445 },
	{ "214A", 165 },
	{ "214B", 166 },
	{ "214C", 167 },
	{ "236A", 215 },
	{ "236B", 216 },
	{ "236C", 217 },
	{ "421A", 531 },
	{ "421B", 532 },
	{ "421C", 533 },
	{ "ARC DRIVE", 204 }
};

const std::map<std::string, int> vCWARA_Map =
{

	{ "6C", 72 },
	{ "j.2C", 73 },
	{ "BUNKER", 130 },
	{ "5D", 62 },
	{ "2D", 63 },
	{ "j.D", 64 },
	{ "DODGE", 69 },
	{ "AIR DODGE", 270 },
	{ "HEAT", 260 },
	{ "THROW", 45 },
	{ "AIR THROW", 271 },
	{ "BACK DASH", 55 },
	{ "BACK AIRDASH", 59 },
	{ "FORWARD DASH", 56 },
	{ "FORWARD AIRDASH", 58 },
	{ "FORWARD SUPER JUMP", 95 },
	{ "623A", 120 },
	{ "623B", 121 },
	{ "623C", 122 },
	{ "22A", 100 },
	{ "22B", 101 },
	{ "22C", 102 },
	{ "j.22A", 470 },
	{ "j.22B", 471 },
	{ "j.22C", 472 },
	{ "214A", 190 },
	{ "214B", 191 },
	{ "214C", 192 },
	{ "j.214A", 475 },
	{ "j.214C", 477 },
	{ "236A", 180 },
	{ "236B", 181 },
	{ "236C", 182 },
	{ "j.236A", 450 },
	{ "j.236B", 451 },
	{ "j.236C", 452 },
	{ "421A", 170 },
	{ "421B", 171 },
	{ "421C", 172 },
	{ "j.421A", 75 },
	{ "j.421B", 77 },
	{ "j.421C", 89 },
	{ "624C", 193 },
	{ "j.624C", 478 },
	{ "ARC DRIVE", 149 }
};

const std::map<std::string, int> vCWARC_Map =
{

	{ "6C", 500 },
	{ "3C", 49 },
	{ "j.2C", 103 },
	{ "BUNKER", 78 },
	{ "5D", 72 },
	{ "2D", 73 },
	{ "j.D", 74 },
	{ "DODGE", 64 },
	{ "AIR DODGE", 270 },
	{ "HEAT", 260 },
	{ "THROW", 55 },
	{ "AIR THROW", 271 },
	{ "BACK DASH", 43 },
	{ "BACK AIRDASH", 69 },
	{ "FORWARD DASH", 46 },
	{ "FORWARD AIRDASH", 68 },
	{ "FORWARD SUPER JUMP", 95 },
	{ "623A", 155 },
	{ "623B", 156 },
	{ "623C", 157 },
	{ "j.623A", 165 },
	{ "j.623B", 167 },
	{ "j.623C", 166 },
	{ "214A", 112 },
	{ "214B", 113 },
	{ "214C", 114 },
	{ "j.214A", 124 },
	{ "j.214B", 125 },
	{ "j.214C", 126 },
	{ "236A", 150 },
	{ "236B", 151 },
	{ "236C", 152 },
	{ "421A", 160 },
	{ "421B", 161 },
	{ "421C", 162 },
	{ "624C", 505 },
	{ "ARC DRIVE", 223 }
};

const std::map<std::string, int> vCWLEN_Map =
{

	{ "4B", 47 },
	{ "3C", 370 },
	{ "j.2C", 56 },
	{ "BUNKER", 101 },
	{ "5D", 134 },
	{ "2D", 135 },
	{ "j.D", 136 },
	{ "DODGE", 68 },
	{ "AIR DODGE", 270 },
	{ "HEAT", 260 },
	{ "THROW", 106 },
	{ "AIR THROW", 271 },
	{ "BACK DASH", 61 },
	{ "BACK AIRDASH", 63 },
	{ "FORWARD DASH", 60 },
	{ "FORWARD AIRDASH", 62 },
	{ "FORWARD SUPER JUMP", 46 },
	{ "623A", 195 },
	{ "623B", 196 },
	{ "623C", 197 },
	{ "22A", 182 },
	{ "22B", 183 },
	{ "22C", 184 },
	{ "22D", 185 },
	{ "214A", 142 },
	{ "214B", 143 },
	{ "214C", 144 },
	{ "j.214A", 179 },
	{ "j.214B", 147 },
	{ "j.214C", 148 },
	{ "236A", 115 },
	{ "236B", 116 },
	{ "236C", 117 },
	{ "j.236A", 119 },
	{ "j.236B", 120 },
	{ "j.236C", 121 },
	{ "421A", 405 },
	{ "421B", 406 },
	{ "421C", 407 },
	{ "624A", 110 },
	{ "624B", 111 },
	{ "624C", 112 },
	{ "ARC DRIVE", 149 }
};

const std::map<std::string, int> vFAKIHA_Map =
{
	{ "4C", 80 }, { "6C", 60 }, { "3C", 445 },
	{ "BUNKER", 91 }, { "5D", 70 }, { "2D", 71 }, { "THROW", 62 },
	{ "BACK DASH", 45 }, { "FORWARD DASH", 43 }, { "FORWARD SUPER JUMP", 85 },
	{ "623A", 205 }, { "623B", 206 }, { "623C", 207 },
	{ "22A", 420 }, { "22B", 421 }, { "22C", 422 },
	{ "214A", 400 }, { "214B", 401 }, { "214C", 402 },
	{ "236A", 114 }, { "236B", 115 }, { "236C", 116 },
	{ "421A", 437 }, { "421B", 438 }, { "421C", 439 },

	 { "HEAT", 260 }, { "ARC DRIVE", 219 }, { "ANOTHER ARC DRIVE", 219 },
	{"filler",0}
};

const std::map<std::string, int> vFAOKO_Map =
{
	{ "4C", 46 }, { "3C", 170 },
	{ "BUNKER", 96 }, { "5D", 54 }, { "2D", 55 }, { "THROW", 63 },
	{ "BACK DASH", 61 }, { "FORWARD DASH", 60 }, { "FORWARD SUPER JUMP", 58 },
	{ "623A", 400 }, { "623B", 401 }, { "623C", 402 },
	{ "22A", 147 }, { "22B", 148 }, { "22C", 149 },
	{ "214A", 72 }, { "214B", 73 }, { "214C", 475 },
	{ "236A", 435 }, { "236B", 436 }, { "236C", 470 },
	{ "421A", 511 }, { "421B", 512 }, { "421C", 530 },
	{ "624A", 575 }, { "624B", 576 }, { "624C", 577 },
	 { "HEAT", 260 }, { "ARC DRIVE", 115 }, { "ANOTHER ARC DRIVE", 265 },
	{"filler",0}
};

const std::map<std::string, int> vFARC_Map =
{
	{ "6C", 459 }, { "3C", 440 },
	{ "BUNKER", 96 }, { "5D", 72 }, { "2D", 73 }, { "THROW", 55 },
	{ "BACK DASH", 43 }, { "FORWARD DASH", 46 }, { "FORWARD SUPER JUMP", 95 },
	{ "623A", 175 }, { "623B", 425 }, { "623C", 177 },
	{ "22A", 150 }, { "22B", 151 }, { "22C", 152 },
	{ "214A", 122 }, { "214B", 123 }, { "214C", 124 },
	{ "236A", 180 }, { "236B", 181 }, { "236C", 441 },
	{ "421A", 155 }, { "421B", 156 }, { "421C", 157 },

	 { "HEAT", 260 }, { "ARC DRIVE", 199 }, { "ANOTHER ARC DRIVE", 265 },
	{"filler",0}
};

const std::map<std::string, int> vFHIME_Map =
{
	{ "6C", 124 },
	{ "BUNKER", 72 }, { "5D", 56 }, { "2D", 57 }, { "THROW", 275 },
	{ "BACK DASH", 61 }, { "FORWARD DASH", 60 }, { "FORWARD SUPER JUMP", 41 },
	{ "623A", 230 }, { "623B", 231 }, { "623C", 232 },
	{ "22A", 170 }, { "22B", 171 }, { "22C", 172 }, { "22D", 89 },
	{ "214A", 140 }, { "214B", 141 }, { "214C", 142 },
	{ "236A", 160 }, { "236B", 161 }, { "236C", 162 },


	 { "HEAT", 260 }, { "ARC DRIVE", 299 }, { "ANOTHER ARC DRIVE", 265 },
	{"filler",0}
};

const std::map<std::string, int> vFHISUI_Map =
{
	{ "6A", 299 }, { "4B", 73 }, { "6C", 526 }, { "3C", 186 },
	{ "BUNKER", 181 }, { "5D", 81 }, { "2D", 82 }, { "THROW", 55 },
	{ "BACK DASH", 67 }, { "FORWARD DASH", 67 }, { "FORWARD SUPER JUMP", 68 },
	{ "623A", 401 }, { "623B", 403 }, { "623C", 402 },
	{ "22A", 430 }, { "22B", 444 }, { "22C", 437 },
	{ "214A", 500 }, { "214B", 501 }, { "214C", 502 },
	{ "236A", 370 }, { "236B", 371 }, { "236C", 372 },


	 { "HEAT", 260 }, { "ARC DRIVE", 199 }, { "ANOTHER ARC DRIVE", 199 },
	{"filler",0}
};

const std::map<std::string, int> vFKOHAKU_Map =
{
	{ "4B", 465 }, { "3C", 520 },
	{ "BUNKER", 235 }, { "5D", 70 }, { "2D", 71 }, { "THROW", 64 },
	{ "BACK DASH", 61 }, { "FORWARD DASH", 60 }, { "FORWARD SUPER JUMP", 63 },

	{ "22A", 125 }, { "22B", 126 }, { "22C", 127 }, { "2222C", 128 }, { "22222C", 459 },
	{ "214A", 402 }, { "214B", 388 }, { "214C", 400 },
	{ "236A", 470 }, { "236B", 471 }, { "236C", 472 },


	 { "HEAT", 260 }, { "ARC DRIVE", 200 }, { "ANOTHER ARC DRIVE", 200 },
	{"filler",0}
};

const std::map<std::string, int> vFKOUMA_Map =
{
	{ "3B", 2 }, { "6C", 76 },
	{ "BUNKER", 180 }, { "5D", 65 }, { "2D", 66 }, { "THROW", 70 },
	{ "BACK DASH", 61 }, { "FORWARD DASH", 60 }, { "FORWARD SUPER JUMP", 57 },
	{ "623A", 465 }, { "623B", 466 }, { "623C", 467 },
	{ "22A", 100 }, { "22B", 101 }, { "22C", 102 },
	{ "214A", 110 }, { "214B", 115 }, { "214C", 117 },
	{ "236A", 125 }, { "236B", 128 }, { "236C", 131 },
	{ "421A", 220 }, { "421B", 221 }, { "421C", 222 },

	 { "HEAT", 260 }, { "ARC DRIVE", 149 }, { "ANOTHER ARC DRIVE", 265 },
	{"filler",0}
};

const std::map<std::string, int> vFLEN_Map =
{
	{ "4B", 210 }, { "3C", 66 },
	{ "BUNKER", 101 }, { "5D", 134 }, { "2D", 135 }, { "THROW", 105 },
	{ "BACK DASH", 61 }, { "FORWARD DASH", 60 }, { "FORWARD SUPER JUMP", 59 },
	{ "623A", 59 }, { "623B", 59 }, { "623C", 59 },

	{ "214A", 390 }, { "214B", 391 }, { "214C", 392 },
	{ "236A", 59 }, { "236B", 59 }, { "236C", 59 },


	 { "HEAT", 260 }, { "ARC DRIVE", 59 }, { "ANOTHER ARC DRIVE", 59 },
	{"filler",0}
};

const std::map<std::string, int> vFMAIDS_Map =
{
	{"None",0}
};

const std::map<std::string, int> vFMECH_Map =
{
	{ "6C", 71 },
	{ "BUNKER", 206 }, { "5D", 81 }, { "2D", 82 }, { "THROW", 55 },
	{ "BACK DASH", 67 }, { "FORWARD DASH", 65 }, { "FORWARD SUPER JUMP", 43 },
	{ "623A", 484 }, { "623B", 485 }, { "623C", 486 },
	{ "22A", 420 }, { "22B", 426 }, { "22C", 432 },
	{ "214A", 510 }, { "214B", 511 }, { "214C", 463 },
	{ "236A", 488 }, { "236B", 489 }, { "236C", 461 },


	 { "HEAT", 260 }, { "ARC DRIVE", 200 }, { "ANOTHER ARC DRIVE", 200 },
	{"filler",0}
};

const std::map<std::string, int> vFMIYAKO_Map =
{
	{ "6C", 405 },
	{ "BUNKER", 120 }, { "5D", 67 }, { "2D", 68 }, { "THROW", 74 },
	{ "BACK DASH", 55 }, { "FORWARD DASH", 54 }, { "FORWARD SUPER JUMP", 85 },
	{ "623A", 450 }, { "623B", 452 }, { "623C", 157 },
	{ "22A", 160 }, { "22B", 161 }, { "22C", 140 },
	{ "214A", 185 }, { "214B", 426 }, { "214C", 435 },
	{ "236A", 407 }, { "236B", 408 }, { "236C", 409 },
	{ "421A", 411 }, { "421B", 412 }, { "421C", 413 },

	 { "HEAT", 260 }, { "ARC DRIVE", 199 }, { "ANOTHER ARC DRIVE", 199 },
	{"filler",0}
};

const std::map<std::string, int> vFNAC_Map =
{
	{ "6C", 79 },
	{ "BUNKER", 169 }, { "5D", 83 }, { "2D", 84 }, { "THROW", 100 },
	{ "BACK DASH", 64 }, { "FORWARD DASH", 63 }, { "FORWARD SUPER JUMP", 43 },
	{ "623A", 172 }, { "623B", 173 }, { "623C", 174 },
	{ "22A", 480 }, { "22B", 485 }, { "22C", 410 },
	{ "214A", 515 }, { "214B", 516 }, { "214C", 517 },
	{ "236A", 570 }, { "236B", 571 }, { "236C", 572 },


	 { "HEAT", 260 }, { "ARC DRIVE", 227 }, { "ANOTHER ARC DRIVE", 227 },
	{"filler",0}
};

const std::map<std::string, int> vFNANAYA_Map =
{

	{ "BUNKER", 96 }, { "5D", 63 }, { "2D", 64 }, { "THROW", 59 },
	{ "BACK DASH", 55 }, { "FORWARD DASH", 56 }, { "FORWARD SUPER JUMP", 90 },
	{ "623A", 118 }, { "623B", 119 }, { "623C", 120 },
	{ "22A", 114 }, { "22B", 115 }, { "22C", 116 },
	{ "214A", 110 }, { "214B", 111 }, { "214C", 112 },
	{ "236A", 160 }, { "236B", 161 }, { "236C", 162 },


	 { "HEAT", 260 }, { "ARC DRIVE", 198 }, { "ANOTHER ARC DRIVE", 198 },
	{"filler",0}
};

const std::map<std::string, int> vFNECO_Map =
{
	{ "6A", 191 },
	{ "BUNKER", 61 }, { "5D", 83 }, { "2D", 84 }, { "THROW", 100 },
	{ "BACK DASH", 64 }, { "FORWARD DASH", 63 }, { "FORWARD SUPER JUMP", 60 },
	{ "623A", 60 }, { "623B", 60 }, { "623C", 60 },
	{ "22A", 150 }, { "22B", 151 }, { "22C", 152 },
	{ "214A", 61 }, { "214B", 61 }, { "214C", 61 },
	{ "236A", 60 }, { "236B", 60 }, { "236C", 60 },


	 { "HEAT", 260 }, { "ARC DRIVE", 61 }, { "ANOTHER ARC DRIVE", 61 },
	{"filler",0}
};

const std::map<std::string, int> vFNECOMECH_Map =
{
	{ "6C", 71 },
	{ "BUNKER", 206 }, { "5D", 81 }, { "2D", 82 }, { "THROW", 55 },
	{ "BACK DASH", 67 }, { "FORWARD DASH", 65 }, { "FORWARD SUPER JUMP", 43 },
	{ "623A", 410 }, { "623B", 411 }, { "623C", 412 },
	{ "22A", 430 }, { "22B", 431 }, { "22C", 432 },
	{ "214A", 420 }, { "214B", 421 }, { "214C", 422 },
	{ "236A", 400 }, { "236B", 401 }, { "236C", 402 },


	 { "HEAT", 260 }, { "ARC DRIVE", 200 }, { "ANOTHER ARC DRIVE", 200 },
	{"filler",0}
};

const std::map<std::string, int> vFNERO_Map =
{
	{ "4C", 60 }, { "6C", 500 }, { "3C", 401 },
	{ "BUNKER", 90 }, { "5D", 73 }, { "2D", 74 }, { "THROW", 65 },
	{ "BACK DASH", 70 }, { "FORWARD DASH", 71 }, { "FORWARD SUPER JUMP", 95 },


	{ "214A", 480 }, { "214B", 485 }, { "214C", 505 },
	{ "236A", 120 }, { "236B", 420 }, { "236C", 122 },
	{ "421A", 452 }, { "421B", 451 }, { "421C", 490 },
	{ "624C", 450 },
	 { "HEAT", 260 }, { "ARC DRIVE", 182 }, { "ANOTHER ARC DRIVE", 265 },
	{"filler",0}
};

const std::map<std::string, int> vFPCIEL_Map =
{
	{ "6B", 154 }, { "3B", 151 }, { "3C", 158 },
	{ "BUNKER", 75 }, { "5D", 70 }, { "2D", 71 }, { "THROW", 275 },
	{ "BACK DASH", 61 }, { "FORWARD DASH", 60 }, { "FORWARD SUPER JUMP", 80 },
	{ "623A", 105 }, { "623B", 106 }, { "623C", 107 },

	{ "214A", 120 }, { "214B", 121 }, { "214C", 122 },
	{ "236A", 110 }, { "236B", 111 }, { "236C", 112 },

	{ "624A", 100 }, { "624B", 101 }, { "624C", 102 },
	 { "HEAT", 260 }, { "ARC DRIVE", 200 }, { "ANOTHER ARC DRIVE", 200 },
	{"filler",0}
};

const std::map<std::string, int> vFRIES_Map =
{
	{ "6B", 374 }, { "6C", 92 },
	{ "BUNKER", 102 }, { "5D", 56 }, { "2D", 57 }, { "THROW", 85 },
	{ "BACK DASH", 71 }, { "FORWARD DASH", 70 }, { "FORWARD SUPER JUMP", 90 },
	{ "623A", 440 }, { "623B", 441 }, { "623C", 442 },

	{ "214A", 437 }, { "214B", 438 }, { "214C", 112 },
	{ "236A", 175 }, { "236B", 179 }, { "236C", 485 },


	 { "HEAT", 260 }, { "ARC DRIVE", 399 }, { "ANOTHER ARC DRIVE", 399 },
	{"filler",0}
};

const std::map<std::string, int> vFROA_Map =
{
	{ "6C", 386 },
	{ "BUNKER", 85 }, { "5D", 80 }, { "2D", 81 }, { "THROW", 269 },
	{ "BACK DASH", 47 }, { "FORWARD DASH", 46 }, { "FORWARD SUPER JUMP", 95 },
	{ "623A", 490 }, { "623B", 491 }, { "623C", 492 },

	{ "214A", 408 }, { "214B", 409 }, { "214C", 445 },
	{ "236A", 477 }, { "236B", 478 }, { "236C", 482 },


	 { "HEAT", 260 }, { "ARC DRIVE", 149 }, { "ANOTHER ARC DRIVE", 149 },
	{"filler",0}
};

const std::map<std::string, int> vFRYOUGI_Map =
{
	{ "4C", 194 }, { "3C", 365 },
	{ "BUNKER", 85 }, { "5D", 75 }, { "2D", 76 }, { "THROW", 70 },
	{ "BACK DASH", 61 }, { "FORWARD DASH", 59 }, { "FORWARD SUPER JUMP", 95 },
	{ "623A", 590 }, { "623B", 591 }, { "623C", 592 },
	{ "22A", 740 }, { "22B", 741 }, { "22C", 742 },
	{ "214A", 515 }, { "214B", 516 }, { "214C", 517 },
	{ "236A", 371 }, { "236B", 372 }, { "236C", 397 },
	{ "421B", 594 }, { "421C", 595 },

	 { "HEAT", 260 }, { "ARC DRIVE", 645 }, { "ANOTHER ARC DRIVE", 645 },
	{"filler",0}
};

const std::map<std::string, int> vFSATSUKI_Map =
{
	{ "3C", 490 },
	{ "BUNKER", 175 }, { "5D", 90 }, { "2D", 91 }, { "THROW", 85 },
	{ "BACK DASH", 81 }, { "FORWARD DASH", 80 }, { "FORWARD SUPER JUMP", 75 },
	{ "623A", 462 }, { "623B", 461 }, { "623C", 142 },
	{ "22A", 380 }, { "22B", 381 }, { "22C", 382 },
	{ "214A", 475 }, { "214B", 476 }, { "214C", 112 },
	{ "236A", 508 }, { "236B", 509 }, { "236C", 510 },
	{ "421A", 450 }, { "421B", 451 }, { "421C", 452 },
	{ "624C", 180 },
	 { "HEAT", 260 }, { "ARC DRIVE", 150 }, { "ANOTHER ARC DRIVE", 150 },
	{"filler",0}
};

const std::map<std::string, int> vFSEIFUKU_Map =
{
	{ "3C", 152 },
	{ "BUNKER", 75 }, { "5D", 70 }, { "2D", 71 }, { "THROW", 275 },
	{ "BACK DASH", 61 }, { "FORWARD DASH", 60 }, { "FORWARD SUPER JUMP", 80 },
	{ "623A", 230 }, { "623B", 231 }, { "623C", 232 },
	{ "22A", 220 }, { "22B", 221 }, { "22C", 222 },
	{ "214A", 480 }, { "214B", 481 }, { "214C", 482 },
	{ "236A", 440 }, { "236B", 441 }, { "236C", 442 },

	{ "624C", 200 },
	 { "HEAT", 260 }, { "ARC DRIVE", 370 }, { "ANOTHER ARC DRIVE", 370 },
	{"filler",0}
};

const std::map<std::string, int> vFSION_Map =
{
	{ "6B", 58 }, { "3B", 61 }, { "6C", 442 }, { "3C", 515 },
	{ "BUNKER", 85 }, { "5D", 65 }, { "2D", 66 }, { "THROW", 62 },
	{ "BACK DASH", 56 }, { "FORWARD DASH", 55 }, { "FORWARD SUPER JUMP", 54 },
	{ "623A", 110 }, { "623B", 55 },
	{ "22A", 428 }, { "22B", 386 }, { "22C", 429 }, { "22D", 65 },
	{ "214A", 115 }, { "214B", 389 }, { "214C", 117 },
	{ "236A", 411 }, { "236B", 412 }, { "236C", 413 },


	 { "HEAT", 260 }, { "ARC DRIVE", 145 }, { "ANOTHER ARC DRIVE", 145 },
	{"filler",0}
};

const std::map<std::string, int> vFTOHNO_Map =
{
	{ "6B", 191 },
	{ "BUNKER", 122 }, { "5D", 63 }, { "2D", 64 }, { "THROW", 59 },
	{ "BACK DASH", 55 }, { "FORWARD DASH", 56 }, { "FORWARD SUPER JUMP", 90 },
	{ "623A", 400 }, { "623B", 56 }, { "623C", 399 },
	{ "22A", 244 }, { "22B", 246 }, { "22C", 234 }, { "22D", 247 },
	{ "214A", 110 }, { "214B", 111 }, { "214C", 112 },
	{ "236A", 405 }, { "236B", 406 }, { "236C", 407 },


	{ "ANOTHER ARC DRIVE", 204 },
	{"filler",0}
};

const std::map<std::string, int> vFVAKIHA_Map =
{
	{ "4C", 80 }, { "6C", 60 }, { "3C", 445 },
	{ "BUNKER", 91 }, { "5D", 70 }, { "2D", 71 }, { "THROW", 62 },
	{ "BACK DASH", 45 }, { "FORWARD DASH", 43 }, { "FORWARD SUPER JUMP", 95 },
	{ "623A", 554 }, { "623B", 206 }, { "623C", 207 },
	{ "22A", 400 }, { "22B", 401 }, { "22C", 402 },
	{ "214A", 180 }, { "214B", 181 }, { "214C", 182 },
	{ "236B", 506 }, { "236C", 507 },


	 { "HEAT", 260 }, { "ARC DRIVE", 540 }, { "ANOTHER ARC DRIVE", 540 },
	{"filler",0}
};

const std::map<std::string, int> vFVSION_Map =
{
	{ "6B", 470 }, { "6C", 225 },
	{ "BUNKER", 85 }, { "5D", 65 }, { "2D", 66 }, { "THROW", 77 },
	{ "BACK DASH", 56 }, { "FORWARD DASH", 57 }, { "FORWARD SUPER JUMP", 90 },
	{ "623A", 125 }, { "623B", 126 }, { "623C", 445 },
	{ "22A", 433 }, { "22B", 433 }, { "22C", 434 },
	{ "214A", 471 }, { "214B", 472 }, { "214C", 473 },
	{ "236A", 410 }, { "236B", 411 }, { "236C", 412 },


	 { "HEAT", 260 }, { "ARC DRIVE", 204 }, { "ANOTHER ARC DRIVE", 204 },
	{"filler",0}
};

const std::map<std::string, int> vFWARA_Map =
{
	{ "6C", 72 },
	{ "BUNKER", 130 }, { "5D", 62 }, { "2D", 63 }, { "THROW", 45 },
	{ "BACK DASH", 55 }, { "FORWARD DASH", 56 }, { "FORWARD SUPER JUMP", 95 },
	{ "623A", 410 }, { "623B", 411 }, { "623C", 412 },
	{ "22A", 100 }, { "22B", 101 }, { "22C", 102 },
	{ "214A", 398 }, { "214B", 399 }, { "214C", 400 },
	{ "236A", 425 }, { "236B", 426 }, { "236C", 427 },
	{ "421A", 170 }, { "421B", 171 }, { "421C", 172 },

	 { "HEAT", 260 }, { "ARC DRIVE", 149 }, { "ANOTHER ARC DRIVE", 149 },
	{"filler",0}
};

const std::map<std::string, int> vFWARC_Map =
{
	{ "3C", 49 },
	{ "BUNKER", 78 }, { "5D", 72 }, { "2D", 73 }, { "THROW", 55 },
	{ "BACK DASH", 43 }, { "FORWARD DASH", 46 }, { "FORWARD SUPER JUMP", 95 },
	{ "623A", 155 }, { "623B", 156 }, { "623C", 157 },

	{ "214A", 112 }, { "214B", 113 }, { "214C", 114 },
	{ "236A", 545 }, { "236B", 540 }, { "236C", 547 },
	{ "421A", 160 }, { "421B", 161 }, { "421C", 162 },

	 { "HEAT", 260 }, { "ARC DRIVE", 224 }, { "ANOTHER ARC DRIVE", 223 },
	{"filler",0}
};

const std::map<std::string, int> vFWLEN_Map =
{

	{ "BUNKER", 101 }, { "5D", 134 }, { "2D", 135 }, { "THROW", 106 },
	{ "BACK DASH", 61 }, { "FORWARD DASH", 60 }, { "FORWARD SUPER JUMP", 46 },
	{ "623A", 480 }, { "623B", 481 }, { "623C", 482 },
	{ "22A", 477 }, { "22B", 477 }, { "22C", 476 }, { "22D", 185 },
	{ "214A", 486 }, { "214B", 487 }, { "214C", 485 },
	{ "236A", 390 }, { "236B", 391 }, { "236C", 392 },


	 { "HEAT", 260 }, { "ARC DRIVE", 150 }, { "ANOTHER ARC DRIVE", 149 },
	{"filler",0}
};



const std::map<std::string, int> vFCIEL_Map =
{
	{ "4B", 90 }, { "3B", 390 }, { "4C", 61 }, { "3C", 475 },
	{ "BUNKER", 78 }, { "5D", 72 }, { "2D", 73 }, { "THROW", 65 },
	{ "BACK DASH", 46 }, { "FORWARD DASH", 45 }, { "FORWARD SUPER JUMP", 80 },
	{ "623A", 485 }, { "623B", 486 }, { "623C", 487 },
	{ "22A", 150 }, { "22B", 151 }, { "22C", 152 },
	{ "214A", 420 }, { "214B", 430 }, { "214C", 425 },
	{ "236A", 470 }, { "236B", 471 }, { "236C", 472 },

	{ "624A", 458 }, { "624B", 459 }, { "624C", 460 },
	 { "HEAT", 260 }, { "ARC DRIVE", 197 },
	{"filler",0}
};

const std::map<std::string, int> vFKOHAMECH_Map =
{
	{ "4B", 465 }, { "3C", 520 },
	{ "BUNKER", 235 }, { "5D", 70 }, { "2D", 71 }, { "THROW", 64 },
	{ "BACK DASH", 61 }, { "FORWARD DASH", 60 }, { "FORWARD SUPER JUMP", 63 },
	{ "623A", 620 }, { "623B", 621 }, { "623C", 622 },
	{ "22A", 685 }, { "22B", 686 }, { "22C", 687 },
	{ "214A", 600 }, { "214B", 601 }, { "214C", 602 },
	{ "236A", 470 }, { "236B", 471 }, { "236C", 472 },


	 { "HEAT", 260 }, { "ARC DRIVE", 200 },
	{"filler",0}
};

const std::map<std::string, int> vHAKIHA_Map =
{
	{ "4C", 80 }, { "6C", 60 },
	{ "BUNKER", 91 }, { "5D", 70 }, { "2D", 71 }, { "DODGE", 54 }, { "THROW", 62 },
	{ "BACK DASH", 45 }, { "FORWARD DASH", 43 }, { "FORWARD SUPER JUMP", 85 },
	{ "623A", 205 }, { "623B", 206 }, { "623C", 207 },
	{ "22A", 160 }, { "22B", 161 }, { "22C", 162 },
	{ "214A", 180 }, { "214B", 181 }, { "214C", 182 },
	{ "236A", 449 }, { "236B", 450 }, { "236C", 451 },


	{ "ARC DRIVE", 219 },
	{"filler",0}
};

const std::map<std::string, int> vHAOKO_Map =
{
	{ "4C", 46 }, { "3C", 170 },
	{ "BUNKER", 96 }, { "5D", 54 }, { "2D", 55 }, { "DODGE", 45 }, { "THROW", 63 },
	{ "BACK DASH", 61 }, { "FORWARD DASH", 60 }, { "FORWARD SUPER JUMP", 58 },
	{ "623A", 68 }, { "623B", 69 }, { "623C", 70 },
	{ "22A", 147 }, { "22B", 148 }, { "22C", 149 },
	{ "214A", 445 }, { "214B", 446 }, { "214C", 447 },
	{ "236A", 108 }, { "236B", 100 }, { "236C", 142 },
	{ "421A", 207 }, { "421B", 208 }, { "421C", 209 },
	{ "624A", 234 }, { "624B", 231 }, { "624C", 235 },
	{ "ARC DRIVE", 115 },
	{"filler",0}
};

const std::map<std::string, int> vHARC_Map =
{
	{ "4C", 104 }, { "3C", 120 },
	{ "BUNKER", 96 }, { "5D", 72 }, { "2D", 73 }, { "DODGE", 64 }, { "THROW", 55 },
	{ "BACK DASH", 43 }, { "FORWARD DASH", 46 }, { "FORWARD SUPER JUMP", 95 },
	{ "623A", 175 }, { "623B", 425 }, { "623C", 177 },
	{ "22A", 150 }, { "22B", 151 }, { "22C", 152 },
	{ "214A", 122 }, { "214B", 123 }, { "214C", 124 },
	{ "236A", 180 }, { "236B", 184 }, { "236C", 188 },
	{ "421A", 155 }, { "421B", 5 }, { "421C", 157 },

	{ "ARC DRIVE", 199 },
	{"filler",0}
};

const std::map<std::string, int> vHCIEL_Map =
{
	{ "4B", 90 }, { "3B", 390 }, { "4C", 61 },
	{ "BUNKER", 78 }, { "5D", 72 }, { "2D", 73 }, { "DODGE", 84 }, { "THROW", 65 },
	{ "BACK DASH", 46 }, { "FORWARD DASH", 45 }, { "FORWARD SUPER JUMP", 80 },
	{ "623A", 225 }, { "623B", 226 }, { "623C", 227 },
	{ "22A", 150 }, { "22B", 151 }, { "22C", 152 },
	{ "214A", 120 }, { "214B", 121 }, { "214C", 122 },
	{ "236A", 505 }, { "236B", 506 }, { "236C", 400 },

	{ "624A", 517 }, { "624B", 516 }, { "624C", 515 },
	{ "ARC DRIVE", 197 },
	{"filler",0}
};

const std::map<std::string, int> vHHIME_Map =
{
	{ "6C", 124 }, { "3C", 121 },
	{ "BUNKER", 72 }, { "5D", 56 }, { "2D", 57 }, { "DODGE", 66 }, { "THROW", 275 },
	{ "BACK DASH", 61 }, { "FORWARD DASH", 60 }, { "FORWARD SUPER JUMP", 41 },
	{ "623A", 230 }, { "623B", 231 }, { "623C", 232 },
	{ "22A", 170 }, { "22B", 171 }, { "22C", 172 }, { "22D", 89 },
	{ "214A", 150 }, { "214B", 151 }, { "214C", 152 },
	{ "236A", 160 }, { "236B", 161 }, { "236C", 162 },


	{ "ARC DRIVE", 299 },
	{"filler",0}
};

const std::map<std::string, int> vHHISUI_Map =
{
	{ "4B", 73 }, { "6B", 186 }, { "6C", 71 },
	{ "BUNKER", 181 }, { "5D", 81 }, { "2D", 82 }, { "DODGE", 104 }, { "THROW", 55 },
	{ "BACK DASH", 67 }, { "FORWARD DASH", 65 }, { "FORWARD SUPER JUMP", 68 },
	{ "623A", 160 }, { "623B", 161 }, { "623C", 162 },
	{ "22A", 410 }, { "22B", 411 }, { "22C", 460 },
	{ "214A", 520 }, { "214B", 521 }, { "214C", 502 },
	{ "236A", 365 }, { "236B", 366 }, { "236C", 372 },


	{ "ARC DRIVE", 199 },
	{"filler",0}
};

const std::map<std::string, int> vHKOHAKU_Map =
{

	{ "BUNKER", 235 }, { "5D", 70 }, { "2D", 71 }, { "DODGE", 65 }, { "THROW", 64 },
	{ "BACK DASH", 61 }, { "FORWARD DASH", 60 }, { "FORWARD SUPER JUMP", 63 },
	{ "623A", 450 }, { "623B", 451 }, { "623C", 452 },
	{ "22A", 490 }, { "22B", 491 }, { "22C", 492 },
	{ "214A", 96 }, { "214B", 97 }, { "214C", 98 },
	{ "236A", 170 }, { "236B", 171 }, { "236C", 172 },

	{ "624C", 112 },
	{ "ARC DRIVE", 200 },
	{"filler",0}
};

const std::map<std::string, int> vHKOHAMECH_Map =
{
	{ "6A", 685 }, { "4B", 685 }, { "6B", 685 },
	{ "BUNKER", 235 }, { "5D", 70 }, { "2D", 71 }, { "DODGE", 65 }, { "THROW", 64 },
	{ "BACK DASH", 61 }, { "FORWARD DASH", 60 }, { "FORWARD SUPER JUMP", 63 },
	{ "623A", 620 }, { "623B", 621 }, { "623C", 622 },
	{ "22A", 685 }, { "22B", 686 }, { "22C", 687 },
	{ "214A", 600 }, { "214B", 601 }, { "214C", 602 },
	{ "236A", 170 }, { "236B", 171 }, { "236C", 172 },

	{ "624C", 60 },
	{ "ARC DRIVE", 200 },
	{"filler",0}
};

const std::map<std::string, int> vHKOUMA_Map =
{
	{ "6C", 76 },
	{ "BUNKER", 180 }, { "5D", 65 }, { "2D", 66 }, { "DODGE", 78 }, { "THROW", 70 },
	{ "BACK DASH", 61 }, { "FORWARD DASH", 60 }, { "FORWARD SUPER JUMP", 57 },
	{ "623A", 417 }, { "623B", 186 }, { "623C", 187 },
	{ "22A", 100 }, { "22B", 101 }, { "22C", 102 },
	{ "214A", 110 }, { "214B", 115 }, { "214C", 117 },
	{ "236A", 125 }, { "236B", 128 }, { "236C", 131 },


	{ "ARC DRIVE", 149 },
	{"filler",0}
};

const std::map<std::string, int> vHLEN_Map =
{
	{ "3C", 66 },
	{ "BUNKER", 101 }, { "5D", 134 }, { "2D", 135 }, { "DODGE", 68 }, { "THROW", 105 },
	{ "BACK DASH", 61 }, { "FORWARD DASH", 60 }, { "FORWARD SUPER JUMP", 59 },
	{ "623A", 59 }, { "623B", 59 }, { "623C", 59 },

	{ "214A", 85 }, { "214B", 86 }, { "214C", 82 },
	{ "236A", 59 }, { "236B", 59 }, { "236C", 59 },
	{ "421A", 415 }, { "421B", 417 }, { "421C", 420 },

	{ "ARC DRIVE", 59 },
	{"filler",0}
};

const std::map<std::string, int> vHMAIDS_Map =
{










	{"filler",0}
};

const std::map<std::string, int> vHMECH_Map =
{
	{ "4B", 172 },
	{ "BUNKER", 206 }, { "5D", 81 }, { "2D", 82 }, { "DODGE", 104 }, { "THROW", 55 },
	{ "BACK DASH", 67 }, { "FORWARD DASH", 65 }, { "FORWARD SUPER JUMP", 43 },
	{ "623A", 500 }, { "623B", 501 }, { "623C", 502 },

	{ "214A", 550 }, { "214B", 551 }, { "214C", 552 },
	{ "236A", 554 }, { "236B", 555 }, { "236C", 556 },

	{ "624A", 390 }, { "624B", 391 }, { "624C", 392 },
	{ "ARC DRIVE", 200 },
	{"filler",0}
};

const std::map<std::string, int> vHMIYAKO_Map =
{
	{ "6A", 1 }, { "4B", 3 }, { "6C", 60 },
	{ "BUNKER", 120 }, { "5D", 67 }, { "2D", 68 }, { "DODGE", 57 }, { "THROW", 74 },
	{ "BACK DASH", 55 }, { "FORWARD DASH", 54 }, { "FORWARD SUPER JUMP", 85 },
	{ "623A", 155 }, { "623B", 156 }, { "623C", 157 },
	{ "22A", 160 }, { "22B", 161 }, { "22C", 162 },
	{ "214A", 185 }, { "214B", 426 }, { "214C", 435 },
	{ "236A", 407 }, { "236B", 408 }, { "236C", 409 },
	{ "421A", 411 }, { "421B", 412 }, { "421C", 413 },

	{ "ARC DRIVE", 199 },
	{"filler",0}
};

const std::map<std::string, int> vHNAC_Map =
{
	{ "6C", 79 }, { "3C", 78 },
	{ "BUNKER", 169 }, { "5D", 83 }, { "2D", 84 }, { "DODGE", 56 }, { "THROW", 100 },
	{ "BACK DASH", 64 }, { "FORWARD DASH", 63 }, { "FORWARD SUPER JUMP", 43 },
	{ "623A", 172 }, { "623B", 173 }, { "623C", 174 },
	{ "22A", 500 }, { "22B", 499 }, { "22C", 498 },
	{ "214A", 540 }, { "214B", 541 }, { "214C", 542 },
	{ "236A", 400 }, { "236B", 401 }, { "236C", 402 },


	{ "ARC DRIVE", 227 },
	{"filler",0}
};

const std::map<std::string, int> vHNANAYA_Map =
{

	{ "BUNKER", 96 }, { "5D", 63 }, { "2D", 64 }, { "DODGE", 78 }, { "THROW", 59 },
	{ "BACK DASH", 55 }, { "FORWARD DASH", 56 }, { "FORWARD SUPER JUMP", 90 },
	{ "623A", 118 }, { "623B", 119 }, { "623C", 500 },
	{ "22A", 114 }, { "22B", 115 }, { "22C", 116 },
	{ "214A", 408 }, { "214B", 409 }, { "214C", 410 },
	{ "236A", 413 }, { "236B", 414 }, { "236C", 415 },


	{ "ARC DRIVE", 198 },
	{"filler",0}
};

const std::map<std::string, int> vHNECO_Map =
{

	{ "BUNKER", 61 }, { "5D", 83 }, { "2D", 84 }, { "DODGE", 56 }, { "THROW", 100 },
	{ "BACK DASH", 64 }, { "FORWARD DASH", 63 }, { "FORWARD SUPER JUMP", 60 },
	{ "623A", 60 }, { "623B", 60 }, { "623C", 60 },
	{ "22A", 150 }, { "22B", 151 }, { "22C", 152 },
	{ "214A", 61 }, { "214B", 61 }, { "214C", 61 },
	{ "236A", 60 }, { "236B", 91 }, { "236C", 60 },


	{ "ARC DRIVE", 61 },
	{"filler",0}
};

const std::map<std::string, int> vHNECOMECH_Map =
{
	{ "6C", 71 },
	{ "BUNKER", 206 }, { "5D", 81 }, { "2D", 82 }, { "DODGE", 104 }, { "THROW", 55 },
	{ "BACK DASH", 67 }, { "FORWARD DASH", 65 }, { "FORWARD SUPER JUMP", 43 },
	{ "623A", 410 }, { "623B", 411 }, { "623C", 412 },
	{ "22A", 430 }, { "22B", 431 }, { "22C", 432 },
	{ "214A", 420 }, { "214B", 421 }, { "214C", 422 },
	{ "236A", 400 }, { "236B", 401 }, { "236C", 402 },


	{ "ARC DRIVE", 200 },
	{"filler",0}
};

const std::map<std::string, int> vHNERO_Map =
{
	{ "4C", 60 }, { "3C", 401 },
	{ "BUNKER", 90 }, { "5D", 73 }, { "2D", 74 }, { "DODGE", 83 }, { "THROW", 65 },
	{ "BACK DASH", 70 }, { "FORWARD DASH", 71 }, { "FORWARD SUPER JUMP", 95 },

	{ "22A", 515 }, { "22B", 516 }, { "22C", 517 },
	{ "214A", 130 }, { "214B", 131 }, { "214C", 133 },
	{ "236A", 120 }, { "236B", 121 }, { "236C", 422 },
	{ "421A", 105 }, { "421B", 106 }, { "421C", 107 },
	{ "624C", 475 },
	{ "ARC DRIVE", 160 },
	{"filler",0}
};

const std::map<std::string, int> vHPCIEL_Map =
{
	{ "6B", 154 }, { "4C", 153 }, { "3C", 157 },
	{ "BUNKER", 75 }, { "5D", 70 }, { "2D", 71 }, { "DODGE", 66 }, { "THROW", 275 },
	{ "BACK DASH", 61 }, { "FORWARD DASH", 60 }, { "FORWARD SUPER JUMP", 80 },
	{ "623A", 105 }, { "623B", 106 }, { "623C", 107 },

	{ "214A", 125 }, { "214B", 126 }, { "214C", 132 },
	{ "236A", 85 }, { "236B", 86 }, { "236C", 87 },


	{ "ARC DRIVE", 200 },
	{"filler",0}
};

const std::map<std::string, int> vHRIES_Map =
{
	{ "6B", 374 }, { "3C", 501 },
	{ "BUNKER", 102 }, { "5D", 56 }, { "2D", 57 }, { "DODGE", 66 }, { "THROW", 85 },
	{ "BACK DASH", 71 }, { "FORWARD DASH", 70 }, { "FORWARD SUPER JUMP", 90 },
	{ "623A", 425 }, { "623B", 426 }, { "623C", 427 },

	{ "214A", 110 }, { "214B", 111 }, { "214C", 112 },
	{ "236A", 125 }, { "236B", 126 }, { "236C", 127 },

	{ "624A", 130 }, { "624B", 131 }, { "624C", 132 },
	{ "ARC DRIVE", 399 },
	{"filler",0}
};

const std::map<std::string, int> vHROA_Map =
{
	{ "6C", 481 },
	{ "BUNKER", 85 }, { "5D", 80 }, { "2D", 81 }, { "DODGE", 64 }, { "THROW", 269 },
	{ "BACK DASH", 47 }, { "FORWARD DASH", 46 }, { "FORWARD SUPER JUMP", 95 },
	{ "623A", 490 }, { "623B", 491 }, { "623C", 492 },

	{ "214A", 500 }, { "214B", 501 }, { "214C", 540 },
	{ "236A", 529 }, { "236B", 530 }, { "236C", 517 },


	{ "ARC DRIVE", 149 },
	{"filler",0}
};

const std::map<std::string, int> vHRYOUGI_Map =
{
	{ "4C", 194 }, { "3C", 402 },
	{ "BUNKER", 85 }, { "5D", 75 }, { "2D", 76 }, { "DODGE", 269 }, { "THROW", 70 },
	{ "BACK DASH", 61 }, { "FORWARD DASH", 60 }, { "FORWARD SUPER JUMP", 95 },
	{ "623A", 310 }, { "623B", 311 }, { "623C", 312 },
	{ "22A", 740 }, { "22B", 741 }, { "22C", 722 }, { "22D", 75 },
	{ "214A", 515 }, { "214B", 516 }, { "214C", 517 },
	{ "236A", 371 }, { "236B", 372 }, { "236C", 397 },
	{ "421A", 596 }, { "421B", 597 }, { "421C", 598 },
	{ "624A", 508 }, { "624B", 510 }, { "624C", 573 },
	{ "ARC DRIVE", 645 },
	{"filler",0}
};

const std::map<std::string, int> vHSATSUKI_Map =
{
	{ "3C", 490 },
	{ "BUNKER", 175 }, { "5D", 90 }, { "2D", 91 }, { "DODGE", 78 }, { "THROW", 85 },
	{ "BACK DASH", 81 }, { "FORWARD DASH", 80 }, { "FORWARD SUPER JUMP", 75 },
	{ "623A", 140 }, { "623B", 141 }, { "623C", 142 },
	{ "22A", 380 }, { "22B", 381 }, { "22C", 382 },
	{ "214A", 110 }, { "214B", 111 }, { "214C", 112 },
	{ "236A", 515 }, { "236B", 516 }, { "236C", 517 },

	{ "624C", 180 },
	{ "ARC DRIVE", 150 },
	{"filler",0}
};

const std::map<std::string, int> vHSEIFUKU_Map =
{
	{ "6B", 240 }, { "6C", 154 },
	{ "BUNKER", 75 }, { "5D", 70 }, { "2D", 71 }, { "DODGE", 66 }, { "THROW", 275 },
	{ "BACK DASH", 61 }, { "FORWARD DASH", 60 }, { "FORWARD SUPER JUMP", 80 },
	{ "623A", 230 }, { "623B", 231 }, { "623C", 232 },
	{ "22A", 220 }, { "22B", 221 }, { "22C", 222 },
	{ "214A", 360 }, { "214B", 361 }, { "214C", 362 },
	{ "236A", 470 }, { "236B", 471 }, { "236C", 472 },

	{ "624C", 200 },
	{ "ARC DRIVE", 370 },
	{"filler",0}
};

const std::map<std::string, int> vHSION_Map =
{
	{ "6A", 1 }, { "6B", 58 }, { "6C", 440 }, { "3C", 61 },
	{ "BUNKER", 85 }, { "5D", 65 }, { "2D", 66 }, { "DODGE", 132 }, { "THROW", 62 },
	{ "BACK DASH", 56 }, { "FORWARD DASH", 55 }, { "FORWARD SUPER JUMP", 54 },
	{ "623A", 110 }, { "623B", 111 }, { "623C", 112 },

	{ "214A", 115 }, { "214B", 116 }, { "214C", 117 },
	{ "236A", 460 }, { "236B", 462 }, { "236C", 413 },
	{ "421A", 81 }, { "421B", 82 }, { "421C", 83 },

	{ "ARC DRIVE", 145 },
	{"filler",0}
};

const std::map<std::string, int> vHTOHNO_Map =
{
	{ "6C", 475 },
	{ "BUNKER", 122 }, { "5D", 63 }, { "2D", 64 }, { "DODGE", 78 }, { "THROW", 59 },
	{ "BACK DASH", 55 }, { "FORWARD DASH", 56 }, { "FORWARD SUPER JUMP", 90 },
	{ "623A", 140 }, { "623B", 141 }, { "623C", 142 },
	{ "22A", 244 }, { "22B", 246 }, { "22C", 234 }, { "22D", 247 },
	{ "214A", 110 }, { "214B", 111 }, { "214C", 112 },
	{ "236A", 105 }, { "236B", 106 }, { "236C", 107 },


	{ "ARC DRIVE", 199 },
	{"filler",0}
};

const std::map<std::string, int> vHVAKIHA_Map =
{
	{ "4C", 80 }, { "6C", 60 },
	{ "BUNKER", 91 }, { "5D", 70 }, { "2D", 71 }, { "DODGE", 54 }, { "THROW", 62 },
	{ "BACK DASH", 45 }, { "FORWARD DASH", 43 }, { "FORWARD SUPER JUMP", 95 },
	{ "623A", 205 }, { "623B", 206 }, { "623C", 207 },
	{ "22A", 160 }, { "22B", 161 }, { "22C", 162 },
	{ "214A", 180 }, { "214B", 181 }, { "214C", 182 },
	{ "236A", 449 }, { "236B", 450 }, { "236C", 451 },


	{ "ARC DRIVE", 540 },
	{"filler",0}
};

const std::map<std::string, int> vHVSION_Map =
{
	{ "6B", 470 }, { "6C", 225 }, { "3C", 61 },
	{ "BUNKER", 85 }, { "5D", 65 }, { "2D", 66 }, { "DODGE", 132 }, { "THROW", 77 },
	{ "BACK DASH", 56 }, { "FORWARD DASH", 57 }, { "FORWARD SUPER JUMP", 90 },
	{ "623A", 125 }, { "623B", 126 }, { "623C", 445 },

	{ "214A", 165 }, { "214B", 166 }, { "214C", 167 },
	{ "236A", 215 }, { "236B", 216 }, { "236C", 217 },


	{ "ARC DRIVE", 205 },
	{"filler",0}
};

const std::map<std::string, int> vHWARA_Map =
{
	{ "6C", 72 },
	{ "BUNKER", 130 }, { "5D", 62 }, { "2D", 63 }, { "DODGE", 69 }, { "THROW", 55 },
	{ "BACK DASH", 55 }, { "FORWARD DASH", 56 }, { "FORWARD SUPER JUMP", 95 },
	{ "623A", 135 }, { "623B", 136 }, { "623C", 137 },
	{ "22A", 100 }, { "22B", 101 }, { "22C", 102 },
	{ "214A", 190 }, { "214B", 191 }, { "214C", 192 },
	{ "236A", 180 }, { "236B", 181 }, { "236C", 182 },
	{ "421A", 170 }, { "421B", 171 }, { "421C", 172 },

	{ "ARC DRIVE", 149 },
	{"filler",0}
};

const std::map<std::string, int> vHWARC_Map =
{
	{ "3C", 49 },
	{ "BUNKER", 78 }, { "5D", 72 }, { "2D", 73 }, { "DODGE", 64 }, { "THROW", 55 },
	{ "BACK DASH", 43 }, { "FORWARD DASH", 46 }, { "FORWARD SUPER JUMP", 95 },
	{ "623A", 155 }, { "623B", 156 }, { "623C", 157 },
	{ "22A", 396 }, { "22B", 397 }, { "22C", 398 },
	{ "214A", 112 }, { "214B", 113 }, { "214C", 114 },
	{ "236A", 563 }, { "236B", 564 }, { "236C", 152 },
	{ "421A", 160 }, { "421B", 161 }, { "421C", 162 },

	{ "ARC DRIVE", 223 },
	{"filler",0}
};

const std::map<std::string, int> vHWLEN_Map =
{
	{ "4B", 47 },
	{ "BUNKER", 101 }, { "5D", 134 }, { "2D", 135 }, { "DODGE", 68 }, { "THROW", 106 },
	{ "BACK DASH", 61 }, { "FORWARD DASH", 60 }, { "FORWARD SUPER JUMP", 46 },
	{ "623A", 195 }, { "623B", 196 }, { "623C", 197 },
	{ "22A", 182 }, { "22B", 495 }, { "22C", 184 }, { "22D", 185 },
	{ "214A", 460 }, { "214B", 461 }, { "214C", 462 },
	{ "236A", 115 }, { "236B", 116 }, { "236C", 117 },
	{ "421A", 500 }, { "421B", 501 }, { "421C", 502 },
	{ "624A", 110 }, { "624B", 111 }, { "624C", 112 },
	{ "ARC DRIVE", 149 },
	{"filler",0}
};