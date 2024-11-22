
#include "DirectX.h"
#include "TASManager.h"
#include <fstream>
#include <string>
#include <optional>
#include <regex>
#include <algorithm>
#include <random> 
#include "DebugInfo.h"

bool enableTAS = false;
bool randomTAS = false;
bool regenTAS = true;

unsigned getRand() {

	static std::random_device rd;
	static std::mt19937 gen(rd());

	return gen();
}

float getRandFloat() {

	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_real_distribution<float> dist(0.0f, 1.0f); // not sure if this is ideal or what i want

	return dist(gen);
}

float getRandNorm() {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::normal_distribution<float> dist(0.0f, 1.0f);

	return dist(gen);
}

std::string strip(const std::string& s) {
	std::string res = s;
	std::regex trim_re("^\\s+|\\s+$");
	res = std::regex_replace(res, trim_re, "");
	return res;
}

int safeStoi(const std::string& s) {
	int res = -1;
	try {
		res = std::stoi(s);
	} catch (...) {
		return -1;
	}
	return res;
}

// -----

float TASManager::buttonMutationRate = 0.05f;
float TASManager::directionMutationRate = 0.05f;
float TASManager::lengthMutationRate = 0.10f;

void TASManager::parseLine(const std::string& l) {

	std::string s = l;

	s = s.substr(0, s.find('#', 0)); // remove everything past the first '#' 

	std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::tolower(c); }); // make the string lowercase

	std::regex removeDupleWhitespaceRe(R"(^\s+|\s+$|\s+(?=\s))");
	s = std::regex_replace(s, removeDupleWhitespaceRe, ""); // remove duplicate/non space whitespace and replace it with a single one, also strip 

	// the format of this map is key(string hash), val is the rest of the string not containing the command
	// constexpr doesnt like maps, which is why im using an array, the size is small enough that it will probs be better that way
	constexpr std::array<std::pair<DWORD, void(*)(TASManager* t, const std::string&)>, 9> parseArr = {{

		{ hashString("pause"), [](TASManager* t, const std::string& data) -> void {
			TASItem res;
			res.command = TASCommand::Pause;
			t->tasData.push_back(res);
		}},

		{ hashString("unpause"), [](TASManager* t, const std::string& data) -> void {
			TASItem res;
			res.command = TASCommand::Unpause;
			t->tasData.push_back(res);
		}},

		{ hashString("p1pos"), [](TASManager* t, const std::string& data) -> void {
			TASItem res;
			res.command = TASCommand::P1XPos;
			res.commandData = safeStoi(data);
			t->tasData.push_back(res);
		}},

		{ hashString("p2pos"), [](TASManager* t, const std::string& data) -> void {
			TASItem res;
			res.command = TASCommand::P2XPos;
			res.commandData = safeStoi(data);
			t->tasData.push_back(res);
		}},

		{ hashString("p1meter"), [](TASManager* t, const std::string& data) -> void {
			TASItem res;
			res.command = TASCommand::P1Meter;
			res.commandData = safeStoi(data);
			t->tasData.push_back(res);
		}},

		{ hashString("p2meter"), [](TASManager* t, const std::string& data) -> void {
			TASItem res;
			res.command = TASCommand::P2Meter;
			res.commandData = safeStoi(data);
			t->tasData.push_back(res);
		}},

		{ hashString("startff"), [](TASManager* t, const std::string& data) -> void {
			TASItem res;
			res.command = TASCommand::StartFF;
			t->tasData.push_back(res);
		}},

		{ hashString("stopff"), [](TASManager* t, const std::string& data) -> void {
			TASItem res;
			res.command = TASCommand::StopFF;
			t->tasData.push_back(res);
		}},

		{ hashString("dl"), [](TASManager* t, const std::string& data) -> void {
			TASItem res;
			res.length = safeStoi(data);
			t->tasData.push_back(res);
		}}

	}};

	std::regex re(R"(^(\S+)(?:\s?)(\S*)?$)");
	std::smatch match;

	if (std::regex_match(s, match, re)) {
		std::string instr = match[1].matched ? match[1].str() : "";
		std::string data = match[2].matched ? match[2].str() : "";
			
		//log("TAS instr:\"%s\" data:\"%s\"", instr.c_str(), data.c_str());

		DWORD hash = hashString(instr);
			
		int index = -1;
		for(size_t i=0; i<parseArr.size(); i++) {
			if (hash == parseArr[i].first) {
				index = i;
				break;
			}
		}

		if (index == -1) {
			// this means its a typical button command, with either "length input" or "input" format
			// or a sequence input like S236 was passed in 

			// if data is blank, it means that this is supposed to be a 1 frame input, so ill move the inputs a lil bit to set that up
			if (data.size() == 0) {
				data = instr;
				instr = "1";
			}

			if (data[0] == 's') { // parse sequence
				std::string temp = " ";
				for (size_t i = 1; i < data.size(); i++) {
					TASItem res;
					res.length = 1;
					temp[0] = data[i];
					res.setData(temp);
					tasData.push_back(res);
				}
			} else { // parse a normal input
				TASItem res;
				res.setLength(instr);
				res.setData(data);
				tasData.push_back(res);
			}

		} else {
			parseArr[index].second(this, data);
		}
	
	}

}

void TASManager::load(const std::string& filename) {

	if(!regenTAS) {
		breed(*this);
		reset();
		return;
	}

	if (randomTAS) {
		randLoad(16);
		return;
	}

	if (!enableTAS) {
		return;
	}

	log("trying to load file");

	tasData.clear();

	std::ifstream inFile(filename);
	if (!inFile.is_open()) {
		log("TASManager couldnt open file %s", filename.c_str());
		return;
	}

	std::string line;
	while (std::getline(inFile, line)) {
		parseLine(line);
	}

	inFile.close();

	reset();

	log("loaded tas file");

	if (TASManager::tasData.size() > 0 && TASManager::tasData[0].command == TASCommand::Unpause) {
		bFreeze = 0;
		needPause = 0;
		_naked_newPauseCallback2_IsPaused = 0;
	}
}

void TASManager::randLoad(int length) {
	if (!enableTAS) {
		return;
	}

	tasData.clear();

	for (int i = 0; i < length; i++) {
		TASItem temp;
		temp.setRand();
		tasData.push_back(temp);
	}

	reset();

}

void TASManager::reset() {

	tasIndex = 0;
	tasCurrentLen = 1; // dont ask
	fitness = 0;
	tasDone = false;

}

void TASManager::setInputs() {

	if (!enableTAS) {
		return;
	}

	if (tasIndex >= tasData.size()) {
		if (!tasDone && tasData.size() != 0) {
			tasDone = true;
			log("TAS fitness %f", fitness);
		}
		return;
	}

	//fitness = MAX(fitness, 11400.0f - playerDataArr[1].health);
	fitness = MAX(fitness, getComboCount());

	if (tasData[tasIndex].command == TASCommand::Nothing) {

		int playerIndex = 0;
		DWORD baseAddr = 0x00771398 + (0x2C * playerIndex);

		// dir
		*(BYTE*)(baseAddr + 0) = tasData[tasIndex].dir;
		// ABCD
		*(BYTE*)(baseAddr + 1) = tasData[tasIndex].a;
		*(BYTE*)(baseAddr + 2) = tasData[tasIndex].b;
		*(BYTE*)(baseAddr + 3) = tasData[tasIndex].c;
		*(BYTE*)(baseAddr + 4) = tasData[tasIndex].d;

		return;
	}

	switch (tasData[tasIndex].command) {

	case TASCommand::P1XPos:
		*(int32_t*)(dwBaseAddress + adP1Base + 0x108) = tasData[tasIndex].commandData;
		break;
	case TASCommand::P2XPos:
		*(int32_t*)(dwBaseAddress + adP2Base + 0x108) = tasData[tasIndex].commandData;
		break;
	case TASCommand::P1Meter:
		*(int32_t*)(dwBaseAddress + adP1Base + 0x0E0) = tasData[tasIndex].commandData;
		break;
	case TASCommand::P2Meter:
		*(int32_t*)(dwBaseAddress + adP2Base + 0x0E0) = tasData[tasIndex].commandData;
		break;
	case TASCommand::RNG:
		SetSeed(tasData[tasIndex].commandDataU32);
		break;
	case TASCommand::Pause:
		bFreeze = 1;
		needPause = 1;
		break;
	case TASCommand::Unpause:	
		bFreeze = 0;
		needPause = 0;
		_naked_newPauseCallback2_IsPaused = 0;
		break;
	case TASCommand::StartFF:
		setFPSLimiter(true);
		break;
	case TASCommand::StopFF:
		setFPSLimiter(false);
		break;
	default: 
		break;
	}

	// i do not want TAS commands to take up a frame, so i will inc the thing and go from there
	// additionally, i will sometimes be using the length variable/button data for other things, so i need to inc the index custom here
	tasIndex++;
	tasCurrentLen = 1; 
	setInputs();
		
}

void TASManager::incInputs() {

	if (!enableTAS) {
		return;
	}

	if (tasIndex >= tasData.size()) {
		return;
	}
		
	if (tasCurrentLen >= tasData[tasIndex].length) {
		tasCurrentLen = 0;
		tasIndex++;
		if (tasIndex >= tasData.size()) {
			return;
		}
	}
		
	tasCurrentLen++;
}

void TASManager::undoInputForPause() {

	if (!enableTAS) {
		return;
	}

	if (tasIndex >= tasData.size()) {
		// this has the consequence of possibly dropping the last input of the TAS when going back
		// i could solve this by adding an extra frame of nothing at the end of all tasses, but idk i dont like that
		return;
	}

	if (tasCurrentLen > 0) {
		tasCurrentLen--;
	}

}

TASManager TASManager::breed(const TASManager& managerA, const TASManager& managerB) {
	// having the breeding in here makes sense but i wonder if it should be in the trainer
	
	TASManager res;

	if (managerA.tasData.size() != managerB.tasData.size()) {
		log("cannot breed TASes of different lengths!");
		return res;
	}
	// rn im having a constant length but tbh, random insertions/deletions would be very helpful


	// based on A's and B's fitness, this decides which is more likely to get passed on 
	// A having greater fitness should lead to this being a higher value
	// considering im going off combo,,, hmm, for now ill say that 1 combos is 5%, but tbh early on it should be a LOT MORE
	// lets say that max combo should give 0.25
	// or well, this is the DIFFERENCE, fitA - fitB
	float fitDelta = managerA.fitness - managerB.fitness;
	float parentSelector = 0.5f + (powf(fabsf(fitDelta), 0.25) * (fitDelta > 0 ? 1.0f : -1.0f));

	for (size_t i = 0; i < managerA.tasData.size(); i++) {

		TASItem A = managerA.tasData[i];
		TASItem B = managerB.tasData[i];
		
		TASItem C;

		// i have read no math behind this
		
		if (A.buttons == B.buttons) { // this check def saves cycles, but isnt needed, sorta makes the code messy
			C.buttons = A.buttons; // should i have a random chance of change here
		} else {
			// should this selection be fitness based, and if so how much fitness -> how much probability?
			C.buttons = getRandFloat() < parentSelector ? A.buttons : B.buttons;
		}
		if (getRandFloat() < buttonMutationRate) {
			BYTE tempButtons = 1 << (getRand() & 0b11);
			C.buttons ^= tempButtons;
		}

		if (A.dir == B.dir) {
			C.dir = A.dir; // should i have a random chance of change here
		} else {
			C.dir = getRandFloat() < parentSelector ? A.dir : B.dir;
		}
		if (getRandFloat() < directionMutationRate) {
			// how should this occur? i feel like it just,, idek
			unsigned tempDir = getRand() % 9;
			if (tempDir >= 5) {
				tempDir++;
			}
			C.dir = tempDir;
		}

		if (A.length == B.length) {
			C.length = A.length; // should i have a random chance of change here
		} else {
			C.length = getRandFloat() < parentSelector ? A.length : B.length;
		}
		if (getRandFloat() < lengthMutationRate) {
			float val = getRandNorm();
			val += val > 0 ? 1.0f : -1.0f; // make sure that no mutation doesnt occur. this is weird
			C.length += val;
			C.length = MAX(C.length, 1);
		}

		res.tasData.push_back(C);
	}

	return res;
}

void TASManager::breed(const TASManager& other) {
	*this = breed(*this, other);
}

// -----

namespace TASTrainer {

	void generatePopulation() {
		for (int i = 0; i < popSize; i++) {
			if (population[i] != NULL) {
				delete population[i];
			}
			population[i] = new TASManager();
			population[i]->randLoad(tasLength);
		}
	}

	void doGeneration() {
		
		// this code is going to be SO weird due to how i need to slip in and out of this function




		// sort by fitness

		std::sort(population.begin(), population.end(), [](const TASManager* a, const TASManager* b) {
			return a->fitness < b->fitness;
		});




	}

	const int tasLength = 16;
	std::array<TASManager*, popSize> population = {}; // these are pointers to make sorting faster

};



