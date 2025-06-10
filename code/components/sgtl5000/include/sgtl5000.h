#pragma once


#include <driver/i2c_master.h>
#include <driver/i2s_std.h>
#include "typedefs.h"

typedef struct {
    i2s_std_gpio_config_t       i2s_pins;                   /*!< Pins used for the codec                    */
    i2s_chan_handle_t           tx_chan;                    /*!< I2S tx Channel Handler                     */
    i2s_chan_handle_t           rx_chan;                    /*!< I2S rx Channel Handler                     */
    i2c_master_bus_handle_t     bus_handle;                 /*!< I2C Bus Identifier                         */
    i2c_master_dev_handle_t     dev_handle;                 /*!< I2C SGTL5000 Identifier                    */
    
    bool initialized;
} sgtl5000_t;







#define I2C_MASTER_FREQ_HZ              400000                      /*!< I2C master clock frequency - 400 kHz       */
#define I2C_MASTER_TX_BUF_DISABLE       0                           /*!< I2C master doesn't need buffer             */
#define I2C_MASTER_RX_BUF_DISABLE       0                           /*!< I2C master doesn't need buffer             */
#define I2C_MASTER_TIMEOUT_MS           1000

#define SGTL5000_ADDR                   0b0001010                   /*!< Address of the SGTL5000 Codec: 0x0A        */

#define NO_READ_ONLY                    0xFFFF                      /*!< Mask used for testing registers            */




// static u16 reverse_endianess(u16 input);

// esp_err_t sgtl_i2c_write(sgtl5000_t* codec, u16 addr, u16 data);

// esp_err_t sgtl_i2c_read(sgtl5000_t* codec, u16 addr, u16* read_buf);

// esp_err_t sgtl_i2c_write_verify(sgtl5000_t* codec, u16 addr, u16 data, u16 read_only_mask);

void configure_sgtl(sgtl5000_t* codec);

// void i2c_init(sgtl5000_t *codec, i2c_master_bus_handle_t bus_handle);

// void i2s_init(sgtl5000_t *codec, i2s_std_gpio_config_t* pins);

esp_err_t sgtl5000_init(sgtl5000_t *codec, i2c_master_bus_handle_t bus_handle, i2s_std_gpio_config_t* pins);

esp_err_t sgtl5000_play_audio(sgtl5000_t *codec, i16 *audio_data, size_t data_size_bytes);

esp_err_t sgtl5000_record_audio(sgtl5000_t *codec, i16 *ret_audio_data, size_t data_size_bytes);

