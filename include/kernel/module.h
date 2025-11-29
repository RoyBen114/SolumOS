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

#ifndef MODULE_H
#define MODULE_H

#include <stddef.h>
#include <stdint.h>

typedef int (*module_init_fn)(void *ctx);
typedef void (*module_fini_fn)(void *ctx);

int module_register(const char *name, module_init_fn init, module_fini_fn fini, void *ctx);
void module_init_all(void);
void module_list(void);

#endif
