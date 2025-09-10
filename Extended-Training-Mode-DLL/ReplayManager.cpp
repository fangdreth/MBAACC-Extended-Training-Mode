
#include "DirectX.h"
#include "ReplayManager.h"
#include "DebugInfo.h"

#include <filesystem>

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define CLAMP(value, min_val, max_val) MAX(MIN((value), (max_val)), (min_val))
#define SAFEMOD(a, b) (((b) + ((a) % (b))) % (b))

namespace fs = std::filesystem;

extern DWORD showCSS;

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
	BYTE facingLeft = playerDataArr[playerIndex].subObj.facingLeft;

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

	__try {
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

	} __except (EXCEPTION_EXECUTE_HANDLER) {
		return NULL;
	}

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

bool Replay::load(const std::string& filePath_) {

	filePath = filePath_;

	int bufferSize = 0;

	if (filePath == "") {
		log("replay filepath cannot be null");
		return false;
	}

	std::ifstream file(filePath, std::ios::binary | std::ios::ate);
	if (!file.good()) {
		log("couldnt find %s", filePath);
		return false;
	}

	bufferSize = file.tellg();
	file.seekg(0, std::ios::beg);

	buffer = (BYTE*)malloc(bufferSize);

	file.read((char*)buffer, bufferSize);

	file.close();

	ReplayFile* r = (ReplayFile*)buffer;

	log("p0Char %d", r->p0Char);
	log("p1Char %d", r->p1Char);

	log("rounds: %d", r->roundCount);

	//printf("%08X\n", ((BYTE*)&r->roundCount) - buffer);

	BYTE* data = buffer + 0x60; // past the header or whatever

	for (int round = 0; round < r->roundCount; round++) {
		Round nextRound;
		data = nextRound.load(data);
		if (data == NULL) {
			return false;
		}
		rounds.push_back(nextRound);
	}

	log("loading %s succeeded", filePath.c_str());


	replayTime = 0;
	replayTime += ((long long)r->second) * (1);
	replayTime += ((long long)r->minute) * (1 * 60);
	replayTime += ((long long)r->hour  ) * (1 * 60 * 60);
	replayTime += ((long long)r->day   ) * (1 * 60 * 60 * 24);
	replayTime += ((long long)r->month ) * (1 * 60 * 60 * 24 * 31);
	replayTime += ((long long)(r->year) - 2009) * (1 * 60 * 60 * 24 * 31 * 366);

	//log("%016X %d", replayTime, r->year);

	//log("%4d-%2d-%2d %2d:%2d:%2d %016X %I64", r->year, r->month, r->day, r->hour, r->minute, r->second, replayTime, replayTime);

	char buffer[64];
	snprintf(buffer, 64, "%04d-%02d-%02d %02d:%02d:%02d", r->year, r->month, r->day, r->hour, r->minute, r->second); // i really should just copy caster's format func

	timeString = std::string(buffer);

	return true;

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
	for(Replay* r : replays) {
		r->~Replay();
	}
	replays.clear();
}

void ReplayManager::load(const std::string& filePath_) {

	log("loading replay %s", filePath_.c_str());

	Replay* replay = new Replay();

	bool res = replay->load(filePath_);

	if (!res) {
		log("failed to load %s", filePath_.c_str());
	}

	//replay.logItem();

	replays.insert(replay);
	//activeReplay = replays.size() - 1;
	
	log("replay manager load succeeded");

}

void ReplayManager::initReplay(Replay* r) {

	if (r == NULL) {
		return;
	}

	log("%s", r->filePath.c_str());

}

void ReplayManager::setInputs() {

	if (replays.size() == 0) {
		return;
	}

	if (activeReplay == NULL) {
		return;
	}

	// this sets the enemy status thing. this is temporary, and needs to be removed/made better
	*(BYTE*)0x0077C1E8 = MANUAL;

	activeReplay->setInputs();

}

void ReplayManager::reset() {
	if (replays.size() == 0) {
		return;
	}

	if (activeReplay == NULL) {
		return;
	}


	activeReplay->reset();
}

void ReplayManager::rollForward() {
	if (replays.size() == 0) {
		return;
	}

	if (activeReplay == NULL) {
		return;
	}

	//static int idk = 0;
	//log("rolling forward! %d", idk++);

	activeReplay->rollForward();
}

void ReplayManager::rollBack() {
	if (replays.size() == 0) {
		return;
	}

	if (activeReplay == NULL) {
		return;
	}

	//static int idk = 0;
	//log("rolling backward! %d", idk++);

	activeReplay->rollForward();
}

Replay* ReplayManager::getReplay(int index) {

	// hopefully this func doesnt take too long

	if (index < 0 || index >= replays.size()) {
		return NULL;
	}

	auto it = std::next(replays.begin(), index);

	return *it; 

}

void ReplayManager::loadAllReplays() {

	log("reloading all replays");

	for (Replay* r : replays) {
		r->~Replay();
	}
	replays.clear();

	std::string path = "./ReplayVS/"; // Change to your target directory

	std::regex filePattern(R"((.*\.rep)$)", std::regex::icase);

	for (const auto& entry : fs::directory_iterator(path)) {
		std::string stringPath = std::string(entry.path().string());
		std::smatch match;
		if (std::regex_match(stringPath, match, filePattern)) {
			std::string fileName = match[1].str();
			load(fileName.c_str());
		}
	}

	replayPage = 0;
	pageCount = (replays.size() + replaysPerPage - 1) / replaysPerPage; // round up to nearest pageCount
}

std::string ReplayManager::getReplayList() {

	std::string res = "\n";

	int startIndex = replayPage * replaysPerPage;
	int endIndex = MIN(startIndex + replaysPerPage, replays.size());

	std::regex filePattern(R"((.*\.rep)$)", std::regex::icase);

	/*
	for (const auto& entry : fs::directory_iterator(path)) {
		std::string stringPath = std::string(entry.path().string());
		std::smatch match;
		if (std::regex_match(stringPath, match, filePattern)) {
			*/

	for (int i = startIndex; i < endIndex; i++) {

		Replay* r = getReplay(i);
		std::string temp = r->filePath;

		size_t pos = temp.find_last_of("/");
		temp = (pos == std::string::npos) ? temp : temp.substr(pos + 1);

		//res += std::to_string(r->replayTime) + " ";]
		res += r->timeString;
		res += " ";
		res += temp;

		if (i != endIndex - 1) {
			res += "\n";
		}

	}


	return res;

}

void ReplayManager::initMenu() {
	
	if (wasMenuInit) {
		return;
	}

	wasMenuInit = true;

	replayMenu.add<int>("Reload Replays",
		[this](int inc, int& opt) mutable -> void {
			this->loadAllReplays();
		},
		[this](int opt) mutable -> std::string {
			return std::to_string(this->replays.size());
		},
		std::wstring(L"P1InputDisplay")
	);

	replayMenu.add<int>("Page:",
		[this](int inc, int& opt) mutable -> void {
			this->replayPage = SAFEMOD(this->replayPage + inc, this->pageCount);
		},
		[this](int opt) mutable -> std::string {
			return std::to_string(this->replayPage + 1);
		},
		std::wstring(L"P1InputDisplay")
	);

	replayMenu.add<int>("Replays:",
		[this](int inc, int& opt) mutable -> void {

			float testMouseY = mousePos.y - 75.0f;

			testMouseY *= 0.1f;

			int replaySelectIndex = (int)testMouseY;

			int replayIndex = replaySelectIndex + (this->replaysPerPage * this->replayPage);
			Replay* r = getReplay(replayIndex);

			this->initReplay(r);

		},
		[this](int opt) mutable -> std::string {
			return this->getReplayList();
		},
		std::wstring(L"P1InputDisplay")
	);

}

void ReplayManager::drawMenu() {

	initMenu();

	Point dupe = anchorPoint;

	showCSS = !replayMenu.unfolded;

	if (replayMenu.unfolded && firstUnfold) {
		firstUnfold = false;
		loadAllReplays();
	}

	replayMenu.draw(dupe);

}

// -----

void drawReplayMenu() {
	//replayManager.drawMenu();
}

