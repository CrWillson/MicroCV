///////////////////////////////////////////////////////////////////////////////////////////////////
/// A generated file which contains constants determined in the Python debugging tool.
///
/// Author: Andrew Huffman
///////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <stdint.h>

// Opencv Imports
#undef EPS
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#define EPS 192

// 18 Total parameters to calibrate

// General constants
constexpr uint8_t IMG_ROWS = 96;
constexpr uint8_t IMG_COLS = 96;

// Stop line constants
constexpr uint8_t STOPBOX_TL_X          = 45;
constexpr uint8_t STOPBOX_TL_Y          = 75;
constexpr uint8_t STOPBOX_BR_X          = 70;
constexpr uint8_t STOPBOX_BR_Y          = 90;

const cv::Point2i STOPBOX_TL(STOPBOX_TL_X,STOPBOX_TL_Y);
const cv::Point2i STOPBOX_BR(STOPBOX_BR_X,STOPBOX_BR_Y);
constexpr uint16_t STOPBOX_AREA = (STOPBOX_BR_X >= STOPBOX_TL_X && STOPBOX_BR_Y >= STOPBOX_TL_Y) 
                                  ? (STOPBOX_BR_X - STOPBOX_TL_X + 1) * (STOPBOX_BR_Y - STOPBOX_TL_Y + 1) : 0;

constexpr uint8_t PERCENT_TO_STOP       = 10;
constexpr uint8_t STOP_GREEN_TOLERANCE  = 20;
constexpr uint8_t STOP_BLUE_TOLERANCE   = 30;


// White line constants
constexpr uint8_t WHITELINE_CROP        = 55;
constexpr uint8_t WHITE_RED_THRESH      = 160;
constexpr uint8_t WHITE_GREEN_THRESH    = 170;
constexpr uint8_t WHITE_BLUE_THRESH     = 100;
constexpr uint16_t WHITE_MIN_SIZE       = 50;
constexpr uint8_t WHITE_CENTER_POS      = 50;


// Car detect constants
constexpr uint8_t CARBOX_TL_X           = 0;
constexpr uint8_t CARBOX_TL_Y           = 40;
constexpr uint8_t CARBOX_BR_X           = 15;
constexpr uint8_t CARBOX_BR_Y           = 70;

const cv::Point2i CARBOX_TL(CARBOX_TL_X,CARBOX_TL_Y);
const cv::Point2i CARBOX_BR(CARBOX_BR_X,CARBOX_BR_Y);
constexpr uint16_t CARBOX_AREA = (CARBOX_BR_X >= CARBOX_TL_X && CARBOX_BR_Y >= CARBOX_TL_Y) 
                                  ? (CARBOX_BR_X - CARBOX_TL_X + 1) * (CARBOX_BR_Y - CARBOX_TL_Y + 1) : 0;

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
