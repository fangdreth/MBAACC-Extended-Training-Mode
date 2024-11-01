#pragma once

#include "DirectX.h"

enum class TASCommand : uint8_t {
    Nothing = 0,
    P1XPos = 1,
    P2XPos = 2,
    P1Meter = 3,
    P2Meter = 4,
    RNG = 5,
    Pause = 6, 
    Unpause = 7, // this command is useless and does nothing since inputs arent exected on pause
    StartFF = 8,
    StopFF = 9,
};

typedef struct TASItem {
    int16_t length;
    union {
        struct {
            uint8_t dir : 4;
            uint8_t a : 1;
            uint8_t b : 1;
            uint8_t c : 1;
            uint8_t d : 1;
        };
        uint8_t data;
    };
    TASCommand command = TASCommand::Nothing;
    union {
        int32_t commandData = 0; // wish i didnt need this here, but i do 
        uint32_t commandDataU32;
    };
    
    // -----
    void logItem() {
        log("%.4d %d %d%d%d%d", length, dir, a, b, c, d);
    }
} TASItem;

static_assert(sizeof(TASItem) == 8, "TASItem must be proper size!");

// i could use a class here, but if only one copy will ever be needed, then there is no point
namespace TASManager {

    void parseLine(const std::string& l);

	void load(const std::string& filename);

	void reset();

    void setInputs();
    void incInputs();

    void undoInputForPause();

    extern int tasIndex;
    extern int tasCurrentLen;
    extern std::vector<TASItem> tasData;

};
