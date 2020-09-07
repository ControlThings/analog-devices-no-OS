

/** A place-holder implementation of SPI communication for ADXL345 */

#include <stdint.h>
#include "adxl345_spi.h"

int spi_write_and_read(spi_desc * spi_desc,
				   uint8_t *buffer,
				   int buffer_len) {
    return -1;
}

int spi_init(spi_desc** spi_desc, spi_init_param* spi_param) {
    return -1;
}

int spi_remove(spi_desc *spi_desc) {
    return -1;
}
