
#include "DebugInfo.h"
#include "DirectX.h"

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


}