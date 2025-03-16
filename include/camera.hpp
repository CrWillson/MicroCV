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

    esp_err_t get_frame(cv::Mat& image);

}