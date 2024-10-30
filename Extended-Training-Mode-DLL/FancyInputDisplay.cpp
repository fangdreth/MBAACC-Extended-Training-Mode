
#include "DirectX.h"
#include "FancyInputDisplay.h"

InputColumn P1InputBar(0x00555134 + 0x02E7, 10.0f, 124.0f, 24);
InputColumn P2InputBar(0x00555C30 + 0x02E7, 545.0f, 124.0f, 24);

void dualInputDisplay() {

	// font size 13
	// 24 rows are displayed

	if (!_naked_newPauseCallback2_IsPaused) {
		P1InputBar.update();
		P2InputBar.update();
	}

	P1InputBar.dragInfo.enable = *(BYTE*)(0x00400000 + adSharedP1InputDisplay) == INPUT_LIST || *(BYTE*)(0x00400000 + adSharedP1InputDisplay) == INPUT_BOTH;
	P1InputBar.dragInfo.enable &= !(!safeWrite() || isPaused());
	if (P1InputBar.dragInfo.enable) {
		P1InputBar.draw();
	}

	P2InputBar.dragInfo.enable = *(BYTE*)(0x00400000 + adSharedP2InputDisplay) == INPUT_LIST || *(BYTE*)(0x00400000 + adSharedP2InputDisplay) == INPUT_BOTH;
	P2InputBar.dragInfo.enable &= !(!safeWrite() || isPaused());
	if (P2InputBar.dragInfo.enable) {
		P2InputBar.draw();
	}

}

void dualInputDisplayReset() {
	P1InputBar.reset();
	P2InputBar.reset();
}

int InputDisplay::hasDrag = 0;

InputDisplay P1InputDisplay(200.0f - 0.0f, 112.0f, 25.0f, 25.0f * 1.2f, &P1InputBar, 1);
InputDisplay P2InputDisplay(378.0f + 0.0f, 112.0f, 25.0f, 25.0f * 1.2f, &P2InputBar, 2);

void drawFancyInputDisplay() {

	dualInputDisplay();

	P1InputDisplay.dragInfo.enable = *(BYTE*)(0x00400000 + adSharedP1InputDisplay) == INPUT_ARCADE || *(BYTE*)(0x00400000 + adSharedP1InputDisplay) == INPUT_BOTH;
	P1InputDisplay.dragInfo.enable &= !(!safeWrite() || isPaused());
	if (P1InputDisplay.dragInfo.enable) {
		P1InputDisplay.draw();
	}

	P2InputDisplay.dragInfo.enable = *(BYTE*)(0x00400000 + adSharedP2InputDisplay) == INPUT_ARCADE || *(BYTE*)(0x00400000 + adSharedP2InputDisplay) == INPUT_BOTH;
	P2InputDisplay.dragInfo.enable &= !(!safeWrite() || isPaused());
	if (P2InputDisplay.dragInfo.enable) {
		P2InputDisplay.draw();
	}
}
