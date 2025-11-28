#ifndef SERIAL_H
#define SERIAL_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

void serial_init(void);
void serial_putc(char c);
void serial_puts(const char *str);
void serial_printk(const char *format, ...);
char serial_read_char(void);
bool serial_read_line(char *buffer, size_t buffer_size);
int serial_scank(const char *format, ...);

#endif