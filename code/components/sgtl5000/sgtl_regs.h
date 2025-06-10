#pragma once


//Taken from https://github.com/NordicPlayground/nRF52-teensy-sgtl5000-audio/blob/master/drv_sgtl5000.h

/* SGTL5000 Register definitions */
#define CHIP_ID            0x0000
#define CHIP_DIG_POWER     0x0002
#define CHIP_CLK_CTRL      0x0004
#define CHIP_I2S_CTRL      0x0006
#define CHIP_SSS_CTRL      0x000A
#define CHIP_ADCDAC_CTRL   0x000E
#define CHIP_DAC_VOL       0x0010
#define CHIP_ANA_ADC_CTRL  0x0020
#define CHIP_ANA_HP_CTRL   0x0022
#define CHIP_ANA_CTRL      0x0024
#define CHIP_LINREG_CTRL   0x0026
#define CHIP_REF_CTRL      0x0028
#define CHIP_MIC_CTRL      0x002A
#define CHIP_LINE_OUT_CTRL 0x002C
#define CHIP_LINE_OUT_VOL  0x002E
#define CHIP_ANA_POWER     0x0030
#define CHIP_PLL_CTRL      0x0032
#define CHIP_TOP_CTRL      0x0034
#define CHIP_SHORT_CTRL    0x003C
#define DAP_CONTROL        0x0100
#define DAP_PEQ            0x0102
#define DAP_AUDIO_EQ       0x0108
#define DAP_AVC_CTRL       0x0124
#define DAP_AVC_THRESHOLD  0x0126
#define DAP_AVC_ATTACK     0x0128
#define DAP_AVC_DECAY      0x012A

/* SGTL5000 Bitfields definitions in use for this example */
#define CHIP_ANA_POWER_DAC_MONO_POS                         14  // While DAC_POWERUP is set, allows DAC to be mono for power saving
#define CHIP_ANA_POWER_PLL_POWERUP_POS                      10
#define CHIP_ANA_POWER_VCOAMP_POWERUP                       8
#define CHIP_ANA_POWER_VAG_POWERUP_POS                      7   // Power up VAG reference buffer
#define CHIP_ANA_POWER_ADC_MONO_POS                         6
#define CHIP_ANA_POWER_REFTOP_POWERUP_POS                   5
#define CHIP_ANA_POWER_HEADPHONE_POWERUP_POS                4
#define CHIP_ANA_POWER_DAC_POWERUP_POS                      3
#define CHIP_ANA_POWER_CAPLESS_HEADPHONE_POWERUP_POS        2
#define CHIP_ANA_POWER_ADC_POWERUP_POS                      1
#define CHIP_ANA_POWER_LINEOUT_POWERUP_POS                  0
#define CHIP_ANA_POWER_DAC_MONO_Mono                        0x0  // (Left only)
#define CHIP_ANA_POWER_DAC_MONO_Stereo                      0x1
#define CHIP_ANA_POWER_PLL_POWERUP_Powerdown                0x0
#define CHIP_ANA_POWER_PLL_POWERUP_Powerup                  0x1
#define CHIP_ANA_POWER_VCOAMP_POWERUP_Powerdown             0x0
#define CHIP_ANA_POWER_VCOAMP_POWERUP_Powerup               0x1
#define CHIP_ANA_POWER_VAG_POWERUP_Powerdown                0x0
#define CHIP_ANA_POWER_VAG_POWERUP_Powerup                  0x1
#define CHIP_ANA_POWER_ADC_MONO_Mono                        0x0  // (Left only)
#define CHIP_ANA_POWER_ADC_MONO_Stereo                      0x1
#define CHIP_ANA_POWER_REFTOP_POWERUP_Powerdown             0x0
#define CHIP_ANA_POWER_REFTOP_POWERUP_Powerup               0x1
#define CHIP_ANA_POWER_HEADPHONE_POWERUP_Powerdown          0x0
#define CHIP_ANA_POWER_HEADPHONE_POWERUP_Powerup            0x1
#define CHIP_ANA_POWER_DAC_POWERUP_Powerdown                0x0
#define CHIP_ANA_POWER_DAC_POWERUP_Powerup                  0x1
#define CHIP_ANA_POWER_CAPLESS_HEADPHONE_POWERUP_Powerdown  0x0
#define CHIP_ANA_POWER_CAPLESS_HEADPHONE_POWERUP_Powerup    0x1
#define CHIP_ANA_POWER_ADC_POWERUP_Powerdown                0x0
#define CHIP_ANA_POWER_ADC_POWERUP_Powerup                  0x1
#define CHIP_ANA_POWER_LINEOUT_POWERUP_Powerdown            0x0
#define CHIP_ANA_POWER_LINEOUT_POWERUP_Powerup              0x1



#define CHIP_TOP_CTRL_INPUT_FREQ_DIV2_POS           3       // Passthrough MCLK PLL
#define CHIP_TOP_CTRL_ENABLE_INT_OSC_POS            11      // Internal osc used for zero cross detectors.
#define CHIP_TOP_CTRL_INPUT_FREQ_DIV2_Passthrough   0x0
#define CHIP_TOP_CTRL_INPUT_FREQ_DIV2_SYS_MCLKdiv2  0x1     // Only used when input clock is above 17MHz



#define CHIP_CLK_CTRL_RATE_MODE_POS             4
#define CHIP_CLK_CTRL_RATE_MODE_SYS_FS          0x0
#define CHIP_CLK_CTRL_RATE_MODE_SYS_FS_DIV2     0x1
#define CHIP_CLK_CTRL_RATE_MODE_SYS_FS_DIV4     0x2
#define CHIP_CLK_CTRL_RATE_MODE_SYS_FS_DIV6     0x3

#define CHIP_CLK_CTRL_SYS_FS_POS                2
#define CHIP_CLK_CTRL_SYS_FS_32_KHZ             0x0
#define CHIP_CLK_CTRL_SYS_FS_44_1_KHZ           0x1
#define CHIP_CLK_CTRL_SYS_FS_48_KHZ             0x2
#define CHIP_CLK_CTRL_SYS_FS_96_KHZ             0x3

#define CHIP_CLK_CTRL_MCLK_FREQ_POS             0
#define CHIP_CLK_CTRL_MCLK_FREQ_256_FS          0x0
#define CHIP_CLK_CTRL_MCLK_FREQ_384_FS          0x1
#define CHIP_CLK_CTRL_MCLK_FREQ_512_FS          0x2
#define CHIP_CLK_CTRL_MCLK_FREQ_USE_PLL         0x3




#define CHIP_REF_CTRL_VAG_VAL_POS           4
#define CHIP_REF_CTRL_VAG_VAL_1_575V        0x1F

#define CHIP_REF_CTRL_BIAS_CTRL_POS         1
#define CHIP_REF_CTRL_BIAS_CTRL_Nominal     0x0
#define CHIP_REF_CTRL_BIAS_CTRL_p12_5       0x1

#define CHIP_REF_CTRL_SMALL_POP_POS         0
#define CHIP_REF_CTRL_SMALL_POP_Normal      0x0
#define CHIP_REF_CTRL_SMALL_POP_Slowdown    0x1



#define CHIP_LINE_OUT_CTRL_OUT_CURRENT_POS      8
#define CHIP_LINE_OUT_CTRL_OUT_CURRENT_0_54mA   0xF
#define CHIP_LINE_OUT_CTRL_OUT_CURRENT_0_36mA   0x3

#define CHIP_LINE_OUT_CTRL_LO_VAGCNTRL_POS      0
#define CHIP_LINE_OUT_CTRL_LO_VAGCNTRL_1_650V   0x22
#define CHIP_LINE_OUT_CTRL_LO_VAGCNTRL_1_175V   0xF



#define CHIP_SHORT_CTRL_LVLADJR_POS                     12
#define CHIP_SHORT_CTRL_LVLADJL_POS                     8
#define CHIP_SHORT_CTRL_LVLADJC_POS                     4
#define CHIP_SHORT_CTRL_MODE_LR_POS                     2
#define CHIP_SHORT_CTRL_MODE_CM_POS                     0
#define CHIP_SHORT_CTRL_LVLADJR_125mA                   0x4
#define CHIP_SHORT_CTRL_LVLADJL_125mA                   0x4
#define CHIP_SHORT_CTRL_LVLADJC_250mA                   0x4
#define CHIP_SHORT_CTRL_MODE_LR_ShortDetectResetLatch   0x1
#define CHIP_SHORT_CTRL_MODE_CM_ShortDetectAutoReset    0x2

#define CHIP_ANA_ADC_CTRL_ADC_VOL_M6DB_POS                  8
#define CHIP_ANA_ADC_CTRL_ADC_VOL_RIGHT_POS                 4
#define CHIP_ANA_ADC_CTRL_ADC_VOL_LEFT_POS                  0
#define CHIP_ANA_ADC_CTRL_ADC_VOL_M6DB_NoChange             0x0
#define CHIP_ANA_ADC_CTRL_ADC_VOL_M6DB_ADCrangereduce6dB    0x1
#define CHIP_ANA_ADC_CTRL_ADC_VOL_RIGHT_0dB                 0x0
#define CHIP_ANA_ADC_CTRL_ADC_VOL_RIGHT_p22_5dB             0xF
#define CHIP_ANA_ADC_CTRL_ADC_VOL_LEFT_0dB                  0x0
#define CHIP_ANA_ADC_CTRL_ADC_VOL_LEFT_p22_5dB              0xF

#define CHIP_ANA_HP_CTRL_HP_VOL_LEFT_POS        0       // Left headphone channel volume. 0.5dB steps. 0x18 is 0dB, maks +12, min -51.5.
#define CHIP_ANA_HP_CTRL_HP_VOL_RIGHT_POS       8       // Right headphone channel volume. 0.5dB steps. 0x18 is 0dB, maks +12, min -51.5.
#define CHIP_ANA_HP_CTRL_HP_VOL_LEFT_neg51_5dB  0x7F
#define CHIP_ANA_HP_CTRL_HP_VOL_LEFT_neg20dB    0x40
#define CHIP_ANA_HP_CTRL_HP_VOL_LEFT_0dB        0x18
#define CHIP_ANA_HP_CTRL_HP_VOL_LEFT_p12dB      0x00
#define CHIP_ANA_HP_CTRL_HP_VOL_RIGHT_neg20dB   0x40


#define CHIP_ANA_CTRL_MUTE_LO_POS           8   // LINEOUT Mute
#define CHIP_ANA_CTRL_SELECT_HP_POS         6   // Select headphone input
#define CHIP_ANA_CTRL_EN_ZCD_HP_POS         5   // Enable headphone zero cross detector
#define CHIP_ANA_CTRL_MUTE_HP_POS           4   // Mute headphone outputs
#define CHIP_ANA_CTRL_SELECT_ADC_POS        2   // Select ADC input
#define CHIP_ANA_CTRL_EN_ZCD_ADC_POS        1   // Enable ADC analog zero cross detector
#define CHIP_ANA_CTRL_MUTE_ADC_POS          0   // Mute ADC analog volume
#define CHIP_ANA_CTRL_MUTE_LO_Unmute        0x0
#define CHIP_ANA_CTRL_MUTE_LO_Mute          0x1
#define CHIP_ANA_CTRL_SELECT_HP_DAC         0x0
#define CHIP_ANA_CTRL_SELECT_HP_LINEIN      0x1
#define CHIP_ANA_CTRL_EN_ZCD_HP_Disabled    0x0
#define CHIP_ANA_CTRL_EN_ZCD_HP_Enabled     0x1
#define CHIP_ANA_CTRL_MUTE_HP_Unmute        0x0
#define CHIP_ANA_CTRL_MUTE_HP_Mute          0x1
#define CHIP_ANA_CTRL_SELECT_ADC_Microphone 0x0
#define CHIP_ANA_CTRL_SELECT_ADC_LINEIN     0x1
#define CHIP_ANA_CTRL_EN_ZCD_ADC_Disabled   0x0
#define CHIP_ANA_CTRL_EN_ZCD_ADC_Enabled    0x1
#define CHIP_ANA_CTRL_MUTE_ADC_Unmute       0x0
#define CHIP_ANA_CTRL_MUTE_ADC_Mute         0x1

#define CHIP_DIG_POWER_ADC_POWERUP_POS          6
#define CHIP_DIG_POWER_DAC_POWERUP_POS          5
#define CHIP_DIG_POWER_DAP_POWERUP_POS          4
#define CHIP_DIG_POWER_I2S_OUT_POWERUP_POS      1
#define CHIP_DIG_POWER_I2S_IN_POWERUP_POS       0
#define CHIP_DIG_POWER_ADC_POWERUP_Disable      0x0
#define CHIP_DIG_POWER_ADC_POWERUP_Enable       0x1
#define CHIP_DIG_POWER_DAC_POWERUP_Disable      0x0
#define CHIP_DIG_POWER_DAC_POWERUP_Enable       0x1
#define CHIP_DIG_POWER_DAP_POWERUP_Disable      0x0
#define CHIP_DIG_POWER_DAP_POWERUP_Enable       0x1
#define CHIP_DIG_POWER_I2S_OUT_POWERUP_Disable  0x0
#define CHIP_DIG_POWER_I2S_OUT_POWERUP_Enable   0x1
#define CHIP_DIG_POWER_I2S_IN_POWERUP_Disable   0x0
#define CHIP_DIG_POWER_I2S_IN_POWERUP_Enable    0x1

#define CHIP_I2S_CTRL_SCLKFREQ_POS      8
#define CHIP_I2S_CTRL_DLEN_POS          4
#define CHIP_I2S_CTRL_SCLKFREQ_32Fs     0x0
#define CHIP_I2S_CTRL_DLEN_16bits       0x3

#define CHIP_SSS_CTRL_DAP_SELECT_POS    6   // Select data source for DAP
#define CHIP_SSS_CTRL_DAC_SELECT_POS    4   // Select data source for DAC
#define CHIP_SSS_CTRL_I2S_SELECT_POS    0   // Select data source for I2S_DOUT
#define CHIP_SSS_CTRL_DAP_SELECT_ADC    0x0
#define CHIP_SSS_CTRL_DAP_SELECT_I2S_IN 0x1
#define CHIP_SSS_CTRL_DAC_SELECT_ADC    0x0
#define CHIP_SSS_CTRL_DAC_SELECT_I2S_IN 0x1
#define CHIP_SSS_CTRL_DAC_SELECT_DAP    0x3
#define CHIP_SSS_CTRL_I2S_SELECT_ADC    0x0
#define CHIP_SSS_CTRL_I2S_SELECT_I2S_IN 0x1
#define CHIP_SSS_CTRL_I2S_SELECT_DAP    0x3

#define CHIP_ADCDAC_CTRL_ADC_HPF_BYPASS_POS         0
#define CHIP_ADCDAC_CTRL_ADC_HPF_FREEZE_POS         1
#define CHIP_ADCDAC_CTRL_DAC_MUTE_LEFT_POS          2
#define CHIP_ADCDAC_CTRL_DAC_MUTE_RIGHT_POS         3
#define CHIP_ADCDAC_CTRL_VOL_EXPO_RAMP_POS          8
#define CHIP_ADCDAC_CTRL_VOL_RAM_EN_POS             9
#define CHIP_ADCDAC_CTRL_VOL_BUSY_DAC_LEFT_POS      12
#define CHIP_ADCDAC_CTRL_VOL_BUSY_DAC_RIGHT_POS     13
#define CHIP_ADCDAC_CTRL_ADC_HPF_BYPASS_Normal      0x0
#define CHIP_ADCDAC_CTRL_ADC_HPF_BYPASS_Bypassed    0x1  // Bypassed and offset not updated
#define CHIP_ADCDAC_CTRL_ADC_HPF_FREEZE_Normal      0x0
#define CHIP_ADCDAC_CTRL_ADC_HPF_FREEZE_Freeze      0x1  // Freeze the ADC high-pass filter offset register. Offset continues to be subtracted from the ADC data stream


#define CHIP_DAP_CONTROL_DAP_EN_POS     0   // Enable/Disable digital audio processing (DAP)
#define CHIP_DAP_CONTROL_DAP_EN_Disable 0x0 // No audio passes through
#define CHIP_DAP_CONTROL_DAP_EN_Enable  0x1 // audio can pass through even if none of DAP functions are enabled

#define DAP_PEQ_EN_POS                  0
#define DAP_PEQ_EN_Disabled             0x0
#define DAP_PEQ_EN_2Filters             0x2
#define DAP_PEQ_EN_7Filters             0x7

#define DAP_AUDIO_EQ_EN_POS             0
#define DAP_AUDIO_EQ_EN_Disabled        0x0
#define DAP_AUDIO_EQ_EN_EnabledPEQ      0x1

#define DAP_AVC_CTRL_EN_POS             0
#define DAP_AVC_CTRL_EN_Disable         0x0
#define DAP_AVC_CTRL_EN_Enable          0x1

#define DAP_AVC_THRESHOLD_THRESH_POS        0
#define DAP_AVC_THRESHOLD_THRESH_neg12dB    0x1473
#define DAP_AVC_THRESHOLD_THRESH_neg18dB    0x0A40

#define DAP_AVC_ATTACK_RATE_POS         0
#define DAP_AVC_ATTACK_RATE_16dB_s      0x0014  // Example value from datasheet, might be incorrect

#define DAP_AVC_DECAY_RATE_POS          0
#define DAP_AVC_DECAY_RATE_2dB_s        0x0028  // Example value from datasheet, might be incorrect


#define CHIP_MIC_CTRL_BIAS_RESISTOR_POS 8   // MIC Bias output impedance adjustment - zero means powered off
#define CHIP_MIC_CTRL_BIAS_RESISTOR_Off 0x0
#define CHIP_MIC_CTRL_BIAS_RESISTOR_2k  0x1
#define CHIP_MIC_CTRL_BIAS_RESISTOR_4k  0x2
#define CHIP_MIC_CTRL_BIAS_RESISTOR_8k  0x3
#define CHIP_MIC_CTRL_BIAS_VOLT_POS     4   // MIC Bias Voltage Adjustment
#define CHIP_MIC_CTRL_BIAS_VOLT_1_25v   0x0
#define CHIP_MIC_CTRL_BIAS_VOLT_1_50v   0x1
#define CHIP_MIC_CTRL_BIAS_VOLT_1_75v   0x2
#define CHIP_MIC_CTRL_BIAS_VOLT_2_00v   0x3
#define CHIP_MIC_CTRL_BIAS_VOLT_2_25v   0x4
#define CHIP_MIC_CTRL_BIAS_VOLT_2_50v   0x5
#define CHIP_MIC_CTRL_BIAS_VOLT_3_00v   0x7
#define CHIP_MIC_CTRL_GAIN_POS          0   // MIC Amplifier Gain
#define CHIP_MIC_CTRL_GAIN_0dB          0x0
#define CHIP_MIC_CTRL_GAIN_p20dB        0x1
#define CHIP_MIC_CTRL_GAIN_p30dB        0x2
#define CHIP_MIC_CTRL_GAIN_p40dB        0x3