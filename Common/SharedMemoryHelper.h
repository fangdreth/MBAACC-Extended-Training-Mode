#pragma once
#include "..\Common\Common.h"

char* pBuffer = nullptr;
const LPCWSTR sSharedName = L"MBAACCExtendedTrainingMode";
bool bInitialized = false;
enum eSharedOffsets {
	SHARE_IDLEHIGHLIGHT, SHARE_BLOCKINGHIGHLIGHT, SHARE_TEMP1HIGHLIGHT, SHARE_TEMP2HIGHLIGHT, SHARE_TEMP3HIGHLIGHT, SHARE_TEMP4HIGHLIGHT,
	SHARE_REVERSALINDEX1, SHARE_REVERSALINDEX2, SHARE_REVERSALINDEX3, SHARE_REVERSALINDEX4, SHARE_REVERSALDELAYFRAMES, SHARE_REVERSALTYPE,
	SHARE_FREEZEKEY, SHARE_FRAMESTEPKEY, SHARE_HITBOXESDISPLAYKEY, SHARE_FRAMEDATADISPLAYKEY, SHARE_HIGHLIGHTSONKEY
};
const int nSharedSize = 17 * sizeof(int);

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

void SaveFreezeKey(int* pnFreezeKey)
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

// I don't love this implementation.  probably going to make separate methods for each instead.
// Putting it in its own class might be better too.
void SetSharedMemory(int* pnIdleHighlightSetting = nullptr,
	int* pnBlockingHighlightSetting = nullptr,
	int* pnTemp1HighlightSetting = nullptr,
	int* pnTemp2HighlightSetting = nullptr,
	int* pnTemp3HighlightSetting = nullptr,
	int* pnTemp4HighlightSetting = nullptr,
	int* pnReversalIndex1 = nullptr,
	int* pnReversalIndex2 = nullptr,
	int* pnReversalIndex3 = nullptr,
	int* pnReversalIndex4 = nullptr,
	int* pnReversalDelayFrames = nullptr,
	int* pnReversalType = nullptr)
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

	if (pnTemp1HighlightSetting)
		CopyMemory(pBuf + nRunningOffset, pnTemp1HighlightSetting, sizeof(int));
	nRunningOffset += sizeof(int);

	if (pnTemp2HighlightSetting)
		CopyMemory(pBuf + nRunningOffset, pnTemp2HighlightSetting, sizeof(int));
	nRunningOffset += sizeof(int);

	if (pnTemp3HighlightSetting)
		CopyMemory(pBuf + nRunningOffset, pnTemp3HighlightSetting, sizeof(int));
	nRunningOffset += sizeof(int);

	if (pnTemp4HighlightSetting)
		CopyMemory(pBuf + nRunningOffset, pnTemp4HighlightSetting, sizeof(int));
	nRunningOffset += sizeof(int);

	if (pnReversalIndex1)
		CopyMemory(pBuf + nRunningOffset, pnReversalIndex1, sizeof(int));
	nRunningOffset += sizeof(int);

	if (pnReversalIndex2)
		CopyMemory(pBuf + nRunningOffset, pnReversalIndex2, sizeof(int));
	nRunningOffset += sizeof(int);

	if (pnReversalIndex3)
		CopyMemory(pBuf + nRunningOffset, pnReversalIndex3, sizeof(int));
	nRunningOffset += sizeof(int);

	if (pnReversalIndex4)
		CopyMemory(pBuf + nRunningOffset, pnReversalIndex4, sizeof(int));
	nRunningOffset += sizeof(int);

	if (pnReversalDelayFrames)
		CopyMemory(pBuf + nRunningOffset, pnReversalDelayFrames, sizeof(int));
	nRunningOffset += sizeof(int);

	if (pnReversalType)
		CopyMemory(pBuf + nRunningOffset, pnReversalType, sizeof(int));
	nRunningOffset += sizeof(int);
}

void GetSharedMemory(std::array<BYTE, 3>* parrIdleHighlightSetting,
	std::array<BYTE, 3>* parrBlockingHighlightSetting,
	std::array<BYTE, 3>* parrTemp1HighlightSetting,
	std::array<BYTE, 3>* parrTemp2HighlightSetting,
	std::array<BYTE, 3>* parrTemp3HighlightSetting,
	std::array<BYTE, 3>* parrTemp4HighlightSetting,
	int* pnReversalIndex1,
	int* pnReversalIndex2,
	int* pnReversalIndex3,
	int* pnReversalIndex4,
	int* pnReversalDelayFrames,
	int* pnReversalType,
	int* pnFreezeKey,
	int* pnFrameStepKey,
	int* pnHitboxesDisplayKey,
	int* pnFrameDataDisplayKey,
	int* pnHighlightsOnKey)
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
		switch (nIdleHighlightSetting)
		{
		default:
		case NO_HIGHLIGHT:
			*parrIdleHighlightSetting = { 255, 255, 255 };
			break;
		case RED_HIGHLIGHT:
			*parrIdleHighlightSetting = { 255, 90, 90 };
			break;
		case GREEN_HIGHLIGHT:
			*parrIdleHighlightSetting = { 90, 255, 90 };
			break;
		case BLUE_HIGHLIGHT:
			*parrIdleHighlightSetting = { 90, 90, 255 };
			break;
		}
	}

	if (parrBlockingHighlightSetting)
	{
		int nBlockingHighlightSetting = ((int*)pBuffer)[SHARE_BLOCKINGHIGHLIGHT];
		switch (nBlockingHighlightSetting)
		{
		default:
		case NO_HIGHLIGHT:
			*parrBlockingHighlightSetting = { 255, 255, 255 };
			break;
		case RED_HIGHLIGHT:
			*parrBlockingHighlightSetting = { 255, 90, 90 };
			break;
		case GREEN_HIGHLIGHT:
			*parrBlockingHighlightSetting = { 90, 255, 90 };
			break;
		case BLUE_HIGHLIGHT:
			*parrBlockingHighlightSetting = { 90, 90, 255 };
			break;
		}
	}

	if (parrTemp1HighlightSetting)
	{
		int nTemp1HighlightSetting = ((int*)pBuffer)[SHARE_TEMP1HIGHLIGHT];
		switch (nTemp1HighlightSetting)
		{
		default:
		case NO_HIGHLIGHT:
			*parrTemp1HighlightSetting = { 255, 255, 255 };
			break;
		case RED_HIGHLIGHT:
			*parrTemp1HighlightSetting = { 255, 90, 90 };
			break;
		case GREEN_HIGHLIGHT:
			*parrTemp1HighlightSetting = { 90, 255, 90 };
			break;
		case BLUE_HIGHLIGHT:
			*parrTemp1HighlightSetting = { 90, 90, 255 };
			break;
		}
	}

	if (parrTemp2HighlightSetting)
	{
		int nTemp2HighlightSetting = ((int*)pBuffer)[SHARE_TEMP2HIGHLIGHT];
		switch (nTemp2HighlightSetting)
		{
		default:
		case NO_HIGHLIGHT:
			*parrTemp2HighlightSetting = { 255, 255, 255 };
			break;
		case RED_HIGHLIGHT:
			*parrTemp2HighlightSetting = { 255, 90, 90 };
			break;
		case GREEN_HIGHLIGHT:
			*parrTemp2HighlightSetting = { 90, 255, 90 };
			break;
		case BLUE_HIGHLIGHT:
			*parrTemp2HighlightSetting = { 90, 90, 255 };
			break;
		}
	}

	if (parrTemp3HighlightSetting)
	{
		int nTemp3HighlightSetting = ((int*)pBuffer)[SHARE_TEMP3HIGHLIGHT];
		switch (nTemp3HighlightSetting)
		{
		default:
		case NO_HIGHLIGHT:
			*parrTemp3HighlightSetting = { 255, 255, 255 };
			break;
		case RED_HIGHLIGHT:
			*parrTemp3HighlightSetting = { 255, 90, 90 };
			break;
		case GREEN_HIGHLIGHT:
			*parrTemp3HighlightSetting = { 90, 255, 90 };
			break;
		case BLUE_HIGHLIGHT:
			*parrTemp3HighlightSetting = { 90, 90, 255 };
			break;
		}
	}

	if (parrTemp4HighlightSetting)
	{
		int nTemp4HighlightSetting = ((int*)pBuffer)[SHARE_TEMP4HIGHLIGHT];
		switch (nTemp4HighlightSetting)
		{
		default:
		case NO_HIGHLIGHT:
			*parrTemp4HighlightSetting = { 255, 255, 255 };
			break;
		case RED_HIGHLIGHT:
			*parrTemp4HighlightSetting = { 255, 90, 90 };
			break;
		case GREEN_HIGHLIGHT:
			*parrTemp4HighlightSetting = { 90, 255, 90 };
			break;
		case BLUE_HIGHLIGHT:
			*parrTemp4HighlightSetting = { 90, 90, 255 };
			break;
		}
	}

	if (pnReversalIndex1)
		*pnReversalIndex1 = ((int*)pBuffer)[SHARE_REVERSALINDEX1];

	if (pnReversalIndex2)
		*pnReversalIndex2 = ((int*)pBuffer)[SHARE_REVERSALINDEX2];

	if (pnReversalIndex3)
		*pnReversalIndex3 = ((int*)pBuffer)[SHARE_REVERSALINDEX3];

	if (pnReversalIndex4)
		*pnReversalIndex4 = ((int*)pBuffer)[SHARE_REVERSALINDEX4];

	if (pnReversalDelayFrames)
		*pnReversalDelayFrames = ((int*)pBuffer)[SHARE_REVERSALDELAYFRAMES];

	if (pnReversalType)
		*pnReversalType = ((int*)pBuffer)[SHARE_REVERSALTYPE];

	if (pnFreezeKey)
		*pnFreezeKey = ((int*)pBuffer)[SHARE_FREEZEKEY];

	if (pnFrameStepKey)
		*pnFrameStepKey = ((int*)pBuffer)[SHARE_FRAMESTEPKEY];

	if (pnHitboxesDisplayKey)
		*pnHitboxesDisplayKey = ((int*)pBuffer)[SHARE_HITBOXESDISPLAYKEY];

	if (pnFrameDataDisplayKey)
		*pnFrameDataDisplayKey = ((int*)pBuffer)[SHARE_FRAMEDATADISPLAYKEY];

	if (pnHighlightsOnKey)
		*pnHighlightsOnKey = ((int*)pBuffer)[SHARE_HIGHLIGHTSONKEY];
}