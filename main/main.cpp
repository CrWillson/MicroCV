// Stdlib imports imports
#include <stdio.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include <iostream>
#include <algorithm>
#include <format>
#include <bitset>
#include <thread>

// Esp imports
#include <esp_err.h>
#include <esp_spiffs.h>
#include <esp_log.h>
#include "constants.hpp"
#include "params.hpp"
#include "sdkconfig.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "driver/gpio.h"
#include "driver/uart.h"

// FreeRTOS imports
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/portmacro.h"

// In-project imports
#include "microcv2.hpp"
#include "camera.hpp"
#include "lcd.hpp"
#include "pipico.hpp"


// This is necessary because it allows ESP-IDF to find the main function,
// even though C++ mangles the function name.
extern "C" {
void app_main(void);
}

// Global variable for the screen object
SSD1306_t screen;

// Function to pack an int8_t and two bools into a 10-bit integer
uint16_t packValues(int8_t num, bool b1, bool b2) {
    // Mask num to ensure it fits into the 8 least significant bits
    uint16_t packedNum = static_cast<uint16_t>(num & 0xFF);

    // Pack the bools into the 9th and 10th bits
    uint16_t packedValue = (b1 << 8) | (b2 << 9) | packedNum;
    return packedValue;
}


/// @brief The main driver loop.
inline void main_loop(void* params = nullptr)
{
    cv::Mat frame = cv::Mat::zeros(96, 96, CV_8UC2);
    auto& pico = PiPico::getInstance();

    int8_t dist = 0;
    int8_t height = 0;
    int64_t prevTick = 0;

    cv::Mat1b redMask = cv::Mat::zeros(frame.size(), CV_8UC1);
    cv::Mat1b carMask = cv::Mat::zeros(frame.size(), CV_8UC1);
    cv::Mat1b whiteMask = cv::Mat::zeros(frame.size(), CV_8UC1);
    cv::Mat1b whiteLine = cv::Mat::zeros(frame.size(), CV_8UC1);

    while (true)
    {

        bool result = ESPCamera::get_frame(frame);
        if (result != ESP_OK) {
            vTaskDelay(1);
            continue;
        }

        // // DEBUG PURPOSES: GENERATE COLOR BARS INSTEAD OF IMAGES
        // MicroCV2::generateColorBars(frame);

        // Get a packet from the Pico if one is available
        esp_err_t pacresult = pico.receivePacket();

        // bool carDetected = MicroCV2::processCarImg(frame, carMask);
        bool carDetected = false;

        bool stopDetected = MicroCV2::processRedImg(frame, redMask);

        bool whiteDetected = MicroCV2::processWhiteImg(frame, whiteMask, whiteLine, dist, height);

        if (pico.photoRequested) {
            pico.sendPacket(dist, stopReady, frame);
        } else {
            pico.sendPacket(dist, stopReady);
        }

        int64_t currTick = esp_timer_get_time();
        int64_t loop_ticks = currTick - prevTick;
        prevTick = currTick;

        LCD::PrintParams params;
        params.loop_ticks = loop_ticks;
        params.frame = redMask | whiteLine | whiteMask;
        params.white_dist = dist;
        params.stop_detected = stopPending;
        // uart_get_buffered_data_len(UART_NUM, &params.other);
        LCD::output_to_screen(screen, params);

        vTaskDelay(1);
    }
}

/// @brief The entry-point.
void app_main(void)
{
    ESPCamera::config_cam();

    // Init screen
    i2c_master_init(&screen, CONFIG_SDA_GPIO, CONFIG_SCL_GPIO, CONFIG_RESET_GPIO);
    ssd1306_init(&screen, LCD::SCREEN_WIDTH, LCD::SCREEN_HEIGHT);

    PiPico::getInstance().init();

    main_loop();
}
