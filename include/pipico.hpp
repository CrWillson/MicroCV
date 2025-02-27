#pragma once

#include <cstdint>
#include "esp_timer.h"
#include "driver/gpio.h"
#include "driver/uart.h"
#include "params.h"
#include "constants.hpp"

class PiPico {
public:
    static PiPico& getInstance() {
        static PiPico instance;
        return instance;
    }

    struct SendPacket
    {
        int8_t whiteDist;
        bool stopDetected;
        uint16_t image[96][96];
    };
    
    struct RecvPacket
    {
        char command[6];
        char label[6];
        int16_t data;
    };

    struct AckPacket
    {
        bool ack;
        char label[6];
    };


    void sendPacket(const uint8_t dist, const bool stopDetected, const uint16_t image[96][96]);
    RecvPacket receivePacket();

    void init();

private:
    PiPico() = default;
    ~PiPico() = default;

    PiPico(const PiPico&) = delete;
    PiPico& operator=(const PiPico&) = delete;

};