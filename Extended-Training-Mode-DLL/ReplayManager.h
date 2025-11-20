#pragma once

#include <set>

#include "FancyMenu.h"

// forward decleration of menu class
template <typename T = int>
class Menu;

#pragma pack(push,1)
typedef struct InputItem {
	BYTE length;
	BYTE dir;
	BYTE held;
	BYTE pressed;
	BYTE released;
	BYTE unknown;
	// ------
	void logItem();
	void setMeltyInput(int playerIndex);
	static std::string getButtonString(BYTE b);
} InputItem;
#pragma pack(pop)

static_assert(sizeof(InputItem) == 6, "Size of input item must be 6 bytes");

class Round {
public:

	Round() {}

	void logItem();

	BYTE* load(BYTE* dataStart);

	void rollForward(int i);
	void rollBack(int i);

	void rollForward();
	void rollBack();

	void setInputs();
	void reset();

	InputItem* inputItems[4] = { NULL, NULL, NULL, NULL }; // lists
	DWORD inputItemsSize[4] = { 0, 0, 0, 0 }; // list size

	int currentItemIndex[4] = { 0, 0, 0, 0 }; // index of the item
	int currentItemLength[4] = { 0, 0, 0, 0 }; // length into the current item

	BYTE* unknownDataPtr = NULL;
	DWORD unknownDataLen = 0;

	int resetFrames = 60;


};

#pragma pack(push,1)
typedef struct ReplayFile {

	BYTE header[16];
	DWORD unknown;
	DWORD unknown2;
	uint16_t year;
	uint16_t month;
	uint16_t day;
	uint16_t hour;
	uint16_t minute;
	uint16_t second;
	DWORD unknown3;
	DWORD stage;
	DWORD unknown4; // just 2???
	DWORD unknown5; // 2????
	DWORD unknown6; /// 2
	DWORD unknown7; // 2?

	DWORD p0Unknown1;
	DWORD p0Char;
	DWORD p0Moon;
	DWORD p0Palette;

	DWORD p1Unknown1;
	DWORD p1Char;
	DWORD p1Moon;
	DWORD p1Palette;

	DWORD roundCount;

	BYTE idek[0x8C];

	// this puts me at 0xEC

} ReplayFile;
#pragma pack(pop)

class Replay {
public:

	Replay() {}
	~Replay();

	BYTE* buffer = NULL;

	std::string filePath;

	bool load(const std::string& filePath_);
	void logItem();

	std::string timeString = "Unknown";
	long long replayTime = -1;

	int currentRound = 0;
	std::vector<Round> rounds;

	void setInputs();
	void reset();

	void rollForward();
	void rollBack();

};

struct ReplayComparator {
	bool operator()(const Replay* a, const Replay* b) const {
		return a->replayTime > b->replayTime;
	}
};

class ReplayManager {
public:

	ReplayManager() {}
	~ReplayManager();

	void load(const std::string& filePath);

	void initReplay(Replay* r);

	void setInputs();
	void reset();

	void rollForward();
	void rollBack();

	bool wasMenuInit = false;
	void initMenu();

	void loadAllReplays();

	std::string getReplayList();

	void drawMenu();
		
	int replayPage = 0;
	int pageCount = 0;
	int replaysPerPage = 25;
	Replay* activeReplay = NULL;
	Replay* getReplay(int index);
	std::set<Replay*, ReplayComparator> replays; // this is done because a move will call the destructor on the replay file!!!

	bool firstUnfold = true;
	Point anchorPoint = Point(175, 35);
	Menu<int> replayMenu = Menu<int>("Replays");

};

extern ReplayManager replayManager;

void drawReplayMenu();
