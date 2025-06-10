#pragma once

#include <driver/uart.h>
#include <driver/gpio.h>
#include <esp_err.h>

#include "typedefs.h"

typedef struct {
    uart_port_t uart_num;
    gpio_num_t tx_pin;
    gpio_num_t rx_pin;
    gpio_num_t ptt_pin;
    gpio_num_t power_on_pin;
    gpio_num_t audio_status_pin;
    bool initialized;
} sa868_t;

//FOR TESTING ONLY, COMMENT OUT ONCE I FIGURE THIS OUT
esp_err_t sa868_send_command(uart_port_t uart_num, const char* command_buf, char* response_buf, u32 response_len);

esp_err_t sa868_init(sa868_t *device);


esp_err_t sa868_send_handshake(sa868_t *device);

esp_err_t sa868_set_frequency(sa868_t *device, u32 tx_freq_khz, u32 rx_freq_khz);
esp_err_t sa868_set_volume(sa868_t *device, u8 volume_level);

esp_err_t sa868_transmit_enable(sa868_t *device);
esp_err_t sa868_transmit_disable(sa868_t *device);

esp_err_t sa868_power_on(sa868_t *device);
esp_err_t sa868_power_off(sa868_t *device);

bool sa868_audio_status(sa868_t *device);