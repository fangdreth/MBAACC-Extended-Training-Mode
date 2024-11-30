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

IDirectInputDevice8* testController = NULL;

BOOL CALLBACK directInputControllerCallback(const DIDEVICEINSTANCE* deviceInst, void* pCtx) {

	DWORD* guid = (DWORD*)&deviceInst->guidInstance;

	//log(L"found device. ID: %08X%08X%08X%08X name: %s", guid[0], guid[1], guid[2], guid[3], deviceInst->tszInstanceName);

	//return DIENUM_CONTINUE;

	if (testController != NULL) {
		testController->Unacquire();
		testController->Release();
		testController = NULL;
	}

	HRESULT hr;

	hr = inputDevice->CreateDevice(deviceInst->guidInstance, &testController, NULL);

	if (FAILED(hr)) {
		log("failed to create IDirectInputDevice8 for game device!");
		testController = NULL;
		return DIENUM_STOP;
	}

	return DIENUM_CONTINUE;
}

void KeyState::refreshDeviceList() {
	// this should start a thread for // inputDevice->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, nullptr, DIEDFL_ATTACHEDONLY);
	// and add them all to a static list. 
	
	HRESULT hr;

#ifndef ISDLL
	// we are in the console window, and need to actually create our own IDirectInput8
	if (inputDevice == NULL) {
		hr = DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&inputDevice, nullptr);
		if (FAILED(hr)) {
			log("directinput8create failed in console window");
			inputDevice = NULL;
			return;
		}
	}
#endif


	//log("refreshDeviceList called");

	if (testController == NULL) {
		hr = inputDevice->EnumDevices(DI8DEVCLASS_GAMECTRL, directInputControllerCallback, NULL, DIEDFL_ATTACHEDONLY);

		if (FAILED(hr)) {
			log("EnumDevices failed");
			testController = NULL;
			return;
		}

		if (testController == NULL) {
			log("testController null");
			return;
		}

		// the data format MUST be set before the device can be acquired
		// the data format CANNOT be changed while the device is acquired
		// cooplevel MUST be set before acquire 


#ifdef ISDLL
		HWND hwnd = (HWND) * (DWORD*)(0x0074dfac);
#else
		HWND hwnd = GetConsoleWindow();
#endif

		hr = testController->SetCooperativeLevel(hwnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
		if (FAILED(hr)) {
			log("SetCooperativeLevel failed");
			return;
		}

		hr = testController->SetDataFormat(&c_dfDIJoystick2);
		if (FAILED(hr)) {
			log("SetDataFormat failed");
			printDirectXError(hr);
			return;
		}

		hr = testController->Acquire();
		if (FAILED(hr)) {
			log("Acquire failed");
			printDirectXError(hr);
			return;
		}

		log("DEVICE SETUP SUCCEEDED");

	}
	

	//log("directInput is OK");

	if (testController == NULL) {
		log("testController was null");
		return;
	}
	
	hr = testController->Poll();
	if (FAILED(hr)) {
		// should i attempt to reacquire the device?
		//testController->Acquire();
		log("poll failed");
		printDirectXError(hr);
		if (hr == DIERR_INPUTLOST) {
			log("DIERR_INPUTLOST, im nulling the device");
			testController->Unacquire();
			testController = NULL;
			return;
		}
		return;
	}

	DIJOYSTATE2 joyState = {};
	memset(&joyState, 0, sizeof(DIJOYSTATE2));
	hr = testController->GetDeviceState(sizeof(DIJOYSTATE2), &joyState);
	if (FAILED(hr)) {
		log("GetDeviceState failed");
		return;
	}

	log("CLEAR");

	printDIJOYSTATE2(joyState);

	//log("everything worked, somehow");

}
