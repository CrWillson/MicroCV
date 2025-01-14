#include "microcv2.hpp"

void MicroCV2::RGB565toRGB888(uint16_t pixel, uint16_t& red, uint16_t& blue, uint16_t& green)
{
    red = (pixel >> 11) & 0x1F;
    green = (pixel >> 5) & 0x3F;
    blue = pixel & 0x1F;

    red = (red * 255) / 31;
    green = (green * 255) / 63;
    blue = (blue * 255) / 31;
}

void MicroCV2::cropImage(cv::Mat& image, const cv::Point2i& BOX_TL, const cv::Point2i& BOX_BR)
{
    cv::Mat mask = cv::Mat::zeros(image.size(), CV_8UC1);
    cv::rectangle(mask, BOX_TL, BOX_BR, cv::Scalar(255), cv::FILLED);
    image.setTo(cv::Scalar(0), ~mask);
}

bool MicroCV2::processRedImg(const cv::Mat& image, cv::Mat1b& mask)
{
    mask = cv::Mat::zeros(image.size(), CV_8UC1);

    uint16_t redCount = 0;
    for (uint8_t y = 0; y < image.rows; ++y) {
        for (uint8_t x = 0; x < image.cols; ++x) {
            uint16_t pixel = (image.at<cv::Vec2b>(y,x)[1] << 8) | image.at<cv::Vec2b>(y,x)[0];

            uint16_t red, green, blue;
            RGB565toRGB888(pixel, red, green, blue);

            if (red >= green + PARAMS::params["STOP_GREEN_TOLERANCE"] && red >= blue + PARAMS::params["STOP_BLUE_TOLERANCE"]) {
                if (x >= PARAMS::params["STOPBOX_TL_X"] && x <= PARAMS::params["STOPBOX_BR_X"] && y >= PARAMS::params["STOPBOX_TL_Y"] && y <= PARAMS::params["STOPBOX_BR_Y"]) {
                    redCount++;
                }
                mask.at<uchar>(y,x) = 255;
            }
        }
    }

    cv::Point2i STOPBOX_TL(PARAMS::params["STOPBOX_TL_X"], PARAMS::params["STOPBOX_TL_Y"]);
    cv::Point2i STOPBOX_BR(PARAMS::params["STOPBOX_BR_X"], PARAMS::params["STOPBOX_BR_Y"]);
    cv::rectangle(mask, STOPBOX_TL, STOPBOX_BR, cv::Scalar(255), 1);

    uint16_t percentRed = (redCount*10000) / PARAMS::params["STOPBOX_AREA"];
    return percentRed >= (PARAMS::params["PERCENT_TO_STOP"]*100);
}

bool MicroCV2::processCarImg(const cv::Mat& image, cv::Mat1b& mask)
{
    mask = cv::Mat::zeros(image.size(), CV_8UC1);

    uint16_t carCount = 0;
    for (uint8_t y = 0; y < image.rows; ++y) {
        for (uint8_t x = 0; x < image.cols; ++x) {
            uint16_t pixel = (image.at<cv::Vec2b>(y,x)[1] << 8) | image.at<cv::Vec2b>(y,x)[0];

            uint16_t red, green, blue;
            RGB565toRGB888(pixel, red, green, blue);

            if (green >= red + PARAMS::params["CAR_RED_TOLERANCE"] && green >= blue + PARAMS::params["CAR_BLUE_TOLERANCE"]) {
                if (x >= PARAMS::params["CARBOX_TL_X"] && x <= PARAMS::params["CARBOX_BR_X"] && y >= PARAMS::params["CARBOX_TL_Y"] && y <= PARAMS::params["CARBOX_BR_Y"]) {
                    carCount++;
                }
                mask.at<uchar>(y,x) = 255;
            }
        }
    }

    uint16_t percentCar = (carCount*10000) / PARAMS::params["CARBOX_AREA"];
    return percentCar >= (PARAMS::params["PERCENT_TO_CAR"]*100);
}

bool MicroCV2::processWhiteImg(const cv::Mat& image, cv::Mat1b& mask, cv::Mat1b& centerLine, int8_t& dist, int8_t& height)
{
    mask = cv::Mat::zeros(image.size(), CV_8UC1);
    centerLine = cv::Mat::zeros(image.size(), CV_8UC1);

    for (uint8_t y = 0; y < image.rows; ++y) {
        for (uint8_t x = 0; x < image.cols; ++x) {
            uint16_t pixel = (image.at<cv::Vec2b>(y,x)[1] << 8) | image.at<cv::Vec2b>(y,x)[0];

            uint16_t red, green, blue;
            RGB565toRGB888(pixel, red, green, blue);

            if (red >= PARAMS::params["WHITE_RED_THRESH"] && green >= PARAMS::params["WHITE_GREEN_THRESH"] && blue >= PARAMS::params["WHITE_BLUE_THRESH"]) {
                mask.at<uchar>(y,x) = 255;
            }
        }
    }

    cropImage(mask, {0, PARAMS::params["WHITELINE_CROP"]}, {95, 95});

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
    if (maxSize < PARAMS::params["WHITE_MIN_SIZE"]) return false;

    cv::Rect boundingBox = cv::boundingRect(contours[maxInd]);
    dist = boundingBox.x + boundingBox.width / 2;
    height = boundingBox.y + boundingBox.height / 2;

    cv::line(centerLine, cv::Point(dist, 0), cv::Point(dist, mask.rows - 1), cv::Scalar(255), 1);
    cv::line(centerLine, cv::Point(0, height), cv::Point(mask.cols - 1, height), cv::Scalar(255), 1);
    dist -= PARAMS::params["WHITE_CENTER_POS"];

    return true;
}