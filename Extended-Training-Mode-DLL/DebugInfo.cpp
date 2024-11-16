
#include "DebugInfo.h"
#include "DirectX.h"

void PlayerData::describe(char* buffer, int bufLen) {

	int playerIndex = (((int)& exists) - 0x00555130) / 0xAFC;

	snprintf(buffer, bufLen, "PLAYER%d P%d S%d\n(%d,%d)", playerIndex, pattern, state, xPos, yPos);
}

// -----

PlayerData* playerDataArr[4] = {
		(PlayerData*)(0x00555130 + (0xAFC * 0)),
		(PlayerData*)(0x00555130 + (0xAFC * 1)),
		(PlayerData*)(0x00555130 + (0xAFC * 2)),
		(PlayerData*)(0x00555130 + (0xAFC * 3)),
};

bool shouldDisplayDebugInfo = false;
void displayDebugInfo() {
	
	if (!shouldDisplayDebugInfo) {
		return;
	}

	for (int i = 0; i < 4; i++) {
		if (playerDataArr[i]->exists) {
			//playerDataArr[i]->display();
		}
	}



}