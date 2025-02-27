#include "camera.hpp"

/// @brief Configures the ESP-32-CAM.
void ESPCamera::config_cam()
{   
    ESP_LOGI(ESPCamera::TAG, "Beginning camera configuration");
 
    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = CAM_PIN_D0;
    config.pin_d1 = CAM_PIN_D1;
    config.pin_d2 = CAM_PIN_D2;
    config.pin_d3 = CAM_PIN_D3;
    config.pin_d4 = CAM_PIN_D4;
    config.pin_d5 = CAM_PIN_D5;
    config.pin_d6 = CAM_PIN_D6;
    config.pin_d7 = CAM_PIN_D7;
    config.pin_xclk = CAM_PIN_XCLK;
    config.pin_pclk = CAM_PIN_PCLK;
    config.pin_vsync = CAM_PIN_VSYNC;
    config.pin_href = CAM_PIN_HREF;
    config.pin_sccb_sda = CAM_PIN_SIOD;
    config.pin_sccb_scl = CAM_PIN_SIOC;
    config.pin_pwdn = CAM_PIN_PWDN;
    config.pin_reset = CAM_PIN_RESET;
    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_RGB565;  // Set the pixel format

    // Set frame size to 96x96
    config.frame_size = FRAMESIZE_96X96;
    config.jpeg_quality = 12;  // JPEG quality (lower is better)
    config.fb_count = 1;  // Only one frame buffer

    // Initialize the camera
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        ESP_LOGE(ESPCamera::TAG, "Camera init failed with error 0x%x", err);
        return;
    }

    ESP_LOGI(ESPCamera::TAG, "Camera initialized successfully with resolution 96x96");
}


esp_err_t ESPCamera::get_frame(cv::Mat& image)
{
    auto* fb = esp_camera_fb_get();
    if (!fb) {
        ESP_LOGE(ESPCamera::TAG, "Camera capture failed");
        return ESP_FAIL;
    }

    // Ensure the format is RGB565
    if (fb->format != PIXFORMAT_RGB565) {
        ESP_LOGE(TAG, "Unsupported format. Expected RGB565.");
        esp_camera_fb_return(fb);
        return ESP_FAIL;
    }

    int width = fb->width;
    int height = fb->height;

    image.create(height, width, CV_8UC2);
    memcpy(image.data, fb->buf, fb->len);

    esp_camera_fb_return(fb);
    return ESP_OK;
}

#ifdef DEBUG_MODE
void ESPCamera::debug::print_matrix(const cv::Mat& mat) 
{
    printf("START IMAGE\n");
    for (int i = 0; i < mat.rows; ++i) {
        for (int j = 0; j < mat.cols; ++j) {
            cv::Vec2b vecpixel = mat.at<cv::Vec2b>(i, j);
            uint16_t pixel = (static_cast<uint16_t>(vecpixel[0]) << 8) | vecpixel[1];
            printf("%d ", pixel);
        }
        printf("\n");
    }
    printf("END IMAGE\n");
}
#endif