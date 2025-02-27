#include "pipico.hpp"

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