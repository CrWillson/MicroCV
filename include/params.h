///////////////////////////////////////////////////////////////////////////////////////////////////
/// A generated file which contains constants determined in the Python debugging tool.
///
/// Author: Andrew Huffman
///////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <stdint.h>
#include "opencv2.hpp"


constexpr uint16_t BOX_AREA(const uint8_t TL_X, const uint8_t TL_Y, const uint8_t BR_X, const uint8_t BR_Y) {
    if (BR_X >= TL_X && BR_Y >= TL_Y) {
        return (BR_X - TL_X + 1) * (BR_Y - TL_Y + 1);
    }
    return 0;
}

constexpr uint8_t CLAMP_CENTER_POS(const uint8_t IMG_COLS, const uint8_t WHITE_CENTER_POS) {
    if (IMG_COLS > 2*WHITE_CENTER_POS) {
        // On left side of image
        return WHITE_CENTER_POS;
    } else {
        // On right side of image
        return IMG_COLS - WHITE_CENTER_POS;
    }
}

// General constants
constexpr uint8_t IMG_ROWS = 96;
constexpr uint8_t IMG_COLS = 96;

// Stop line constants
constexpr uint8_t STOPBOX_TL_X          = 20;
constexpr uint8_t STOPBOX_TL_Y          = 75;
constexpr uint8_t STOPBOX_BR_X          = 45;
constexpr uint8_t STOPBOX_BR_Y          = 90;

const cv::Point2i STOPBOX_TL(STOPBOX_TL_X,STOPBOX_TL_Y);
const cv::Point2i STOPBOX_BR(STOPBOX_BR_X,STOPBOX_BR_Y);
constexpr uint16_t STOPBOX_AREA = BOX_AREA(STOPBOX_TL_X, STOPBOX_TL_Y, STOPBOX_BR_X, STOPBOX_BR_Y);

constexpr uint8_t PERCENT_TO_STOP       = 10;
constexpr uint8_t STOP_GREEN_TOLERANCE  = 20;
constexpr uint8_t STOP_BLUE_TOLERANCE   = 30;


// White line constants
constexpr uint8_t WHITELINE_CROP        = 50;
constexpr uint8_t WHITE_RED_THRESH      = 170;
constexpr uint8_t WHITE_GREEN_THRESH    = 170;
constexpr uint8_t WHITE_BLUE_THRESH     = 100;
constexpr uint16_t WHITE_MIN_SIZE       = 40;
constexpr uint8_t WHITE_CENTER_POS      = 28; // 42;
constexpr uint8_t MAX_WHITE_DIST = CLAMP_CENTER_POS(IMG_COLS, WHITE_CENTER_POS);


// Car detect constants
constexpr uint8_t CARBOX_TL_X           = 0;
constexpr uint8_t CARBOX_TL_Y           = 40;
constexpr uint8_t CARBOX_BR_X           = 15;
constexpr uint8_t CARBOX_BR_Y           = 70;

const cv::Point2i CARBOX_TL(CARBOX_TL_X,CARBOX_TL_Y);
const cv::Point2i CARBOX_BR(CARBOX_BR_X,CARBOX_BR_Y);
constexpr uint16_t CARBOX_AREA = BOX_AREA(CARBOX_TL_X, CARBOX_TL_Y, CARBOX_BR_X, CARBOX_BR_Y);

constexpr uint8_t PERCENT_TO_CAR        = 10;
constexpr uint8_t CAR_RED_TOLERANCE     = 50;
constexpr uint8_t CAR_BLUE_TOLERANCE    = 30;





/*
// 25 Total parameters to calibrate in the old system
constexpr uint8_t expected_line_pos = 66;

constexpr uint8_t expected_red_y = 82;
constexpr uint8_t expected_red_radius = 2;

constexpr uint8_t outside_thresh_min_hue = 8;
constexpr uint8_t outside_thresh_min_sat = 0;
constexpr uint8_t outside_thresh_min_val = 238;

constexpr uint8_t outside_thresh_max_hue = 179;
constexpr uint8_t outside_thresh_max_sat = 255;
constexpr uint8_t outside_thresh_max_val = 255;

constexpr uint8_t outside_cropping_top = 51;
constexpr uint8_t outside_cropping_bottom = 0;
constexpr uint8_t outside_cropping_left = 22;
constexpr uint8_t outside_cropping_right = 0;

constexpr uint16_t outside_min_detect_area = 139;

constexpr uint8_t stop_thresh_min_hue = 0;
constexpr uint8_t stop_thresh_min_sat = 49;
constexpr uint8_t stop_thresh_min_val = 156;

constexpr uint8_t stop_thresh_max_hue = 29;
constexpr uint8_t stop_thresh_max_sat = 159;
constexpr uint8_t stop_thresh_max_val = 247;

constexpr uint8_t stop_cropping_top = 48;
constexpr uint8_t stop_cropping_bottom = 0;
constexpr uint8_t stop_cropping_left = 0;
constexpr uint8_t stop_cropping_right = 0;

constexpr uint16_t stop_min_detect_area = 139;
*/
