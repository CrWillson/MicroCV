#pragma once
///////////////////////////////////////////////////////////////////////////////////////////////////
/// Helper functions for communicating with the LCD screen.
///
/// Author: Andrew Huffman
///////////////////////////////////////////////////////////////////////////////////////////////////


// Stdlib imports
#include <vector>
#include <string>

// LCD imports
#include "ssd1306.h"

// Opencv Imports
#undef EPS
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#define EPS 192

namespace LCD {

/// @brief Parameters which are available to the LCD screen printing.
class PrintParams {
public:
    PrintParams(): frame(cv::Mat1b()), outside_line_slope(0), start_tick(0), outside_dist_from_ideal(0), stop_detected(false) {}

    cv::Mat1b frame;                /// @brief The image to print to the screen.
    float outside_line_slope;       /// @brief The slope of the detected outside line.
    TickType_t start_tick;          /// @brief The number of ticks which have thus far passed.
    int outside_dist_from_ideal;    /// @brief The number of pixels the line on the screen is from its ideal, calibrated position.
    bool stop_detected;             /// @brief Whether the stop line has been detected.
};


/// @brief The native width of the screen.
constexpr uint8_t SCREEN_WIDTH = 128;


/// @brief The native heigth of the screen.
constexpr uint8_t SCREEN_HEIGHT = 64;


/// @brief If passed into one of the print functions as a row, prints oen after the previous.
constexpr int APPEND_ROW = -1;


/// @brief Draws a string to the LCD screen.
/// @param screen The screen to draw to.
/// @param string The string.
/// @param row The row of the screen to draw at.
void lcd_draw_string(SSD1306_t& screen, std::string& string, int row = APPEND_ROW);


/// @brief Writes a number of lines to the LCD screen, from top to bottom.
/// @param screen The screen to write to.
/// @param lines The lines to write to the screen.
/// @param start_row The row to start printing on.
void lcd_draw_string(SSD1306_t& screen, std::vector<std::string>& lines, int start_row = APPEND_ROW);


/// @brief Draws a string of this format in a row: "{preamble} {data}"
/// @param screen The scren to print to.
/// @param preamble The preamble.
/// @param data The data.
/// @param row The row on which to print.
void lcd_draw_data(SSD1306_t& screen, std::string preamble, int data, int row = APPEND_ROW);


/// @brief Draws a string of this format in a row: "{preamble} {data}"
/// @param screen The scren to print to.
/// @param preamble The preamble.
/// @param data The data.
/// @param row The row on which to print.
void lcd_draw_data(SSD1306_t& screen, std::string preamble, bool data, int row = APPEND_ROW);


/// @brief Writes an OpenCV matrix to the screen. This also clears the screen.
/// @param screen The screen to write to.
/// @param bin_mat The matrix to write. Should be a binary mask.
void lcd_draw_matrix(SSD1306_t& screen, const cv::Mat& bin_mat);


void output_to_screen(SSD1306_t& screen, PrintParams& params);
}
