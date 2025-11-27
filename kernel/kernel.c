#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include "../boot/info.h"
#include "../lib/serial.h"
#include "../lib/screen.h"

void kernel_main() 
{
    serial_init();
    vga_init();
    parse_mb_info();
}