#pragma once

// Opencv Imports
#include "opencv2.hpp"


#include "params.h"
#include "constants.hpp"
#include "esp_log.h"
#include "driver/uart.h"

using contour_t = std::vector<cv::Point2i>;

namespace MicroCV2 {

    constexpr const char *TAG = "MICROCV";

    void RGB565toRGB888(const uint16_t pixel, uint16_t& red, uint16_t& green, uint16_t& blue);

    constexpr uint16_t RGB888toRGB565(const uint8_t r, const uint8_t g, const uint8_t b);

    /**
     * Set all pixels outside the specified box to (0,0,0)
     */
    void cropImage(cv::Mat& image, const cv::Point2i& BOX_TL, const cv::Point2i& BOX_BR);

    bool isStopLine(const uint16_t red, const uint16_t green, const uint16_t blue);

    bool isWhiteLine(const uint16_t red, const uint16_t green, const uint16_t blue);

    /**
     * Return true if the stop line is detected
     */
    bool processRedImg(const cv::Mat& img, cv::Mat1b& mask);

    bool processCarImg(const cv::Mat& img, cv::Mat1b& mask);

    bool processWhiteImg(const cv::Mat& img, cv::Mat1b& mask, cv::Mat1b& centerLine, int8_t& dist, int8_t& height);

    // bool maskAllColors(const cv::Mat& img, cv::Mat1b& redMask, uint16_t& rCount, cv::Mat1b& whiteMask, uint16_t& wCount);

    void generateColorBars(cv::Mat& image);

}