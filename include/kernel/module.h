/* Module registration API for Solum Microkernel */
#ifndef KERNEL_MODULE_H
#define KERNEL_MODULE_H

#include <stddef.h>
#include <stdint.h>

typedef int (*module_init_fn)(void *ctx);
typedef void (*module_fini_fn)(void *ctx);

int module_register(const char *name, module_init_fn init, module_fini_fn fini, void *ctx);
void module_init_all(void);
void module_list(void);

#endif
