
#include "DebugInfo.h"
#include "DirectX.h"
#include <string>

void EffectData::describe(char* buffer, int bufLen) {

	DWORD offset = ((DWORD)&exists);
	int index = -1;
	const char* entityString = "PLAYER";
	if (offset >= 0x0067BDE8) {
		index = (offset - 0x0067BDE8) / 0x33C;
		entityString = "EFFECT";
	} else {
		index = (offset - 0x00555130) / 0xAFC;
	}

	int bufferOffset = 0;
	bufferOffset = snprintf(buffer, bufLen, "%s%d P%d S%d\n(%d,%d)\n", entityString, index, pattern, state, xPos, yPos);

	if (attackDataPtr != NULL) {
		snprintf(buffer + bufferOffset, bufLen - bufferOffset, "DMG%d\nPROR%d", attackDataPtr->damage, attackDataPtr->proration);
	}

}

PatternData* EffectData::getPatternDataPtr(int p) {
	// doing this in a more normal way could never get me the results i wanted
	__try {
		DWORD temp = (DWORD)(playerDataArr[0].ha6DataPtr->subData1->subData2->ptrToPatternDataArr);
		return (PatternData*)*(DWORD*)(temp + (4 * playerDataArr[0].pattern));
	} __except (EXCEPTION_EXECUTE_HANDLER) {
		return NULL;
	}
}

AnimationData* EffectData::getAnimationDataPtr(int p, int s) { 
	__try {
		PatternData* pattern = getPatternDataPtr(p);
		//DWORD temp = (DWORD)pattern->ptrToAnimationDataArr->animationDataArr;
		//return (AnimationData*)(temp + (0x54 * s));
		return &(pattern->ptrToAnimationDataArr->animationDataArr[s]);
	} __except (EXCEPTION_EXECUTE_HANDLER) {
		return NULL;
	}
}

// -----

PlayerData* playerDataArr = (PlayerData*)(0x00555130);
EffectData* effectDataArr = (EffectData*)(0x0067BDE8);

bool shouldDisplayDebugInfo = false;
void displayDebugInfo() {

	if (!shouldDisplayDebugInfo) {
		return;
	}

	const int bufferLen = 2048;
	char buffer[bufferLen];

	int xIndex = 100;
	int xDif = 75;

	for (int i = 0; i < 4; i++) {
		if (playerDataArr[i].exists) {
			playerDataArr[i].describe(buffer, bufferLen);

			TextDraw(xIndex, 0, 6, 0xFFFFFFFF, buffer);
			xIndex += xDif;
		}
	}

	for (int i = 0; i < 50; i++) {
		if (effectDataArr[i].exists) {
			effectDataArr[i].describe(buffer, bufferLen);

			TextDraw(xIndex, 0, 6, 0xFFFFFFFF, buffer);
			xIndex += xDif;
		}
	}

	DWORD testPatternDataPtr = (DWORD)playerDataArr[0].getPatternDataPtr(playerDataArr[0].pattern);
	TextDraw(100, 200, 8, 0xFF00FF00, "pattern: %08X %08X", (DWORD)playerDataArr[0].patternDataPtr, testPatternDataPtr);

	DWORD testAnimationDataPtr = (DWORD)playerDataArr[0].getAnimationDataPtr(playerDataArr[0].pattern, playerDataArr[0].state);
	TextDraw(100, 216, 8, 0xFF00FF00, "state:   %08X %08X", (DWORD)playerDataArr[0].animationDataPtr, testAnimationDataPtr);


	static KeyState cKey('C');
	if (cKey.keyDown()) {
		char buffer[128];
		int bufLoc = 0;
		for (int i = 0; i < 32; i++) {
			bufLoc += snprintf(buffer + bufLoc, 128 - bufLoc, "%02X\n", (BYTE)playerDataArr[0].patternDataPtr->patternName[i]);

			if (i > 0 && (BYTE)playerDataArr[0].patternDataPtr->patternName[i] == 0 && (BYTE)playerDataArr[0].patternDataPtr->patternName[i - 1] == 0) {
				break;
			}
		}


		writeClipboard(std::string(buffer));
	}

}