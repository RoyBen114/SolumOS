#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <boot/info.h>
#include <lib/serial.h>
#include <lib/screen.h>

void kernel_main() 
{
    serial_init();
    vga_init();
    parse_mb_info();

    vga_printf("Welcome to Solum OS!\n");
    serial_printf("Welcome to Solum OS!\n");
    vga_printf("Version (a0.01)\n");
    serial_printf("Version (a0.01)\n");
    vga_printf("By Roy 2025\n");
    serial_printf("By Roy 2025\n");
}