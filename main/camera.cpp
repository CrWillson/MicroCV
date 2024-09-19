#include "camera.hpp"

/// @brief Configures the ESP-32-CAM.
void ESPCamera::config_cam()
{    
    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sccb_sda = SIOD_GPIO_NUM;
    config.pin_sccb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_RGB565; 
    config.frame_size = FRAMESIZE_96X96;
    config.jpeg_quality = 12;
    config.fb_count = 1;

    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        ESP_LOGE("TAG", "Camera initialization failed");
        // Handle error
    }
}


cv::Mat ESPCamera::get_frame(camera_fb_t** fb_p)
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
        ESP_LOGE("TAG", "Camera capture failed");
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