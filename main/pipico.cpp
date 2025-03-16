#include "pipico.hpp"
#include "communication_types.hpp"
#include "driver/uart.h"
#include "freertos/idf_additions.h"
#include "hal/uart_types.h"
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
    
    uart_param_config(UART_NUM_0, &uart_config);
    uart_driver_install(UART_NUM_0, 1024 * 2, 0, 0, NULL, 0);
}


void PiPico::sendPacket(const uint8_t dist, const bool stopDetected)
{
    // Send a synchronization byte
    uart_write_bytes(UART_NUM_0, reinterpret_cast<const char*>(&SYNC_BYTES), sizeof(SYNC_BYTES));
    
    EspToPicoPacket sendPacket;
    sendPacket.whiteDist = dist;
    sendPacket.stopDetected = stopDetected;
    sendPacket.imageIncluded = false;

    // printf(reinterpret_cast<const char*>(&sendPacket));

    uart_write_bytes(UART_NUM_0, reinterpret_cast<const char*>(&sendPacket), sizeof(sendPacket));
}

void PiPico::sendPacket(const uint8_t dist, const bool stopDetected, const cv::Mat &image)
{
    // Send a synchronization byte
    uart_write_bytes(UART_NUM_0, reinterpret_cast<const char*>(&SYNC_BYTES), sizeof(SYNC_BYTES));
    
    EspToPicoPacket sendPacket;
    sendPacket.whiteDist = dist;
    sendPacket.stopDetected = stopDetected;
    sendPacket.imageIncluded = true;

    uart_write_bytes(UART_NUM_0, reinterpret_cast<const char*>(&sendPacket), sizeof(sendPacket));
    
    // uart_write_bytes(UART_NUM_0, image.data, IMG_SIZE);
    // for (int row = 0; row < IMG_ROWS; ++row) {
    //     for (int col = 0; col < IMG_COLS; ++col) {
    //         // For each pixel in the image, access both channels (CV_8UC2)
    //         uint8_t byte1 = image.at<cv::Vec2b>(row, col)[0];  // First channel
    //         uint8_t byte2 = image.at<cv::Vec2b>(row, col)[1];  // Second channel

    //         // Send each byte individually
    //         uart_write_bytes(UART_NUM_0, (const char*)&byte1, 1);  // Send first byte
    //         uart_write_bytes(UART_NUM_0, (const char*)&byte2, 1);  // Send second byte
    //     }
    // }

    print_matrix_string(image);
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

void PiPico::print_matrix_string(const cv::Mat& mat) 
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

void PiPico::print_matrix(const cv::Mat& mat) 
{
    for (int i = 0; i < mat.rows; ++i) {
        for (int j = 0; j < mat.cols; ++j) {
            cv::Vec2b vecpixel = mat.at<cv::Vec2b>(i, j);

            uint8_t highByte = static_cast<uint8_t>(vecpixel[0]);
            uint8_t lowByte = static_cast<uint8_t>(vecpixel[1]);

            uart_write_bytes(UART_NUM_0, &highByte, sizeof(highByte));
            uart_write_bytes(UART_NUM_0, &lowByte, sizeof(lowByte));

            vTaskDelay(1);
        }
        vTaskDelay(1);
    }

}