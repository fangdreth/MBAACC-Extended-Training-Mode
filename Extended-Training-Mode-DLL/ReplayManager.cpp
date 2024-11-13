
#include "DirectX.h"
#include "ReplayManager.h"

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
	log("len:%4d dir:%d hld:%5s prs:%5s rel:%5s", length, dir, getButtonString(held).c_str() , getButtonString(pressed).c_str(), getButtonString(released).c_str());
}

// -----

ReplayInputs::ReplayInputs(BYTE* replayData, int playerIndex_) {
	size = *(DWORD*)replayData;
	inputItems = (InputItem*)(replayData + 4);
	playerIndex = playerIndex_;
	reset();
}

InputItem* ReplayInputs::getNextInput() {
	
	if (index >= size) {
		return NULL;
	}

	InputItem* res = &inputItems[index];
	indexLength++;
	if (indexLength > inputItems[index].length) {
		index++;
		indexLength = 0;
	}
	return res;
}

void ReplayInputs::setNextInput() {

	if (resetFrames > 0) {
		resetFrames--;
		return;
	}

	InputItem* item = getNextInput();
	if (item == NULL) {
		return;
	}

	DWORD baseAddr = 0x00771398 + (0x2C * playerIndex);

	// dir
	*(BYTE*)(baseAddr + 0) = item->dir;
	// ABCDE
	*(BYTE*)(baseAddr + 1) = !!((item->held | item->pressed ) & 0b00001);
	*(BYTE*)(baseAddr + 2) = !!((item->held | item->pressed ) & 0b00010); 
	*(BYTE*)(baseAddr + 3) = !!((item->held | item->pressed ) & 0b00100);
	*(BYTE*)(baseAddr + 4) = !!((item->held | item->pressed ) & 0b01000);
	*(BYTE*)(baseAddr + 5) = !!((item->held | item->pressed ) & 0b10000);

}

void ReplayInputs::reset() {
	index = 0;
	indexLength = 0;
	resetFrames = 60;
}

// -----

Replay::Replay(const char* filePath_) {
	
	filePath = filePath_;

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

	// look, there is def some way to read these files in a way that isnt trash, but this is all i have rn
	// i really need to spend more time on the replay file format

	BYTE* p1ReplayData = NULL;
	BYTE* p2ReplayData = NULL;

	for (int i = 0; i < bufferSize - 8; i++) {
		if (*(uint64_t*)(buffer + i) == 0x000000FF000000FF) {
			if (p1ReplayData == NULL) {
				p1ReplayData = buffer + i + 8;
			} else if (p2ReplayData == NULL) {
				p2ReplayData = buffer + i + 8;
				break;
			} else {
				log("replay somehow found more than 2 player input buffers??");
				return;
			}
		}
	}

	if (p1ReplayData == NULL || p2ReplayData == NULL) {
		log("replay was unable to find 2 player input buffers");
		return;
	}

	//log("replay p1:%08X p2:%08X", *(DWORD*)p1ReplayData, *(DWORD*)p2ReplayData);

	p1Inputs = ReplayInputs(p1ReplayData, 0);
	p2Inputs = ReplayInputs(p2ReplayData, 1);

	log("loaded replay successfully");

}

Replay::~Replay() {
	if (buffer != NULL) {
		free(buffer);
	}
}

void Replay::inputReader() {
	p1Inputs.setNextInput();
	p2Inputs.setNextInput();
}

void Replay::reset() {
	p1Inputs.reset();
	p2Inputs.reset();
}

// -----

ReplayManager replayManager;

ReplayManager::~ReplayManager() {
	for (Replay* r : replays) {
		if (r != NULL) {
			free(r);
		}
	}
	replays.clear();
}

void ReplayManager::load(const char* filePath) {

	Replay* r = new Replay(filePath);
	replays.push_back(r);
	activeReplay = replays.size() - 1;

	log("replay manager load called");

}

void ReplayManager::setInputs() {
	
	if (replays.size() == 0) {
		return;
	}

	// this sets the enemy status thing. this is temporary, and needs to be removed/made better
	*(BYTE*)0x0077C1E8 = MANUAL;

	replays[activeReplay]->inputReader();

}

void ReplayManager::reset() {
	if (replays.size() == 0) {
		return;
	}

	replays[activeReplay]->reset();
}
