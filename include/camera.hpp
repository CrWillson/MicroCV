#pragma once

#include "opencv2.hpp"

#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/portmacro.h"
#include "esp_camera.h"
#include "esp_log.h"
#include "constants.hpp"

namespace ESPCamera {
    
    constexpr const char *TAG = "CAMERA";

    /// @brief Configures the ESP-32-CAM.
    void config_cam();

    cv::Mat get_frame(camera_fb_t** fb_p);

    esp_err_t get_frame(cv::Mat& image);

    namespace debug {
        void print_matrix(const cv::Mat& mat);
    }
}