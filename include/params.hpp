///////////////////////////////////////////////////////////////////////////////////////////////////
/// A generated file which contains constants determined in the Python debugging tool.
///
/// Author: Andrew Huffman
///////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <stdint.h>
#include <map>
#include <string>

constexpr uint16_t BOX_AREA(const uint8_t TL_X, const uint8_t TL_Y, const uint8_t BR_X, const uint8_t BR_Y) {
    if (BR_X >= TL_X && BR_Y >= TL_Y) {
        return (BR_X - TL_X + 1) * (BR_Y - TL_Y + 1);
    }
    return 0;
}

// 16 Total parameters to calibrate

struct PARAMS {

    static std::map<std::string, uint16_t> params;
    static auto& getParams() { return params; }
    // // General constants
    // static uint8_t IMG_ROWS;
    // static uint8_t IMG_COLS;

    // // Stop line constants
    // static uint8_t STOPBOX_TL_X;
    // static uint8_t STOPBOX_TL_Y;
    // static uint8_t STOPBOX_BR_X;
    // static uint8_t STOPBOX_BR_Y;
    // static uint16_t STOPBOX_AREA;

    // static uint8_t PERCENT_TO_STOP;
    // static uint8_t STOP_GREEN_TOLERANCE;
    // static uint8_t STOP_BLUE_TOLERANCE;

    // // White line constants
    // static uint8_t WHITELINE_CROP;
    // static uint8_t WHITE_RED_THRESH;
    // static uint8_t WHITE_GREEN_THRESH;
    // static uint8_t WHITE_BLUE_THRESH;
    // static uint16_t WHITE_MIN_SIZE;
    // static uint8_t WHITE_CENTER_POS;

    // // Car detect constants
    // static uint8_t CARBOX_TL_X;
    // static uint8_t CARBOX_TL_Y;
    // static uint8_t CARBOX_BR_X;
    // static uint8_t CARBOX_BR_Y;
    // static uint16_t CARBOX_AREA;

    // static uint8_t PERCENT_TO_CAR;
    // static uint8_t CAR_RED_TOLERANCE;
    // static uint8_t CAR_BLUE_TOLERANCE;

};


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