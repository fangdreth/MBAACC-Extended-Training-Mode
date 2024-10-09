
#include "SaveState.h"

SaveStateManager saveStateManager;

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
	//P1ControlledCharacter     = *(DWORD*)(0x00400000 + adP1ControlledCharacter		 );
	//P1NextControlledCharacter = *(DWORD*)(0x00400000 + adP1NextControlledCharacter	 );
	//P2ControlledCharacter     = *(DWORD*)(0x00400000 + adP2ControlledCharacter		 );
	//P2NextControlledCharacter = *(DWORD*)(0x00400000 + adP2NextControlledCharacter	 );
	
	memcpy((void*)&playerSaves, (void*)0x00555130, 0xAFC * 4);

	
	// should we also save hit effect data?

	// effects start at 0x0067BDE8

	bool foundActiveEffect = false;
	for (int index = 0; index < 1000; index += 16) {
		
		foundActiveEffect = false;
		
		for (int i = 0; i < 16; i++) {
			if (*(BYTE*)(0x0067BDE8 + (0x33C * (index + i)))) {
				foundActiveEffect = true;
				break;
			}
		}

		// the capacity is already there, we need to put something in it. this will also serve to clear out higher index effects when/if going back
		memcpy((void*)&effects.data[index], (void*)(0x0067BDE8 + (0x33C * index)), 0x33C * 16);
		effects.size += 16;// what am i doing

		if (!foundActiveEffect) {
			break;
		}

		// vec starts with 16 capacity. give it 16 more. this might not be the fastest
		effects.addCapacity(16);
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
	//*(DWORD*)(0x00400000 + adP1ControlledCharacter		 ) = P1ControlledCharacter     ;
	//*(DWORD*)(0x00400000 + adP1NextControlledCharacter	 ) = P1NextControlledCharacter ;
	//*(DWORD*)(0x00400000 + adP2ControlledCharacter		 ) = P2ControlledCharacter     ;
	//*(DWORD*)(0x00400000 + adP2NextControlledCharacter	 ) = P2NextControlledCharacter ;

	memcpy((void*)0x00555130, (void*)&playerSaves, 0xAFC * 4);

	memcpy((void*)0x0067BDE8, (void*)effects.data, 0x33C * effects.size);


}

int SaveState::totalMemory() {
	return sizeof(SaveState) + effects.totalMemory();
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

void SaveStateManager::load(int dir) {

	if (states.size() == 0) {
		return;
	}

	if (dir == 1 && currentState == states.size() - 1) {
		// we are at the front of our buffer, just go generate a new frame instead. pause checking occurs AFTER framecallback(i think) which is why needpause=2
		needPause = 2;
		_naked_newPauseCallback2_IsPaused = false;
		return;
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
}

int SaveStateManager::totalMemory() {
	
	int res = 0;

	for (int i = 0; i < states.size(); i++) {
		res += states[i]->totalMemory();
	}

	return res;
}
