#pragma once

#include "DebugInfo.h"
#include <vector>
#include<map>

extern std::map<std::string, char*> dummyReversalMap;

std::string getReadableInput(CommandData* cmd);

int getPatternFromInput(PlayerData* PD, const char input[20]);

CommandData* getCommandDataFromInput(PlayerData* PD, const char input[20]);

int getIDFromPattern(PlayerData* playerData, int nPattern, int nthMatch = 1);

int getIDFromCmd(PlayerData* playerData, const char* cmd, int nthMatch = 1);

int getPatternFromCmd(PlayerData* playerData, const char* cmd, int stance = 7, int nthMatch = 1);

bool checkCmdVars(PlayerData* playerData, int ID);

bool tryCmdID(PlayerData* playerData, int ID);

bool tryCmdPattern(PlayerData* playerData, int nPattern);

void tryUnivCmd(PlayerData* playerData, byte buttons, byte directions);

void SetBurstFlags(PlayerData* playerData);

bool tryBurst(PlayerData* playerData);

//dir : input as numeric e.g. 236
//buttons : logical and, A = 1, B = 2, C = 4, D = 8
void setBufferCmd(PlayerData* playerData, WORD dir, WORD buttons);

bool tryBufferCmd(PlayerData* playerData);

void tryNormalsAndMovement(ActorData* actorData, byte inputButton, byte inputDirection);

//returns AND of 1 for held shield and 2 for ex shield
byte getShieldCancel(PlayerData* playerData, int pat);

//returns AND of 1 = stand, 2 = airborne, 4 = crouch
byte getCmdStance(PlayerData* playerData, int ID);

//returns AND of 1 = stand, 2 = airborne, 4 = crouch
byte getPatStance(PlayerData* playerData, int pat);
