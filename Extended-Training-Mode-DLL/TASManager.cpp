
#include "TASManager.h"
#include <fstream>
#include <string>
#include <optional>
#include <regex>


std::string strip(const std::string& s) {
	std::string res = s;
	std::regex trim_re("^\\s+|\\s+$");
	res = std::regex_replace(res, trim_re, "");
	return res;
}

namespace TASManager {
	
	std::vector<TASItem> tasData;
	int tasIndex = 0;
	int tasCurrentLen = 0;

	void parseLine(const std::string& l) {
		TASItem res;
		res.data = 0;

		std::string output = l;

		// make the string lowercase
		std::transform(output.begin(), output.end(), output.begin(), [](unsigned char c) { return std::tolower(c); });

		std::regex trim_re("^\\s+|\\s+$");
		output = std::regex_replace(output, trim_re, "");

		if (output.size() == 0) {
			return;
		}

		if (output[0] == '#') {
			return;
		}

		std::regex whitespace_re("\\s+");
		output = std::regex_replace(output, whitespace_re, " ");

		std::regex removeAfterHashtag("^[^#]*");
		std::smatch match;

		if (std::regex_search(output, match, removeAfterHashtag)) {
			output = match.str();
		}

		std::regex re("(?:(.*?)(?: ))?(.+)");

		res.length = 1;

		if (std::regex_match(output, match, re)) {
			if (match[1].matched) {
				
				try {
					res.length = std::stoi(match[1].str());
				} catch(...) {
					return;
				}
				if (res.length < 1) {
					return;
				}
			}
			output = match[2].str();
		} else {
			return;
		}

		if (output[0] == 'S' || output[0] == 's') {
			for (int i = 1; i < output.size(); i++) {
				char c = output[i];

				TASItem temp;
				temp.data = 0;
				temp.length = 1;
				

				bool validItem = true;
				switch (c) {
					case '0':
					case '1':
					case '2':
					case '3':
					case '4':
					case '6':
					case '7':
					case '8':
					case '9':
						temp.dir = c - '0';
						break;
					case '5':
						temp.dir = 0;
						break;
					case 'a':
					case 'A':
						temp.a = true;
						break;
					case 'b':
					case 'B':
						temp.b = true;
						break;
					case 'c':
					case 'C':
						temp.c = true;
						break;
					case 'd':
					case 'D':
						temp.d = true;
						break;
					default:
						validItem = false;
						break;
				}

				if (validItem) {
					tasData.push_back(temp);
				}
				
			}
			return;
		}

		if (output.size() >= 3 && output.substr(0, 2) == "dl") {

			try {
				res.length = std::stoi(strip(output.substr(2, std::string::npos)));
			} catch(...) {
				return;
			}
			if (res.length < 1) {
				return;
			}

			tasData.push_back(res);
			return;
		}

		res.data = 0;
		res.dir = 5;
		

		for (int i = 0; i < output.size(); i++) {

			if (output[i] == '#') {
				break;
			}

			if (output[i] >= '0' && output[i] <= '9') {
				res.dir = output[i] - '0';
				if (res.dir == 5) {
					res.dir = 0;
				}
				continue;
			}

			switch (output[i]) {
			case 'a':
			case 'A':
				res.a = true;
				break;
			case 'b':
			case 'B':
				res.b = true;
				break;
			case 'c':
			case 'C':
				res.c = true;
				break;
			case 'd':
			case 'D':
				res.d = true;
				break;
			default:
				break;
			}

		}

		tasData.push_back(res);
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

		int playerIndex = 0;
		DWORD baseAddr = 0x00771398 + (0x2C * playerIndex);

		// dir
		*(BYTE*)(baseAddr + 0) = tasData[tasIndex].dir;
		// ABCD
		*(BYTE*)(baseAddr + 1) = tasData[tasIndex].a;
		*(BYTE*)(baseAddr + 2) = tasData[tasIndex].b;
		*(BYTE*)(baseAddr + 3) = tasData[tasIndex].c;
		*(BYTE*)(baseAddr + 4) = tasData[tasIndex].d;
		
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
