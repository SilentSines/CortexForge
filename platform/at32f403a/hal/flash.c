#include "hal.h"

#define FLASH_CACHE_EN  1
#define FLASH_PAGE_ADDR_MASK ((uint32_t)(~(FLASH_PAGE_SIZE - 1U)))
#define FLASH_TOTAL_SIZE (FLASH_PAGE_SIZE * FALSH_PAGE_NUM)
#define FLASH_WRITE_SIZE  4 

#if FLASH_WRITE_SIZE == 2
    typedef uint16_t flash_data_t;
    #define FLASH_DATA_MASK  0xFFFF
    #define FLASH_PRO_TYPE  FLASH_TYPEPROGRAM_HALFWORD
#elif FLASH_WRITE_SIZE == 4
    typedef uint32_t flash_data_t;
    #define FLASH_DATA_MASK  0xFFFFFFFF
    #define FLASH_PRO_TYPE  FLASH_TYPEPROGRAM_WORD
#elif FLASH_WRITE_SIZE == 8
    typedef uint64_t flash_data_t;
    #define FLASH_DATA_MASK  0xFFFFFFFFFFFFFFFF
    #define FLASH_PRO_TYPE  FLASH_TYPEPROGRAM_DOUBLEWORD
#else
    #error "Unsupported FLASH_WRITE_SIZE (must be 1, 2, 4, or 8)"
#endif

enum
{
    FLASH_EQU, 
    FLASH_WRITE,
    FLASH_ERASE,
    FLASH_FORCE_ERASE,
};

#if FLASH_CACHE_EN == 1
static uint8_t page_buf[FLASH_PAGE_SIZE] __attribute__((aligned(FLASH_WRITE_SIZE))); 
#endif

static uint8_t flash_check_page(const uint8_t *old_buf, const uint8_t *new_buf, uint32_t size)
{
    uint32_t i;
    flash_data_t old, new;
    bool is_diff = false;
    flash_data_t *old_p = (flash_data_t *)old_buf;
    flash_data_t *new_p = (flash_data_t *)new_buf;

    for (i = 0; i < (size / FLASH_WRITE_SIZE); i++)
	{
        old = old_p[i];
        new = new_p[i];
		if (old == new)
		{
            continue;
        }
        is_diff = true;
        if (old != FLASH_DATA_MASK)
        {
            return FLASH_ERASE;
        }
	}
    return (is_diff ? FLASH_WRITE : FLASH_EQU);	
}

static bool flash_erase_page(uint32_t page_addr)
{
    flash_status_type status = FLASH_OPERATE_DONE;

    status = flash_operation_wait_for(ERASE_TIMEOUT);
    if ((status == FLASH_PROGRAM_ERROR) || (status == FLASH_EPP_ERROR))
    {
        flash_flag_clear(FLASH_PRGMERR_FLAG | FLASH_EPPERR_FLAG);
    }   
    else if (status == FLASH_OPERATE_TIMEOUT)
    {    
        return false;
    }
    CRITICAL_SECTION(status = flash_sector_erase(page_addr));
    if (status != FLASH_OPERATE_DONE)
    {    
        return false;
    }
    return true;
}

static bool flash_write_page(const uint8_t *buf, uint32_t addr, uint32_t size, bool force_erase)
{
#if FLASH_CACHE_EN == 0
    uint8_t page_buf[FLASH_PAGE_SIZE] __attribute__((aligned(FLASH_WRITE_SIZE))); 
#endif

    bool res = true;
    uint8_t flash_state;
    uint32_t i, target_pos, page_addr;
    const flash_data_t *write_ptr;
    uint32_t write_len;
    uint32_t write_addr;

    target_pos = addr & (FLASH_PAGE_SIZE - 1);
    page_addr = addr & FLASH_PAGE_ADDR_MASK;

    if (force_erase)
    {
        flash_state = FLASH_FORCE_ERASE;
    }
    else
    {
    #if WDT_EN
        wdt_feed();
    #endif
        flash_read(page_buf, page_addr, FLASH_PAGE_SIZE);
        flash_state = flash_check_page(page_buf + target_pos, buf, size);
    }
    
    if (flash_state == FLASH_EQU)
    {
        return res;
    }

    flash_unlock();

    if ((flash_state == FLASH_ERASE) || (flash_state == FLASH_FORCE_ERASE))
    {
    #if WDT_EN
        wdt_feed();
    #endif    
        if (flash_erase_page(page_addr) == false)
        {
            res = false;
            goto error;
        }
    }

#if WDT_EN
    wdt_feed();
#endif 
    if ((flash_state == FLASH_WRITE) || (flash_state == FLASH_FORCE_ERASE))
    {
        write_ptr = (const flash_data_t *)buf;
        write_len = size;
        write_addr = addr;
        
        if (flash_state == FLASH_WRITE)
        {
            flash_data_t *old_ptr = (flash_data_t *)(page_buf + target_pos);
            for (i = 0; i < write_len; i += FLASH_WRITE_SIZE)
            {
                if (*write_ptr != *old_ptr)
                {
                    if (flash_word_program(write_addr, *write_ptr) != FLASH_OPERATE_DONE)
                    {
                        res = false;
                        goto error;
                    }
                }
                write_addr += FLASH_WRITE_SIZE;
                write_ptr++;
                old_ptr++;
            }
            goto skip_write;  
        }
    }
    else
    {
        memcpy(page_buf + target_pos, buf, size);
        write_ptr = (const flash_data_t *)page_buf;
        write_len = FLASH_PAGE_SIZE;
        write_addr = page_addr;
    }
    for (i = 0; i < write_len; i += FLASH_WRITE_SIZE)
    {
        if(flash_word_program(write_addr, *write_ptr) != FLASH_OPERATE_DONE)
        {
            res = false;
            goto error; 
        }
        write_addr += FLASH_WRITE_SIZE;
        write_ptr++;
    }

skip_write:
error:
    flash_lock();

#if WDT_EN
    wdt_feed();
#endif 

    return res;
}

bool flash_erase(uint32_t addr)
{
    uint32_t page_addr = addr & FLASH_PAGE_ADDR_MASK;
    flash_unlock();
#if WDT_EN
    wdt_feed();
#endif 
    bool res =  flash_erase_page(page_addr);
    flash_lock();
    return res;
}

bool flash_write(const uint8_t *buf, uint32_t addr, uint32_t size, bool force_erase)
{
    uint32_t page_start;
    uint32_t page_end;

    if (((uint32_t)buf & (FLASH_WRITE_SIZE - 1)) || (addr & (FLASH_WRITE_SIZE - 1)) || (size & (FLASH_WRITE_SIZE - 1))) 
    {
        return false;
    }

    if ((addr < FLASH_BASE) || ((addr + size) > (FLASH_BASE + FLASH_TOTAL_SIZE)) || (!size))
    {
        return false;
    }

    page_start = addr / FLASH_PAGE_SIZE;
    page_end = (addr + size - 1) / FLASH_PAGE_SIZE;
    if (page_start != page_end)
    {
        return false;
    }
    
    return flash_write_page(buf, addr, size, force_erase);
}

void flash_read(uint8_t *buf, uint32_t addr, uint32_t size)
{
    if ((addr < FLASH_BASE) || ((addr + size) > (FLASH_BASE + FLASH_TOTAL_SIZE)))
    {
        return;
    }

    if (!((uint32_t)buf & 0x03) && !(addr & 0x03))
    {
        uint32_t *dst = (uint32_t *)buf;
        while (size >= 4)
        {
            *dst++ = *(volatile uint32_t *)addr;
            addr += 4;
            size -= 4;
        }
        buf = (uint8_t *)dst;
        while (size > 0)
        {
            *buf++ = *(volatile uint8_t *)addr;
            addr++;
            size--;
        }
    }
    else
    {
        while ((addr & 0x03) && (size > 0))
        {
            *buf++ = *(volatile uint8_t *)addr;
            addr++;
            size--;
        }
        while(size >= 4)
        {
            uint32_t data = *(volatile uint32_t *)addr;

            addr += 4;
            size -= 4;
            buf[0] = (data) & 0xFF;
            buf[1] = (data >> 8) & 0xFF;
            buf[2] = (data >> 16) & 0xFF;
            buf[3] = (data >> 24) & 0xFF;
            buf += 4;
        }
        while (size > 0)
        {
            *buf++ = *(volatile uint8_t *)addr;
            addr++;
            size--;
        }
    }
}