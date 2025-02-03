
#include "DirectX.h"
#include "ReplayManager.h"
#include "DebugInfo.h"

/*

one could, in theory, run/render a whole game headless in order to get a whole replay file loaded fast?
still need to figure out their file format though

i should also start being normal and using namespaces for single classes

*/

std::string InputItem::getButtonString(BYTE b) {
	std::string res;

	res += (b & 0b00001) ? "A" : " ";
	res += (b & 0b00010) ? "B" : " ";
	res += (b & 0b00100) ? "C" : " ";
	res += (b & 0b01000) ? "D" : " ";
	res += (b & 0b10000) ? "E" : " ";

	return res;
}

void InputItem::logItem() {
	log("\t\tlen:%4d dir:%d hld:%5s prs:%5s rel:%5s", length, dir, getButtonString(held).c_str(), getButtonString(pressed).c_str(), getButtonString(released).c_str());
}

void InputItem::setMeltyInput(int playerIndex) {

	DWORD baseAddr = 0x00771398 + (0x2C * playerIndex);

	BYTE tempDir = dir;
	BYTE facingLeft = playerDataArr[playerIndex].facingLeft; // facingLeft/isOpponentToLeft ugh THIS IS A MAJOR THING THAT NEEDS TO BE FIGURED OUT

	if (facingLeft) {
		constexpr int dirLookup[] = { 0, 3, 2, 1, 6, 5, 4, 9, 8, 7 };
		tempDir = dirLookup[tempDir];
	}

	// dir
	*(BYTE*)(baseAddr + 0) = tempDir;
	// ABCDE
	*(BYTE*)(baseAddr + 1) = !!((held | pressed) & 0b00001);
	*(BYTE*)(baseAddr + 2) = !!((held | pressed) & 0b00010);
	*(BYTE*)(baseAddr + 3) = !!((held | pressed) & 0b00100);
	*(BYTE*)(baseAddr + 4) = !!((held | pressed) & 0b01000);
	*(BYTE*)(baseAddr + 5) = !!((held | pressed) & 0b10000);

}

// -----

BYTE* Round::load(BYTE* dataStart) {

	dataStart += 0x8C;

	for (int i = 0; i < 4; i++) {
		inputItemsSize[i] = *(DWORD*)(dataStart);
		dataStart += 4;
		inputItems[i] = (InputItem*)dataStart;
		inputItems[i] = (InputItem*)dataStart;
		dataStart += 6 * inputItemsSize[i];
	}

	unknownDataLen = *(DWORD*)(dataStart); // rng?? has to be right
	dataStart += 4;
	unknownDataPtr = dataStart;
	dataStart += (4 * unknownDataLen);

	// some sort of footer. not sure
	dataStart += 4 * 36;

	return dataStart;

}

void Round::logItem() {

	for (int playerIndex = 0; playerIndex < 4; playerIndex++) {
		if (inputItemsSize[playerIndex] == 0) {
			continue;
		}

		log("\tP%d", playerIndex);

		for (int i = 0; i < inputItemsSize[playerIndex]; i++) {
			inputItems[playerIndex][i].logItem();
		}

	}


}

void Round::rollForward(int i) {

	if (currentItemIndex[i] == inputItemsSize[i]) {
		return;
	}

	currentItemLength[i]++;

	if (currentItemLength[i] >= inputItems[i][currentItemIndex[i]].length) {
		currentItemLength[i] = 0;
		currentItemIndex[i]++;
	}


}

void Round::rollBack(int i) {

	if (currentItemIndex[i] == 0 && currentItemLength[i] == 0) {
		return;
	}

	currentItemLength[i]--;

	if (currentItemLength[i] < 0) {

		currentItemIndex[i]--;

		if (currentItemIndex[i] < 0) {
			currentItemLength[i] = 0;
			currentItemIndex[i] = 0;
			return;
		}

		currentItemLength[i] = inputItems[i][currentItemIndex[i]].length;

	}

}

void Round::rollForward() {
	for (int i = 0; i < 4; i++) {
		rollForward(i);
	}
}

void Round::rollBack() {
	for (int i = 0; i < 4; i++) {
		rollBack(i);
	}
}

void Round::setInputs() {

	/*
	if (resetFrames > 0) {
		resetFrames--;
		return;
	}
	*/

	for (int i = 0; i < 4; i++) {

		if (currentItemIndex[i] == inputItemsSize[i]) {
			continue;
		}

		//log("attempting p%d index %5d len %5d", i, currentItemIndex[i], currentItemLength[i]);

		inputItems[i][currentItemIndex[i]].setMeltyInput(i);

		//rollForward(i);

	}

}

void Round::reset() {

	resetFrames = 60;

	memset(currentItemIndex, 0, 4 * sizeof(currentItemIndex[0]));
	memset(currentItemLength, 0, 4 * sizeof(currentItemLength[0]));

}

// -----

Replay::~Replay() {
	free(buffer);
	buffer = NULL;
}

void Replay::load(const char* filePath) {


	int bufferSize = 0;

	if (filePath == NULL) {
		log("replay filepath cannot be null");
		return;
	}

	std::ifstream file(filePath, std::ios::binary | std::ios::ate);
	if (!file.good()) {
		log("couldnt find %s", filePath);
		return;
	}

	bufferSize = file.tellg();
	file.seekg(0, std::ios::beg);

	buffer = (BYTE*)malloc(bufferSize);

	file.read((char*)buffer, bufferSize);


	ReplayFile* r = (ReplayFile*)buffer;

	log("p0Char %d", r->p0Char);
	log("p1Char %d", r->p1Char);

	log("rounds: %d", r->roundCount);

	//printf("%08X\n", ((BYTE*)&r->roundCount) - buffer);

	BYTE* data = buffer + 0x60; // past the header or whatever

	for (int round = 0; round < r->roundCount; round++) {
		Round nextRound;
		data = nextRound.load(data);
		rounds.push_back(nextRound);
	}

	log("loading %s succeeded", filePath);

}

void Replay::logItem() {
	for (int i = 0; i < rounds.size(); i++) {
		log("round %d", i);
		rounds[i].logItem();
	}
}

void Replay::setInputs() {
	if (rounds.size() == 0) {
		return;
	}

	if (currentRound < 0 || currentRound >= rounds.size()) {
		log("%d invalid in Replay::setInputs there are only %d rounds", currentRound, rounds.size());
		return;
	}

	rounds[currentRound].setInputs();
}

void Replay::reset() {
	if (rounds.size() == 0) {
		return;
	}

	if (currentRound < 0 || currentRound >= rounds.size()) {
		log("%d invalid in Replay::reset", currentRound);
		return;
	}

	rounds[currentRound].reset();
}

void Replay::rollForward() {
	if (rounds.size() == 0) {
		return;
	}

	if (currentRound < 0 || currentRound >= rounds.size()) {
		log("%d invalid in Replay::rollForward", currentRound);
		return;
	}

	rounds[currentRound].rollForward();
}

void Replay::rollBack() {
	if (rounds.size() == 0) {
		return;
	}

	if (currentRound < 0 || currentRound >= rounds.size()) {
		log("%d invalid in Replay::rollBack", currentRound);
		return;
	}

	rounds[currentRound].rollBack();
}


// -----

ReplayManager replayManager;

ReplayManager::~ReplayManager() {
	for (int i = 0; i < replays.size(); i++) {
		replays[i]->~Replay();
	}
	replays.clear();
}

void ReplayManager::load(const char* filePath) {

	log("loading replay %s", filePath);

	replays.clear();

	Replay* replay = new Replay();

	replay->load(filePath);

	//replay.logItem();

	replays.push_back(replay);
	activeReplay = replays.size() - 1;

	log("replay manager load called");

}

void ReplayManager::setInputs() {

	if (replays.size() == 0) {
		return;
	}

	if (activeReplay < 0 || activeReplay >= replays.size()) {
		log("%d invalid in ReplayManager::setInputs", activeReplay);
		return;
	}

	// this sets the enemy status thing. this is temporary, and needs to be removed/made better
	*(BYTE*)0x0077C1E8 = MANUAL;

	replays[activeReplay]->setInputs();

}

void ReplayManager::reset() {
	if (replays.size() == 0) {
		return;
	}

	if (activeReplay < 0 || activeReplay >= replays.size()) {
		log("%d invalid in ReplayManager::reset", activeReplay);
		return;
	}

	replays[activeReplay]->reset();
}

void ReplayManager::rollForward() {
	if (replays.size() == 0) {
		return;
	}

	if (activeReplay < 0 || activeReplay >= replays.size()) {
		log("%d invalid in ReplayManager::rollForward", activeReplay);
		return;
	}

	static int idk = 0;
	log("rolling forward! %d", idk++);

	replays[activeReplay]->rollForward();
}

void ReplayManager::rollBack() {
	if (replays.size() == 0) {
		return;
	}

	if (activeReplay < 0 || activeReplay >= replays.size()) {
		log("%d invalid in ReplayManager::rollBack", activeReplay);
		return;
	}

	static int idk = 0;
	log("rolling backward! %d", idk++);

	replays[activeReplay]->rollBack();
}

