#include "params.hpp"

std::map<std::string, uint16_t> PARAMS::params = {
    {"IMG_ROWS",                96},
    {"IMG_COLS",                96},
    {"STOPBOX_TL_X",            20},
    {"STOPBOX_TL_Y",            75},
    {"STOPBOX_BR_X",            45},
    {"STOPBOX_BR_Y",            90},
    {"STOPBOX_AREA",            BOX_AREA(20, 75, 45, 90)},
    {"PERCENT_TO_STOP",         10},
    {"STOP_GREEN_TOLERANCE",    20},
    {"STOP_BLUE_TOLERANCE",     30},
    {"WHITELINE_CROP",          50},
    {"WHITE_RED_THRESH",        160},
    {"WHITE_GREEN_THRESH",      170},
    {"WHITE_BLUE_THRESH",       100},
    {"WHITE_MIN_SIZE",          40},
    {"WHITE_CENTER_POS",        42},
    {"CARBOX_TL_X",             0},
    {"CARBOX_TL_Y",             40},
    {"CARBOX_BR_X",             15},
    {"CARBOX_BR_Y",             70},
    {"CARBOX_AREA",             BOX_AREA(0, 40, 15, 70)},
    {"PERCENT_TO_CAR",          10},
    {"CAR_RED_TOLERANCE",       50},
    {"CAR_BLUE_TOLERANCE",      30}
};

// // General constants
// uint8_t PARAMS::IMG_ROWS = 96;
// uint8_t PARAMS::IMG_COLS = 96;

// // Stop line constants
// uint8_t PARAMS::STOPBOX_TL_X          = 20;
// uint8_t PARAMS::STOPBOX_TL_Y          = 75;
// uint8_t PARAMS::STOPBOX_BR_X          = 45;
// uint8_t PARAMS::STOPBOX_BR_Y          = 90;
// uint16_t PARAMS::STOPBOX_AREA = BOX_AREA(STOPBOX_TL_X, STOPBOX_TL_Y, STOPBOX_BR_X, STOPBOX_BR_Y);

// uint8_t PARAMS::PERCENT_TO_STOP       = 10;
// uint8_t PARAMS::STOP_GREEN_TOLERANCE  = 20;
// uint8_t PARAMS::STOP_BLUE_TOLERANCE   = 30;


// // White line constants
// uint8_t PARAMS::WHITELINE_CROP        = 50;
// uint8_t PARAMS::WHITE_RED_THRESH      = 160;
// uint8_t PARAMS::WHITE_GREEN_THRESH    = 170;
// uint8_t PARAMS::WHITE_BLUE_THRESH     = 100;
// uint16_t PARAMS::WHITE_MIN_SIZE       = 40;
// uint8_t PARAMS::WHITE_CENTER_POS      = 42;


// // Car detect constants
// uint8_t PARAMS::CARBOX_TL_X           = 0;
// uint8_t PARAMS::CARBOX_TL_Y           = 40;
// uint8_t PARAMS::CARBOX_BR_X           = 15;
// uint8_t PARAMS::CARBOX_BR_Y           = 70;
// uint16_t PARAMS::CARBOX_AREA = BOX_AREA(CARBOX_TL_X, CARBOX_TL_Y, CARBOX_BR_X, CARBOX_BR_Y);

// uint8_t PARAMS::PERCENT_TO_CAR        = 10;
// uint8_t PARAMS::CAR_RED_TOLERANCE     = 50;
// uint8_t PARAMS::CAR_BLUE_TOLERANCE    = 30;
