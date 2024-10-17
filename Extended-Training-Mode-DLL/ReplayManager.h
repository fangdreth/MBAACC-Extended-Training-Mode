#pragma once

#include "DirectX.h"

typedef struct InputItem {
	BYTE length;
	BYTE dir;
	BYTE held;
	BYTE pressed;
	BYTE released;
	BYTE unknown;
	void logItem();
	static std::string getButtonString(BYTE b);
} InputItem;

class Replay {
public:

	Replay(const char* filePath_);

	~Replay();

	void inputReader();

	const char* filePath = NULL;
	BYTE* buffer = NULL;
	int bufferSize = 0;

	DWORD inputCount = 0;
	InputItem* inputItems = NULL;

};

class ReplayManager {
public:

	~ReplayManager();

	void load(const char* filePath);

	std::vector<Replay*> replays; // using pointers because of move/copy issues, but namespaces cant have destructors. ig this is a class 

};

extern ReplayManager replayManager;
