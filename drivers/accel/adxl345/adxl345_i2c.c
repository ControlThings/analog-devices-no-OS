#include <stdio.h>
#include <stdbool.h>

#include "esp_log.h"
#include "driver/gpio.h"
#include "driver/i2c.h"
#include "adxl345_i2c.h"
#include "driver/i2c.h"

#define TAG "adxl345_i2c"

static const gpio_num_t SDA = GPIO_NUM_13;
static const gpio_num_t SCL = GPIO_NUM_16;

static const i2c_port_t i2c_port = I2C_NUM_0;

static const int adxl345_addr = 0x53;
static const int timeout = 1000;

#define I2C_MASTER_ACK_EN   true    /*!< Enable ack check for master */
#define I2C_MASTER_ACK_DIS  false   /*!< Disable ack check for master */


static bool cmd_valid = false;
i2c_cmd_handle_t cmd;

int i2c_write(i2c_desc *desc, uint8_t *buffer, int buffer_len, int stop_condition_ctrl) {
    cmd = i2c_cmd_link_create();
    cmd_valid = true;
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (adxl345_addr << 1) | I2C_MASTER_WRITE, I2C_MASTER_ACK_EN);
    i2c_master_write(cmd, buffer, buffer_len, I2C_MASTER_ACK_EN);
    
    if (stop_condition_ctrl) {
        i2c_master_stop(cmd);
        esp_err_t err = i2c_master_cmd_begin(i2c_port, cmd, pdMS_TO_TICKS(timeout));
        i2c_cmd_link_delete(cmd);
        cmd_valid = false;

        if (err != ESP_OK) {
            ESP_LOGE(TAG, "i2c_write: i2c_master_cmd_begin error ");
        }
    }

    return 0;
}

int i2c_read(i2c_desc *desc, uint8_t *buffer, int buffer_len, int stop_condition_ctrl) {
    if (!cmd_valid) {
        ESP_LOGE(TAG, "cmd context is not valid!");
    }
    else {
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, (adxl345_addr << 1) | I2C_MASTER_READ, I2C_MASTER_ACK_EN);
        i2c_master_read(cmd, buffer, buffer_len, I2C_MASTER_LAST_NACK);
        i2c_master_stop(cmd);
        esp_err_t err = i2c_master_cmd_begin(i2c_port, cmd, pdMS_TO_TICKS(timeout));
        i2c_cmd_link_delete(cmd);
        cmd_valid = false;
        if (err != ESP_OK) {
            ESP_LOGE(TAG, "i2c_read: i2c_master_cmd_begin error");
        }
    }
    return 0;
}

int i2c_init(i2c_desc** target_dest, i2c_init_param* init_parm) {
    esp_err_t ret;
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = SDA;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = SCL;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = 4*100000; //Clock speed in hz
    esp_err_t err = i2c_param_config(i2c_port, &conf);
    if (!err) err = i2c_driver_install(i2c_port, conf.mode, 0, 0, 0);

    ret = err;
    if (ret == ESP_OK) {
        ESP_LOGI(TAG, "I2C init OK");
        return 0;
    }
    else {
        ESP_LOGE(TAG, "I2C init not OK");
        return -1;
    }
}

int i2c_remove(i2c_desc *desc) {
    esp_err_t ret = i2c_driver_delete(i2c_port);
    if (ret == ESP_OK) {
        return 0;
    }
    else {
        return -1;
    }
}
