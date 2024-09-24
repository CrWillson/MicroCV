#pragma once

// Opencv Imports
#undef EPS
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#define EPS 192

#include "params.h"
#include "constants.hpp"
#include "driver/uart.h"

using contour_t = std::vector<cv::Point2i>;

namespace MicroCV {

uint8_t get_lane_center(const cv::Mat1b& mask, const uint8_t start_row = 0);

contour_t get_line(const cv::Mat1b& mask);

float get_slope(const contour_t& contour);

void apply_cropping(cv::Mat& frame, const uint16_t top, const uint16_t bottom, const uint16_t left, const uint16_t right);

void outside_line_detection(cv::Mat& hsv, cv::Mat1b& thresh, cv::Point2i& center_point, float& slope);

bool rectangles_overlap(cv::Rect2i rect1, cv::Rect2i rect2);

void stop_line_detection(cv::Mat& hsv, cv::Mat1b& thresh, bool& detected);

namespace debug {
    
    void print_thresh(cv::Mat1b& thresh);
}
}