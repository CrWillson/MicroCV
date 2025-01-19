#pragma once

// Opencv Imports
#include "opencv2.hpp"


#include "params.h"
#include "constants.hpp"
#include "driver/uart.h"

using contour_t = std::vector<cv::Point2i>;

namespace MicroCV2 {

    void RGB565toRGB888(uint16_t pixel, uint16_t& red, uint16_t& blue, uint16_t& green);

    /**
     * Set all pixels outside the specified box to (0,0,0)
     */
    void cropImage(cv::Mat& image, const cv::Point2i& BOX_TL, const cv::Point2i& BOX_BR);

    /**
     * Return true if the stop line is detected
     */
    bool processRedImg(const cv::Mat& img, cv::Mat1b& mask);

    bool processCarImg(const cv::Mat& img, cv::Mat1b& mask);

    bool processWhiteImg(const cv::Mat& img, cv::Mat1b& mask, cv::Mat1b& centerLine, int8_t& dist, int8_t& height);

    // bool maskAllColors(const cv::Mat& img, cv::Mat1b& redMask, uint16_t& rCount, cv::Mat1b& whiteMask, uint16_t& wCount);

}