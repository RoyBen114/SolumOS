#ifndef IO_H
#define IO_H

#define TEXT_COLOR_BLACK 0x00
#define TEXT_COLOR_BLUE 0x01
#define TEXT_COLOR_GREEN 0x02
#define TEXT_COLOR_RED 0x04
#define TEXT_COLOR_WHITE 0x07

void serial_init();
void serial_printf(const char *format, ...);
void text_mode_puts(uint64_t fb, int x, int y, const char* str, uint8_t color);
void text_mode_clear(uint64_t fb, uint8_t color);

#endif