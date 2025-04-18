#pragma once

#include <stdint.h>
#include "constants.hpp"
#include "opencv2.hpp"

namespace Params {

// Stop line constants

inline uint8_t STOPBOX_TL_X          = 15;          ///< Top Left corner of the stop detection box - X
inline uint8_t STOPBOX_TL_Y          = 75;          ///< Top Left corner of the stop detection box - Y
inline uint8_t STOPBOX_BR_X          = 40;          ///< Bottom Right corner of the stop detection box - X
inline uint8_t STOPBOX_BR_Y          = 85;          ///< Bottom Right corner of the stop detection box - Y

inline uint8_t PERCENT_TO_STOP       = 20;          ///< Percentage of the stop detection box that needs to be red to register a stop
inline uint8_t STOP_GREEN_TOLERANCE  = 15;          ///< How much more red than green must a pixel be to be "red"
inline uint8_t STOP_BLUE_TOLERANCE   = 20;          ///< How much more red than blue must a pixel be to be "blue"


// White line constants

constexpr uint8_t WHITE_VERTICAL_CROP   = 50;       ///< Height to remove all white pixels above
constexpr uint8_t WHITE_HORIZONTAL_CROP = 75;       ///< Width to remove all white pixels to the right of
inline    uint8_t WHITE_RED_THRESH      = 240;      ///< How red does a pixel need to be to be "white"
inline    uint8_t WHITE_GREEN_THRESH    = 240;      ///< How green does a pixel need to be to be "white"
inline    uint8_t WHITE_BLUE_THRESH     = 240;      ///< How blue does a pixel need to be to be "white"
constexpr uint16_t WHITE_MIN_SIZE       = 50;       ///< Minimum size of a white blob to be considered as the white line
inline    uint8_t WHITE_CENTER_POS      = 28;       ///< X-Pos that the robot will attempt to keep the white line at


// Car detect constants

constexpr uint8_t CARBOX_TL_X           = 0;
constexpr uint8_t CARBOX_TL_Y           = 50;
constexpr uint8_t CARBOX_BR_X           = 15;
constexpr uint8_t CARBOX_BR_Y           = 70;

constexpr uint8_t PERCENT_TO_CAR        = 8;
constexpr uint8_t CAR_RED_TOLERANCE     = 50;
constexpr uint8_t CAR_BLUE_TOLERANCE    = 30;

}