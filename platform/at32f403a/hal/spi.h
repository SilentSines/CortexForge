#ifndef SPI_H
#define SPI_H

void spi_bus_init(spi_type *spi, uint8_t mode);
void spi_set_speed(spi_type *spi, spi_mclk_freq_div_type div);
void spi_set_data_size(spi_type *spi, spi_frame_bit_num_type bit_num);
void spi_send_poll(spi_type *spi, const void *buf, uint32_t size);
void spi_recv_poll(spi_type *spi, void *buf, uint32_t size);
void spi_send_dma(spi_type *spi, const void *buf, uint32_t size, cb_t func);
void spi_recv_dma(spi_type *spi, void *buf, uint32_t size, cb_t func);
bool spi_is_busy(spi_type *spi);

#endif