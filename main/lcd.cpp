#include "lcd.hpp"

int curr_row = 0;

void LCD::lcd_draw_string(SSD1306_t& screen, std::string& string, int row)
{
    if (-1 == row)
    {
        row = curr_row;
    }

    ssd1306_display_text(&screen, row, string.data(), string.size(), false);
    curr_row = row + 1;
}


/// @brief Writes a parameter to the LCD screen.
/// @param screen The screen to write to.
/// @param lines The lines to write to the screen.
void LCD::lcd_draw_string(SSD1306_t& screen, std::vector<std::string>& lines, int start_row)
{
    if (-1 == start_row)
    {
        start_row = curr_row;
    }

    for (size_t i = 0; i < lines.size(); i++)
    {
        std::string& str = lines[i];
        lcd_draw_string(screen, str, start_row + i);
    }
    curr_row = lines.size() + start_row;
}


void LCD::lcd_draw_data(SSD1306_t& screen, std::string preamble, int data, int row)
{
    std::string arg = preamble + " " + std::to_string(data);
    lcd_draw_string(screen, arg, row);
}

void LCD::lcd_draw_data(SSD1306_t& screen, std::string preamble, double data, int row)
{
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1) << data;
    std::string arg = preamble + " " + oss.str();
    lcd_draw_string(screen, arg, row);
}

void LCD::lcd_draw_data(SSD1306_t& screen, std::string preamble, bool data, int row)
{
    std::string arg = preamble + " " + (data ? "t" : "f");
    lcd_draw_string(screen, arg, row);
}




/// @brief Writes a binary matrix to a provided LCD screen.
/// @param screen The LCD screen to write to.
/// @param bin_mat The binary matrix to write.
void LCD::lcd_draw_matrix(SSD1306_t& screen, const cv::Mat& bin_mat)
{
    curr_row = 0;
    for (uint8_t row = 0; row < bin_mat.rows; row++)
    {
        for (uint8_t col = 0; col < bin_mat.cols; col++)
        {
            const bool invert = (0 == bin_mat.at<uint8_t>(row, col));
            _ssd1306_pixel(&screen, col, row, invert);
        }
    }
    ssd1306_show_buffer(&screen);
}


/// @brief Prints data about the detection process to the LCD screen.
/// @param screen The screen to print to.
/// @param params A struct containing values to print to the screen.
void LCD::output_to_screen(SSD1306_t& screen, PrintParams params)
{
    cv::resize(params.frame, params.frame, cv::Size(LCD::SCREEN_WIDTH, LCD::SCREEN_HEIGHT));

    std::ostringstream frStr;
    const double framerate = 1000000.0 / params.loop_ticks;
    frStr << "FR: " << std::setprecision(3) << framerate;

    std::vector<std::string> lines = {frStr.str()};
    int baseline = 0;
    cv::Size textSize;
    auto origin = cv::Point(35,8);
    int yOffset = origin.y;

    auto font = cv::FONT_HERSHEY_PLAIN;
    float fontScale = 0.7;
    int fontThick = 1;
    int lineSpacing = 2;

    for (const auto& line : lines) {
        textSize = cv::getTextSize(line, font, fontScale, fontThick, &baseline);

        cv::rectangle(params.frame, 
                        cv::Point(origin.x, yOffset - textSize.height),
                        cv::Point(origin.x + textSize.width, yOffset + baseline),
                        cv::Scalar(0), cv::FILLED);
        
        cv::putText(params.frame, line, cv::Point(origin.x, yOffset), font, fontScale, cv::Scalar(255), fontThick);
        yOffset += textSize.height + lineSpacing;
    }

    LCD::lcd_draw_matrix(screen, params.frame);
}
