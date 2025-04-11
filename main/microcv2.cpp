#include "microcv2.hpp"
#include "constants.hpp"
#include "params.hpp"
#include <algorithm>

using contour_t = std::vector<cv::Point2i>;

void MicroCV2::RGB565toRGB888(const uint16_t pixel, uint16_t& red, uint16_t& green, uint16_t& blue)
{
    red = (pixel >> 11) & 0x1F;
    green = (pixel >> 5) & 0x3F;
    blue = pixel & 0x1F;

    red = (red * 255) / 31;
    green = (green * 255) / 63;
    blue = (blue * 255) / 31;
}

constexpr uint16_t MicroCV2::RGB888toRGB565(const uint8_t r, const uint8_t g, const uint8_t b)
{
    uint16_t red = (r * 31) / 255;
    uint16_t green = (g * 63) / 255;
    uint16_t blue = (b * 31) / 255;

    return (red << 11) | (green << 5) | blue;
}

void MicroCV2::cropImage(cv::Mat& image, const cv::Point2i& BOX_TL, const cv::Point2i& BOX_BR)
{
    cv::Mat mask = cv::Mat::zeros(image.size(), CV_8UC1);
    cv::rectangle(mask, BOX_TL, BOX_BR, cv::Scalar(255), cv::FILLED);
    image.setTo(cv::Scalar(0), ~mask);
}

bool MicroCV2::isStopLine(const uint16_t red, const uint16_t green, const uint16_t blue)
{
    if (red >= green + Params::STOP_GREEN_TOLERANCE && red >= blue + Params::STOP_BLUE_TOLERANCE) { 
        return true;
    }
    return false;
}

bool MicroCV2::isWhiteLine(const uint16_t red, const uint16_t green, const uint16_t blue)
{
    if (red >= Params::WHITE_RED_THRESH && green >= Params::WHITE_GREEN_THRESH && blue >= Params::WHITE_BLUE_THRESH) { 
        return true;
    }
    return false;
}


bool MicroCV2::processRedImg(const cv::Mat& image, cv::Mat1b& mask)
{
    mask = cv::Mat::zeros(image.size(), CV_8UC1);

    uint16_t redCount = 0;
    for (uint8_t y = 0; y < image.rows; ++y) {
        for (uint8_t x = 0; x < image.cols; ++x) {
            cv::Vec2b vecpixel = image.at<cv::Vec2b>(y, x);
            uint16_t pixel = (static_cast<uint16_t>(vecpixel[0]) << 8) | vecpixel[1];

            uint16_t red, green, blue;
            RGB565toRGB888(pixel, red, green, blue);

            if (isStopLine(red, green, blue) && !isWhiteLine(red, green, blue)) {
                if (x >= Params::STOPBOX_TL_X && x <= Params::STOPBOX_BR_X && y >= Params::STOPBOX_TL_Y && y <= Params::STOPBOX_BR_Y) {
                    redCount++;
                    mask.at<uchar>(y,x) = 255;
                }
            }
        }
    }

    const cv::Point2i STOPBOX_TL(Params::STOPBOX_TL_X,Params::STOPBOX_TL_Y);
    const cv::Point2i STOPBOX_BR(Params::STOPBOX_BR_X,Params::STOPBOX_BR_Y);
    cv::rectangle(mask, STOPBOX_TL, STOPBOX_BR, cv::Scalar(255), 1);

    const uint16_t STOPBOX_AREA = BOX_AREA(Params::STOPBOX_TL_X, Params::STOPBOX_TL_Y, Params::STOPBOX_BR_X, Params::STOPBOX_BR_Y);

    uint16_t percentRed = (redCount*10000) / STOPBOX_AREA;
    return percentRed >= (Params::PERCENT_TO_STOP*100);
}

bool MicroCV2::processCarImg(const cv::Mat &image, cv::Mat1b &mask)
{
    mask = cv::Mat::zeros(image.size(), CV_8UC1);

    uint16_t carCount = 0;
    for (uint8_t y = 0; y < image.rows; ++y) {
        for (uint8_t x = 0; x < image.cols; ++x) {
            cv::Vec2b vecpixel = image.at<cv::Vec2b>(y, x);
            uint16_t pixel = (static_cast<uint16_t>(vecpixel[0]) << 8) | vecpixel[1];

            uint16_t red, green, blue;
            RGB565toRGB888(pixel, red, green, blue);

            if (green >= red + Params::CAR_RED_TOLERANCE && green >= blue + Params::CAR_BLUE_TOLERANCE) {
                if (x >= Params::CARBOX_TL_X && x <= Params::CARBOX_BR_X && y >= Params::CARBOX_TL_Y && y <= Params::CARBOX_BR_Y) {
                    carCount++;
                    mask.at<uint8_t>(y,x) = 255;
                }
            }
        }
    }

    const cv::Point2i CARBOX_TL(Params::CARBOX_TL_X,Params::CARBOX_TL_Y);
    const cv::Point2i CARBOX_BR(Params::CARBOX_BR_X,Params::CARBOX_BR_Y);
    cv::rectangle(mask, CARBOX_TL, CARBOX_BR, cv::Scalar(255), 1);

    const uint16_t CARBOX_AREA = BOX_AREA(Params::CARBOX_TL_X, Params::CARBOX_TL_Y, Params::CARBOX_BR_X, Params::CARBOX_BR_Y);

    uint16_t percentCar = (carCount*10000) / CARBOX_AREA;
    return percentCar >= (Params::PERCENT_TO_CAR*100);
}

bool MicroCV2::processWhiteImg(const cv::Mat& image, cv::Mat1b& mask, cv::Mat1b& centerLine, int8_t& dist, int8_t& height)
{
    mask = cv::Mat::zeros(image.size(), CV_8UC1);
    centerLine = cv::Mat::zeros(image.size(), CV_8UC1);

    cv::line(centerLine, cv::Point(Params::WHITE_CENTER_POS, Params::WHITE_VERTICAL_CROP-5), 
             cv::Point(Params::WHITE_CENTER_POS, Params::WHITE_VERTICAL_CROP+5), cv::Scalar(255), 1);

    // Mask and crop the image in one pass
    for (uint8_t y = Params::WHITE_VERTICAL_CROP; y < image.rows; ++y) {
        for (uint8_t x = 0; x < Params::WHITE_HORIZONTAL_CROP; ++x) {
            cv::Vec2b vecpixel = image.at<cv::Vec2b>(y, x);
            uint16_t pixel = (static_cast<uint16_t>(vecpixel[0]) << 8) | vecpixel[1];

            uint16_t red, green, blue;
            RGB565toRGB888(pixel, red, green, blue);

            if (isWhiteLine(red, green, blue)) {
                mask.at<uchar>(y,x) = 255;
            }
        }
    }

    // cropImage(mask, {0, WHITE_VERTICAL_CROP}, {WHITE_HORIZONTAL_CROP, 95});

    // Find all contours
    std::vector<contour_t> contours;
    cv::findContours(mask, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
    if (contours.size() == 0) {
        return false;
    }

    // Sort the contours to put the largest at index 0
    std::sort(contours.begin(), contours.end(), [](const contour_t& a, const contour_t& b) {
            return cv::contourArea(a) > cv::contourArea(b);
    });
    if (cv::contourArea(contours[0]) < Params::WHITE_MIN_SIZE) return false;
    


    cv::Point bottomLeft = contours[0][0];
    cv::Point leftTop = contours[0][0];

    for (const auto& pt : contours[0]) {
        // bottom left point
        if (pt.y > bottomLeft.y || (bottomLeft.y == pt.y && pt.x < bottomLeft.x)) {
            bottomLeft = pt;
        }
        // left top point
        if (pt.x < leftTop.x || (pt.x == leftTop.x && pt.y < leftTop.y)) {
            leftTop = pt;
        }
    }

    // Find the line through the bottomLeft and leftTop points
    float slope = (float)(bottomLeft.y - leftTop.y) / (bottomLeft.x - leftTop.x);
    float y_intercept = leftTop.y - slope * leftTop.x;

    // Draw the line
    int16_t p1_x, p1_y, p2_x, p2_y;
    p1_y = 0;
    p2_y = mask.rows - 1;
    p1_x = (p1_y - y_intercept) / slope;
    p2_x = (p2_y - y_intercept) / slope;
    cv::line(centerLine, cv::Point(p1_x, p1_y), cv::Point(p2_x, p2_y), cv::Scalar(255), 1);

    // Point where the slope line intersects the WHITE_VERTICAL_CROP line
    cv::Point intersectionPoint;        
    intersectionPoint.y = Params::WHITE_VERTICAL_CROP;
    intersectionPoint.x = (intersectionPoint.y - y_intercept) / slope;
    cv::line(centerLine, cv::Point(intersectionPoint.x, 0), cv::Point(intersectionPoint.x, mask.rows - 1), cv::Scalar(255), 1);
    
    dist = intersectionPoint.x - Params::WHITE_CENTER_POS;

    uint8_t MAX_WHITE_DIST = CLAMP_CENTER_POS(IMG_COLS, Params::WHITE_CENTER_POS);

    if (dist > MAX_WHITE_DIST) dist = MAX_WHITE_DIST;
    if (dist < -MAX_WHITE_DIST) dist = -MAX_WHITE_DIST;

    return true;
}


void MicroCV2::generateColorBars(cv::Mat& image)
{
    uint16_t colors[] = {
        RGB888toRGB565(255, 0, 0),    // Red        - 0xf800
        RGB888toRGB565(0, 255, 0),    // Green      - 0x07e0
        RGB888toRGB565(0, 0, 255),    // Blue       - 0x001f
        RGB888toRGB565(255, 255, 0),  // Yellow     - 0xffe0
        RGB888toRGB565(0, 255, 255),  // Cyan       - 0x07ff
        RGB888toRGB565(255, 0, 255)   // Magenta    - 0xf81f
    };

    int barWidth = IMG_COLS / 6; // Each bar takes 1/6 of the width

    for (int y = 0; y < IMG_ROWS; y++) {
        for (int x = 0; x < IMG_COLS; x++) {
            int barIndex = x / barWidth; // Determine which color bar to use
            uint16_t color = colors[barIndex];

            // Store color as two bytes (little-endian)
            cv::Vec2b& pixel = image.at<cv::Vec2b>(y, x);
            pixel[0] = color & 0xFF;       // Lower byte
            pixel[1] = (color >> 8) & 0xFF; // Upper byte
        }
    }
}