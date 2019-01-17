/*
 *  mfsum : A Subset Sum Solver
 *  Copyright 2018 Mike Fusaro
 *
 *  This file is part of mfsum.
 *
 *  mfsum is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  mfsum is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with mfsum.  If not, see <http://www.gnu.org/licenses/>.
 */

#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

static void *(*libc_malloc)(size_t) = NULL;
static uint64_t allocated = 0;

void print_heap_usage(void)
{
    fprintf(stderr, "%" PRIu64 " bytes allocated\n", allocated);
}

static void __mtrace_init(void)
{
    libc_malloc = dlsym(RTLD_NEXT, "malloc");
    if (libc_malloc == NULL)
    {
        fprintf(stderr, "Error in `dlsym`: %s\n", dlerror());
        return;
    }
}

void* malloc(size_t size)
{
    if (libc_malloc == NULL) __mtrace_init();
    if (size == 0) return 0;
    void *p = libc_malloc(size);
    if (p == NULL)
    {
        fprintf(stderr, "Unable to allocate %ld bytes\n", size);
        print_heap_usage();
        exit(EXIT_FAILURE);
    }
    allocated += size;
    return p;
}
