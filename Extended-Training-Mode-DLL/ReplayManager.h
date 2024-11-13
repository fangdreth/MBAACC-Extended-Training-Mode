#pragma once



typedef struct InputItem {
	BYTE length;
	BYTE dir;
	BYTE held;
	BYTE pressed;
	BYTE released;
	BYTE unknown;
	// ------
	void logItem();
	static std::string getButtonString(BYTE b);
} InputItem;

static_assert(sizeof(InputItem) == 6, "Size of input item must be 6 bytes");

class ReplayInputs {
public:

	ReplayInputs() {}
	ReplayInputs(BYTE* replayData, int playerIndex_);

	InputItem* getNextInput();
	void setNextInput();
	void reset();

	int resetFrames = 0;
	int index = 0;
	int indexLength = 0; // the amount of time we have been on the current input
	int size = 0;
	InputItem* inputItems = NULL;
	int playerIndex = 0;

};

class Replay {
public:

	Replay(const char* filePath_);

	~Replay();

	void reset();

	void inputReader();

	void setInput(InputItem* inputItem);

	const char* filePath = NULL;
	BYTE* buffer = NULL;
	int bufferSize = 0;

	ReplayInputs p1Inputs;
	ReplayInputs p2Inputs;

};

class ReplayManager {
public:

	~ReplayManager();

	void load(const char* filePath);

	void setInputs();

	void reset();

	int activeReplay = -1;
	std::vector<Replay*> replays; // using pointers because of move/copy issues, but namespaces cant have destructors. ig this is a class 

};

extern ReplayManager replayManager;
