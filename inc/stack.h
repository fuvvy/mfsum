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

#ifndef STACK_H_INCLUDE
#define STACK_H_INCLUDE

#include <stddef.h>
#include <stdint.h>
#include "uthash.h"

#define INIT_SIZE	512
#define STACK_EMPTY	-1

#define stack_empty(sp) (sp->top == STACK_EMPTY)
#define stack_full(sp) (sp->top == sp->size - 1)

typedef uint64_t stack_e;

typedef struct
{
    stack_e *memory;
    size_t size;
    size_t top;
    int top_val;
    UT_hash_handle hh;
} stack;

void stack_init(stack **sp);
void stack_free(stack *sp);
void stack_copy(stack **out, stack *in);
void stack_push(stack *sp, stack_e element);
stack_e stack_pop(stack *sp);
stack_e stack_peek(stack *sp);
void stack_print(stack *sp);

#endif /* STACK_H_INCLUDE */
