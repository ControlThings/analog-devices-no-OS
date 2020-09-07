#ifndef ADXL345_I2C_H
#define ADXL345_I2C_H

#include <stdint.h>

/** A place-holder implementation of I2C communication for ADXL345 */
typedef int i2c_desc;
typedef int i2c_init_param;

int i2c_write(i2c_desc *,
				   uint8_t *buffer,
				   int buffer_len, int stop_condition_ctrl);

int i2c_read(i2c_desc *,
				   uint8_t *buffer,
				   int buffer_len, int stop_condition_ctrl);                   

int i2c_init(i2c_desc**, i2c_init_param*);

int i2c_remove(i2c_desc *);

#endif //ADXL345_I2C_H