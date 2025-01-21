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
#include "camera.hpp"
#include "lcd.hpp"
#include "microcv2.hpp"


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
    cv::Mat frame;
    int8_t dist = 0;
    int8_t height = 0;
    int64_t prevTick = 0;

    while (true)
    {
        bool result = ESPCamera::get_frame(frame);
        if (result != ESP_OK) {
            vTaskDelay(1);
            continue;
        }
        
        cv::Mat1b redMask;
        bool stopDetected = MicroCV2::processRedImg(frame, redMask);

        cv::Mat1b whiteMask;
        cv::Mat1b whiteLine;
        bool whiteDetected = MicroCV2::processWhiteImg(frame, whiteMask, whiteLine, dist, height);

        // cv::Mat1b carMask;
        // bool carDetected = MicroCV2::processCarImg(frame, carMask);
    

        auto packedByte = packValues(dist, stopDetected, whiteDetected);
        std::string byteString = std::bitset<10>(packedByte).to_string() + "\n";
        //uart_write_bytes(UART_NUM, byteString.c_str(), byteString.size());
        printf(byteString.c_str());


        int64_t currTick = esp_timer_get_time();
        int64_t loop_ticks = currTick - prevTick;
        prevTick = currTick;

        if (LCD::screenMutex.try_lock()) {
            LCD::screenMutex.unlock();

            LCD::PrintParams params;
            params.loop_ticks = loop_ticks;
            params.frame = whiteLine | whiteMask | redMask;
            params.dist = dist;
            params.height = height;
            params.stop_detected = stopDetected;
            params.car_detected = whiteDetected;

            std::thread([&]() {
                LCD::output_to_screen(screen, params);
            }).detach();
            //LCD::output_to_screen(screen, params);
        } else {
            ESP_LOGI(LCD::TAG, "Screen busy, skipping frame.");
        }

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

    // Init tx pin
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wmissing-field-initializers"
    uart_config_t uart_config = {
        .baud_rate = tx_baud,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    #pragma GCC diagnostic pop

    uart_param_config(UART_NUM_0, &uart_config);
    uart_driver_install(UART_NUM_0, 1024 * 2, 0, 0, NULL, 0);

    main_loop();
}
