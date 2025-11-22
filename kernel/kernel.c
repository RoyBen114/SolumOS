#include <stdint.h>

int xpos = 0;
int ypos = 0;
volatile unsigned char *video = (volatile unsigned char *)0xB8000;

struct boot_info
{
    uint64_t framebuffer_addr;
    uint32_t framebuffer_width;
    uint32_t framebuffer_height;
    uint32_t framebuffer_pitch;
    uint32_t framebuffer_bpp;
};

//===================================================↓serial functions↓==================================================
static inline uint8_t inb(uint16_t port)
{
    uint8_t ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void outb(uint16_t port, uint8_t val)
{
    asm volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

void serial_init()
{
    outb(0x3F8 + 1, 0x00);
    outb(0x3F8 + 3, 0x80);
    outb(0x3F8 + 0, 0x0C);
    outb(0x3F8 + 1, 0x00);
    outb(0x3F8 + 3, 0x03);
    outb(0x3F8 + 2, 0xC7);
    outb(0x3F8 + 4, 0x0B);
}

void serial_puts(const char *str)
{
    while (*str)
    {
        while ((inb(0x3F8 + 5) & 0x20) == 0)
            ;
        outb(0x3F8, *str++);
    }
}

void serial_put_hex(uint64_t value)
{
    char hex_chars[] = "0123456789ABCDEF";
    char buffer[17];
    buffer[16] = '\0';

    for (int i = 15; i >= 0; i--)
    {
        buffer[i] = hex_chars[value & 0xF];
        value >>= 4;
    }

    serial_puts("0x");
    serial_puts(buffer);
}

void serial_put_dec(uint32_t value)
{
    char buffer[11];
    char *ptr = buffer + 10;
    *ptr = '\0';

    if (value == 0)
    {
        serial_puts("0");
        return;
    }

    while (value > 0)
    {
        *--ptr = '0' + (value % 10);
        value /= 10;
    }

    serial_puts(ptr);
}
//===================================================↑serial functions↑==================================================
void kernel_main(struct boot_info *info) 
{
    serial_init();
    serial_puts("\n === Debug ===\n");
    serial_puts("Kernel initialization complete\n");
}