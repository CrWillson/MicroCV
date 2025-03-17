#pragma once

#include <cstdint>
#include <string>
#include "sdkconfig.h"
#include "driver/gpio.h"
#include "driver/uart.h"

// FreeRTOS imports
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/portmacro.h"

#include "params.h"
#include "constants.hpp"
#include "opencv2.hpp"
#include "communication_types.hpp"

class PiPico {
public:
    static PiPico& getInstance() {
        static PiPico instance;
        return instance;
    }


    void init();

    void sendPacket(const uint8_t dist, const bool stopDetected);
    void sendPacket(const uint8_t dist, const bool stopDetected, const cv::Mat &image);

    void sendAck(const bool ack, const std::string& label);
    esp_err_t receivePacket();

    bool photoRequested = false;

private:
    PiPico() = default;
    ~PiPico() = default;

    PiPico(const PiPico&) = delete;
    PiPico& operator=(const PiPico&) = delete;

    void process_command(const PicoToEspPacket& packet);

    void print_matrix(const cv::Mat& mat);
};