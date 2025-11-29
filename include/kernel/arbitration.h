/* Simple arbitration/resource manager API */
#ifndef KERNEL_ARBITRATION_H
#define KERNEL_ARBITRATION_H

#include <stdint.h>

int arbitration_init(void);
int request_resource(const char *name, uint32_t owner_id);
int release_resource(const char *name, uint32_t owner_id);

#endif
