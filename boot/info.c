#include <stdint.h>
#include <stddef.h>
#include "info.h"

extern uint64_t multiboot2_info_addr;

int is_graphics_mode = 0;
struct multiboot2_info *mbi;
uint8_t *current_tag;
struct multiboot2_tag *tag;
struct multiboot2_tag_framebuffer *fb_info;

void parse_mb_info()
{
    mbi = (struct multiboot2_info *)multiboot2_info_addr;
    current_tag = mbi->tags;

    while (1)
    {
        tag = (struct multiboot2_tag *)current_tag;

        // find framebuffer tag (8)
        if (tag->type == 8)
        {
            fb_info = (struct multiboot2_tag_framebuffer *)tag;

            if (fb_info->fb_width > 80 || fb_info->fb_height > 25 || fb_info->fb_bpp > 16)
            {
                is_graphics_mode = 1;
            }
            else
            {
                is_graphics_mode = 0;
            }
            break;
        }

        current_tag += tag->size;
        if ((uint64_t)current_tag % 8 != 0)
        {
            current_tag += 8 - ((uint64_t)current_tag % 8);
        }
    }
}