#include "pipico.hpp"
#include "communication_types.hpp"
#include "driver/uart.h"
#include "hal/uart_types.h"

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
    
    uart_param_config(UART_NUM_0, &uart_config);
    uart_driver_install(UART_NUM_0, 1024 * 2, 0, 0, NULL, 0);
}


void PiPico::sendPacket(const uint8_t dist, const bool stopDetected)
{
    // // Send a synchronization byte
    // uart_write_bytes(UART_NUM_0, &SYNC_BYTE, sizeof(SYNC_BYTE));
    
    EspToPicoPacket sendPacket;
    sendPacket.whiteDist = dist;
    sendPacket.stopDetected = stopDetected;
    sendPacket.imageIncluded = false;

    // printf(reinterpret_cast<const char*>(&sendPacket));

    uart_write_bytes(UART_NUM_0, reinterpret_cast<const char*>(&sendPacket), sizeof(sendPacket));
}

void PiPico::sendPacket(const uint8_t dist, const bool stopDetected, const cv::Mat &image)
{
    // // Send a synchronization byte
    // uart_write_bytes(UART_NUM_0, &SYNC_BYTE, sizeof(SYNC_BYTE));
    
    EspToPicoPacket sendPacket;
    sendPacket.whiteDist = dist;
    sendPacket.stopDetected = stopDetected;
    sendPacket.imageIncluded = true;

    uart_write_bytes(UART_NUM_0, reinterpret_cast<const char*>(&sendPacket), sizeof(sendPacket));
    
    for (int y = 0; y < image.rows; y++) {
        for (int x = 0; x < image.cols; x++) {
            cv::Vec2b vecpixel = image.at<cv::Vec2b>(y, x);
            uint16_t pixel = (static_cast<uint16_t>(vecpixel[0]) << 8) | vecpixel[1];

            uart_write_bytes(UART_NUM_0, &pixel, sizeof(pixel));
        }
    }

    // uart_write_bytes(UART_NUM_0, reinterpret_cast<const char*>(&sendPacket), sizeof(sendPacket));
}

void PiPico::sendAck(const bool ack, const std::string& label)
{
    AckPacket ackPacket;
    ackPacket.ack = ack;
    strncpy(ackPacket.label, label.c_str(), sizeof(ackPacket.label) - 1);
    ackPacket.label[sizeof(ackPacket.label) - 1] = '\0'; // Ensure null-termination

    uart_write_bytes(UART_NUM_0, reinterpret_cast<const char*>(&ackPacket), sizeof(ackPacket));
}


PicoToEspPacket PiPico::receivePacket()
{
    PicoToEspPacket recvPacket;
    int length = uart_read_bytes(UART_NUM_0, reinterpret_cast<uint8_t*>(&recvPacket), sizeof(recvPacket), 20 / portTICK_PERIOD_MS);
    if (length == sizeof(recvPacket)) {
        return recvPacket;
    } else {
        // Handle error or return an empty/default packet
        return PicoToEspPacket();
    }
}