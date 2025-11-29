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
#include <lib/string.h>
#include <kernel/module.h>

#define MAX_MODULES 16

struct module_entry {
    const char *name;
    module_init_fn init;
    module_fini_fn fini;
    void *ctx;
    int inited;
};

static struct module_entry modules[MAX_MODULES];

int module_register(const char *name, module_init_fn init, module_fini_fn fini, void *ctx)
{
    for (int i = 0; i < MAX_MODULES; i++) {
        if (!modules[i].name) {
            modules[i].name = name;
            modules[i].init = init;
            modules[i].fini = fini;
            modules[i].ctx = ctx;
            modules[i].inited = 0;
            return i;
        }
    }
    return -1;
}

void module_init_all(void)
{
    for (int i = 0; i < MAX_MODULES; i++) {
        if (modules[i].name && modules[i].init && !modules[i].inited) {
            modules[i].inited = 1;
            modules[i].init(modules[i].ctx);
        }
    }
}

void module_list(void)
{
    for (int i = 0; i < MAX_MODULES; i++) {
        if (modules[i].name) {
            /* In PoC we avoid complex printing dependencies; caller can query list */
        }
    }
}
