/* Simple IPC API for Solum Microkernel (PoC) */
#ifndef KERNEL_IPC_H
#define KERNEL_IPC_H

#include <stddef.h>
#include <stdint.h>

typedef int (*ipc_handler_fn)(const void *msg, size_t len, void *ctx);

int ipc_register_handler(uint32_t service_id, ipc_handler_fn handler, void *ctx);
int ipc_send(uint32_t service_id, const void *msg, size_t len);
void ipc_init(void);

#endif
