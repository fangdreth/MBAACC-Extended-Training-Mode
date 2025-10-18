#pragma once
#include <deque>
#include "..\Common\types.h"

/*

tbh i think an important priority here is to minimize the size of savestates
we could only record the changes between each state maybe?

*/

typedef struct PlayerSave {
	/*
	a thought:
	the player and effect structs are very similar. basically sorta kinda the same
	if you squint and have horrid vision.
	what about only backing up the first 0x33C of each player?
	*/
	// BYTE data[0xAFC];

	BYTE data[0x33C];
} PlayerSave;

typedef struct EffectSave {
	BYTE data[0x33C];
} EffectSave;

// ideally, memory would be aligned for quick load/save, but idrk
typedef struct SaveState {

	PlayerSave P1;
	PlayerSave P2;

	// a bit weird/stupid, but lets only alloc these when they are actually needed
	// i could(and probs should) do the same with P1 and P2,,, but ehh
	PlayerSave* P3 = NULL;
	PlayerSave* P4 = NULL;

	// reasoning for [16] is that copying 16 effects at a time is faster
	// reasoning for not using vector: cannot resize without default init. and for 0x33C * 16, thats way to much
	Vec<EffectSave> effects = Vec<EffectSave>(0); // yes. this is intended

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
	
	DWORD P1ControlledCharacter;
	DWORD P1NextControlledCharacter;
	DWORD P2ControlledCharacter;
	DWORD P2NextControlledCharacter;

	//DWORD reallyNotSure;
	DWORD slowMo;

	int P1Inactionable;
	int P2Inactionable;
	int FrameTimer;
	int TrueFrameTimer;

	void save();
	void load();

	int totalMemory();

	~SaveState();

} SaveState;

class SaveStateManager {
public:

	SaveStateManager() { }

	void save();

	bool load(int dir);

	int totalMemory();

	void log();

	int currentState = 0;
	// i would prefer something more not-externally-managed, but this is fine. if performance takes a hit, use circ buffer
	// tbh i should just use circbuffer for this, but i dont want to alloc all the memory upfront
	std::deque<SaveState*> states; 

	// my thought is that a typical round is what,, 1:30? so why not store that many
	const int maxStates = 60 * 90;
};

extern SaveStateManager saveStateManager;
