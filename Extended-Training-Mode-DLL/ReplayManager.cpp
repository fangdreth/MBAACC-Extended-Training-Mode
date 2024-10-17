
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

	inputCount = *(DWORD*)(buffer + 0xEC);

	inputItems = (InputItem*)(buffer + 0xF0);

	for (int i = 0; i < inputCount; i++) {
		inputItems[i].logItem();
	}

	

}

Replay::~Replay() {
	if (buffer != NULL) {
		free(buffer);
	}
}

void Replay::inputReader() {

	// set the next input	

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

	log("replay manager load called");

}

