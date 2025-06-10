#include "sgtl5000.h"


#include <driver/i2c_master.h>
#include <driver/i2s_std.h>
#include <freertos/FreeRTOS.h>
#include <esp_err.h>
#include <esp_log.h>

#include "typedefs.h"
#include "sgtl_regs.h"


#define I2S_TIMEOUT              portMAX_DELAY//pdMS_TO_TICKS(10)



static u16 reverse_endianess(u16 input) 
{
    return (input >> 8) | (input << 8);
}

static esp_err_t sgtl_i2c_write(sgtl5000_t* codec, u16 addr, u16 data)
{
    
    u16 reversed_addr = reverse_endianess(addr);
    data = reverse_endianess(data);

    u16 write_buf [2];
    write_buf[0] = reversed_addr;
    write_buf[1] = data;

    esp_err_t err = i2c_master_transmit(codec->dev_handle, (u8*) write_buf, 4, -1);

    if(err != ESP_OK){
        return err;
    }


    return ESP_OK;
}



static esp_err_t sgtl_i2c_read(sgtl5000_t* codec, u16 addr, u16* read_buf)
{
    // u8 raw_data[] = {0, 0};
    u16 reversed_addr = reverse_endianess(addr);

    esp_err_t err = i2c_master_transmit_receive(codec->dev_handle, (u8*) &reversed_addr, 2, (u8*) read_buf, 2, -1);

    if(err != ESP_OK){
        return err;
    }

    *read_buf = reverse_endianess(*read_buf);


    return ESP_OK;
}


static esp_err_t sgtl_i2c_write_verify(sgtl5000_t* codec, u16 addr, u16 data, u16 read_only_mask)
{

    u16 check = 0;
    esp_err_t err = sgtl_i2c_write(codec, addr, data);

    if(err != ESP_OK){
        return err;
    }

    err = sgtl_i2c_read(codec, addr, &check);

    if(err != ESP_OK){
        return err;
    }

    check = (check & read_only_mask);

    if (check != data) {
        ESP_LOGE("SGTL5000", "Attempted to write to register 0x%04X, Expected 0x%04X, got 0x%04X", addr, data, check);
        return ESP_ERR_INVALID_RESPONSE;
    }else {
        return ESP_OK;
    }

}

void configure_sgtl(sgtl5000_t* codec)
{

    //clear bits 12 and 13 of CHIP_ANA_POWER: 0x4260,  because we're driving VDDD externally with 2.0V
    ESP_ERROR_CHECK(sgtl_i2c_write_verify(codec,  CHIP_ANA_POWER,

                                            (CHIP_ANA_POWER_REFTOP_POWERUP_Powerup << CHIP_ANA_POWER_REFTOP_POWERUP_POS),

                                            NO_READ_ONLY));


    // Write CHIP_REF_CTRL 0x01F1,66 to Set ground, ADC, DAC reference voltage, should be set to VDDA/2, so 1.65V
    // also set "bias currents for the analog blocks", use 0x01FD to follow example if having issues
    // Also does slow ramp up for VAG
    ESP_ERROR_CHECK(sgtl_i2c_write_verify(codec,  CHIP_REF_CTRL,

                                            (CHIP_REF_CTRL_VAG_VAL_1_575V << CHIP_REF_CTRL_VAG_VAL_POS)     |
                                            (CHIP_REF_CTRL_BIAS_CTRL_p12_5 << CHIP_REF_CTRL_BIAS_CTRL_POS), 
                                            
                                            NO_READ_ONLY));

    //Write CHIP_LINE_OUT_CTRL 0x0322 to set lineout bias voltage and current
    ESP_ERROR_CHECK(sgtl_i2c_write_verify(codec,  CHIP_LINE_OUT_CTRL, 
                                            (CHIP_LINE_OUT_CTRL_OUT_CURRENT_0_36mA << CHIP_LINE_OUT_CTRL_OUT_CURRENT_POS) | 
                                            (CHIP_LINE_OUT_CTRL_LO_VAGCNTRL_1_650V << CHIP_LINE_OUT_CTRL_LO_VAGCNTRL_POS),

                                            NO_READ_ONLY));

    ESP_ERROR_CHECK(sgtl_i2c_write_verify(codec,  CHIP_ANA_CTRL, 
                                            (CHIP_ANA_CTRL_MUTE_LO_Mute << CHIP_ANA_CTRL_MUTE_LO_POS)           |
                                            (CHIP_ANA_CTRL_MUTE_HP_Mute << CHIP_ANA_CTRL_MUTE_HP_POS)           |
                                            (CHIP_ANA_CTRL_SELECT_ADC_LINEIN << CHIP_ANA_CTRL_SELECT_ADC_POS)   |
                                            (CHIP_ANA_CTRL_EN_ZCD_ADC_Enabled << CHIP_ANA_CTRL_EN_ZCD_ADC_POS)  |
                                            (CHIP_ANA_CTRL_MUTE_ADC_Mute << CHIP_ANA_CTRL_MUTE_ADC_POS), 
                                            
                                            NO_READ_ONLY));

    ESP_ERROR_CHECK(sgtl_i2c_write_verify(codec,  CHIP_DIG_POWER, 

                                            (CHIP_DIG_POWER_ADC_POWERUP_Enable << CHIP_DIG_POWER_ADC_POWERUP_POS) |
                                            (CHIP_DIG_POWER_DAC_POWERUP_Enable << CHIP_DIG_POWER_DAC_POWERUP_POS) |
                                            // (CHIP_DIG_POWER_DAP_POWERUP_Enable << CHIP_DIG_POWER_DAP_POWERUP_POS) |
                                            (CHIP_DIG_POWER_I2S_OUT_POWERUP_Enable << CHIP_DIG_POWER_I2S_OUT_POWERUP_POS) |
                                            (CHIP_DIG_POWER_I2S_IN_POWERUP_Enable << CHIP_DIG_POWER_I2S_IN_POWERUP_POS), 

                                            NO_READ_ONLY));

    ESP_ERROR_CHECK(sgtl_i2c_write_verify(codec,  CHIP_ANA_POWER,

                                            (CHIP_ANA_POWER_DAC_MONO_Stereo << CHIP_ANA_POWER_DAC_MONO_POS) |
                                            (CHIP_ANA_POWER_VAG_POWERUP_Powerup << CHIP_ANA_POWER_VAG_POWERUP_POS) |
                                            (CHIP_ANA_POWER_ADC_MONO_Stereo << CHIP_ANA_POWER_ADC_MONO_POS) |
                                            (CHIP_ANA_POWER_REFTOP_POWERUP_Powerup << CHIP_ANA_POWER_REFTOP_POWERUP_POS) |
                                            (CHIP_ANA_POWER_DAC_POWERUP_Powerup << CHIP_ANA_POWER_DAC_POWERUP_POS) |
                                            (CHIP_ANA_POWER_ADC_POWERUP_Powerup << CHIP_ANA_POWER_ADC_POWERUP_POS) |
                                            (CHIP_ANA_POWER_LINEOUT_POWERUP_Powerup << CHIP_ANA_POWER_LINEOUT_POWERUP_POS),
                                            
                                            NO_READ_ONLY));

    ESP_ERROR_CHECK(sgtl_i2c_write_verify(codec,  CHIP_LINE_OUT_VOL,

                                            (CHIP_LINE_OUT_CTRL_OUT_CURRENT_0_54mA << CHIP_LINE_OUT_CTRL_OUT_CURRENT_POS) |
                                            (CHIP_LINE_OUT_CTRL_LO_VAGCNTRL_1_175V << CHIP_LINE_OUT_CTRL_LO_VAGCNTRL_POS),
                                            
                                            NO_READ_ONLY));




    // I2S config
    ESP_ERROR_CHECK(sgtl_i2c_write_verify(codec,  CHIP_I2S_CTRL, 

                                            (CHIP_I2S_CTRL_SCLKFREQ_32Fs << CHIP_I2S_CTRL_SCLKFREQ_POS) |
                                            (CHIP_I2S_CTRL_DLEN_16bits << CHIP_I2S_CTRL_DLEN_POS),

                                            NO_READ_ONLY));

    // Write CHIP_CLK_CTRL 0x0010 to set sample rate and Mclk frequency - 16kHz and 256*32 kHz = 8.192 MHz respectively
    // Write 0x0013 to set PLL derived
    ESP_ERROR_CHECK(sgtl_i2c_write_verify(codec,  CHIP_CLK_CTRL, 

                                            (CHIP_CLK_CTRL_RATE_MODE_SYS_FS_DIV2 << CHIP_CLK_CTRL_RATE_MODE_POS) |
                                            (CHIP_CLK_CTRL_SYS_FS_32_KHZ << CHIP_CLK_CTRL_SYS_FS_POS) |
                                            (CHIP_CLK_CTRL_MCLK_FREQ_256_FS << CHIP_CLK_CTRL_MCLK_FREQ_POS),

                                            NO_READ_ONLY));

    // Write CHIP_I2S_CTRL 0x0130, to configure i2s format and stuff
    ESP_ERROR_CHECK(sgtl_i2c_write_verify(codec,  CHIP_I2S_CTRL, 

                                            (CHIP_I2S_CTRL_SCLKFREQ_32Fs << CHIP_I2S_CTRL_SCLKFREQ_POS) |
                                            (CHIP_I2S_CTRL_DLEN_16bits << CHIP_I2S_CTRL_DLEN_POS),

                                            NO_READ_ONLY));
    
    // write chip sss ctrl nothing, because 0x0010 is already beautiful on its own. Modify it later for DAP if needed
    ESP_ERROR_CHECK(sgtl_i2c_write_verify(codec,  CHIP_SSS_CTRL,

                                            (CHIP_SSS_CTRL_DAC_SELECT_I2S_IN << CHIP_SSS_CTRL_DAC_SELECT_POS) |
                                            (CHIP_SSS_CTRL_I2S_SELECT_ADC << CHIP_SSS_CTRL_I2S_SELECT_POS),

                                            NO_READ_ONLY));


    // Configuring the ADC and DAC
    
    // Write CHIP_ADCDAC_CTRL to unmute dac
    ESP_ERROR_CHECK(sgtl_i2c_write_verify(codec,  CHIP_ADCDAC_CTRL,

                                            0x0000,

                                            0x030F));       // disable dac mute

    ESP_ERROR_CHECK(sgtl_i2c_write_verify(codec,  CHIP_DAC_VOL,

                                            0x3C3C, 
                                            
                                            NO_READ_ONLY));  // digital gain, 0dB
    
    ESP_ERROR_CHECK(sgtl_i2c_write_verify(codec,  CHIP_ANA_ADC_CTRL,

                                            0x0, 
                                                
                                            NO_READ_ONLY));  // digital gain, 0dB

    // unmute ADC, LINEOUT
    ESP_ERROR_CHECK(sgtl_i2c_write_verify(codec,  CHIP_ANA_CTRL,

                                            (CHIP_ANA_CTRL_MUTE_HP_Mute << CHIP_ANA_CTRL_MUTE_HP_POS) |
                                            (CHIP_ANA_CTRL_SELECT_ADC_LINEIN << CHIP_ANA_CTRL_SELECT_ADC_POS) |
                                            (CHIP_ANA_CTRL_EN_ZCD_ADC_Enabled << CHIP_ANA_CTRL_EN_ZCD_ADC_POS) |
                                            (CHIP_ANA_CTRL_MUTE_ADC_Unmute << CHIP_ANA_CTRL_MUTE_ADC_POS),

                                            NO_READ_ONLY));

}

static void i2c_init(sgtl5000_t *codec, i2c_master_bus_handle_t bus_handle)
{
    codec->bus_handle = bus_handle;

    i2c_device_config_t dev_config = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = SGTL5000_ADDR,
        .scl_speed_hz = I2C_MASTER_FREQ_HZ
    };


    ESP_ERROR_CHECK(i2c_master_bus_add_device(bus_handle, &dev_config, &(codec->dev_handle)));
}

static void i2s_init(sgtl5000_t *codec, i2s_std_gpio_config_t *pins)
{
    codec->i2s_pins = *pins;
    /* Setp 1: Determine the I2S channel configuration and allocate both channels
     * The default configuration can be generated by the helper macro,
     * it only requires the I2S controller id and I2S role */
    i2s_chan_config_t chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_AUTO, I2S_ROLE_MASTER);
    ESP_ERROR_CHECK(i2s_new_channel(&chan_cfg, &codec->tx_chan, &codec->rx_chan));

    // MCLK multiple is 512 to allow the SGTL5000 to turn on and be properly configured.
    i2s_std_config_t sgtl_cfg = {
        .clk_cfg  = { .sample_rate_hz = 16000, .clk_src = I2S_CLK_SRC_DEFAULT, .mclk_multiple = I2S_MCLK_MULTIPLE_512 },
        .slot_cfg = I2S_STD_PHILIPS_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_MONO),
        .gpio_cfg = codec->i2s_pins
    };

    /* Initialize the channels */
    ESP_ERROR_CHECK(i2s_channel_init_std_mode(codec->tx_chan, &sgtl_cfg));
    ESP_ERROR_CHECK(i2s_channel_init_std_mode(codec->rx_chan, &sgtl_cfg));

    ESP_ERROR_CHECK(i2s_channel_enable(codec->tx_chan));
    ESP_ERROR_CHECK(i2s_channel_enable(codec->rx_chan));
}


/**
 * @brief i2c master initialization
 */
esp_err_t sgtl5000_init(sgtl5000_t *codec, i2c_master_bus_handle_t bus_handle, i2s_std_gpio_config_t* pins)
{
    i2s_init(codec, pins);

    ESP_LOGI("SGTL Startup", "MCLK Signal Started");

    i2c_init(codec, bus_handle);

    // u16 data = 0;
    // esp_err_t err = sgtl_i2c_read(codec, 0x0, &data);
    // if(err != ESP_OK || data != 0xA011){
    //     ESP_LOGE("SGTL Startup", "Encountered Error, I2C test read failed: expected 0x0A11, Got 0x%04X. Error Name: %s", data, esp_err_to_name(err));
    //     return ESP_ERR_INVALID_STATE;
    // }

    ESP_LOGI("SGTL Startup", "I2C Successfully Started");

    // configure_sgtl(codec);

    ESP_LOGI("SGTL Startup", "SGTL Successfully Configured");

    codec->initialized = true;

    return ESP_OK;
}


esp_err_t sgtl5000_play_audio(sgtl5000_t *codec, i16 *audio_data, size_t data_size_bytes)
{
    if (!codec || !codec->tx_chan || !audio_data) {
        return ESP_ERR_INVALID_ARG;
    }

    size_t bytes_written = 0;
    // size_t total_written = 0;
    void *data_ptr = (void *) audio_data;

    // while (total_written < data_size) {
    //     esp_err_t ret = i2s_channel_write(codec->tx_chan, data_ptr + total_written,
    //                                       data_size - total_written, &bytes_written, I2S_TIMEOUT);
    //     if (ret != ESP_OK) {
    //         return ret;
    //     }

    //     total_written += bytes_written;
    // }

    esp_err_t ret = i2s_channel_write(codec->tx_chan, data_ptr,
                                              data_size_bytes, &bytes_written, I2S_TIMEOUT);

    return ret;
}

esp_err_t sgtl5000_record_audio(sgtl5000_t *codec, i16 *ret_audio_data, size_t data_size_bytes)
{
    if (!codec || !codec->rx_chan || !ret_audio_data) {
        return ESP_ERR_INVALID_ARG;
    }

    size_t bytes_written = 0;
    // size_t total_written = 0;
    void *data_ptr = (void *) ret_audio_data;

    // while (total_written < data_size) {
    //     esp_err_t ret = i2s_channel_write(codec->tx_chan, data_ptr + total_written,
    //                                       data_size - total_written, &bytes_written, I2S_TIMEOUT);
    //     if (ret != ESP_OK) {
    //         return ret;
    //     }

    //     total_written += bytes_written;
    // }

    esp_err_t ret = i2s_channel_read(codec->rx_chan, data_ptr,
                                              data_size_bytes, &bytes_written, I2S_TIMEOUT);

    if(bytes_written != data_size_bytes){
        return ESP_ERR_NOT_FINISHED;
    }

    return ret;
}