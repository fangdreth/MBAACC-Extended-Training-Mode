

#include <ws2tcpip.h>
#include <winsock2.h>

#include "Common.h"

#include <vector>
#include <map>
#include <unordered_map>

#include <iostream>
#include <cwctype>
#include <algorithm>
#include <dinput.h>
#include <dxerr.h>
#include <Xinput.h>

#include "..\MBAACC-Extended-Training-Mode\Logger.h"

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

// -----



// -----

bool GetOpenSAVFileName(HANDLE hMBAAHandle, DWORD dwBaseAddress, std::wstring* pwsFileName)
{
	const uint8_t nOne = 1;
	const uint8_t nZero = 0;
	WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedFreezeOverride), &nOne, 1, 0);

	char pcFileName[MAX_PATH];

	OPENFILENAME ofn;
	ZeroMemory(pcFileName, sizeof(pcFileName));
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = (LPWSTR)L"Save State\0*.sav\0";
	ofn.lpstrFile = (LPWSTR)pcFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = (LPWSTR)L"Open Save State";
	ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

	bool bResult = GetOpenFileNameW(&ofn);
	WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedFreezeOverride), &nZero, 1, 0);
	if (bResult)
	{
		*pwsFileName = std::wstring(ofn.lpstrFile);
		return true;
	}
	return false;
}

bool GetSaveSAVFileName(HANDLE hMBAAHandle, DWORD dwBaseAddress, std::wstring* pwsFileName)
{
	const uint8_t nOne = 1;
	const uint8_t nZero = 0;
	WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedFreezeOverride), &nOne, 1, 0);

	char pcFileName[MAX_PATH];

	OPENFILENAME ofn;
	ZeroMemory(pcFileName, sizeof(pcFileName));
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFilter = (LPWSTR)L"Save State\0*.sav\0";
	ofn.lpstrFile = (LPWSTR)pcFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = (LPWSTR)L"Create Save State";
	ofn.lpstrDefExt = L"sav";
	ofn.Flags = OFN_DONTADDTORECENT | OFN_OVERWRITEPROMPT;

	bool bResult = GetSaveFileNameW(&ofn);
	WriteProcessMemory(hMBAAHandle, (LPVOID)(dwBaseAddress + adSharedFreezeOverride), &nZero, 1, 0);
	if (bResult)
	{
		*pwsFileName = std::wstring(ofn.lpstrFile);
		return true;
	}
	return false;
}

void CreateRegistryKey()
{
	try
	{
		SECURITY_DESCRIPTOR SD;
		SECURITY_ATTRIBUTES SA;
		InitializeSecurityDescriptor(&SD, SECURITY_DESCRIPTOR_REVISION);
		//SetSecurityDescriptorDacl(&SD, true, 0, false);
		SA.nLength = sizeof(SA);
		SA.lpSecurityDescriptor = &SD;
		SA.bInheritHandle = false;

		DWORD dwFunc;
		HKEY hKey;
		LPCTSTR sk = L"Software\\MBAACC-Extended-Training-Mode";
		LONG openResult = RegCreateKeyExW(HKEY_CURRENT_USER, sk, 0, (LPTSTR)NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, &SA, &hKey, &dwFunc);
		RegCloseKey(hKey);
	} catch (...)
	{
	}
}

LONG ReadFromRegistry(std::wstring sKey, uint8_t* nValue)
{
	LONG openResult = -1;

	try
	{
		DWORD dwValue = NULL;
		HKEY hKey;
		LPCTSTR sk = L"Software\\MBAACC-Extended-Training-Mode";
		DWORD dwType = REG_DWORD;
		DWORD dwSize = sizeof(nValue);

		openResult = RegOpenKeyEx(HKEY_CURRENT_USER, sk, 0, KEY_READ, &hKey);
		if (openResult == 0)
			openResult = RegQueryValueEx(hKey, sKey.c_str(), 0, &dwType, (LPBYTE)&dwValue, &dwSize);
		if (openResult == 0)
			*nValue = (int)dwValue;

		RegCloseKey(hKey);
	} catch (...)
	{
	}

	return openResult;
}

LONG ReadFromRegistry(std::wstring sKey, int* pnValue)
{
	LONG openResult = -1;

	try
	{
		DWORD dwValue = NULL;
		HKEY hKey;
		LPCTSTR sk = L"Software\\MBAACC-Extended-Training-Mode";
		DWORD dwType = REG_DWORD;
		DWORD dwSize = sizeof(pnValue);

		openResult = RegOpenKeyEx(HKEY_CURRENT_USER, sk, 0, KEY_READ, &hKey);
		if (openResult == 0)
			openResult = RegQueryValueEx(hKey, sKey.c_str(), 0, &dwType, (LPBYTE)&dwValue, &dwSize);
		if (openResult == 0)
			*pnValue = (int)dwValue;

		RegCloseKey(hKey);
	} catch (...)
	{
	}

	return openResult;
}

LONG SetRegistryValue(std::wstring sKey, int nValue)
{
	LONG openResult = -1;

	try
	{
		HKEY hKey;
		LPCTSTR sk = L"Software\\MBAACC-Extended-Training-Mode\\";

		openResult = RegOpenKeyEx(HKEY_CURRENT_USER, sk, 0, KEY_WRITE, &hKey);
		if (openResult == 0)
			openResult = RegSetValueEx(hKey, sKey.c_str(), 0, REG_DWORD, (unsigned char*)&nValue, sizeof(nValue));

		RegCloseKey(hKey);
	} catch (...)
	{
	}
	return openResult;
}

LONG SetRegistryValue(std::wstring sKey, float fValue)
{
	LONG openResult = -1;

	try
	{
		HKEY hKey;
		LPCTSTR sk = L"Software\\MBAACC-Extended-Training-Mode\\";

		openResult = RegOpenKeyEx(HKEY_CURRENT_USER, sk, 0, KEY_WRITE, &hKey);
		if (openResult == 0)
			openResult = RegSetValueEx(hKey, sKey.c_str(), 0, REG_DWORD, (unsigned char*)&fValue, sizeof(fValue));

		RegCloseKey(hKey);
	} catch (...)
	{
	}
	return openResult;
}

LONG SetRegistryValue(std::wstring sKey, bool bValue)
{
	return SetRegistryValue(sKey, bValue ? 1 : 0);
}

LONG SetRegistryValue(std::wstring sKey, std::string sValue)
{
	LONG openResult = -1;

	try
	{
		HKEY hKey;
		LPCTSTR sk = L"Software\\MBAACC-Extended-Training-Mode\\";
		char pcVal[MAX_PATH];
		strcpy_s(pcVal, sValue.c_str());

		openResult = RegOpenKeyEx(HKEY_CURRENT_USER, sk, 0, KEY_WRITE, &hKey);
		if (openResult == 0)
			openResult = RegSetValueEx(hKey, sKey.c_str(), 0, REG_SZ, (LPBYTE)pcVal, strlen(pcVal) + 1);

		RegCloseKey(hKey);
	} catch (...)
	{
	}
	return openResult;
}

LONG ReadFromRegistry(std::wstring sKey, bool* pbValue)
{
	uint8_t nValue = 0;
	LONG openResult = ReadFromRegistry(sKey, &nValue);
	if (openResult == 0)
		*pbValue = nValue > 0 ? true : false;
	return openResult;
}

LONG ReadFromRegistry(std::wstring sKey, std::string* psValue)
{
	LONG openResult = -1;

	try
	{
		char pcValue[MAX_PATH];
		HKEY hKey;
		LPCTSTR sk = L"Software\\MBAACC-Extended-Training-Mode";
		DWORD dwType = REG_SZ;
		DWORD dwSize = MAX_PATH;

		openResult = RegOpenKeyEx(HKEY_CURRENT_USER, sk, 0, KEY_READ, &hKey);
		if (openResult == 0)
			openResult = RegQueryValueEx(hKey, sKey.c_str(), 0, &dwType, (LPBYTE)&pcValue, &dwSize);
		if (openResult == 0)
			*psValue = std::string(pcValue);

		RegCloseKey(hKey);
	} catch (...)
	{
	}

	return openResult;
}

LONG ReadFromRegistry(std::wstring sKey, float* pfValue)
{
	LONG openResult = -1;

	try
	{
		float pfTempValue = 0.0f;
		HKEY hKey;
		LPCTSTR sk = L"Software\\MBAACC-Extended-Training-Mode";
		DWORD dwType = REG_DWORD;
		DWORD dwSize = MAX_PATH;

		openResult = RegOpenKeyEx(HKEY_CURRENT_USER, sk, 0, KEY_READ, &hKey);
		if (openResult == 0)
			openResult = RegQueryValueEx(hKey, sKey.c_str(), 0, &dwType, (LPBYTE)&pfTempValue, &dwSize);
		if (openResult == 0)
			*pfValue = (float)pfTempValue;

		RegCloseKey(hKey);
	} catch (...)
	{
	}

	return openResult;
}

LONG DeleteRegistry()
{
	LONG openResult = -1;

	try
	{
		HKEY hKey;
		LPCTSTR sk = L"Software\\MBAACC-Extended-Training-Mode";

		openResult = RegOpenKeyEx(HKEY_CURRENT_USER, sk, 0, KEY_ALL_ACCESS, &hKey);
		if (openResult == ERROR_SUCCESS)
		{
			openResult = RegDeleteKeyW(HKEY_CURRENT_USER, sk);
			if (openResult != ERROR_SUCCESS)
				LogError("Unable to delete registry key");
		} else
		{
			LogError("Unable to open registry key");
		}

		RegCloseKey(hKey);
	} catch (...)
	{
	}

	return openResult;
}

