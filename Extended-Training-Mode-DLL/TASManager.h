#pragma once

#include "DirectX.h"

typedef struct TASItem {
    uint16_t length;
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
    uint8_t _padding;
    // -----
    void logItem() {
        log("%.4d %d %d%d%d%d", length, dir, a, b, c, d);
    }
} TASItem;

static_assert(sizeof(TASItem) == 4, "TASItem must be a byte!");

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
