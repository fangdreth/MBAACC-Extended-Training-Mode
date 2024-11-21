
#include "DirectX.h"
#include "TASManager.h"
#include <fstream>
#include <string>
#include <optional>
#include <regex>
#include <algorithm>

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

namespace TASManager {
	
	std::vector<TASItem> tasData;
	int tasIndex = 0;
	int tasCurrentLen = 0;

	void parseLine(const std::string& l) {

		std::string s = l;

		s = s.substr(0, s.find('#', 0)); // remove everything past the first '#' 

		std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::tolower(c); }); // make the string lowercase

		std::regex removeDupleWhitespaceRe(R"(^\s+|\s+$|\s+(?=\s))");
		s = std::regex_replace(s, removeDupleWhitespaceRe, ""); // remove duplicate/non space whitespace and replace it with a single one, also strip 

		// the format of this map is key(string hash), val is the rest of the string not containing the command
		// constexpr doesnt like maps, which is why im using an array, the size is small enough that it will probs be better that way
		constexpr std::array<std::pair<DWORD, void(*)(const std::string&)>, 9> parseArr = {{

			{ hashString("pause"), [](const std::string& data) -> void {
				TASItem res;
				res.command = TASCommand::Pause;
				tasData.push_back(res);
			}},

			{ hashString("unpause"), [](const std::string& data) -> void {
				TASItem res;
				res.command = TASCommand::Unpause;
				tasData.push_back(res);
			}},

			{ hashString("p1pos"), [](const std::string& data) -> void {
				TASItem res;
				res.command = TASCommand::P1XPos;
				res.commandData = safeStoi(data);
				tasData.push_back(res);
			}},

			{ hashString("p2pos"), [](const std::string& data) -> void {
				TASItem res;
				res.command = TASCommand::P2XPos;
				res.commandData = safeStoi(data);
				tasData.push_back(res);
			}},

			{ hashString("p1meter"), [](const std::string& data) -> void {
				TASItem res;
				res.command = TASCommand::P1Meter;
				res.commandData = safeStoi(data);
				tasData.push_back(res);
			}},

			{ hashString("p2meter"), [](const std::string& data) -> void {
				TASItem res;
				res.command = TASCommand::P2Meter;
				res.commandData = safeStoi(data);
				tasData.push_back(res);
			}},

			{ hashString("startff"), [](const std::string& data) -> void {
				TASItem res;
				res.command = TASCommand::StartFF;
				tasData.push_back(res);
			}},

			{ hashString("stopff"), [](const std::string& data) -> void {
				TASItem res;
				res.command = TASCommand::StopFF;
				tasData.push_back(res);
			}},

			{ hashString("dl"), [](const std::string& data) -> void {
				TASItem res;
				res.length = safeStoi(data);
				tasData.push_back(res);
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
			for(int i=0; i<parseArr.size(); i++) {
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
					for (int i = 1; i < data.size(); i++) {
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
				parseArr[index].second(data);
			}
	
		}

	}

	void load(const std::string& filename) {

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

	void reset() {

		tasIndex = 0;
		tasCurrentLen = 1; // dont ask

	}

	void setInputs() {

		if (!enableTAS) {
			return;
		}

		if (tasIndex >= tasData.size()) {
			return;
		}

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

	void incInputs() {

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

	void undoInputForPause() {

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

};
