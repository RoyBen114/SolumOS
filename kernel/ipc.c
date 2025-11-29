/*
 * Copyright (C) 2025 Roy Roy123ty@hotmail.com
 *
 * This file is part of Solum OS
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stddef.h>
#include <stdint.h>
#include <kernel/ipc.h>
#include <lib/serial.h>

#define MAX_IPC_SERVICES 64

static ipc_handler_fn handlers[MAX_IPC_SERVICES];
static void *handler_ctx[MAX_IPC_SERVICES];

void ipc_init(void)
{
    for (int i = 0; i < MAX_IPC_SERVICES; i++) handlers[i] = NULL;
}

int ipc_register_handler(uint32_t service_id, ipc_handler_fn handler, void *ctx)
{
    if (service_id >= MAX_IPC_SERVICES) return -1;
    handlers[service_id] = handler;
    handler_ctx[service_id] = ctx;
    return 0;
}

int ipc_send(uint32_t service_id, const void *msg, size_t len)
{
    if (service_id >= MAX_IPC_SERVICES) return -1;
    if (!handlers[service_id]) return -2;
    /* Directly call handler in this prototype */
    return handlers[service_id](msg, len, handler_ctx[service_id]);
}
