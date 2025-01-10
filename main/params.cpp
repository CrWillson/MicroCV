#include "params.hpp"

namespace PARAMS {

    // General constants
    uint8_t IMG_ROWS = 96;
    uint8_t IMG_COLS = 96;

    // Stop line constants
    uint8_t STOPBOX_TL_X          = 20;
    uint8_t STOPBOX_TL_Y          = 75;
    uint8_t STOPBOX_BR_X          = 45;
    uint8_t STOPBOX_BR_Y          = 90;
    uint16_t STOPBOX_AREA = BOX_AREA(STOPBOX_TL_X, STOPBOX_TL_Y, STOPBOX_BR_X, STOPBOX_BR_Y);

    uint8_t PERCENT_TO_STOP       = 10;
    uint8_t STOP_GREEN_TOLERANCE  = 20;
    uint8_t STOP_BLUE_TOLERANCE   = 30;


    // White line constants
    uint8_t WHITELINE_CROP        = 50;
    uint8_t WHITE_RED_THRESH      = 160;
    uint8_t WHITE_GREEN_THRESH    = 170;
    uint8_t WHITE_BLUE_THRESH     = 100;
    uint16_t WHITE_MIN_SIZE       = 40;
    uint8_t WHITE_CENTER_POS      = 42;


    // Car detect constants
    uint8_t CARBOX_TL_X           = 0;
    uint8_t CARBOX_TL_Y           = 40;
    uint8_t CARBOX_BR_X           = 15;
    uint8_t CARBOX_BR_Y           = 70;
    uint16_t CARBOX_AREA = BOX_AREA(CARBOX_TL_X, CARBOX_TL_Y, CARBOX_BR_X, CARBOX_BR_Y);

    uint8_t PERCENT_TO_CAR        = 10;
    uint8_t CAR_RED_TOLERANCE     = 50;
    uint8_t CAR_BLUE_TOLERANCE    = 30;

}