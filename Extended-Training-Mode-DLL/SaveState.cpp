
#include "SaveState.h"
#include "DirectX.h"
#include "..\Common\Common.h"
#include "dllmain.h"
#include <shobjidl.h> 

SaveStateManager saveStateManager;

SaveState::~SaveState() {
	if (P3 != NULL) {
		free(P3);
	}

	if (P4 != NULL) {
		free(P4);
	}
}

void SaveState::save() {

	slowMo = *(WORD*)(0x0055d208);
	//reallyNotSure = *(DWORD*)(0x0055FD04);
	
	//GlobalFreeze              = *(BYTE*) (0x00400000 + adGlobalFreeze				 );
	//SaveDestinationCamX       = *(DWORD*)(0x00400000 + adSaveDestinationCamX		 );
	//SaveDestinationCamY       = *(DWORD*)(0x00400000 + adSaveDestinationCamY		 );
	
	//SaveCurrentCamX           = *(DWORD*)(0x00400000 + adSaveCurrentCamX			 );
	//SaveCurrentCamXCopy       = *(DWORD*)(0x00400000 + adSaveCurrentCamXCopy		 );
	//SaveCurrentCamY           = *(DWORD*)(0x00400000 + adSaveCurrentCamY			 );
	//SaveCurrentCamYCopy       = *(DWORD*)(0x00400000 + adSaveCurrentCamYCopy		 );
	//SaveCurrentCamZoom        = *(DWORD*)(0x00400000 + adSaveCurrentCamZoom			 );
	//SaveDestinationCamZoom    = *(DWORD*)(0x00400000 + adSaveDestinationCamZoom		 );
	P1ControlledCharacter     = *(DWORD*)(0x00400000 + adP1ControlledCharacter		 );
	P1NextControlledCharacter = *(DWORD*)(0x00400000 + adP1NextControlledCharacter	 );
	P2ControlledCharacter     = *(DWORD*)(0x00400000 + adP2ControlledCharacter		 );
	P2NextControlledCharacter = *(DWORD*)(0x00400000 + adP2NextControlledCharacter	 );

	P1Inactionable            = *(int*)(0x00400000 + adP1Inaction);
	P2Inactionable            = *(int*)(0x00400000 + adP2Inaction);
	FrameTimer                = *(int*)(0x00400000 + adFrameCount);
	TrueFrameTimer            = *(int*)(0x00400000 + adTrueFrameCount);
	
	//memcpy((void*)&playerSaves, (void*)0x00555130, 0xAFC * 4);

	memcpy((void*)&P1, (void*)(0x00555130 + (0xAFC * 0)), PlayerSaveSize);
	memcpy((void*)&P2, (void*)(0x00555130 + (0xAFC * 1)), PlayerSaveSize);
	//memcpy((void*)&P3, (void*)(0x00555130 + (0xAFC * 2)), 0x33C);
	//memcpy((void*)&P4, (void*)(0x00555130 + (0xAFC * 3)), 0x33C);

	if (*(BYTE*)(0x00555130 + (0xAFC * 2)) != 0) {
		P3 = (PlayerSave*)malloc(1 * sizeof(PlayerSave));
		memcpy((void*)P3, (void*)(0x00555130 + (0xAFC * 2)), PlayerSaveSize);
	}
	
	if (*(BYTE*)(0x00555130 + (0xAFC * 3)) != 0) {
		P4 = (PlayerSave*)malloc(1 * sizeof(PlayerSave));
		memcpy((void*)P4, (void*)(0x00555130 + (0xAFC * 3)), PlayerSaveSize);
	}

	// should we also save hit effect data? (would need to fix hit effect pausing for that)

	// effects start at 0x0067BDE8

	constexpr int chunkSize = 4; // higher this is, more memory, faster(probs), and safer tbh

	bool foundActiveEffect = false;
	for (int index = 0; index < 1000; index += chunkSize) {
		
		foundActiveEffect = false;
		
		// this is chunkSize * 2 out of paranoia, and leads to shit code
		for (int i = 0; i < chunkSize*2; i++) {
			if (*(BYTE*)(0x0067BDE8 + (0x33C * (index + i)))) {
				foundActiveEffect = true;
				break;
			}
		}

		if (!foundActiveEffect) {
			break;
		}

		// vec starts with 16 capacity. give it 16 more. this might not be the fastest
		effects.addCapacity(chunkSize);

		// the capacity is already there, we need to put something in it. this will also serve to clear out higher index effects when/if going back
		memcpy((void*)&effects.data[index], (void*)(0x0067BDE8 + (0x33C * index)), 0x33C * chunkSize);
		effects.size += chunkSize;// what am i doing
	}
}

void SaveState::load() {

	*(WORD*)(0x0055d208) = slowMo;
	//*(DWORD*)(0x0055FD04) = reallyNotSure;

	//*(BYTE*) (0x00400000 + adGlobalFreeze				 ) = GlobalFreeze              ;
	//*(DWORD*)(0x00400000 + adSaveDestinationCamX		 ) = SaveDestinationCamX       ;
	//*(DWORD*)(0x00400000 + adSaveDestinationCamY		 ) = SaveDestinationCamY       ;
	
	//*(DWORD*)(0x00400000 + adSaveCurrentCamX			 ) = SaveCurrentCamX           ;
	//*(DWORD*)(0x00400000 + adSaveCurrentCamXCopy		 ) = SaveCurrentCamXCopy       ;	 
	//*(DWORD*)(0x00400000 + adSaveCurrentCamY			 ) = SaveCurrentCamY           ;
	//*(DWORD*)(0x00400000 + adSaveCurrentCamYCopy		 ) = SaveCurrentCamYCopy       ;
	//*(DWORD*)(0x00400000 + adSaveCurrentCamZoom			 ) = SaveCurrentCamZoom        ;
	//*(DWORD*)(0x00400000 + adSaveDestinationCamZoom		 ) = SaveDestinationCamZoom    ;
	*(DWORD*)(0x00400000 + adP1ControlledCharacter		 ) = P1ControlledCharacter     ;
	*(DWORD*)(0x00400000 + adP1NextControlledCharacter	 ) = P1NextControlledCharacter ;
	*(DWORD*)(0x00400000 + adP2ControlledCharacter		 ) = P2ControlledCharacter     ;
	*(DWORD*)(0x00400000 + adP2NextControlledCharacter	 ) = P2NextControlledCharacter ;

	*(int*)(0x00400000 + adP1Inaction                    ) = P1Inactionable;
	*(int*)(0x00400000 + adP2Inaction                    ) = P2Inactionable;
	*(int*)(0x00400000 + adFrameCount                    ) = FrameTimer;
	*(int*)(0x00400000 + adTrueFrameCount                ) = TrueFrameTimer;

	//memcpy((void*)0x00555130, (void*)&playerSaves, 0xAFC * 4);
	
	memcpy((void*)(0x00555130 + (0xAFC * 0)), (void*)&P1, PlayerSaveSize);
	memcpy((void*)(0x00555130 + (0xAFC * 1)), (void*)&P2, PlayerSaveSize);

	if (P3 != NULL) {
		memcpy((void*)(0x00555130 + (0xAFC * 2)), (void*)P3, PlayerSaveSize);
	}

	if (P4 != NULL) {
		memcpy((void*)(0x00555130 + (0xAFC * 3)), (void*)P4, PlayerSaveSize);
	}
	
	


	if (effects.data != NULL) {
		// restore previous effects
		memcpy((void*)0x0067BDE8, (void*)effects.data, 0x33C * effects.size);
	}

	// need to disable future effects. could do this with a memcpy but thats trash, just toggle off the enable
	// as for how many,,, not sure, hoping 16 is good
	DWORD addr = 0x0067BDE8 + (0x33C * effects.size);
	for (int i = 0; i < 16; i++) {
		*(BYTE*)(addr) = 0;
		addr += 0x33C;
	}

}

int SaveState::totalMemory() {
	int res = sizeof(SaveState) + effects.totalMemory();

	if (P3 != NULL) {
		res += sizeof(PlayerSave);
	}

	if (P4 != NULL) {
		res += sizeof(PlayerSave);
	}

	return res;
}

void SaveStateManager::save() {

	// if we go forward in time, but still have some states from a previous future, delete them
	// this deleting might end up taking,,, a while, and maybe should be offloaded somewhere else 
	while (states.size() > 0 && currentState < states.size() - 1) {
		delete states.back();
		states.pop_back();
	}

	SaveState* temp = new SaveState();
	temp->save();

	// make sure we arent accidentally move copying, using a pointer is just easier for that tbh
	currentState++;
	states.emplace_back(temp);

	if (states.size() > maxStates) {
		delete states.front();
		states.pop_front();
		currentState--;
	}

}

bool SaveStateManager::load(int dir) {

	if (states.size() == 0) {
		return false;
	}

	if (dir == 1 && currentState == states.size() - 1) {
		// we are at the front of our buffer, just go generate a new frame instead. pause checking occurs AFTER framecallback(i think) which is why needpause=2
		needPause = 2;
		_naked_newPauseCallback2_IsPaused = false;
		return true;
	}

	currentState += dir;
	currentState = CLAMP(currentState, 0, states.size() - 1);

	//delete states.back();
	//states.pop_back();
	//states.back()->load();

	//long long startTime = getMicroSec();
	states[currentState]->load();
	//long long endTime = getMicroSec();
	//long long totalTime = endTime - startTime;
	//log("%3lld.%03lld", totalTime / 1000, totalTime % 1000);

	return false;
}

int SaveStateManager::totalMemory() {
	
	int res = 0;

	for (int i = 0; i < states.size(); i++) {
		res += states[i]->totalMemory();
	}

	return res;
}

void SaveStateManager::log() {

	float totalMB = ((float)saveStateManager.totalMemory()) / ((float)(1 << 20));
	int count = saveStateManager.states.size();
	float avgMB = totalMB / ((float)count);

	TextDraw(200, 200, 12, 0xFFFFFFFF, "savestates: %d, %.4f MB", count, totalMB);
	TextDraw(200, 214, 12, 0xFFFFFFFF, "avg: %.4f KB", (1024.0f) * avgMB);
	TextDraw(200, 228, 12, 0xFFFFFFFF, "pred: %.4f MB", maxStates * avgMB);

}


void FullSave::save() {
	memcpy(CameraZoom, (void*)0x0054eb70, 0x4 * 3);
	memcpy(CameraCoordsDestination, (void*)0x00555124, 0x4 * 3);
	memcpy((void*)&P1, (void*)(0x00555130 + (0xAFC * 0)), PlayerSaveSize);
	memcpy((void*)&P2, (void*)(0x00555130 + (0xAFC * 1)), PlayerSaveSize);
	memcpy((void*)&P3, (void*)(0x00555130 + (0xAFC * 2)), PlayerSaveSize);
	memcpy((void*)&P4, (void*)(0x00555130 + (0xAFC * 3)), PlayerSaveSize);
	memcpy(PlayerData, (void*)0x00557db8, 0x830);
	memcpy(StopSituation, (void*)0x00558600, 0xF38);
	FrameCount = *(DWORD*)(0x00400000 + adFrameCount);
	SlowMo = *(WORD*)(0x0055d208);
	memcpy(CameraCoords, (void*)0x0055dec4, 0x4 * 3);
	TrueFrameCount = *(DWORD*)(0x00400000 + adTrueFrameCount);
	GlobalFreeze = *(DWORD*)(0x00400000 + adGlobalFreeze);
	memcpy(RNG, (void*)0x00564068, 0xe4);
	memcpy(CameraCoordsNext, (void*)0x00564b14, 0x4 * 3);
	memcpy(Effects, (void*)(0x0067BDE8), 0x33C * 1000);

	IsSaved = true;
}

void FullSave::load(bool LoadRNG) {
	memcpy((void*)0x0054eb70, CameraZoom, 0x4 * 3);
	memcpy((void*)0x00555124, CameraCoordsDestination, 0x4 * 3);
	memcpy((void*)(0x00555130 + (0xAFC * 0)), (void*)&P1, PlayerSaveSize);
	memcpy((void*)(0x00555130 + (0xAFC * 1)), (void*)&P2, PlayerSaveSize);
	memcpy((void*)(0x00555130 + (0xAFC * 2)), (void*)&P3, PlayerSaveSize);
	memcpy((void*)(0x00555130 + (0xAFC * 3)), (void*)&P4, PlayerSaveSize);
	memcpy((void*)0x00557db8, PlayerData, 0x830);
	memcpy((void*)0x00558600, StopSituation, 0xF38);
	*(DWORD*)(0x00400000 + adFrameCount) = FrameCount;
	*(WORD*)(0x0055d208) = SlowMo;
	memcpy((void*)0x0055dec4, CameraCoords, 0x4 * 3);
	*(DWORD*)(0x00400000 + adTrueFrameCount) = TrueFrameCount;
	*(DWORD*)(0x00400000 + adGlobalFreeze) = GlobalFreeze;
	if (LoadRNG) memcpy((void*)0x00564068, RNG, 0xe4);
	memcpy((void*)0x00564b14, CameraCoordsNext, 0x4 * 3);
	memcpy((void*)(0x0067BDE8), Effects, 0x33C * 1000);
}

static bool LoadFileExplorer(std::wstring& filePath)
{
	IFileOpenDialog* pFileOpen;
	HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

	if (SUCCEEDED(hr))
	{
		hr = pFileOpen->Show(NULL); // Display the dialog
		if (SUCCEEDED(hr))
		{
			IShellItem* pItem;
			hr = pFileOpen->GetResult(&pItem);
			if (SUCCEEDED(hr))
			{
				PWSTR pszFilePath;
				hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
				if (SUCCEEDED(hr))
				{
					filePath = pszFilePath;
					CoTaskMemFree(pszFilePath);
					pItem->Release();
					pFileOpen->Release();
					return true;
				}
				pItem->Release();
			}
		}
		pFileOpen->Release();
	}
	return false;
}

static bool SaveFileExplorer(std::wstring& filePath)
{
    IFileSaveDialog* pFileSave;
    HRESULT hr = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_INPROC_SERVER, IID_IFileSaveDialog, reinterpret_cast<void**>(&pFileSave));

    if (SUCCEEDED(hr))
    {
        hr = pFileSave->Show(NULL); // Display the dialog
        if (SUCCEEDED(hr))
        {
            IShellItem* pItem;
            hr = pFileSave->GetResult(&pItem);
            if (SUCCEEDED(hr))
            {
                PWSTR pszFilePath;
                hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
                if (SUCCEEDED(hr))
                {
					filePath = pszFilePath;
                    CoTaskMemFree(pszFilePath);
					pItem->Release();
					pFileSave->Release();
					return true;
                }
                pItem->Release();
            }
        }
		pFileSave->Release();
    }
	return false;
}

void FullSave::saveToFile() {
	if (!IsSaved) return;
	try
	{
		std::wstring wsFileName;
		if (SaveFileExplorer(wsFileName))
		{
			std::ofstream SaveOutFile;
			SaveOutFile.open(wsFileName);
			SaveOutFile.write(reinterpret_cast<char*>(this), sizeof(*this));
			SaveOutFile.close();
		}
	}
	catch (...) {
		std::string sErrorString = "UNABLE TO CREATE SAVE STATE FILE";
		int nReturnVal = MessageBoxA(NULL, sErrorString.c_str(), "", MB_ICONWARNING);
		LogError("UNABLE TO CREATE SAVE STATE FILE");
	}
}

void FullSave::loadFromFile() {
	try
	{
		std::wstring wsFileName;
		if (LoadFileExplorer(wsFileName))
		{
			std::ifstream SaveInFile;
			SaveInFile.open(wsFileName);
			SaveInFile.read(reinterpret_cast<char*>(this), sizeof(*this));
			SaveInFile.close();
			IsSaved = true;
		}
	}
	catch (...)
	{
		std::wstring wsErrorString = L"UNABLE TO PARSE SAVE STATE FILE";
		int nReturnVal = MessageBoxW(NULL, wsErrorString.c_str(), L"", MB_ICONWARNING);
		LogError("UNABLE TO PARSE SAVE STATE FILE");
	}
}

void FullSave::unsave() {
	IsSaved = false;
}