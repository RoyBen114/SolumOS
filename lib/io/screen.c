#include <stdint.h>
#include <stddef.h>
#include "io.h"

// 在文本模式下输出字符
void text_mode_putchar(uint64_t fb, int x, int y, char c, uint8_t color) {
    uint16_t* video = (uint16_t*)fb;
    if (x < 0 || x >= 80 || y < 0 || y >= 25) return;
    video[y * 80 + x] = (color << 8) | c;
}

// 在文本模式下输出字符串
void text_mode_puts(uint64_t fb, int x, int y, const char* str, uint8_t color) {
    while (*str) {
        text_mode_putchar(fb, x, y, *str, color);
        x++;
        str++;
        if (x >= 80) {
            x = 0;
            y++;
            if (y >= 25) y = 24;
        }
    }
}

// 清空文本模式屏幕
void text_mode_clear(uint64_t fb, uint8_t color) {
    uint16_t* video = (uint16_t*)fb;
    for (int i = 0; i < 80 * 25; i++) {
        video[i] = (color << 8) | ' ';
    }
}
