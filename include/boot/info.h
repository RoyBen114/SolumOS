#ifndef INFO_H
#define INFO_H

struct multiboot2_tag
{
    uint32_t type;
    uint32_t size;
};

struct multiboot2_tag_framebuffer
{
    uint32_t type;
    uint32_t size;
    uint64_t fb_addr;
    uint32_t fb_pitch;
    uint32_t fb_width;
    uint32_t fb_height;
    uint8_t fb_bpp;
    uint8_t fb_type;
    uint8_t reserved;
};

struct multiboot2_info
{
    uint32_t total_size;
    uint32_t reserved;
    uint8_t tags[];
};

void parse_mb_info(void);

extern int is_graphics_mode;
extern struct multiboot2_info *mbi;
extern uint8_t *current_tag;
extern struct multiboot2_tag *tag;
extern struct multiboot2_tag_framebuffer *fb_info;

#endif

