#pragma once


#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

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

	void load(const char* filePath);
	void logItem();

	int currentRound = 0;
	std::vector<Round> rounds;

	void setInputs();
	void reset();

	void rollForward();
	void rollBack();


};

class ReplayManager {
public:

	~ReplayManager();

	void load(const char* filePath);

	void setInputs();
	void reset();

	void rollForward();
	void rollBack();

	int activeReplay = -1;
	std::vector<Replay*> replays; // this is done because a move will call the destructor on the replay file!!!

};

extern ReplayManager replayManager;
