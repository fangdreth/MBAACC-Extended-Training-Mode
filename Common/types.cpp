


#include <chrono>


#include "Common.h"
#include <Windows.h>

#include "types.h"

long long getMicroSec() {
	return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
	//return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now()).count();
}

long long getNanoSec() {
	return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
}

XINPUT_STATE* KeyState::xState = new XINPUT_STATE();
XINPUT_STATE* KeyState::prevxState = new XINPUT_STATE();

KeyState::KeyState() {
	nKey = 0x00;
	nHeldKeyCounter = 0;
}

KeyState::KeyState(int k)
{
	nKey = k;
	nHeldKeyCounter = 0;
}

void KeyState::setKey(int vKey_)
{
	if (nKey != vKey_)
	{
		// reset the key state here to prevent it from firing
		// when it gets changed
		prevState = true;
		nKey = vKey_;
	}
}

bool KeyState::isFocused()
{
	return GetActiveWindow() == GetForegroundWindow();
}

bool KeyState::keyHeld()
{
	if (!isFocused()) {
		freqHeldCounter = 0;
		return false;
	}

	bool res = nKey != 0x0 && GetAsyncKeyState(nKey) & 0x8000;
	if (res) {
		freqHeldCounter++;
	} else {
		freqHeldCounter = 0;
	}
	return res;
}

bool KeyState::keyDown()
{
	if (nKey == 0x0 || !isFocused()) {
		return false;
	}

	tempState = false;
	if (GetAsyncKeyState(nKey) & 0x8000)
		tempState = true;

	bool res = false;
	if (!prevState && tempState)
	{
		res = true;
	}

	prevState = tempState;

	return res;
}

void KeyState::updateControllers() {

	// tbh, ideally, this should be called once a frame, and grab the WHOLE keyboard.

	// swap the current and prev states
	std::swap(xState, prevxState);

	XINPUT_STATE tempState;
	ZeroMemory(&tempState, sizeof(XINPUT_STATE));
	ZeroMemory(xState, sizeof(XINPUT_STATE));

	for (int i = 0; i < 4; i++) {

		DWORD dwResult = XInputGetState(i, &tempState);

		if (dwResult == ERROR_SUCCESS) {

			// this currently combines all devices into one. there is most likely a better implementation of this!
			xState->Gamepad.wButtons |= tempState.Gamepad.wButtons;

			xState->Gamepad.bLeftTrigger = MAX(xState->Gamepad.bLeftTrigger, tempState.Gamepad.bLeftTrigger);
			xState->Gamepad.bRightTrigger = MAX(xState->Gamepad.bRightTrigger, tempState.Gamepad.bRightTrigger);

			// i want to be sure to get the max of any stick pos
			xState->Gamepad.sThumbLX = abs(xState->Gamepad.sThumbLX) > abs(tempState.Gamepad.sThumbLX) ? xState->Gamepad.sThumbLX : tempState.Gamepad.sThumbLX;
			xState->Gamepad.sThumbLY = abs(xState->Gamepad.sThumbLY) > abs(tempState.Gamepad.sThumbLY) ? xState->Gamepad.sThumbLY : tempState.Gamepad.sThumbLY;
			xState->Gamepad.sThumbRX = abs(xState->Gamepad.sThumbRX) > abs(tempState.Gamepad.sThumbRX) ? xState->Gamepad.sThumbRX : tempState.Gamepad.sThumbRX;
			xState->Gamepad.sThumbRY = abs(xState->Gamepad.sThumbRY) > abs(tempState.Gamepad.sThumbRY) ? xState->Gamepad.sThumbRY : tempState.Gamepad.sThumbRY;
		}
	}
}

void KeyState::showControllerState() {

	short wButtons = xState->Gamepad.wButtons;
	short wprevButtons = prevxState->Gamepad.wButtons;

	log("CLEAR");

	log("A %d", !!(wButtons & XINPUT_GAMEPAD_A));
	log("B %d", !!(wButtons & XINPUT_GAMEPAD_B));
	log("X %d", !!(wButtons & XINPUT_GAMEPAD_X));
	log("Y %d", !!(wButtons & XINPUT_GAMEPAD_Y));

	log("U %d", !!(wButtons & XINPUT_GAMEPAD_DPAD_UP));
	log("D %d", !!(wButtons & XINPUT_GAMEPAD_DPAD_DOWN));
	log("L %d", !!(wButtons & XINPUT_GAMEPAD_DPAD_LEFT));
	log("R %d", !!(wButtons & XINPUT_GAMEPAD_DPAD_RIGHT));

	log("START %d", !!(wButtons & XINPUT_GAMEPAD_START));
	log("BACK  %d", !!(wButtons & XINPUT_GAMEPAD_BACK));

	log(" ");

	log("left  X %6d", xState->Gamepad.sThumbLX);
	log("left  Y %6d", xState->Gamepad.sThumbLY);
	log("thum    %d", !!(wButtons & XINPUT_GAMEPAD_LEFT_THUMB));

	log(" ");

	log("right X %6d", xState->Gamepad.sThumbRX);
	log("right Y %6d", xState->Gamepad.sThumbRY);
	log("thumb   %d", !!(wButtons & XINPUT_GAMEPAD_RIGHT_THUMB));

	log(" ");

	log("lTrig %6d", xState->Gamepad.bLeftTrigger);
	log("lBtn  %d", !!(wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER));
	log("rTrig %6d", xState->Gamepad.bRightTrigger);
	log("rBtn  %d", !!(wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER));

}

short KeyState::pressedButtons() {
	return (prevxState->Gamepad.wButtons ^ xState->Gamepad.wButtons) & xState->Gamepad.wButtons;
}

short KeyState::releasedButtons() {
	return (prevxState->Gamepad.wButtons ^ xState->Gamepad.wButtons) & prevxState->Gamepad.wButtons;
}
