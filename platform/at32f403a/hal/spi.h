#ifndef SPI_H
#define SPI_H

typedef enum
{
    SPI_DATA_SIZE_8BIT  = 0x00,
    SPI_DATA_SIZE_16BIT = 0x01,
} spi_data_size_t;

typedef enum
{
    SPI_DIV_2    = 0x00,
    SPI_DIV_4    = 0x01,
    SPI_DIV_8    = 0x02,
    SPI_DIV_16   = 0x03,
    SPI_DIV_32   = 0x04,
    SPI_DIV_64   = 0x05,
    SPI_DIV_128  = 0x06,
    SPI_DIV_256  = 0x07,
    SPI_DIV_512  = 0x08,
    SPI_DIV_1024 = 0x09,
} spi_freq_div_t;

void spi_bus_init(spi_type *spi, uint8_t mode);
void spi_set_speed(spi_type *spi, spi_freq_div_t div);
void spi_set_data_size(spi_type *spi, spi_data_size_t size);
void spi_send_poll(spi_type *spi, const void *buf, uint32_t size);
void spi_recv_poll(spi_type *spi, void *buf, uint32_t size);
void spi_send_dma(spi_type *spi, const void *buf, uint32_t size, cb_t func);
void spi_recv_dma(spi_type *spi, void *buf, uint32_t size, cb_t func);
bool spi_is_busy(spi_type *spi);

#endif