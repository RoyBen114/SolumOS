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
#include <lib/serial.h>
#include <kernel/arbitration.h>

#define MAX_RESOURCES 32

struct resource_entry {
    const char *name;
    uint32_t owner;
};

static struct resource_entry resources[MAX_RESOURCES];

int arbitration_init(void)
{
    for (int i = 0; i < MAX_RESOURCES; i++) {
        resources[i].name = NULL;
        resources[i].owner = 0;
    }
    return 0;
}

int request_resource(const char *name, uint32_t owner_id)
{
    for (int i = 0; i < MAX_RESOURCES; i++) {
        if (resources[i].name && k_strcmp(resources[i].name, name) == 0) {
            if (resources[i].owner == 0) {
                resources[i].owner = owner_id;
                return 0; /* granted */
            }
            return -1; /* busy */
        }
    }
    /* new resource */
    for (int i = 0; i < MAX_RESOURCES; i++) {
        if (!resources[i].name) {
            resources[i].name = name;
            resources[i].owner = owner_id;
            return 0;
        }
    }
    return -2; /* no space */
}

int release_resource(const char *name, uint32_t owner_id)
{
    for (int i = 0; i < MAX_RESOURCES; i++) {
        if (resources[i].name && k_strcmp(resources[i].name, name) == 0) {
            if (resources[i].owner == owner_id) {
                resources[i].owner = 0;
                return 0;
            }
            return -1; /* not owner */
        }
    }
    return -2; /* not found */
}
