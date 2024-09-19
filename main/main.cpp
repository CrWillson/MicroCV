// Opencv Imports
#undef EPS
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#define EPS 192

// Esp imports
#include <esp_err.h>
#include <esp_spiffs.h>
#include <esp_log.h>
#include "sdkconfig.h"
#include "esp_camera.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "driver/uart.h"

#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "ssd1306.h"

// This is necessary because it allows ESP-IDF to find the main function,
// even though C++ mangles the function name.
extern "C" {
void app_main(void);
}

using contour_t = std::vector<cv::Point2i>;

cv::Mat get_frame(camera_fb_t** fb_p)
{
    // If a previous picture has been taken, give the frame-buffer back.
    if (*fb_p != nullptr)
    {
        esp_camera_fb_return(*fb_p);
    }

    // Take the picture.
    *fb_p = esp_camera_fb_get();
    auto fb = *fb_p;
    if (!fb) {
        ESP_LOGE("LOG", "Camera capture failed");
        return cv::Mat();
    }

    // Build the OpenCV matrix.
    // CV_8UC2 is two-channel color, with 8-bit channels.
    auto result = cv::Mat(fb->height, fb->width, CV_8UC2, fb->buf);

    // Flip the bytes of the matrix so that it can be processed using OpenCV functions.
    for (auto row = 0; row < result.rows; row++)
    {
        for (auto col = 0; col < result.cols; col++)
        {
            auto& pixel = result.at<cv::Vec2b>(row, col);
            const auto temp = pixel[0];
            pixel[0] = pixel[1];
            pixel[1] = temp;
        }
    }

    return result;
}

void app_main(void)
{
    ESP_LOGE("LOG", "TEST LOG");
    
    [[maybe_unused]] camera_fb_t* fb = nullptr;

    //cv::Mat working_frame = get_frame(&fb);

    gpio_set_direction(GPIO_NUM_33, GPIO_MODE_OUTPUT);

    bool ON = true;
    while (true) {
        ESP_LOGI("LOG", "TOGGLING LED");
        gpio_set_level(GPIO_NUM_33, ON);

        ON = !ON;
        vTaskDelay(200);
    }

    short data = 0x69;
    uart_write_bytes(UART_NUM_0, &data, 1);
}
