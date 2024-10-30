
#include "TASManager.h"
#include <fstream>
#include <string>
#include <optional>
#include <regex>

namespace TASManager {
	
	std::vector<TASItem> tasData;
    int tasIndex = 0;
    int tasCurrentLen = 0;

	std::optional<TASItem> parseLine(const std::string& l) {
		TASItem res;

        std::string output = l;

        std::regex trim_re("^\\s+|\\s+$");
        output = std::regex_replace(output, trim_re, "");

        if (output.size() == 0) {
            return std::optional<TASItem>();
        }

        if (output[0] == '#') {
            return std::optional<TASItem>();
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
                res.length = std::stoi(match[1].str());
                if (res.length < 1) {
                    return std::optional<TASItem>();
                }
            }
            output = match[2].str();
        } else {
            return std::optional<TASItem>();
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

		return res;
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
            std::optional<TASItem> res = parseLine(line);
            if (!res.has_value()) {
                continue;
            }

            tasData.push_back(res.value());
           
            //res.value().logItem();

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
