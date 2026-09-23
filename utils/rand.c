#include "rand.h"

RAND_TYPE rand_buf[RAND_LEN];

void rand_arr_init(uint16_t size)
{
    uint16_t  i;

    for(i = 0; i < size; i++)
    {
        rand_buf[i] = i;    
    }
}

void rand_generate(uint32_t base, uint16_t size)
{
    RAND_TYPE temp;
    uint16_t i,j;

    if (size > RAND_LEN)
    {
        return;
    }
    
    rand_arr_init(size);
    srand(base);

    for(i = size - 1; i > 0; i--) 
    {
        j = rand() % (i + 1);
        temp = rand_buf[i];
        rand_buf[i] = rand_buf[j];
        rand_buf[j] = temp;
    }
}

RAND_TYPE* rand_get(void)
{
    return rand_buf;
}