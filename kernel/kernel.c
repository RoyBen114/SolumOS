#include <stdint.h>

// 引导信息结构
struct boot_info {
    uint32_t boot_type;
    void *memory_map_ptr;
    uint32_t memory_map_entries;
    uint64_t framebuffer_addr;
    uint32_t framebuffer_width;
    uint32_t framebuffer_height;
    uint32_t framebuffer_pitch;
    uint32_t framebuffer_bpp;
};

// 简单的 VGA 文本模式输出
void vga_puts(const char *str, uint8_t color) {
    volatile uint16_t *vga_buffer = (volatile uint16_t*)0xB8000;
    static uint16_t cursor_pos = 0;
    
    for (int i = 0; str[i] != 0; i++) {
        switch (str[i]) {
            case '\n':
                cursor_pos = (cursor_pos / 80 + 1) * 80;
                break;
            default:
                vga_buffer[cursor_pos] = (color << 8) | str[i];
                cursor_pos++;
                break;
        }
        
        if (cursor_pos >= 80 * 25) {
            // 简单滚屏
            for (int j = 0; j < 80 * 24; j++) {
                vga_buffer[j] = vga_buffer[j + 80];
            }
            for (int j = 80 * 24; j < 80 * 25; j++) {
                vga_buffer[j] = (color << 8) | ' ';
            }
            cursor_pos = 80 * 24;
        }
    }
}

// 简单的帧缓冲区文本输出（使用 8x8 字体）
void fb_puts(const char *str, uint32_t color, struct boot_info *info) {
    if (info->framebuffer_addr == 0) return;
    
    // 简单的 8x8 字体（只包含 ASCII 可打印字符）
    static const uint8_t font[96][8] = {
        // 这里应该包含完整的 8x8 字体数据
        // 为简洁起见，只实现空格和几个字母
        [0] = {0}, // 空格
        ['S' - 32] = {0x3C, 0x42, 0x40, 0x3C, 0x02, 0x42, 0x3C, 0x00},
        ['o' - 32] = {0x00, 0x00, 0x3C, 0x42, 0x42, 0x42, 0x3C, 0x00},
        ['l' - 32] = {0x00, 0x00, 0x20, 0x20, 0x20, 0x20, 0x18, 0x00},
        ['u' - 32] = {0x00, 0x00, 0x42, 0x42, 0x42, 0x46, 0x3A, 0x00},
        ['m' - 32] = {0x00, 0x00, 0x42, 0x66, 0x5A, 0x42, 0x42, 0x00},
        // 可以继续添加其他字符...
    };
    
    uint8_t *fb = (uint8_t*)(uintptr_t)info->framebuffer_addr;
    static uint32_t x = 10, y = 10;
    
    for (int i = 0; str[i] != 0; i++) {
        char c = str[i];
        if (c < 32 || c > 126) c = ' ';  // 不可打印字符显示为空格
        
        const uint8_t *glyph = font[c - 32];
        
        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 8; col++) {
                if (glyph[row] & (1 << (7 - col))) {
                    uint32_t pixel_x = x + col;
                    uint32_t pixel_y = y + row;
                    
                    if (pixel_x < info->framebuffer_width && 
                        pixel_y < info->framebuffer_height) {
                        uint32_t *pixel = (uint32_t*)(fb + pixel_y * info->framebuffer_pitch + pixel_x * 4);
                        *pixel = color;
                    }
                }
            }
        }
        
        x += 9;  // 字符宽度 + 1像素间距
        if (x + 8 >= info->framebuffer_width) {
            x = 10;
            y += 9;
        }
    }
}

void kernel_main(struct boot_info *info) {
    // 立即显示测试消息
    if (info->boot_type == 0) {
        // BIOS 模式：使用 VGA
        vga_puts("SolumOS - BIOS Boot Successful!\n", 0x0F);
        vga_puts("Testing VGA output...\n", 0x0A);
    } else {
        // UEFI 模式：使用帧缓冲区
        fb_puts("SolumOS - UEFI Boot", 0x00FF0000, info);  // 红色文本
        fb_puts("Testing Framebuffer...", 0x0000FF00, info); // 绿色文本
    }
    
    // 主循环
    while (1) {
        // 简单的延迟
        for (volatile int i = 0; i < 1000000; i++);
    }
}