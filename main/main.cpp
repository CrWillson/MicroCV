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

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "ssd1306.h"

#include <stdio.h>
#include <vector>

#include "camera.hpp"


// This is necessary because it allows ESP-IDF to find the main function,
// even though C++ mangles the function name.
extern "C" {
void app_main(void);
}

using contour_t = std::vector<cv::Point2i>;

void app_main(void)
{
    ESP_LOGI("INIT", "Initializing Camera");
    ESPCamera::config_cam();
    
    camera_fb_t* fb = nullptr;

    ESP_LOGI("INIT", "Getting test frame");
    cv::Mat working_frame = ESPCamera::get_frame(&fb);

    gpio_set_direction(GPIO_NUM_33, GPIO_MODE_OUTPUT);

    bool ON = true;
    while (true) {
        ESP_LOGI("LOG", "TOGGLING LED");
        gpio_set_level(GPIO_NUM_33, ON);

        ON = !ON;
        vTaskDelay(200);
    }
}
