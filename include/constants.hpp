#pragma once

#include <cstdint>

// The number of times I have watched this program recompile over 1000 files has nearly driven me insane.
// ~ Caleb Willson, 9/2024

#define CAM_PIN_PWDN    32
#define CAM_PIN_RESET   -1
#define CAM_PIN_XCLK    0
#define CAM_PIN_SIOD    26
#define CAM_PIN_SIOC    27

#define CAM_PIN_D7      35
#define CAM_PIN_D6      34
#define CAM_PIN_D5      39
#define CAM_PIN_D4      36
#define CAM_PIN_D3      21
#define CAM_PIN_D2      19
#define CAM_PIN_D1      18
#define CAM_PIN_D0      5
#define CAM_PIN_VSYNC   25
#define CAM_PIN_HREF    23
#define CAM_PIN_PCLK    22


// The pin to write to.
#define TX_GPIO GPIO_NUM_1
#define UART_NUM UART_NUM_0
constexpr uint32_t UART_TX_BAUD = 250000;

constexpr uint8_t IMG_ROWS = 96;
constexpr uint8_t IMG_COLS = 96;
constexpr uint8_t IMG_BPP = 2;
constexpr uint16_t IMG_SIZE = IMG_ROWS * IMG_COLS * IMG_BPP;