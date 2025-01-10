#include "sdcard.hpp"

esp_err_t SDCard::mount_sd_card()
{
    // SD card configuration
    sdmmc_host_t host = SDMMC_HOST_DEFAULT();
    sdmmc_slot_config_t slot_config = SDMMC_SLOT_CONFIG_DEFAULT();
    
    // GPIO configuration specific to ESP32-CAM
    slot_config.width = 1;           // 1-bit mode for ESP32-CAM
    gpio_set_pull_mode(GPIO_NUM_15, GPIO_PULLUP_ONLY);   // CMD
    gpio_set_pull_mode(GPIO_NUM_2, GPIO_PULLUP_ONLY);    // D0
    gpio_set_pull_mode(GPIO_NUM_4, GPIO_PULLUP_ONLY);    // D1
    gpio_set_pull_mode(GPIO_NUM_12, GPIO_PULLUP_ONLY);   // D2
    gpio_set_pull_mode(GPIO_NUM_13, GPIO_PULLUP_ONLY);   // D3

    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = false,
        .max_files = 5,
        .allocation_unit_size = 16 * 1024
    };

    sdmmc_card_t *card;
    esp_err_t ret = esp_vfs_fat_sdmmc_mount(MOUNT_POINT, &host, &slot_config, &mount_config, &card);

    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(SDCard::TAG, "Failed to mount filesystem. "
                     "If you want the card to be formatted, set format_if_mount_failed = true.");
        } else {
            ESP_LOGE(SDCard::TAG, "Failed to initialize the card (%s). "
                     "Make sure SD card lines have pull-up resistors in place.", esp_err_to_name(ret));
        }
        return ret;
    }

    // Card has been initialized, print its properties
    sdmmc_card_print_info(stdout, card);
    ESP_LOGI(SDCard::TAG, "SD card mounted successfully.");
    return ESP_OK;
}


esp_err_t SDCard::load_params_from_file()
{
    std::ifstream file(PARAMS_FILE);
    if (!file.is_open()) {
        ESP_LOGE(TAG, "Failed to open parameters file: %s", PARAMS_FILE);
        return ESP_FAIL;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string key, value;

        if (std::getline(ss, key, '=') && std::getline(ss, value)) {
            try {
                int intValue = std::stoi(value);

            } catch (const std::exception &e) {
                ESP_LOGE(TAG, "Invalid value for key %s: %s", key.c_str(), value.c_str());
            }
        }
    }
}
