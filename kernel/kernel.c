#include <stdint.h>

struct boot_info {
    uint32_t boot_type;
};

// 端口输入函数
static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

// 端口输出函数
static inline void outb(uint16_t port, uint8_t val) {
    asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

// 串口初始化
void serial_init() {
    outb(0x3F8 + 1, 0x00);
    outb(0x3F8 + 3, 0x80);
    outb(0x3F8 + 0, 0x0C);
    outb(0x3F8 + 1, 0x00);
    outb(0x3F8 + 3, 0x03);
    outb(0x3F8 + 2, 0xC7);
    outb(0x3F8 + 4, 0x0B);
}

void serial_puts(const char* str) {
    while (*str) {
        while ((inb(0x3F8 + 5) & 0x20) == 0);
        outb(0x3F8, *str++);
    }
}

// 简单的VGA文本输出
void vga_puts(const char *str, uint8_t color) {
    volatile uint16_t *vga_buffer = (volatile uint16_t*)0xB8000;
    int i;
    for (i = 0; str[i] != 0; i++) {
        vga_buffer[i] = (color << 8) | str[i];
    }
}

// 清除VGA屏幕
void vga_clear(uint8_t color) {
    volatile uint16_t *vga_buffer = (volatile uint16_t*)0xB8000;
    for (int i = 0; i < 80 * 25; i++) {
        vga_buffer[i] = (color << 8) | ' ';
    }
}

void kernel_main(struct boot_info *info) {
    // 初始化串口
    serial_init();
    serial_puts("\n=== Minimal Kernel Started ===\n");
    
    // 立即清除屏幕并显示基本消息
    vga_clear(0x00); // 黑色背景
    
    if (info->boot_type == 0) {
        serial_puts("Boot type: BIOS\n");
        vga_puts("SolumOS - BIOS Mode", 0x0F); // 白色文本
    } else {
        serial_puts("Boot type: UEFI\n");
        vga_puts("SolumOS - UEFI Mode", 0x0F); // 白色文本
    }
    
    serial_puts("VGA text should be visible now\n");
    
    // 测试：在屏幕不同位置显示字符
    volatile uint16_t *vga = (volatile uint16_t*)0xB8000;
    
    // 在第二行显示
    vga[80] = 0x0F41; // 'A'
    vga[81] = 0x0F42; // 'B' 
    vga[82] = 0x0F43; // 'C'
    
    // 在屏幕中心显示消息
    vga[12 * 80 + 35] = 0x0A48; // 'H' - 绿色
    vga[12 * 80 + 36] = 0x0A45; // 'E'
    vga[12 * 80 + 37] = 0x0A4C; // 'L'
    vga[12 * 80 + 38] = 0x0A4C; // 'L'
    vga[12 * 80 + 39] = 0x0A4F; // 'O'
    
    serial_puts("Kernel initialization complete\n");
    
    while (1) {
        asm volatile ("hlt");
    }
}