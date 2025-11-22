#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

extern uint64_t multiboot2_info_addr;

//===================================================↓serial functions↓==================================================
static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void outb(uint16_t port, uint8_t val) {
    asm volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

static int serial_is_transmit_empty() {
    return inb(0x3F8 + 5) & 0x20;
}

void serial_init() {
    outb(0x3F8 + 1, 0x00); 
    outb(0x3F8 + 3, 0x80);
    outb(0x3F8 + 0, 0x03);
    outb(0x3F8 + 1, 0x00); 
    outb(0x3F8 + 3, 0x03); 
    outb(0x3F8 + 2, 0xC7); 
    outb(0x3F8 + 4, 0x0B); 
}

void serial_putc(char c) {
    while (!serial_is_transmit_empty());
    outb(0x3F8, c);
    
    if (c == '\n') {
        while (!serial_is_transmit_empty());
        outb(0x3F8, '\r');
    }
}

void serial_puts(const char *str) {
    while (*str) {
        serial_putc(*str++);
    }
}

void serial_put_dec(uint64_t value) {
    char buffer[21];
    char *ptr = buffer + 20;
    *ptr = '\0';
    
    if (value == 0) {
        serial_putc('0');
        return;
    }
    
    while (value > 0) {
        *--ptr = '0' + (value % 10);
        value /= 10;
    }
    
    serial_puts(ptr);
}

void serial_put_hex(uint64_t value) {
    const char hex_chars[] = "0123456789ABCDEF";
    char buffer[17];
    buffer[16] = '\0';
    
    for (int i = 15; i >= 0; i--) {
        buffer[i] = hex_chars[value & 0xF];
        value >>= 4;
    }
    
    serial_puts("0x");
    serial_puts(buffer);
}

void serial_put_hex_lower(uint64_t value) {
    const char hex_chars[] = "0123456789abcdef";
    char buffer[17];
    buffer[16] = '\0';
    
    for (int i = 15; i >= 0; i--) {
        buffer[i] = hex_chars[value & 0xF];
        value >>= 4;
    }
    
    serial_puts("0x");
    serial_puts(buffer);
}

void serial_put_ptr(void *ptr) {
    serial_put_hex((uint64_t)(uintptr_t)ptr);
}

void serial_put_dec_signed(int64_t value) {
    if (value < 0) {
        serial_putc('-');
        value = -value;
    }
    serial_put_dec((uint64_t)value);
}

void serial_printf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    
    while (*format) {
        if (*format == '%') {
            format++;
            
            int long_count = 0;
            while (*format == 'l') {
                long_count++;
                format++;
            }
            
            switch (*format) {
                case 'c': {
                    char c = (char)va_arg(args, int);
                    serial_putc(c);
                    break;
                }
                
                case 's': {
                    char *str = va_arg(args, char*);
                    if (str == NULL) {
                        serial_puts("(null)");
                    } else {
                        serial_puts(str);
                    }
                    break;
                }
                
                case 'd': 
                case 'i': {
                    if (long_count >= 2) {
                        int64_t num = va_arg(args, int64_t);
                        serial_put_dec_signed(num);
                    } else if (long_count == 1) {
                        long num = va_arg(args, long);
                        serial_put_dec_signed(num);
                    } else {
                        int num = va_arg(args, int);
                        serial_put_dec_signed(num);
                    }
                    break;
                }
                
                case 'u': {
                    if (long_count >= 2) {
                        uint64_t num = va_arg(args, uint64_t);
                        serial_put_dec(num);
                    } else if (long_count == 1) {
                        unsigned long num = va_arg(args, unsigned long);
                        serial_put_dec(num);
                    } else {
                        unsigned int num = va_arg(args, unsigned int);
                        serial_put_dec(num);
                    }
                    break;
                }
                
                case 'x': {
                    if (long_count >= 2) {
                        uint64_t num = va_arg(args, uint64_t);
                        serial_put_hex_lower(num);
                    } else if (long_count == 1) {
                        unsigned long num = va_arg(args, unsigned long);
                        serial_put_hex_lower(num);
                    } else {
                        unsigned int num = va_arg(args, unsigned int);
                        serial_put_hex_lower((uint64_t)num);
                    }
                    break;
                }
                
                case 'X': {
                    if (long_count >= 2) {
                        uint64_t num = va_arg(args, uint64_t);
                        serial_put_hex(num);
                    } else if (long_count == 1) {
                        unsigned long num = va_arg(args, unsigned long);
                        serial_put_hex(num);
                    } else {
                        unsigned int num = va_arg(args, unsigned int);
                        serial_put_hex((uint64_t)num);
                    }
                    break;
                }
                
                case 'p': {
                    void *ptr = va_arg(args, void*);
                    serial_put_ptr(ptr);
                    break;
                }
                
                case '%': {
                    serial_putc('%');
                    break;
                }
                
                default: {
                    serial_putc('%');
                    serial_putc(*format);
                    break;
                }
            }
        } else {
            serial_putc(*format);
        }
        format++;
    }
    
    va_end(args);
}
//===================================================↑serial functions↑==================================================

// 根据BPP定义颜色格式
#define COLOR_BLACK16     0x0000
#define COLOR_WHITE16     0xFFFF
#define COLOR_RED16       0xF800
#define COLOR_GREEN16     0x07E0
#define COLOR_BLUE16      0x001F

#define COLOR_BLACK24     0x000000
#define COLOR_WHITE24     0xFFFFFF
#define COLOR_RED24       0xFF0000
#define COLOR_GREEN24     0x00FF00
#define COLOR_BLUE24      0x0000FF

#define COLOR_BLACK32     0x00000000
#define COLOR_WHITE32     0xFFFFFFFF
#define COLOR_RED32       0xFFFF0000
#define COLOR_GREEN32     0xFF00FF00
#define COLOR_BLUE32      0xFF0000FF

#define MULTIBOOT2_TAG_TYPE_FRAMEBUFFER 8

// Framebuffer信息结构
struct framebuffer_info {
    uint64_t address;
    uint32_t width;
    uint32_t height;
    uint32_t pitch;
    uint32_t bpp;
    uint32_t type;
};

struct multiboot2_tag {
    uint32_t type;
    uint32_t size;
};

struct multiboot2_tag_framebuffer {
    uint32_t type;
    uint32_t size;
    uint64_t framebuffer_addr;
    uint32_t framebuffer_pitch;
    uint32_t framebuffer_width;
    uint32_t framebuffer_height;
    uint8_t framebuffer_bpp;
    uint8_t framebuffer_type;
    uint8_t reserved;
};

struct multiboot2_info {
    uint32_t total_size;
    uint32_t reserved;
    uint8_t tags[];
};

// 全局framebuffer信息
static struct framebuffer_info fb_info = {0};
static uint8_t* backbuffer = NULL;

// 颜色转换函数
uint32_t convert_color(uint32_t color, uint32_t target_bpp) {
    switch (target_bpp) {
        case 16: {
            // 从32位ARGB转换为16位RGB565
            uint8_t r = (color >> 19) & 0x1F;  // 5 bits
            uint8_t g = (color >> 10) & 0x3F;  // 6 bits  
            uint8_t b = (color >> 3) & 0x1F;   // 5 bits
            return (r << 11) | (g << 5) | b;
        }
        case 24: {
            // 从32位ARGB转换为24位RGB
            return color & 0xFFFFFF;
        }
        case 32:
        default:
            return color;
    }
}

// 绘制像素函数
void draw_pixel(uint8_t* buffer, int x, int y, uint32_t color, uint32_t bpp, uint32_t width, uint32_t pitch) {
    if (x < 0 || x >= (int)width || y < 0 || y >= (int)fb_info.height) return;
    
    uint32_t converted_color = convert_color(color, bpp);
    uint32_t bytes_per_pixel = bpp / 8;
    uint32_t offset = y * pitch + x * bytes_per_pixel;
    
    switch (bytes_per_pixel) {
        case 2: // 16 bpp
            *(uint16_t*)(buffer + offset) = (uint16_t)converted_color;
            break;
        case 3: // 24 bpp
            buffer[offset] = (converted_color >> 16) & 0xFF;     // R
            buffer[offset + 1] = (converted_color >> 8) & 0xFF;  // G
            buffer[offset + 2] = converted_color & 0xFF;         // B
            break;
        case 4: // 32 bpp
            *(uint32_t*)(buffer + offset) = converted_color;
            break;
    }
}

// 初始化framebuffer
struct framebuffer_info* framebuffer_init(uint64_t multiboot_info_addr) {
    struct multiboot2_info* mbi = (struct multiboot2_info*)multiboot_info_addr;
    uint8_t* current_tag = mbi->tags;
    
    serial_printf("Searching for framebuffer tag...\n");
    serial_printf("Multiboot info total size: %u\n", mbi->total_size);
    
    while (1) {
        struct multiboot2_tag* tag = (struct multiboot2_tag*)current_tag;
        
        if (tag->type == 0) {
            serial_printf("End tag found\n");
            break;
        }
        
        serial_printf("Tag type: %u, size: %u\n", tag->type, tag->size);
        
        if (tag->type == MULTIBOOT2_TAG_TYPE_FRAMEBUFFER) {
            struct multiboot2_tag_framebuffer* fb_tag = 
                (struct multiboot2_tag_framebuffer*)tag;
            
            fb_info.address = fb_tag->framebuffer_addr;
            fb_info.width = fb_tag->framebuffer_width;
            fb_info.height = fb_tag->framebuffer_height;
            fb_info.pitch = fb_tag->framebuffer_pitch;
            fb_info.bpp = fb_tag->framebuffer_bpp;
            fb_info.type = fb_tag->framebuffer_type;
            
            serial_printf("Framebuffer found:\n");
            serial_printf("  Address: 0x%llx\n", fb_info.address);
            serial_printf("  Width: %u\n", fb_info.width);
            serial_printf("  Height: %u\n", fb_info.height);
            serial_printf("  Pitch: %u\n", fb_info.pitch);
            serial_printf("  BPP: %u\n", fb_info.bpp);
            serial_printf("  Type: %u\n", fb_info.type);
            
            // 使用汇编中分配的后缓冲区
            extern uint8_t framebuffer_backbuffer[];
            backbuffer = framebuffer_backbuffer;
            
            serial_printf("Backbuffer at: 0x%llx\n", (uint64_t)backbuffer);
            
            // 清空后缓冲区
            uint32_t backbuffer_size = fb_info.height * fb_info.pitch;
            for (uint32_t i = 0; i < backbuffer_size; i++) {
                backbuffer[i] = 0;
            }
            
            serial_printf("Framebuffer initialized successfully\n");
            return &fb_info;
        }
        
        current_tag += tag->size;
        if ((uint64_t)current_tag % 8 != 0) {
            current_tag += 8 - ((uint64_t)current_tag % 8);
        }
    }
    
    serial_printf("No framebuffer tag found!\n");
    return NULL;
}

// 绘制矩形
void framebuffer_draw_rect(int x, int y, int width, int height, uint32_t color) {
    if (!backbuffer) {
        serial_printf("Error: backbuffer is NULL\n");
        return;
    }
    
    // 边界检查
    if (x < 0) x = 0;
    if (y < 0) y = 0;
    if (x >= (int)fb_info.width) return;
    if (y >= (int)fb_info.height) return;
    
    int end_x = x + width;
    int end_y = y + height;
    if (end_x > (int)fb_info.width) end_x = fb_info.width;
    if (end_y > (int)fb_info.height) end_y = fb_info.height;
    
    serial_printf("Drawing rect: (%d,%d) to (%d,%d), color=0x%x\n", 
                 x, y, end_x, end_y, color);
    
    for (int row = y; row < end_y; row++) {
        for (int col = x; col < end_x; col++) {
            draw_pixel(backbuffer, col, row, color, fb_info.bpp, fb_info.width, fb_info.pitch);
        }
    }
}

// 交换缓冲区
void framebuffer_swap_buffers(void) {
    if (!backbuffer) {
        serial_printf("Error: backbuffer is NULL in swap\n");
        return;
    }
    
    if (!fb_info.address) {
        serial_printf("Error: framebuffer address is 0\n");
        return;
    }
    
    uint8_t* frontbuffer = (uint8_t*)fb_info.address;
    uint32_t buffer_size = fb_info.height * fb_info.pitch;
    
    serial_printf("Swapping buffers:\n");
    serial_printf("  Resolution: %dx%d\n", fb_info.width, fb_info.height);
    serial_printf("  BPP: %u\n", fb_info.bpp);
    serial_printf("  Pitch: %u\n", fb_info.pitch);
    serial_printf("  Frontbuffer: 0x%llx\n", (uint64_t)frontbuffer);
    serial_printf("  Backbuffer: 0x%llx\n", (uint64_t)backbuffer);
    serial_printf("  Buffer size: %u bytes\n", buffer_size);
    
    // 将后缓冲区复制到前缓冲区
    for (uint32_t i = 0; i < buffer_size; i++) {
        frontbuffer[i] = backbuffer[i];
    }
    
    serial_printf("Buffer swap completed successfully\n");
}

void kernel_main() 
{
    serial_init();
    serial_printf("=== SolumOS Kernel Started ===\n");
    serial_printf("Multiboot info structure at: 0x%llx\n", multiboot2_info_addr);
    
    struct framebuffer_info* fb_info = framebuffer_init(multiboot2_info_addr);
    
    if (!fb_info) {
        serial_printf("CRITICAL ERROR: Framebuffer initialization failed!\n");
        serial_printf("System will now halt...\n");
        while (1) {
            asm volatile ("hlt");
        }
    }
    
    serial_printf("Starting graphics test pattern...\n");
    
    // 根据BPP选择合适的颜色
    uint32_t black_color, red_color, green_color, blue_color;
    
    switch (fb_info->bpp) {
        case 16:
            black_color = COLOR_BLACK16;
            red_color = COLOR_RED16;
            green_color = COLOR_GREEN16;
            blue_color = COLOR_BLUE16;
            break;
        case 24:
            black_color = COLOR_BLACK24;
            red_color = COLOR_RED24;
            green_color = COLOR_GREEN24;
            blue_color = COLOR_BLUE24;
            break;
        case 32:
        default:
            black_color = COLOR_BLACK32;
            red_color = COLOR_RED32;
            green_color = COLOR_GREEN32;
            blue_color = COLOR_BLUE32;
            break;
    }
    
    serial_printf("Using colors for %u bpp\n", fb_info->bpp);
    
    // 清空整个屏幕
    framebuffer_draw_rect(0, 0, fb_info->width, fb_info->height, black_color);
    
    // 计算屏幕中心
    int center_x = fb_info->width / 2;
    int center_y = fb_info->height / 2;
    
    serial_printf("Screen center: (%d, %d)\n", center_x, center_y);
    
    // 根据屏幕大小调整方块尺寸
    int rect_size;
    if (fb_info->width <= 80 || fb_info->height <= 25) {
        // 文本模式或小分辨率
        rect_size = 10;
    } else {
        // 图形模式
        rect_size = 100;
    }
    
    // 绘制彩色方块
    framebuffer_draw_rect(
        center_x - rect_size/2, 
        center_y - rect_size/2, 
        rect_size, 
        rect_size, 
        red_color
    );
    
    framebuffer_draw_rect(
        center_x - rect_size/4, 
        center_y - rect_size/4, 
        rect_size/2, 
        rect_size/2, 
        green_color
    );
    
    framebuffer_draw_rect(
        center_x - rect_size/8, 
        center_y - rect_size/8, 
        rect_size/4, 
        rect_size/4, 
        blue_color
    );
    
    serial_printf("Test pattern drawn, swapping buffers...\n");
    
    // 交换缓冲区
    framebuffer_swap_buffers();
    
    serial_printf("=== Graphics test completed successfully ===\n");
    
    // 主循环
    while (1) {
        asm volatile ("hlt");
    }
}