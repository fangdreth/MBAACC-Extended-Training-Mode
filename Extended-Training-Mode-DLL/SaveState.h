#pragma once
#include <deque>
#include "DirectX.h"

/*

tbh i think an important priority here is to minimize the size of savestates
we could only record the changes between each state maybe?

*/

typedef struct PlayerSave {
	BYTE data[0xAFC];
} PlayerSave;

typedef struct EffectSave {
	BYTE data[0x33C];
} EffectSave;

// ideally, memory would be aligned for quick load/save, but idrk
typedef struct SaveState {

	union {
		struct {
			PlayerSave P1;
			PlayerSave P2;
			PlayerSave P3;
			PlayerSave P4;
		};
		PlayerSave playerSaves[4];
	};

	// reasoning for [16] is that copying 16 effects at a time is faster
	// reasoning for not using vector: cannot resize without default init. and for 0x33C * 16, thats way to much
	Vec<EffectSave> effects = Vec<EffectSave>(16);

	// put other variables in here

	//BYTE GlobalFreeze;
	//DWORD SaveDestinationCamX;
	//DWORD SaveDestinationCamY;
	//DWORD SaveCurrentCamX;
	//DWORD SaveCurrentCamXCopy;
	//DWORD SaveCurrentCamY;
	//DWORD SaveCurrentCamYCopy;
	//DWORD SaveCurrentCamZoom;
	//DWORD SaveDestinationCamZoom;
	//DWORD P1ControlledCharacter;
	//DWORD P1NextControlledCharacter;
	//DWORD P2ControlledCharacter;
	//DWORD P2NextControlledCharacter;

	//DWORD reallyNotSure;
	DWORD slowMo;

	void save();
	void load();

	int totalMemory();

} SaveState;

class SaveStateManager {
public:

	SaveStateManager() { }

	void save();

	void load(int dir);

	int totalMemory();

	int currentState = 0;
	// i would prefer something more not-externally-managed, but this is fine. if performance takes a hit, use circ buffer
	// tbh i should just use circbuffer for this, but i dont want to alloc all the memory upfront
	std::deque<SaveState*> states; 

	// my thought is that a typical round is what,, 1:30? so why not store that many
	const int maxStates = 60 * 90;
};

extern SaveStateManager saveStateManager;
