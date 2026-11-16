#include "Commands.h"

std::map<std::string, char*> dummyReversalMap = {};

std::string getReadableInput(CommandData* cmd) {
	std::string readable = "";
	char* cmdInput = cmd->input;
	if (cmd->airOK)
	{
		readable += "j.";
	}
	for (int i = 0; i < 20; i++)
	{
		if (cmdInput[i] == '\xff')
		{
			break;
		}
		else if (cmdInput[i] <= 0x9)
		{
			readable += cmdInput[i] + 0x30;
		}
		else
		{
			readable += cmdInput[i];
		}
	}
	return readable;
}

int getPatternFromInput(PlayerData* PD, const char input[20]) {
	char buffer[20];
	char inputCopy[20];
	char readableInput[20] = { 0 };
	for (int i = 0; i < PD->cmdFileDataPtr->cmdDataPtr->count; i++)
	{
		if (PD->cmdFileDataPtr->cmdDataPtr->array[i] != 0)
		{
			int length = 0;
			snprintf(inputCopy, 20, "%s", PD->cmdFileDataPtr->cmdDataPtr->array[i]->input);
			for (int j = 0; j < 20; j++)
			{
				if (inputCopy[j] == '\xFF')
				{
					readableInput[j] = 0;
					length = j;
					break;
				}
				else if (inputCopy[j] >= 0x0 && inputCopy[j] <= 0x9)
				{
					readableInput[j] = inputCopy[j] + 0x30;
				}
				else
				{
					readableInput[j] = inputCopy[j];
				}
			}

			bool isMatch = true;
			for (int k = 0; k < length; k++)
			{
				if (readableInput[k] != input[k]) isMatch = false;
			}
			if (isMatch)
			{
				return PD->cmdFileDataPtr->cmdDataPtr->array[i]->pattern;
			}
		}
	}
	return -1;
}

CommandData* getCommandDataFromInput(PlayerData* PD, const char input[20]) {
	char buffer[20];
	char inputCopy[20];
	char readableInput[20] = { 0 };
	for (int i = 0; i < PD->cmdFileDataPtr->cmdDataPtr->count; i++) {
		if (PD->cmdFileDataPtr->cmdDataPtr->array[i] != 0) {
			int length = 0;
			snprintf(inputCopy, 20, "%s", PD->cmdFileDataPtr->cmdDataPtr->array[i]->input);
			for (int j = 0; j < 20; j++) {
				if (inputCopy[j] == '\xFF') {
					readableInput[j] = 0;
					length = j;
					break;
				}
				else if (inputCopy[j] >= 0x0 && inputCopy[j] <= 0x9) {
					readableInput[j] = inputCopy[j] + 0x30;
				}
				else {
					readableInput[j] = inputCopy[j];
				}
			}

			bool isMatch = true;
			for (int k = 0; k < length; k++) {
				if (readableInput[k] != input[k]) isMatch = false;
			}
			if (isMatch) {
				return PD->cmdFileDataPtr->cmdDataPtr->array[i];
			}
		}
	}
	return nullptr;
}

int getIDFromPattern(PlayerData* playerData, int nPattern, int nthMatch) {
	int retVal = -1;
	for (int i = 0; i < playerData->cmdFileDataPtr->cmdDataPtr->count; i++) {
		if (playerData->cmdFileDataPtr->cmdDataPtr->array[i])
		{
			if (playerData->cmdFileDataPtr->cmdDataPtr->array[i]->pattern == nPattern) {
				retVal = playerData->cmdFileDataPtr->cmdDataPtr->array[i]->ID;
				nthMatch--;
				if (nthMatch == 0) return retVal;
			};
		}
	}
	return -1;
}

int getIDFromCmd(PlayerData* playerData, const char* cmd, int nthMatch) {
	int retVal = -1;
	for (int i = 0; i < playerData->cmdFileDataPtr->cmdDataPtr->count; i++) {
		if (playerData->cmdFileDataPtr->cmdDataPtr->array[i])
		{
			if (strcmp(cmd, playerData->cmdFileDataPtr->cmdDataPtr->array[i]->input) == 0) {
				retVal = playerData->cmdFileDataPtr->cmdDataPtr->array[i]->ID;
				nthMatch--;
				if (nthMatch == 0) return retVal;
			};
		}
	}
	return -1;
}

int getPatternFromCmd(PlayerData* playerData, const char* cmd, int stance, int nthMatch) {
	int retVal = -1;
	for (int i = 0; i < playerData->cmdFileDataPtr->cmdDataPtr->count; i++) {
		if (playerData->cmdFileDataPtr->cmdDataPtr->array[i])
		{
			if (strcmp(cmd, playerData->cmdFileDataPtr->cmdDataPtr->array[i]->input) == 0 &&
				playerData->cmdFileDataPtr->cmdDataPtr->array[i]->flagset1 & stance) {
				retVal = playerData->cmdFileDataPtr->cmdDataPtr->array[i]->pattern;
				nthMatch--;
				if (nthMatch == 0) return retVal;
			};
		}
	}
	return -1;
}

bool checkCmdVars(PlayerData* playerData, int ID) {
	const DWORD MBAA_CheckCmdVars = 0x0046d430;
	__asm {
		mov ecx, ID;
		mov eax, playerData;
		call[MBAA_CheckCmdVars];
	}
	return;
}

bool tryCmdID(PlayerData* playerData, int ID) {
	const DWORD MBAA_CheckValidCommandConditions = 0x0046cea0;
	__asm {
		push ID;
		mov eax, playerData;
		call[MBAA_CheckValidCommandConditions];
		add esp, 0x4;
	}
	return;
}

bool tryCmdPattern(PlayerData* playerData, int nPattern) {
	if (nPattern < 41) {
		return false;
	}
	else if (nPattern == 260 || nPattern == playerData->cmdFileDataPtr->groundThrowPat || nPattern == playerData->cmdFileDataPtr->airThrowPat) {
		playerData->subObj.targetPattern = nPattern;
	}
	int id = -1;
	int meterMem = 0;
	bool didCmd = false;
	for (int i = 0; i < playerData->cmdFileDataPtr->cmdDataPtr->count; i++) {
		if (playerData->cmdFileDataPtr->cmdDataPtr->array[i])
		{
			if (playerData->cmdFileDataPtr->cmdDataPtr->array[i]->pattern == nPattern) {
				id = playerData->cmdFileDataPtr->cmdDataPtr->array[i]->ID;
				if (checkCmdVars(playerData, id)) {
					meterMem = playerData->cmdFileDataPtr->cmdDataPtr->array[i]->meterSpend;
					playerData->cmdFileDataPtr->cmdDataPtr->array[i]->meterSpend = 0;
					didCmd = tryCmdID(playerData, id);
					playerData->cmdFileDataPtr->cmdDataPtr->array[i]->meterSpend = meterMem;
					if (didCmd) return true;
				}
			};
		}
	}
	return false;
}

void tryUnivCmd(PlayerData* playerData, byte buttons, byte directions) {
	const DWORD MBAA_UniversalCommands = 0x004666b0;
	__asm {
		mov ecx, playerData;
		add ecx, 0x4;
		push directions;
		push buttons;
		call[MBAA_UniversalCommands];
		add esp, 0x8;
	}
}

void SetBurstFlags(PlayerData* playerData) {
	const DWORD MBAA_SetBurstFlags = 0x00464390;
	__asm {
		mov ecx, playerData;
		add ecx, 0x4;
		call[MBAA_SetBurstFlags];
	}
}

bool tryBurst(PlayerData* playerData) {
	if (playerData->subObj.hitstunTimeRemaining != 0 && playerData->subObj.burstLock == 0 && playerData->subObj.hitstop == 0) {
		int stance = playerData->subObj.animationDataPtr->stateData->stance;
		if (stance == 0 || stance == 2) {
			if (playerData->subObj.hitstunTimeRemaining < 0) {
				return 0;
			}
		}
		else if (stance == 1) {
			if (9 < playerData->subObj.untechTimeElapsed) {
				return 0;
			}
			if (playerData->subObj.completedHitVectors != 0) {
				return 0;
			}
		}
		SetBurstFlags(playerData);
		return 1;
	}
	return 0;
}

//dir : input as numeric e.g. 236
//buttons : logical and, A = 1, B = 2, C = 4, D = 8
void setBufferCmd(PlayerData* playerData, WORD dir, WORD buttons) {
	WORD dirCount = (WORD)log10(dir);
	playerData->dirInputs[0] = dirCount;
	int i = dirCount * 2 + 1;
	while (dir > 0) {
		playerData->dirInputs[i] = dir % 10;
		playerData->dirInputs[i + 1] = 1;
		dir = dir / 10;
		i -= 2;
	}
	//WORD dirCount = dir[0] * 2 + 3;
	//memcpy(playerData->dirInputs, dir, dirCount * 0x2);
	playerData->aInputs[0] = 0;
	playerData->aInputs[1] = buttons & 1;
	playerData->aInputs[2] = 1;

	playerData->bInputs[0] = 0;
	playerData->bInputs[1] = buttons & 2;
	playerData->bInputs[2] = 1;

	playerData->cInputs[0] = 0;
	playerData->cInputs[1] = buttons & 4;
	playerData->cInputs[2] = 1;

	playerData->dInputs[0] = 0;
	playerData->dInputs[1] = buttons & 8;
	playerData->dInputs[2] = 1;
}

bool tryBufferCmd(PlayerData* playerData) {
	const DWORD MBAA_GetHighestPriorityValidCommand = 0x0046d510;
	__asm {
		mov eax, playerData;
		call[MBAA_GetHighestPriorityValidCommand];
	}
	return;
}

void tryNormalsAndMovement(ActorData* actorData, byte inputButton, byte inputDirection) {
	const DWORD MBAA_ManageNormalAttacksAndMovements = 0x004666b0;
	__asm {
		mov ecx, actorData;
		push inputDirection;
		push inputButton;
		call[MBAA_ManageNormalAttacksAndMovements];
	}
	return;
}

//returns AND of 1 for held shield and 2 for ex shield
byte getShieldCancel(PlayerData* playerData, int pat) {
	byte exOnly = playerData->subObj.moon == 0 ? 0x2 : 0x0;
	byte retVal = 0x0;
	if (pat == playerData->cmdFileDataPtr->ShieldCounter_Ground ||
		pat == playerData->cmdFileDataPtr->ShieldCounter_Air ||
		pat == playerData->cmdFileDataPtr->ShieldCounter_Crouch) {
		return 0x3;
	}
	if (pat <= 40 || //universal normals and movement
		pat == 260 || //heat activation
		pat == playerData->cmdFileDataPtr->groundThrowPat || //throws
		pat == playerData->cmdFileDataPtr->airThrowPat) return exOnly;
	int ID = getIDFromPattern(playerData, pat, 1);
	if (ID == -1) return 0x0;
	CommandData* cmd = playerData->cmdFileDataPtr->cmdDataPtr->array[ID];
	int cancelType = playerData->cmdFileDataPtr->cmdDataPtr->array[ID]->cancelType;
	if (cmd->noCancel) { // if no cancel
		if (cmd->guardCancelOK && cmd->whiffCancelOnly) { // covers D > D
			retVal = 0x3;
		}
		else {
			retVal = 0x0;
		}
	}
	else if (cancelType == 1 || cancelType == 2) { //if special or ex
		retVal = 0x3;
	}
	else if (cancelType == 0) { //if normal or movement
		retVal = exOnly;
	}

	ID = getIDFromPattern(playerData, pat, 2);
	if (ID == -1) return retVal;
	cmd = playerData->cmdFileDataPtr->cmdDataPtr->array[ID];
	cancelType = playerData->cmdFileDataPtr->cmdDataPtr->array[ID]->cancelType;
	if (cmd->noCancel) {
		if (cmd->guardCancelOK && cmd->whiffCancelOnly) {  // covers D > D
			retVal = 0x3;
		}
		else {
			retVal = 0x0;
		}
	}
	else if (cancelType == 1 || cancelType == 2) {
		retVal |= 0x3;
	}
	else if (cancelType == 0) {
		retVal |= exOnly;
	}
	return retVal;
}

//returns AND of 1 = stand, 2 = airborne, 4 = crouch
byte getCmdStance(PlayerData* playerData, int ID) {
	return playerData->cmdFileDataPtr->cmdDataPtr->array[ID]->flagset1 & 0x7;
}

//returns AND of 1 = stand, 2 = airborne, 4 = crouch
byte getPatStance(PlayerData* playerData, int pat) {
	if ((0 <= pat && pat <= 6) || (35 <= pat && pat <= 37) || pat == 260 || pat == playerData->cmdFileDataPtr->groundThrowPat) { //ground universal normals, normal jumps, heat activation, ground throw
		return 0x5;
	}
	else if ((7 <= pat && pat <= 9) || (38 <= pat && pat <= 40) || pat == playerData->cmdFileDataPtr->airThrowPat) { //air universal normals, normal double jumps, air throw
		return 0x2;
	}
	else if (pat == playerData->cmdFileDataPtr->ShieldCounter_Ground || pat == playerData->cmdFileDataPtr->ShieldCounter_Crouch) {
		int retVal = 0x0;
		retVal += 0x1 * (pat == playerData->cmdFileDataPtr->ShieldCounter_Ground);
		retVal += 0x4 * (pat == playerData->cmdFileDataPtr->ShieldCounter_Crouch);
		return retVal;
	}
	else if (pat == playerData->cmdFileDataPtr->ShieldCounter_Air) {
		return 0x2;
	}
	int ID = getIDFromPattern(playerData, pat);
	return getCmdStance(playerData, ID);
}
