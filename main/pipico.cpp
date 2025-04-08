#include "pipico.hpp"
#include "communication_types.hpp"
#include "driver/uart.h"
#include "freertos/idf_additions.h"
#include "hal/uart_types.h"
#include "params.hpp"
#include <cstdio>
#include <string>

void PiPico::init()
{
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wmissing-field-initializers"
    uart_config_t uart_config = {
        .baud_rate = UART_TX_BAUD,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    #pragma GCC diagnostic pop
    
    uart_param_config(UART_NUM, &uart_config);
    uart_driver_install(UART_NUM, 1024 * 2, 0, 0, NULL, 0);
}


void PiPico::sendPacket(const uint8_t dist, const bool stopDetected)
{
    // Send a synchronization byte
    uart_write_bytes(UART_NUM, reinterpret_cast<const char*>(&SYNC_BYTES), sizeof(SYNC_BYTES));
    
    EspToPicoPacket sendPacket;
    sendPacket.whiteDist = dist;
    sendPacket.stopDetected = stopDetected;
    sendPacket.imageIncluded = false;

    uart_write_bytes(UART_NUM, reinterpret_cast<const char*>(&sendPacket), sizeof(sendPacket));
}

void PiPico::sendPacket(const uint8_t dist, const bool stopDetected, const cv::Mat &image)
{
    // Send a synchronization byte
    uart_write_bytes(UART_NUM, reinterpret_cast<const char*>(&SYNC_BYTES), sizeof(SYNC_BYTES));
    
    EspToPicoPacket sendPacket;
    sendPacket.whiteDist = dist;
    sendPacket.stopDetected = stopDetected;
    sendPacket.imageIncluded = true;

    uart_write_bytes(UART_NUM, reinterpret_cast<const char*>(&sendPacket), sizeof(sendPacket));
    print_matrix(image);

    // Mark the request as complete
    photoRequested = false;
}

esp_err_t PiPico::receivePacket()
{
    // check if there are enough buffered bytes for the sync bytes + packet
    constexpr uint8_t expectedBytes = sizeof(SYNC_BYTES) + sizeof(PicoToEspPacket);
    size_t buffSize;

    uart_get_buffered_data_len(UART_NUM, &buffSize);
    if (buffSize < expectedBytes) {
        return ESP_FAIL;
    }

    // Check for the sync bytes
    bool syncFound = false;
    uint32_t syncBuffer = 0;
    while (!syncFound) {
        uint8_t byte;
        uart_read_bytes(UART_NUM, &byte, 1, 20 / portTICK_PERIOD_MS);
        syncBuffer = (syncBuffer >> 8) | (byte << 24);
        if (syncBuffer == SYNC_BYTES) {
            syncFound = true;
        }
        vTaskDelay(1);
    }

    // Read the rest of the packet
    PicoToEspPacket recvPacket;
    int length = uart_read_bytes(UART_NUM, reinterpret_cast<uint8_t*>(&recvPacket), sizeof(recvPacket), 20 / portTICK_PERIOD_MS);
    if (length == sizeof(recvPacket)) {
        process_command(recvPacket);
        uart_flush(UART_NUM);
        return ESP_OK;
    } else {
        // Handle error or return an empty/default packet
        uart_flush(UART_NUM);
        return ESP_FAIL;
    }
    
}

void PiPico::process_command(const PicoToEspPacket &packet)
{
    switch (packet.command) {
        case CMD_REQUEST_IMAGE:
            photoRequested = true;
            break;
        case CMD_SET_PARAM:
            // Set the correct parameter
            if (packet.label == PARAM_WHITE_RED_LIMIT) {
                Params::WHITE_RED_THRESH = static_cast<uint8_t>(packet.data);
            } else if (packet.label == PARAM_WHITE_GREEN_LIMIT) {
                Params::WHITE_GREEN_THRESH = static_cast<uint8_t>(packet.data);
            } else if (packet.label == PARAM_WHITE_BLUE_LIMIT) {
                Params::WHITE_BLUE_THRESH = static_cast<uint8_t>(packet.data);
            } else if (packet.label == PARAM_PERCENT_TO_STOP) {
                Params::PERCENT_TO_STOP = static_cast<uint8_t>(packet.data);
            } else if (packet.label == PARAM_STOP_GREEN_TOLERANCE) {
                Params::STOP_GREEN_TOLERANCE = static_cast<uint8_t>(packet.data);
            } else if (packet.label == PARAM_STOP_BLUE_TOLERANCE) {
                Params::STOP_BLUE_TOLERANCE = static_cast<uint8_t>(packet.data);
            } else if (packet.label == PARAM_STOPBOX_TL_X) {
                Params::STOPBOX_TL_X = static_cast<uint8_t>(packet.data);
            } else if (packet.label == PARAM_STOPBOX_TL_Y) {
                Params::STOPBOX_TL_Y = static_cast<uint8_t>(packet.data);
            } else if (packet.label == PARAM_STOPBOX_BR_X) {
                Params::STOPBOX_BR_X = static_cast<uint8_t>(packet.data);
            } else if (packet.label == PARAM_STOPBOX_BR_Y) {
                Params::STOPBOX_BR_Y = static_cast<uint8_t>(packet.data);
            } else if (packet.label == PARAM_WHITE_CENTER_POS) {
                Params::WHITE_CENTER_POS = static_cast<uint8_t>(packet.data);
            } else {
                // Handle unknown parameter
            }
            break;
        default:
            break;
    }
}

void PiPico::print_matrix(const cv::Mat &mat)
{
    for (int i = 0; i < mat.rows; ++i) {
        std::string currLine;
        for (int j = 0; j < mat.cols; ++j) {
            cv::Vec2b vecpixel = mat.at<cv::Vec2b>(i, j);
            uint16_t pixel = (static_cast<uint16_t>(vecpixel[1]) << 8) | vecpixel[0];
            // convert the pixel to a string in hex
            char buffer[5];
            sprintf(buffer, "%04X", pixel); // 4 hex digits
            auto strpixel = std::string(buffer);
            currLine += strpixel;
            vTaskDelay(1);
        }
        currLine += "\n";
        printf("%s", currLine.c_str());
        vTaskDelay(1);
    }
}