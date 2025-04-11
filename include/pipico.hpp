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

#include "params.hpp"
#include "constants.hpp"
#include "opencv2.hpp"
#include "communication_types.hpp"

/**
 * @brief Class to manage the interaction with the connected Pi Pico
 * 
 */
class PiPico {
public:
    /**
     * @brief Get the Meyer's singleton instance of the PiPico
     * 
     * @return PiPico& 
     */
    static PiPico& getInstance() {
        static PiPico instance;
        return instance;
    }

    /**
     * @brief Initialize the pico
     * 
     */
    void init();

    /**
     * @brief Send a regular data packet to the Pi Pico
     * 
     * @param dist - The distance to the white line
     * @param stopDetected - Was a stop detected
     */
    void sendPacket(const uint8_t dist, const bool stopDetected);

    /**
     * @brief Send a data packet with an image to the Pi Pico
     * 
     * @param dist - the distance to the white line
     * @param stopDetected - Was a stop detected
     * @param image - The image to send
     */
    void sendPacket(const uint8_t dist, const bool stopDetected, const cv::Mat &image);

    /**
     * @brief Load in a packet from the Pi Pico if one is available.
     * @details If not enough data is available in the UART buffer for a full packet, this function will simply do nothing.
     * 
     * @return esp_err_t - ESP_OK if a packet was read in
     */
    esp_err_t receivePacket();

    /// @brief Is the Pi Pico waiting for a photo to be sent to it
    bool photoRequested = false;

private:
    PiPico() = default;
    ~PiPico() = default;

    PiPico(const PiPico&) = delete;
    PiPico& operator=(const PiPico&) = delete;

    /**
     * @brief Process the incoming command from the Pi Pico
     * 
     * @param packet 
     */
    void process_command(const PicoToEspPacket& packet);

    /**
     * @brief Debug function used for printing an entire OpenCV matrix to the Pi Pico
     * 
     * @param mat - The matrix to print
     */
    void print_matrix(const cv::Mat& mat);
};