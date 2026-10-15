#ifndef FLASH_H
#define FLASH_H

#define FLASH_PAGE_NUM 256
#define FLASH_PAGE_ADDR(x) (FLASH_BASE + (FLASH_PAGE_SIZE * (x)))

bool flash_erase(uint32_t addr);
bool flash_write(const uint8_t *buf, uint32_t addr, uint32_t size, bool force_erase);
void flash_read(uint8_t *buf, uint32_t addr, uint32_t size);

#endif