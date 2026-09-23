#ifndef RAND_H
#define RAND_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define RAND_LEN 256
#define RAND_TYPE uint8_t

void rand_generate(uint32_t base, uint16_t size);
RAND_TYPE* rand_get(void);

#endif

