#pragma once

// SD Card Imports
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
#include "driver/sdmmc_host.h"
#include "driver/sdspi_host.h"
#include "ff.h"
#include <dirent.h>

#include <fstream>
#include <sstream>

#include "constants.hpp"
#include "params.hpp"

namespace SDCard {

    static constexpr char* MOUNT_POINT  = "/sdcard";
    static constexpr char* PARAMS_FILE  = "/sdcard/PARAMS.TXT";
    static constexpr char* TAG          = "SDCARD";

    esp_err_t mount_sd_card();

    esp_err_t load_params_from_file();


}