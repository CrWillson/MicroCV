// Stdlib imports imports
#include <stdio.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include <iostream>
#include <algorithm>


// Esp imports
#include <esp_err.h>
#include <esp_spiffs.h>
#include <esp_log.h>
#include "sdkconfig.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "driver/uart.h"


// FreeRTOS imports
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/portmacro.h"


// In-project imports
#include "camera.hpp"
#include "lcd.hpp"
#include "microcv.hpp"


static char TAG[]="lane_detection";


// This is necessary because it allows ESP-IDF to find the main function,
// even though C++ mangles the function name.
extern "C" {
void app_main(void);
}



camera_fb_t* fb = nullptr;
SSD1306_t screen; // The screen device struct.

/// @brief The main driver loop.
inline void main_loop()
{
    while (true)
    {
        // Crop the current frame so that it will fit on the screen.
        cv::Mat working_frame = ESPCamera::get_frame(&fb);
        if (working_frame.size[0] == 0)
        {
            vTaskDelay(1);
            continue;
        }

        #if(CALIBRATION_MODE == 1)
        ESPCamera::debug::print_matrix(working_frame);
        #endif

        #if(CALIBRATION_MODE == 0)

        const auto start_tick = xTaskGetTickCount();

        // Get into the right color space for thresholding.
        cv::Mat bgr;
        cv::cvtColor(working_frame, bgr, cv::COLOR_BGR5652BGR);
        cv::Mat hsv;
        cv::cvtColor(bgr, hsv, cv::COLOR_BGR2HSV, 3);

        // Perform detection on the outsid line.
        cv::Mat hsv_outside = hsv.clone();
        cv::Mat1b outside_thresh;
        cv::Point2i outside_line_center;
        float outside_line_slope;
        MicroCV::outside_line_detection(hsv_outside, outside_thresh, outside_line_center, outside_line_slope);
        int outside_dist_from_ideal = outside_line_center.x - expected_line_pos;

        // Perform detection on the stop line.
        cv::Mat1b stop_thresh;
        bool detected;
        MicroCV::stop_line_detection(hsv, stop_thresh, detected);

        // Write to the screen.
        LCD::PrintParams params;
        params.start_tick = start_tick;
        params.frame = outside_thresh | stop_thresh;
        params.outside_dist_from_ideal = outside_dist_from_ideal;
        params.outside_line_slope = outside_line_slope;
        params.stop_detected = detected;
        output_to_screen(screen, params);

        #if PRINT_COMBINED_THRESH == 1
        MicroCV::debug::print_thresh(params.frame);
        #endif

        // Write to TX.
        //auto dist_string = std::to_string(outside_dist_from_ideal);
        //uart_write_bytes(UART_NUM, "D", 1);
        //uart_write_bytes(UART_NUM, dist_string.data(), dist_string.size());
        //uart_write_bytes(UART_NUM, "E", 1);

        constexpr short data = 0x69;
        printf("%x\n", data);

        #endif // CALIBRATION_MODE == 0

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
    #if(CALIBRATION_MODE == 0)
    uart_config_t uart_config = {
        .baud_rate = tx_baud,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    uart_param_config(UART_NUM_0, &uart_config);
    uart_driver_install(UART_NUM_0, 1024 * 2, 0, 0, NULL, 0);
    #endif

    main_loop();
}
