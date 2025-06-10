#include <stdio.h>
#include <sdkconfig.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>
#include <driver/i2c_master.h>
#include <driver/ledc.h>
#include <driver/i2s_std.h>

#include <nvs.h>
#include <nvs_flash.h>
#include <esp_system.h>
#include <esp_bt.h>
#include <esp_bt_main.h>
#include <esp_bt_device.h>
#include <esp_gap_bt_api.h>
#include <esp_hf_ag_api.h>
#include <esp_console.h>

#include <app_hf_msg_set.h>
#include <bt_app_core.h>
#include <bt_app_hf.h>

#include "typedefs.h"
#include "sgtl5000.h"
#include "sa868.h"

// #include "funky.h"

#define I2S_SDOUT                       GPIO_NUM_25                 /*!< GPIO number used for I2S Serial Data Output*/
#define I2S_SDIN                        GPIO_NUM_26                 /*!< GPIO number used for I2S Serial Data Input */
#define I2S_BCLK                        GPIO_NUM_27                 /*!< GPIO number used for I2S Serial Clock      */
#define I2S_LRCLK                       GPIO_NUM_14                 /*!< GPIO number used for I2S Left/Right Clock  */
#define I2S_MCLK                        GPIO_NUM_0                  /*!< GPIO number used for I2S Master Clock      */

#define I2C_MASTER_SCL_IO               GPIO_NUM_32                 /*!< GPIO number used for I2C master clock      */
#define I2C_MASTER_SDA_IO               GPIO_NUM_33                 /*!< GPIO number used for I2C master data       */
#define I2C_MASTER_NUM                  I2C_NUM_0                   /*!< I2C port number for master dev             */



sgtl5000_t codec;

sa868_t radio;

#define BUFF_SIZE           2048


#define TABLE_SIZE         100
// #define TABLE_SIZE         FUNKY_SAMPLE_NO
#define TABLE_SIZE_BYTE    200
// // Produce a sine audio
// static const i16 sine_int16[TABLE_SIZE] = {
//      0,    2057,    4107,    6140,    8149,   10126,   12062,   13952,   15786,   17557,
//  19260,   20886,   22431,   23886,   25247,   26509,   27666,   28714,   29648,   30466,
//  31163,   31738,   32187,   32509,   32702,   32767,   32702,   32509,   32187,   31738,
//  31163,   30466,   29648,   28714,   27666,   26509,   25247,   23886,   22431,   20886,
//  19260,   17557,   15786,   13952,   12062,   10126,    8149,    6140,    4107,    2057,
//      0,   -2057,   -4107,   -6140,   -8149,  -10126,  -12062,  -13952,  -15786,  -17557,
// -19260,  -20886,  -22431,  -23886,  -25247,  -26509,  -27666,  -28714,  -29648,  -30466,
// -31163,  -31738,  -32187,  -32509,  -32702,  -32767,  -32702,  -32509,  -32187,  -31738,
// -31163,  -30466,  -29648,  -28714,  -27666,  -26509,  -25247,  -23886,  -22431,  -20886,
// -19260,  -17557,  -15786,  -13952,  -12062,  -10126,   -8149,   -6140,   -4107,   -2057,
// };

// static const i16 sine_int16[TABLE_SIZE] = {
//     0,    0,    0,    0,    0,   0,   0,   0,   0,   0,
//     0,    0,    0,    0,    0,   0,   0,   0,   0,   0,
//     0,    0,    0,    0,    0,   0,   0,   0,   0,   0,
//     0,    0,    0,    0,    0,   0,   0,   0,   0,   0,
//     0,    0,    0,    0,    0,   0,   0,   0,   0,   0,
//     30000,   30000,   30000,  30000,   30000,  30000,  30000,  30000,  30000,  30000,
//     30000,   30000,   30000,  30000,   30000,  30000,  30000,  30000,  30000,  30000,
//     30000,   30000,   30000,  30000,   30000,  30000,  30000,  30000,  30000,  30000,
//     30000,   30000,   30000,  30000,   30000,  30000,  30000,  30000,  30000,  30000,
//     30000,   30000,   30000,  30000,   30000,  30000,  30000,  30000,  30000,  30000,
// };


static const i16 sine_int16[TABLE_SIZE] = {
    0,    0,    0,    0,    0,   0,   0,   0,   0,   0,
    0,    0,    0,    0,    0,   0,   0,   0,   0,   0,
    0,    0,    0,    0,    0,   0,   0,   0,   0,   0,
    0,    0,    0,    0,    0,   0,   0,   0,   0,   0,
    0,    0,    0,    0,    0,   0,   0,   0,   0,   0,
    0,    0,    0,    0,    0,   0,   0,   0,   0,   0,
    0,    0,    0,    0,    0,   0,   0,   0,   0,   0,
    0,    0,    0,    0,    0,   0,   0,   0,   0,   0,
    0,    0,    0,    0,    0,   0,   0,   0,   0,   0,
    0,    0,    0,    0,    0,   0,   0,   0,   0,   0,
};

TaskHandle_t i2s_tx;

void tx_task(){
    static uint32_t index = 0;
    uint8_t *w_buf = (uint8_t *)calloc(1, BUFF_SIZE);
    assert(w_buf); // Check if w_buf allocation success

    uint8_t *data = (uint8_t *)sine_int16;

    // uint8_t *data = (uint8_t *)funky;

    // for (uint32_t i = 0; i < BUFF_SIZE; i++) {
    //     if (index >= FUNKY_SAMPLE_NO*2) {
    //         index = 0;
    //     }

    //     w_buf[i] = data[index];
    //     index++;
    // }
    // size_t w_bytes = BUFF_SIZE;


    while (1) {
        for (uint32_t i = 0; i < BUFF_SIZE; i++) {
            if (index >= TABLE_SIZE*2) {
                index = 0;
            }
            
            w_buf[i] = data[index];
            index++;
        }
        size_t w_bytes = BUFF_SIZE;
        /* Write i2s data */
        esp_err_t err = sgtl5000_play_audio(&codec, (i16*) w_buf, BUFF_SIZE);
        if (err == ESP_OK) {
            printf("Write Task: i2s write %d bytes\n", w_bytes);
        } else {
            // printf("Write Task: i2s write failed\n");
            ESP_LOGE("I2S TESTING", "I2S Write Failed, reason: %s", esp_err_to_name(err));
        }
        vTaskDelay(pdMS_TO_TICKS(4));
    }
    free(w_buf);
    vTaskDelete(NULL);
}

void rx_task(){
    static uint32_t index = 0;
    uint8_t *r_buf = (uint8_t *)calloc(1, BUFF_SIZE);
    assert(r_buf);

    while(1){
        esp_err_t err = sgtl5000_record_audio(&codec, (i16*) r_buf, BUFF_SIZE);
        if(err == ESP_OK){
            sgtl5000_play_audio(&codec, (i16*) r_buf, BUFF_SIZE);
        }
    }

}






#define BT_HF_AG_TAG    "HF_AG_DEMO_MAIN"

static const char local_device_name[] = "Suck My Balls";

/* event for handler "hf_ag_hdl_stack_up */
enum {
    BT_APP_EVT_STACK_UP = 0,
};

static char *bda2str(esp_bd_addr_t bda, char *str, size_t size)
{
    if (bda == NULL || str == NULL || size < 18) {
        return NULL;
    }

    uint8_t *p = bda;
    sprintf(str, "%02x:%02x:%02x:%02x:%02x:%02x",
            p[0], p[1], p[2], p[3], p[4], p[5]);
    return str;
}

/* handler for bluetooth stack enabled events */
static void bt_hf_hdl_stack_evt(uint16_t event, void *p_param)
{
    ESP_LOGD(BT_HF_TAG, "%s evt %d", __func__, event);
    switch (event)
    {
        case BT_APP_EVT_STACK_UP:
        {
            esp_bt_gap_set_device_name(local_device_name);

            esp_hf_ag_register_callback(bt_app_hf_cb);

            // init and register for HFP_AG functions
            esp_hf_ag_init();

            /*
            * Set default parameters for Legacy Pairing
            * Use variable pin, input pin code when pairing
            */
            esp_bt_pin_type_t pin_type = ESP_BT_PIN_TYPE_VARIABLE;
            esp_bt_pin_code_t pin_code;
            pin_code[0] = '0';
            pin_code[1] = '0';
            pin_code[2] = '0';
            pin_code[3] = '0';
            esp_bt_gap_set_pin(pin_type, 4, pin_code);

            /* set discoverable and connectable mode, wait to be connected */
            esp_bt_gap_set_scan_mode(ESP_BT_CONNECTABLE, ESP_BT_GENERAL_DISCOVERABLE);
            break;
        }
        default:
            ESP_LOGE(BT_HF_TAG, "%s unhandled evt %d", __func__, event);
            break;
    }
}

void bt_init(char* bda_str){
    /* Initialize NVS — it is used to store PHY calibration data */
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_BLE));

    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    if ((ret = esp_bt_controller_init(&bt_cfg)) != ESP_OK) {
        ESP_LOGE(BT_HF_TAG, "%s initialize controller failed: %s", __func__, esp_err_to_name(ret));
        return;
    }
    if ((ret = esp_bt_controller_enable(ESP_BT_MODE_CLASSIC_BT)) != ESP_OK) {
        ESP_LOGE(BT_HF_TAG, "%s enable controller failed: %s", __func__, esp_err_to_name(ret));
        return;
    }
    esp_bluedroid_config_t bluedroid_cfg = BT_BLUEDROID_INIT_CONFIG_DEFAULT();
    if ((ret = esp_bluedroid_init_with_cfg(&bluedroid_cfg)) != ESP_OK) {
        ESP_LOGE(BT_HF_TAG, "%s initialize bluedroid failed: %s", __func__, esp_err_to_name(ret));
        return;
    }
    if ((ret = esp_bluedroid_enable()) != ESP_OK) {
        ESP_LOGE(BT_HF_TAG, "%s enable bluedroid failed: %s", __func__, esp_err_to_name(ret));
        return;
    }

    ESP_LOGI(BT_HF_TAG, "Own address:[%s]", bda2str((uint8_t *)esp_bt_dev_get_address(), bda_str, sizeof(bda_str)));
    /* create application task */
    bt_app_task_start_up();

    /* Bluetooth device name, connection mode and profile set up */
    bt_app_work_dispatch(bt_hf_hdl_stack_evt, BT_APP_EVT_STACK_UP, NULL, 0, NULL);

    esp_console_repl_t *repl = NULL;
    esp_console_repl_config_t repl_config = ESP_CONSOLE_REPL_CONFIG_DEFAULT();
    esp_console_dev_uart_config_t uart_config = ESP_CONSOLE_DEV_UART_CONFIG_DEFAULT();
    repl_config.prompt = "hfp_ag>";

    // init console REPL environment
    ESP_ERROR_CHECK(esp_console_new_repl_uart(&uart_config, &repl_config, &repl));

    /* Register commands */
    register_hfp_ag();
    printf("\n ==================================================\n");
    printf(" |       Steps to test hfp_ag                     |\n");
    printf(" |                                                |\n");
    printf(" |  1. Print 'help' to gain overview of commands  |\n");
    printf(" |  2. Setup a service level connection           |\n");
    printf(" |  3. Run hfp_ag to test                         |\n");
    printf(" |                                                |\n");
    printf(" =================================================\n\n");

    // start console REPL
    ESP_ERROR_CHECK(esp_console_start_repl(repl));
}

// static void print_bytes(u16 input) {
//     u8* input_bytes = (u8*) &input;
//     ESP_LOGI("Endianess test", "First Byte of raw: 0x%02X \tSecond Byte: 0x%02X", input_bytes[0], input_bytes[1]);

//     u16 reversed = reverse_endianess(input);
//     u8* reversed_bytes = (u8*) &reversed;
//     ESP_LOGI("Endianess test", "First Byte of reversed: 0x%02X \tSecond Byte: 0x%02X", reversed_bytes[0], reversed_bytes[1]);
// }


i2c_master_bus_handle_t bus_handle;




void app_main(void)
{

    i2c_master_bus_config_t bus_config = {
        .i2c_port = I2C_MASTER_NUM,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = false
    };
    ESP_ERROR_CHECK(i2c_new_master_bus(&bus_config, &bus_handle));

    i2s_std_gpio_config_t i2s_pins = 
    {
        .mclk = I2S_MCLK,
        .bclk = I2S_BCLK,
        .ws = I2S_LRCLK,
        .dout = I2S_SDOUT,
        .din = I2S_SDIN,
        .invert_flags = {
            .bclk_inv = false,
            .mclk_inv = false,
            .ws_inv = false
        }
    };

    ESP_ERROR_CHECK(sgtl5000_init(&codec, bus_handle, &i2s_pins));

    char bda_str[18] = {0};

    bt_init(bda_str);

    
    // xTaskCreate(tx_task, "serial", 4096, NULL, 3, &i2s_tx);

    while(true){
        vTaskDelay(portMAX_DELAY);
    }
}
