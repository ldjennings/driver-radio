#include "sa868.h"

#include <string.h>
#include <freertos/FreeRTOS.h>
#include <esp_log.h>

#define SA868_UART_BUF_SIZE         256

#define UART_TIMEOUT              pdMS_TO_TICKS(10) // Timeout in ms

static QueueHandle_t uart0_queue;

const char* SA868_TAG = "SA868";


esp_err_t sa868_send_command(uart_port_t uart_num, const char* command_buf, char* response_buf, u32 response_len)
{
    int len = strlen(command_buf);
    // Write the command to the UART
    int bytes_written = uart_write_bytes(uart_num, (const char*)command_buf, len);
    if (bytes_written != len) {
        ESP_LOGE("SA868", "Failed to send full command over UART (sent %d bytes, expected %d)", bytes_written, len);
        return ESP_FAIL;
    }


    memset(response_buf, 0, response_len);
    uart_event_t event;
    if (xQueueReceive(uart0_queue, (void *)&event, pdMS_TO_TICKS(200))) {
        if(event.type == UART_PATTERN_DET){
            int bytes_read = uart_read_bytes(uart_num, response_buf, response_len - 1, 0);
            if (bytes_read <= 0) {
                ESP_LOGE(SA868_TAG, "No response from SA868");
                return ESP_ERR_TIMEOUT;
            }
            response_buf[bytes_read] = '\0';
            return ESP_OK;
        } else {
            ESP_LOGW(SA868_TAG, "Unexpected UART event: %d", event.type);
            return ESP_FAIL;
        }
    } else {
        ESP_LOGW(SA868_TAG, "Timed out waiting for response");
        return ESP_ERR_TIMEOUT;
    }


    return ESP_OK;
}

void configure_gpios(sa868_t *device)
{
    gpio_set_direction(device->ptt_pin, GPIO_MODE_OUTPUT);
    gpio_set_direction(device->power_on_pin, GPIO_MODE_OUTPUT);

    gpio_set_direction(device->audio_status_pin, GPIO_MODE_INPUT);

}


esp_err_t sa868_init(sa868_t *device)
{
    const uart_config_t uart_config = {
        .baud_rate = 9600,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };

    ESP_ERROR_CHECK(uart_param_config(device->uart_num, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(device->uart_num, device->tx_pin, device->rx_pin, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
    
    ESP_ERROR_CHECK(uart_driver_install(device->uart_num, SA868_UART_BUF_SIZE, SA868_UART_BUF_SIZE, 5, &uart0_queue, 0));
    ESP_ERROR_CHECK(uart_enable_pattern_det_baud_intr(device->uart_num, '\n', 1, 9, 0, 0));

    configure_gpios(device);

    device->initialized = true;
    return ESP_OK;
}

esp_err_t sa868_send_handshake(sa868_t *device){
    if(!device->initialized) {
        return ESP_ERR_INVALID_STATE;
    }

    // char cmd[64];
    char response[32];

    const char* cmd = "AT+DMOCONNECT\r\n";

    esp_err_t err = sa868_send_command(device->uart_num, cmd, response, 64);

    if(err == ESP_OK){
        if(strstr(cmd, "+DMOCONNECT:0")){
            return ESP_OK;
        } else {
            ESP_LOGE(SA868_TAG, "Abnormal Working Status: %s", response);
            return ESP_ERR_INVALID_STATE;
        }
    }

    return err;

}

esp_err_t sa868_set_frequency(sa868_t *device, u32 tx_freq_khz, u32 rx_freq_khz)
{
    if(!device->initialized) {
        return ESP_ERR_INVALID_STATE;
    }

    char cmd[64];
    char response[64];
    // Don't forget to add the zeros after the Frequencies! the decimals need to go to four places
    snprintf(cmd, sizeof(cmd), "AT+DMOSETGROUP=1,%lu.%lu0,%lu.%lu0,0000,0,0000\r\n",
             tx_freq_khz / 1000, tx_freq_khz % 1000, rx_freq_khz / 1000, rx_freq_khz % 1000); // SA868 uses kHz units

    // ESP_LOGI(SA868_TAG, "Attempted command: %s", cmd);

    esp_err_t err = sa868_send_command(device->uart_num, cmd, response, 64);

    if(err == ESP_OK){
        if(strstr(response, "+DMOSETGROUP:0")){
            // ESP_LOGI(SA868_TAG, "Response gotten: %s", response);
            return ESP_OK;
        } else {
            ESP_LOGE(SA868_TAG, "Frequency Setting out of Range: TX: %lu.%lu, RX: %lu.%lu. String: %s, Response: %s", 
                                tx_freq_khz / 1000,tx_freq_khz % 1000, rx_freq_khz / 1000, rx_freq_khz % 1000, esp_err_to_name(err), response);
            return ESP_ERR_INVALID_ARG;
        }
    }

    return err;
}

esp_err_t sa868_set_volume(sa868_t *device, u8 volume_level)
{
    if(!device->initialized) {
        return ESP_ERR_INVALID_STATE;
    }

    if (volume_level > 8) volume_level = 8;
    // if (volume_level < 0) volume_level = 0; No need to check this because it's unsigned

    char cmd[32];
    char response[32];
    snprintf(cmd, sizeof(cmd), "AT+DMOSETVOLUME=%d\r\n", volume_level);
    esp_err_t err = sa868_send_command(device->uart_num, cmd, response, 32);

    if(err == ESP_OK){
        if(strstr(response, "+DMOSETVOLUME:0")){
            // ESP_LOGI(SA868_TAG, "Response gotten: %s", response);
            return ESP_OK;
        } else {
            ESP_LOGE(SA868_TAG, "Volume Setup Failed: Attempted Value: %d", volume_level);
            return ESP_FAIL;
        }
    }

    return err;
}

esp_err_t sa868_transmit_start(sa868_t *device)
{
    if(!device->initialized) {
        return ESP_ERR_INVALID_STATE;
    }

    return gpio_set_level(device->ptt_pin, 0); // active LOW
}

esp_err_t sa868_transmit_stop(sa868_t *device)
{
    if(!device->initialized) {
        return ESP_ERR_INVALID_STATE;
    }

    return gpio_set_level(device->ptt_pin, 1);
}


esp_err_t sa868_power_on(sa868_t *device)
{
    if(!device->initialized) {
        return ESP_ERR_INVALID_STATE;
    }

    return gpio_set_level(device->power_on_pin, 1); // active LOW
}

esp_err_t sa868_power_off(sa868_t *device)
{
    if(!device->initialized) {
        return ESP_ERR_INVALID_STATE;
    }

    return gpio_set_level(device->power_on_pin, 0);
}

//Returns whether or not it's outputting audio signals.
bool sa868_audio_status(sa868_t *device)
{
    if(!device->initialized) {
        return ESP_ERR_INVALID_STATE;
    }

    return !gpio_get_level(device->audio_status_pin); // active LOW
}


