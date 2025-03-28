#include "microcv2.hpp"
#include "constants.hpp"

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

// bool MicroCV2::maskAllColors(const cv::Mat& image, cv::Mat1b& redMask, uint16_t& rCount, cv::Mat1b& whiteMask, uint16_t& wCount)
// {
//     redMask = cv::Mat::zeros(image.size(), CV_8UC1);
//     whiteMask = cv::Mat::zeros(image.size(), CV_8UC1);

//     for (uint8_t y = 0; y < image.rows; ++y) {
//         for (uint8_t x = 0; x < image.cols; ++x) {
//             uint16_t pixel = (image.at<cv::Vec2b>(y,x)[1] << 8) | image.at<cv::Vec2b>(y,x)[0];

//             uint16_t red, green, blue;
//             RGB565toRGB888(pixel, red, green, blue);

//             if (red >= green + STOP_GREEN_TOLERANCE && red >= blue + STOP_BLUE_TOLERANCE) {
//                 if (x >= STOPBOX_TL.x && x <= STOPBOX_BR.x && y >= STOPBOX_TL.y && y <= STOPBOX_BR.y) {
//                     rCount++;
//                 }
//                 redMask.at<uchar>(y,x) = 255;
//             }
//         }
//     }
// }

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

    const uint16_t STOPBOX_AREA = Params::BOX_AREA(Params::STOPBOX_TL_X, Params::STOPBOX_TL_Y, Params::STOPBOX_BR_X, Params::STOPBOX_BR_Y);

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
                if (x >= Params::CARBOX_TL.x && x <= Params::CARBOX_BR.x && y >= Params::CARBOX_TL.y && y <= Params::CARBOX_BR.y) {
                    carCount++;
                    mask.at<uint8_t>(y,x) = 255;
                }
            }
        }
    }

    cv::rectangle(mask, Params::CARBOX_TL, Params::CARBOX_BR, cv::Scalar(255), 1);

    uint16_t percentCar = (carCount*10000) / Params::CARBOX_AREA;
    return percentCar >= (Params::PERCENT_TO_CAR*100);
}

bool MicroCV2::processWhiteImg(const cv::Mat& image, cv::Mat1b& mask, cv::Mat1b& centerLine, int8_t& dist, int8_t& height)
{
    mask = cv::Mat::zeros(image.size(), CV_8UC1);
    centerLine = cv::Mat::zeros(image.size(), CV_8UC1);

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

    std::vector<contour_t> contours;
    cv::findContours(mask, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
    if (contours.size() == 0) {
        return false;
    }

    int maxSize = 0;
    int maxInd = 0;
    for (int i = 0; i < contours.size(); i++) {
        int size = cv::contourArea(contours[i]);
        if (size > maxSize) {
            maxSize = size;
            maxInd = i;
        }
    }
    if (maxSize < Params::WHITE_MIN_SIZE) return false;

    cv::Rect boundingBox = cv::boundingRect(contours[maxInd]);
    dist = boundingBox.x + boundingBox.width / 2;
    height = boundingBox.y + boundingBox.height / 2;

    cv::Point leftmost_topmost = contours[maxInd][0];

    // Iterate through the contour to find the topmost-leftmost point
    for (const auto& point : contours[maxInd]) {
        if (point.x < leftmost_topmost.x || 
            (point.x == leftmost_topmost.x && point.y < leftmost_topmost.y)) {
            leftmost_topmost = point;
        }
    } 
    dist = leftmost_topmost.x;
    height = leftmost_topmost.y;

    cv::line(centerLine, cv::Point(dist, 0), cv::Point(dist, mask.rows - 1), cv::Scalar(255), 1);
    cv::line(centerLine, cv::Point(0, height), cv::Point(mask.cols - 1, height), cv::Scalar(255), 1);
    
    dist -= Params::WHITE_CENTER_POS;

    if (dist > Params::MAX_WHITE_DIST) dist = Params::MAX_WHITE_DIST;
    if (dist < -Params::MAX_WHITE_DIST) dist = -Params::MAX_WHITE_DIST;

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