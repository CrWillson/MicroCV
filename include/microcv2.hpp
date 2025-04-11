#pragma once

// Opencv Imports
#include "opencv2.hpp"

#include "params.hpp"
#include "constants.hpp"
#include "esp_log.h"
#include "driver/uart.h"

using contour_t = std::vector<cv::Point2i>;

namespace MicroCV2 {
    /// Tag used for ESP logs
    constexpr const char *TAG = "MICROCV";

    /**
    * @brief Calculate the area of a box defined by four points
    * 
    * @param TL_X - Top left X
    * @param TL_Y - Top left Y
    * @param BR_X - Bottom right X
    * @param BR_Y - Bottom right Y
    * @return uint16_t - The area of the box 
    */
    constexpr uint16_t BOX_AREA(const uint8_t TL_X, const uint8_t TL_Y, const uint8_t BR_X, const uint8_t BR_Y) {
        if (BR_X >= TL_X && BR_Y >= TL_Y) {
            return (BR_X - TL_X + 1) * (BR_Y - TL_Y + 1);
        }
        return 0;
    }

    /**
    * @brief Find the maximum distance a point can be from the center line without being off the edge of the image
    * 
    * @param IMG_COLS - The number of columns in the image
    * @param WHITE_CENTER_POS - The position of the center line
    * @return uint8_t - The max distance from the center line still on the image
    */
    constexpr uint8_t CLAMP_CENTER_POS(const uint8_t IMG_COLS, const uint8_t WHITE_CENTER_POS) {
        if (IMG_COLS > 2*WHITE_CENTER_POS)
            return WHITE_CENTER_POS;
        return IMG_COLS - WHITE_CENTER_POS;
    }

    /**
     * @brief Convert a single 16-bit RGB565 pixel to 3 8-bit RGB888 pixels
     * 
     * @param pixel - The 16-bit RGB565 pixel
     * @param red - The red value output
     * @param green - The green value output
     * @param blue - The blue value output
     */
    void RGB565toRGB888(const uint16_t pixel, uint16_t& red, uint16_t& green, uint16_t& blue);

    /**
     * @brief Convert an RGB888 value into its corresponding RGB565 value
     * 
     * @param r - The red value
     * @param g - The green value
     * @param b - The blue value
     * @return uint16_t - The RGB565 output
     */
    constexpr uint16_t RGB888toRGB565(const uint8_t r, const uint8_t g, const uint8_t b);

    /**
     * @brief Set all pixels outside the specified box to black
     * 
     * @param image - The image to crop
     * @param BOX_TL - The top left corner of the box
     * @param BOX_BR - The bottom right corner of the box
     * @note The image is modified in place
     */
    void cropImage(cv::Mat& image, const cv::Point2i& BOX_TL, const cv::Point2i& BOX_BR);

    /**
     * @brief Return true if a pixel is red enough to be considered a stop line
     * 
     * @param red
     * @param green 
     * @param blue 
     */
    bool isStopLine(const uint16_t red, const uint16_t green, const uint16_t blue);

    /**
     * @brief Return true if a pixel is white enough to be considered a white line
     * 
     * @param red 
     * @param green 
     * @param blue 
     */
    bool isWhiteLine(const uint16_t red, const uint16_t green, const uint16_t blue);

    /**
     * @brief Process a frame for everything related to the stop line.
     * 
     * @param img - Input image
     * @param mask - Output mask of all red pixels
     * @return Whether the stop line was detected or not
     * @note The image and mask are modified in place
     */
    bool processRedImg(const cv::Mat& img, cv::Mat1b& mask);

    /**
     * @warning OBSTACLE AND CAR DETECTION IS CURRENTLY NOT WORKING OR USED (4/8/2025)
     * @brief Process a frame for everything related to detecting obstacles or other cars.
     * 
     * @param img - Input image
     * @param mask - Output mask of all obstacle pixels
     * @return Whether an obstacle was detected or not
     * @note The image and mask are modified in place
     */
    bool processCarImg(const cv::Mat& img, cv::Mat1b& mask);

    /**
     * @brief Process a frame for everything related to the white line.
     * 
     * @param img - Input image
     * @param mask - Output mask of all white pixels
     * @param centerLine - Additional output mask showing other reference lines and points
     * @param dist - The reported distance to the white line
     * @return Whether the white line was detected or not
     * @note The image and mask are modified in place
     */
    bool processWhiteImg(const cv::Mat& img, cv::Mat1b& mask, cv::Mat1b& centerLine, int8_t& dist, int8_t& height);

    /**
     * @brief Generate a test image of color bars
     * 
     * @param image - The matrix to load the color bars into
     * @note This function is for debug purposes only
     */
    void generateColorBars(cv::Mat& image);

}