/*
 *  mfsum : A Subset Sum Solver
 *  Copyright 2013 Mike Fusaro
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

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "stack.h"

void stack_init(stack **sp)
{
    *sp = malloc(sizeof **sp);
    stack_e *new_memory = malloc(INIT_SIZE * sizeof(stack_e));
    (*sp)->memory = new_memory;
    (*sp)->size = INIT_SIZE;
    (*sp)->top = STACK_EMPTY;
}

void stack_free(stack *sp)
{
    free(sp->memory);
    free(sp);
}

void stack_grow(stack *sp)
{
    sp->memory = realloc(sp->memory, (sp->size * 2) * sizeof(stack_e));
    sp->size *= 2;
}

void stack_copy(stack **target, stack *source)
{
    *target = malloc(sizeof **target);
    stack_e *copied_memory = malloc(source->size * sizeof(stack_e));
    memcpy(copied_memory, source->memory, source->size * sizeof(stack_e));
    (*target)->memory = copied_memory;
    (*target)->size = source->size;
    (*target)->top = source->top;
    (*target)->top_val = source->top_val;
}

void stack_push(stack *sp, stack_e element)
{
    if (stack_full(sp)) stack_grow(sp);
    sp->top_val = (int)element;
    sp->memory[++sp->top] = element;
}

stack_e stack_pop(stack *sp)
{
    if (stack_empty(sp)) return STACK_EMPTY;
    sp->top_val = sp->memory[sp->top - 1];
    return sp->memory[sp->top--];
}

stack_e stack_peek(stack *sp)
{
    if (stack_empty(sp)) return STACK_EMPTY;
    return sp->memory[sp->top];
}

void stack_print(stack *sp)
{
    size_t save_top = sp->top;
    stack_e save_top_val = sp->top_val;
    while (!stack_empty(sp)) printf("%" PRIu64 "\n", stack_pop(sp));
    sp->top_val = save_top_val;
    sp->top = save_top;
}
