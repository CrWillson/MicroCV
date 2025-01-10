#pragma once

// SD Card Imports
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
#include "driver/sdmmc_host.h"
#include "driver/sdspi_host.h"
#include "ff.h"
#include <dirent.h>

#include "constants.hpp"

namespace SDCard {

    static constexpr char *TAG = "SDCARD";

    esp_err_t mount_sd_card();

}