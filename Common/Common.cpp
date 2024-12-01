#include "Common.h"

#ifdef ISDLL
void DrawLog(char* s);
#endif

#ifndef ISDLL
IDirectInput8* inputDevice = NULL;
#endif

void __stdcall ___log(const char* msg) {
	const char* ipAddress = "127.0.0.1";
	unsigned short port = 17474;

	int msgLen = strlen(msg);

	const char* message = msg;

	WSADATA wsaData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0) {
		return;
	}

	SOCKET sendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sendSocket == INVALID_SOCKET) {
		WSACleanup();
		return;
	}

	sockaddr_in destAddr;
	destAddr.sin_family = AF_INET;
	destAddr.sin_port = htons(port);
	if (inet_pton(AF_INET, ipAddress, &destAddr.sin_addr) <= 0) {
		closesocket(sendSocket);
		WSACleanup();
		return;
	}

	int sendResult = sendto(sendSocket, message, msgLen, 0, (sockaddr*)&destAddr, sizeof(destAddr));
	if (sendResult == SOCKET_ERROR) {
		closesocket(sendSocket);
		WSACleanup();
		return;
	}

	closesocket(sendSocket);
	WSACleanup();
}

void __stdcall log(const char* format, ...) {

#ifdef ISDLL
	//static char buffer[1024]; // no more random char buffers everywhere.
	char* buffer = (char*)malloc(1024);
	va_list args;
	va_start(args, format);
	vsnprintf(buffer, 1024, format, args);
	___log(buffer);
	va_end(args);
	DrawLog(buffer);
	#ifdef ENABLEFILELOG
		writeLog(buffer);
	#endif
#else
	char buffer[1024];
	va_list args;
	va_start(args, format);
	vsnprintf(buffer, 1024, format, args);
	___log(buffer);
	va_end(args);
#endif

}

void __stdcall ___log(const wchar_t* msg) {
	const char* ipAddress = "127.0.0.1";
	unsigned short port = 17474;

	int msgLen = wcslen(msg) * sizeof(wchar_t);

	const wchar_t* message = msg;

	WSADATA wsaData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0) {
		return;
	}

	SOCKET sendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sendSocket == INVALID_SOCKET) {
		WSACleanup();
		return;
	}

	sockaddr_in destAddr;
	destAddr.sin_family = AF_INET;
	destAddr.sin_port = htons(port);
	if (inet_pton(AF_INET, ipAddress, &destAddr.sin_addr) <= 0) {
		closesocket(sendSocket);
		WSACleanup();
		return;
	}

	int sendResult = sendto(sendSocket, (char*)message, msgLen, 0, (sockaddr*)&destAddr, sizeof(destAddr));
	if (sendResult == SOCKET_ERROR) {
		closesocket(sendSocket);
		WSACleanup();
		return;
	}

	closesocket(sendSocket);
	WSACleanup();
}

void __stdcall log(const wchar_t* format, ...) {
	// this not going to the screen log isnt ideal, but i cannot even print unicode
	static wchar_t buffer[4096];
	va_list args;
	va_start(args, format);
	_vsnwprintf_s(buffer, 4096, format, args);
	___log(buffer);
	va_end(args);

}

void printDirectXError(HRESULT hr) {
	const char* errorStr = DXGetErrorStringA(hr);
	const char* errorDesc = DXGetErrorDescriptionA(hr);

	log("err: %s\n    %s", errorStr, errorDesc);
}

void printDIJOYSTATE2(const DIJOYSTATE2& state) {

	const int bufLen = 1024;
	static char buffer[bufLen];
	int bufOff = 0;


	bufOff += snprintf(buffer + bufOff, bufLen - bufOff, "DIJOYSTATE2:\n");

#define printDIJOYSTATE2_HELPER(name) bufOff += snprintf(buffer + bufOff, bufLen - bufOff, "%10s %5d ", #name, state.name)

#define printDIJOYSTATE2_NEWLINE() bufOff += snprintf(buffer + bufOff, bufLen - bufOff, "\n")

	printDIJOYSTATE2_HELPER(lX);
	printDIJOYSTATE2_HELPER(lY);
	printDIJOYSTATE2_HELPER(lZ);

	printDIJOYSTATE2_NEWLINE();

	printDIJOYSTATE2_HELPER(lRx);
	printDIJOYSTATE2_HELPER(lRy);
	printDIJOYSTATE2_HELPER(lRz);

	printDIJOYSTATE2_NEWLINE();

	printDIJOYSTATE2_HELPER(lVX);
	printDIJOYSTATE2_HELPER(lVY);
	printDIJOYSTATE2_HELPER(lVZ);

	printDIJOYSTATE2_NEWLINE();

	printDIJOYSTATE2_HELPER(lVRx);
	printDIJOYSTATE2_HELPER(lVRy);
	printDIJOYSTATE2_HELPER(lVRz);

	printDIJOYSTATE2_NEWLINE();

	printDIJOYSTATE2_HELPER(lAX);
	printDIJOYSTATE2_HELPER(lAY);
	printDIJOYSTATE2_HELPER(lAZ);

	printDIJOYSTATE2_NEWLINE();

	printDIJOYSTATE2_HELPER(lARx);
	printDIJOYSTATE2_HELPER(lARy);
	printDIJOYSTATE2_HELPER(lARz);

	printDIJOYSTATE2_NEWLINE();

	printDIJOYSTATE2_HELPER(lFX);
	printDIJOYSTATE2_HELPER(lFY);
	printDIJOYSTATE2_HELPER(lFZ);

	printDIJOYSTATE2_NEWLINE();

	printDIJOYSTATE2_HELPER(lFRx);
	printDIJOYSTATE2_HELPER(lFRy);
	printDIJOYSTATE2_HELPER(lFRz);

	printDIJOYSTATE2_NEWLINE();

	printDIJOYSTATE2_HELPER(rgdwPOV[0]);
	printDIJOYSTATE2_HELPER(rgdwPOV[1]);
	printDIJOYSTATE2_HELPER(rgdwPOV[2]);
	printDIJOYSTATE2_HELPER(rgdwPOV[3]);

	printDIJOYSTATE2_NEWLINE();

	log(buffer);
}

XINPUT_STATE* KeyState::xState = new XINPUT_STATE();
XINPUT_STATE* KeyState::prevxState = new XINPUT_STATE();

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

			xState->Gamepad.bLeftTrigger  = MAX(xState->Gamepad.bLeftTrigger,   tempState.Gamepad.bLeftTrigger);
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


