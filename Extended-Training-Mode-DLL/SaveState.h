#pragma once
#include <deque>
#include "..\Common\types.h"

/*

tbh i think an important priority here is to minimize the size of savestates
we could only record the changes between each state maybe?

*/

const int PlayerSaveSize = 0x3e8;

typedef struct PlayerSave {
	/*
	a thought:
	the player and effect structs are very similar. basically sorta kinda the same
	if you squint and have horrid vision.
	what about only backing up the first 0x33C of each player?

	im saving up until the input buffers to get the thing that remembers which normals have been
	used in a string - gonp
	*/
	// BYTE data[0xAFC];

	BYTE data[0x3e8];
} PlayerSave;

typedef struct EffectSave {
	BYTE data[0x33C];
} EffectSave;

typedef struct PlayerDataSave {
	BYTE data[0x20c];
};

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

typedef struct FullSave {

	DWORD CameraZoom[3];							// 0x0054eb70 - 0x0054eb7c
	DWORD CameraCoordsDestination[3];				// 0x00555124 - 0x00555130
	PlayerSave P1;
	PlayerSave P2;
	PlayerSave P3;
	PlayerSave P4;
	PlayerDataSave PlayerData[4];					// 0x00557db8 - 0x005585e8
	BYTE StopSituation[0xf38];						// 0x00558600 - 0x00559538
	DWORD FrameCount;								// 0x0055d1cc
	DWORD SlowMo;									// 0x0055d208
	DWORD CameraCoords[3];							// 0x0055dec4 - 0x0055ded8
	DWORD TrueFrameCount;							// 0x00562a40
	DWORD GlobalFreeze;								// 0x00562a48
	BYTE RNG[0xe4];									// 0x00564068 - 0x0056414c
	DWORD CameraCoordsNext[3];						// 0x00564b14 - 0x00564b20
	EffectSave Effects[1000];						// 0x0067bde8 - 0x00746048

	bool IsSaved;

	void save();
	void load(bool LoadRNG);

	void xport();
	void nport();

	void unsave();

} FullSave;

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

	FullSave* FullSaves[3] = {new FullSave(), new FullSave(), new FullSave()};

	void SaveToFile();
	void LoadFromFile();

	// my thought is that a typical round is what,, 1:30? so why not store that many
	const int maxStates = 60 * 90;
};

extern SaveStateManager saveStateManager;

const char* const saveMoonMap = "CFH";