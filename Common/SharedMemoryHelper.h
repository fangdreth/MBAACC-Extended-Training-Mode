#pragma once
#include "..\Common\Common.h"

#ifdef Obsolete
char* pBuffer = nullptr;
const LPCWSTR sSharedName = L"MBAACCExtendedTrainingMode";
bool bInitialized = false;
enum eSharedOffsets {
	SHARE_IDLEHIGHLIGHT, SHARE_BLOCKINGHIGHLIGHT, SHARE_HITHIGHLIGHT, SHARE_ARMORHIGHLIGHT, SHARE_THROWPROTECTIONHIGHLIGHT, SHARE_TEMP4HIGHLIGHT,
	SHARE_REVERSALINDEX1, SHARE_REVERSALINDEX2, SHARE_REVERSALINDEX3, SHARE_REVERSALINDEX4, SHARE_REVERSALDELAYFRAMES, SHARE_REVERSALTYPE,
	SHARE_FREEZEKEY, SHARE_FRAMESTEPKEY, SHARE_HITBOXESDISPLAYKEY, SHARE_FRAMEDATADISPLAYKEY, SHARE_HIGHLIGHTSONKEY, SHARE_SAVESTATEKEY
};
const int nSharedSize = 18 * sizeof(int);

std::array<BYTE, 3> CreateColorArray(int nHighlightID)
{
	switch (nHighlightID)
	{
	default:
	case NO_HIGHLIGHT:
		return { 255, 255, 255 };
	case RED_HIGHLIGHT:
		return { 255, 90, 90 };
	case YELLOW_HIGHLIGHT:
		return { 255, 255, 90 };
	case GREEN_HIGHLIGHT:
		return { 90, 255, 90 };
	case BLUE_HIGHLIGHT:
		return { 90, 90, 255 };
	case PURPLE_HIGHLIGHT:
		return { 255, 90, 255 };
	case BLACK_HIGHLIGHT:
		return { 90, 90, 90 };
	}
}

void InitializeSharedMemoryHelper()
{
	HANDLE hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, nSharedSize, sSharedName);
	if (hMapFile == NULL) {
		std::cerr << "CreateFileMapping failed: " << GetLastError() << std::endl;
		return;
	}

	pBuffer = (char*)MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, nSharedSize);
	if (pBuffer == NULL) {
		std::cerr << "MapViewOfFile failed: " << GetLastError() << std::endl;
		CloseHandle(hMapFile);
		return;
	}
	bInitialized = true;
}

/*void SaveFreezeKey(int* pnFreezeKey)
{
	if (!bInitialized)
		InitializeSharedMemoryHelper();

	CopyMemory(pBuffer + SHARE_FREEZEKEY * sizeof(int), pnFreezeKey, sizeof(int));
}

void SaveFrameStepKey(int* pnFrameStepKey)
{
	if (!bInitialized)
		InitializeSharedMemoryHelper();

	CopyMemory(pBuffer + SHARE_FRAMESTEPKEY * sizeof(int), pnFrameStepKey, sizeof(int));
}

void SaveHitboxesDisplayKey(int* pnHitboxesDisplayKey)
{
	if (!bInitialized)
		InitializeSharedMemoryHelper();

	CopyMemory(pBuffer + SHARE_HITBOXESDISPLAYKEY * sizeof(int), pnHitboxesDisplayKey, sizeof(int));
}

void SaveFrameDataDisplayKey(int* pnFrameDataDisplayKey)
{
	if (!bInitialized)
		InitializeSharedMemoryHelper();

	CopyMemory(pBuffer + SHARE_FRAMEDATADISPLAYKEY * sizeof(int), pnFrameDataDisplayKey, sizeof(int));
}

void SaveHighlightsOnKey(int* pnHighlightsOnKey)
{
	if (!bInitialized)
		InitializeSharedMemoryHelper();

	CopyMemory(pBuffer + SHARE_HIGHLIGHTSONKEY * sizeof(int), pnHighlightsOnKey, sizeof(int));
}

void SaveSaveStateKey(int* pnSaveStateKey)
{
	if (!bInitialized)
		InitializeSharedMemoryHelper();

	CopyMemory(pBuffer + SHARE_SAVESTATEKEY * sizeof(int), pnSaveStateKey, sizeof(int));
}*/

// I don't love this implementation.  probably going to make separate methods for each instead.
// Putting it in its own class might be better too.
void SetSharedMemory(int* pnIdleHighlightSetting = nullptr,
	int* pnBlockingHighlightSetting = nullptr,
	int* pnHitHighlightSetting = nullptr,
	int* pnArmorHighlightSetting = nullptr,
	int* pnThrowProtectionHighlightSetting = nullptr,
	int* pnTemp4HighlightSetting = nullptr)
{
	static bool bSharedMemoryInit = false;
	static HANDLE hMapFile = NULL;
	static char* pBuf = NULL;

	if (!bSharedMemoryInit) {
		hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, nSharedSize, sSharedName);
		if (hMapFile == NULL) {
			std::cerr << "CreateFileMapping failed: " << GetLastError() << std::endl;
			return;
		}

		pBuf = (char*)MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, nSharedSize);
		if (pBuf == NULL) {
			std::cerr << "MapViewOfFile failed: " << GetLastError() << std::endl;
			CloseHandle(hMapFile);
			return;
		}
		bSharedMemoryInit = true;
	}

	//BYTE arrSendBuffer[nSharedSize];

	//*(DWORD*)&(arrSendBuffer[0 * sizeof(int)]) = nIdleHighlightSetting;
	//*(DWORD*)&(arrSendBuffer[1 * sizeof(int)]) = nBlockingHighlightSetting;

	//CopyMemory(pBuf, &arrSendBuffer, nSharedSize);

	int nRunningOffset = 0;

	if (pnIdleHighlightSetting)
		CopyMemory(pBuf + nRunningOffset, pnIdleHighlightSetting, sizeof(int));
	nRunningOffset += sizeof(int);

	if (pnBlockingHighlightSetting)
		CopyMemory(pBuf + nRunningOffset, pnBlockingHighlightSetting, sizeof(int));
	nRunningOffset += sizeof(int);

	if (pnHitHighlightSetting)
		CopyMemory(pBuf + nRunningOffset, pnHitHighlightSetting, sizeof(int));
	nRunningOffset += sizeof(int);

	if (pnArmorHighlightSetting)
		CopyMemory(pBuf + nRunningOffset, pnArmorHighlightSetting, sizeof(int));
	nRunningOffset += sizeof(int);

	if (pnThrowProtectionHighlightSetting)
		CopyMemory(pBuf + nRunningOffset, pnThrowProtectionHighlightSetting, sizeof(int));
	nRunningOffset += sizeof(int);

	if (pnTemp4HighlightSetting)
		CopyMemory(pBuf + nRunningOffset, pnTemp4HighlightSetting, sizeof(int));
	nRunningOffset += sizeof(int);
}

void GetSharedMemory(std::array<BYTE, 3>* parrIdleHighlightSetting,
	std::array<BYTE, 3>* parrBlockingHighlightSetting,
	std::array<BYTE, 3>* parrHitHighlightSetting,
	std::array<BYTE, 3>* parrArmorHighlightSetting,
	std::array<BYTE, 3>* parrThrowProtectionHighlightSetting,
	std::array<BYTE, 3>* parrTemp4HighlightSetting)
{
	if (!bInitialized)
		InitializeSharedMemoryHelper();

	/*static bool bSharedMemoryInit = false;
	static HANDLE hMapFile = NULL;
	static LPVOID pBuf = NULL;

	if (!bSharedMemoryInit)
	{
		hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, sSharedName);
		if (hMapFile == NULL)
		{
			return;
		}

		pBuf = MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, nSharedSize);
		if (pBuf == NULL)
		{
			CloseHandle(hMapFile);
			return;
		}
		bSharedMemoryInit = true;
	}*/

	if (parrIdleHighlightSetting)
	{
		int nIdleHighlightSetting = ((int*)pBuffer)[SHARE_IDLEHIGHLIGHT];
		*parrIdleHighlightSetting = CreateColorArray(nIdleHighlightSetting);
	}

	if (parrBlockingHighlightSetting)
	{
		int nBlockingHighlightSetting = ((int*)pBuffer)[SHARE_BLOCKINGHIGHLIGHT];
		*parrBlockingHighlightSetting = CreateColorArray(nBlockingHighlightSetting);
	}

	if (parrHitHighlightSetting)
	{
		int nHitHighlightSetting = ((int*)pBuffer)[SHARE_HITHIGHLIGHT];
		*parrHitHighlightSetting = CreateColorArray(nHitHighlightSetting);
	}

	if (parrArmorHighlightSetting)
	{
		int nArmorHighlightSetting = ((int*)pBuffer)[SHARE_ARMORHIGHLIGHT];
		*parrArmorHighlightSetting = CreateColorArray(nArmorHighlightSetting);
	}

	if (parrThrowProtectionHighlightSetting)
	{
		int nThrowProtectionHighlightSetting = ((int*)pBuffer)[SHARE_THROWPROTECTIONHIGHLIGHT];
		*parrThrowProtectionHighlightSetting = CreateColorArray(nThrowProtectionHighlightSetting);
	}

	if (parrTemp4HighlightSetting)
	{
		int nTemp4HighlightSetting = ((int*)pBuffer)[SHARE_TEMP4HIGHLIGHT];
		*parrTemp4HighlightSetting = CreateColorArray(nTemp4HighlightSetting);
	}
}

#endif