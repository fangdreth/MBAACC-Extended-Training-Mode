
#include "DebugInfo.h"
#include "DirectX.h"

void PlayerData::display() {

}

// -----

static union {
	PlayerData* playerDataArr[4] = {
		(PlayerData*)(0x00555130 + (0xAFC * 0)),
		(PlayerData*)(0x00555130 + (0xAFC * 1)),
		(PlayerData*)(0x00555130 + (0xAFC * 2)),
		(PlayerData*)(0x00555130 + (0xAFC * 3)),
	};
	struct {
		PlayerData* P1Data;
		PlayerData* P2Data;
		PlayerData* P3Data;
		PlayerData* P4Data;
	};
};

bool shouldDisplayDebugInfo = false;
void displayDebugInfo() {
	
	if (!shouldDisplayDebugInfo) {
		return;
	}

	for (int i = 0; i < 4; i++) {
		if (playerDataArr[i]->exists) {
			playerDataArr[i]->display();
		}
	}



}