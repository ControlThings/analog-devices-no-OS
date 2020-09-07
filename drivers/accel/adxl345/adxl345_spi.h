#ifndef ADXL345_SPI_H
#define ADXL345_SPI_H

#include <stdint.h>

/** A place-holder implementation of SPI communication for ADXL345 */
typedef int spi_desc;
typedef int spi_init_param;

int spi_write_and_read(spi_desc *,
				   uint8_t *buffer,
				   int buffer_len);

int spi_init(spi_desc**, spi_init_param*);

int spi_remove(spi_desc *);

#endif //ADXL345_SPI_H