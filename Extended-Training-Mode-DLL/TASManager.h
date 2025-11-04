#pragma once

extern bool enableTAS;
extern bool randomTAS;
extern bool regenTAS;
extern bool fixTAS2v2;

int safeStoi(const std::string& s);

unsigned getRand();
float getRandFloat();
float getRandNorm();

enum class TASCommand : uint8_t {
	Nothing = 0,
	P1XPos = 1,
	P2XPos = 2,
	P3XPos = 3,
	P4XPos = 4,
	P1Meter = 5,
	P2Meter = 6,
	P3Meter = 7,
	P4Meter = 8,
	RNG = 9, // not implemented
	Pause = 10, 
	Unpause = 11, 
	StartFF = 12,
	StopFF = 13,
	FN1 = 14,
};

#pragma pack(push,1)
typedef struct TASItem {
	union {
		struct {
			uint8_t dir : 4; // direction 5 MUST be 0 !			
			uint8_t a : 1;
			uint8_t b : 1;
			uint8_t c : 1;
			uint8_t d : 1;
		};
		struct { // i cannot have a union/struct with less than a byte, which is why this is here
			uint8_t __unused : 4;
			uint8_t buttons : 4;
		};
		uint8_t data = 0;
	};
	
	int16_t length = 0;
	
	TASCommand command = TASCommand::Nothing;
	union {
		int32_t commandData = 0;
		uint32_t commandDataU32;
	};
	
	// -----

	void setRand() {
		unsigned r = getRand();
		dir = r % 9;
		if (dir >= 5) {
			dir++;
		}
		r >>= 4; // dont use the same bits for direction as buttons
		
		/*
		int buttonVal = r & 0b11;
		
		if (buttonVal == 1) {
			a = 1;
		} else if (buttonVal == 2) {
			b = 1;
		} else if(buttonVal == 3) {
			c = 1;
		}

		r >>= 2;
		*/

		buttons = r & 0b1111;
		r >>= 4;

		/*
		if (buttonVal == 0) { // this prevents the use of charged buttons on first setup,, but idk. maybe i should trust randomization more
			length = r & 63;
		} else {
			length = 1;
		}*/

		length = r & 63;

		// do i even want to give this thing the option of shielding? or pressing multiple buttons at once?
	}

	void logItem() {
		log("%.4d %d %d%d%d%d", length, dir, a, b, c, d);
	}

	inline void setLength(const std::string& s) {
		length = safeStoi(s);
	}

	inline void setData(const std::string_view& s) {
		size_t i = 0;
		char ch;
		while (true) {
			ch = s[i++];
			if (ch == '\0') {
				break;
			}
			if (ch >= '0' && ch <= '9') {
				dir = ch - '0';
				if (dir == 5) {
					dir = 0;
				}
			} else if (ch == 'a') {
				a = true;
			} else if (ch == 'b') {
				b = true;
			} else if (ch == 'c') {
				c = true;
			} else if (ch == 'd') {
				d = true;
			}
		}
	}

} TASItem;
#pragma pack(pop)

static_assert(sizeof(TASCommand) == 1, "TASCommand must be size of 1");
static_assert(sizeof(TASItem) == 8, "TASItem must be proper size!");

class TASManager {
public:
	void parseLine(const std::string& l);

	void load(const std::string& filename);
	void randLoad(int length);

	void reset();

	void setInputs(int playerIndex = 0);
	void incInputs();

	void undoInputForPause();

	static TASManager breed(const TASManager& managerA, const TASManager& managerB);
	void breed(const TASManager& other);

	int tasIndex = 0;
	int tasCurrentLen = 0;
	std::vector<TASItem> tasData;

	float fitness = 0.0f;
	bool tasDone = false;
	static float buttonMutationRate; // the fact that you cannot init static variables in this same area is INSANE
	static float directionMutationRate;
	static float lengthMutationRate;

};

namespace TASTrainer {

	void generatePopulation();

	void doGeneration();

	extern const int tasLength;
	constexpr int popSize = 16;
	extern std::array<TASManager*, popSize> population;

};

