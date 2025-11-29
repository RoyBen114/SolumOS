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

#ifndef IPC_H
#define IPC_H

#include <stddef.h>
#include <stdint.h>

typedef int (*ipc_handler_fn)(const void *msg, size_t len, void *ctx);

int ipc_register_handler(uint32_t service_id, ipc_handler_fn handler, void *ctx);
int ipc_send(uint32_t service_id, const void *msg, size_t len);
void ipc_init(void);

#endif
