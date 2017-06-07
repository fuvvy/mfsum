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
#include "mfsum.h"
#include "stack.h"
#include "uthash.h"

stack *stack_hash = NULL;

/* list_t comparison function */
int list_cmp(const void *a, const void *b)
{
    list_t ia, ib;
    ia = *(const list_t *)a;
    ib = *(const list_t *)b;
    return (ia < ib) ? -1 : ((ia == ib) ? 0 : 1);
}

/* integer sort function */
void sort_list(list_t array[], size_t len)
{
    qsort(array, len, sizeof *array, list_cmp);
}

/* Delete and free a stack from the hash */
void del_hash_item(int key)
{
    stack *s;
    HASH_FIND_INT(stack_hash, &key, s);
    if (s != NULL)
    {
        HASH_DEL(stack_hash, s);
        stack_free(s);
    }
}

/* Add value x to each member of array A. */
void add_to_list(list_t A[], size_t a, list_t x)
{
    stack *s;
    for (int i = a - 1; i >= 0; i--)
    {
        int find_val = (int)A[i];
        HASH_FIND_INT(stack_hash, &find_val, s);
        if (s == NULL)
        {
            stack_init(&s);
            stack_push(s, A[i]);
        }
        else
        {
            stack *copy;
            HASH_DEL(stack_hash, s);
            stack_copy(&copy, s);
            HASH_ADD_INT(stack_hash, top_val, copy);
        }
        A[i] += x;
        stack_push(s, A[i]);
        HASH_ADD_INT(stack_hash, top_val, s);
    }
}

/*
 * Merge two arrays. The input lists must be sorted.
 * The input lists will be free'd.
 */
size_t merge_lists(list_t **R, list_t A[], list_t B[], size_t a, size_t b)
{
    size_t i, j, k;

    /* Merge the lists */
    i = j = k = 0;
    list_t *temp1 = malloc((a + b) * sizeof *temp1);
    while (1)
    {
        if (A[i] <= B[j])
        {
            temp1[k] = A[i];
            i++;
        }
        else if (A[i] > B[j])
        {
            temp1[k] = B[j];
            j++;
        }
        k++;
        if (i == a)
        {
            for (; j < b; j++, k++) temp1[k] = B[j];
            break;
        }
        if (j == b)
        {
            for (; i < a; i++, k++) temp1[k] = A[i];
            break;
        }
    }

    /* Remove duplicates */
    list_t *temp2 = malloc((a + b) * sizeof *temp2);
    for (i = 0, k = 0; i < (a + b);)
    {
        temp2[k] = temp1[i];
        for (j = i + 1; j < (a + b); j++)
        {
            if (temp1[j] != temp1[i]) break;
            if (temp1[j] == temp1[i]) del_hash_item((int)temp1[j]);
        }
        i = j;
        k++;
    }

    *R = malloc(k * sizeof **R);
    memcpy(*R, temp2, k * sizeof **R);
    free(temp1);
    free(temp2);
    free(A);
    free(B);
    return k;
}

/* The input list MUST be sorted. */
size_t trim_list(list_t **R, list_t L[], size_t l, list_t x)
{
    size_t i;

    /* Remove all elements > the target sum */
    for (i = 0; i < l; i++) if (L[i] > x) break;
    for (size_t j = i; j < l; j++)
    {
        stack *s;
        int find_val = (int)L[j];
        HASH_FIND_INT(stack_hash, &find_val, s);
        if (s != NULL)
        {
            HASH_DEL(stack_hash, s);
            stack_pop(s);
            HASH_ADD_INT(stack_hash, top_val, s);
        }
    }

    *R = malloc(i * sizeof **R);
    memcpy(*R, L, i * sizeof **R);
    free(L);
    return i;
}

/* The input list MUST be sorted */
size_t prune_list(list_t **R, list_t L[], size_t l, double appx)
{
    list_t *temp = malloc(l * sizeof *temp);
    temp[0] = L[0];
    list_t last = L[0];
    size_t count = 1;
    for (size_t i = 1; i < l; i++)
    {
        if (L[i] > last * (1.0 + appx))
        {
            temp[count++] = L[i];
            last = L[i];
        }
        else del_hash_item((int)L[i]);
    }
    *R = malloc(count * sizeof **R);
    memcpy(*R, temp, count * sizeof **R);
    free(L);
    free(temp);
    return count;
}

/* Free the global stack-hash object */
void free_hash(void)
{
    stack *s, *tmp;
    HASH_ITER(hh, stack_hash, s, tmp)
    {
        HASH_DEL(stack_hash, s);
        stack_free(s);
    }
    HASH_CLEAR(hh, stack_hash);
}

/* Calculate the subset sum of set S given target t */
ret_t mfsum(result_t **res, job_t *job)
{
    *res = malloc(sizeof **res);
    (*res)->sum = NULL;
    (*res)->lsum = 0;
    (*res)->rsum = 0;

    if (job->tsum < 0) return ENEGTAR;
    if (job->appx < 0 || job->appx >= 1) return EAPPXOOR;
    for (size_t i = 0; i < job->lset; i++)
        if (job->set[i] < 0) return ENEGSET;

    /* Perform the calculation */
    size_t length = 1;
    list_t *list = malloc(length * sizeof *list);
    list[0] = 0;
    sort_list(job->set, job->lset);
    double epsilon = job->appx / (2.0 * job->lset);
    for (size_t i = 0; i < job->lset; i++)
    {
        list_t *temp = malloc(length * sizeof *temp);
        memcpy(temp, list, length * sizeof *list);
        add_to_list(temp, length, job->set[i]);
        length = merge_lists(&list, list, temp, length, length);
        if (job->appx > 0) length = prune_list(&list, list, length, epsilon);
        length = trim_list(&list, list, length, job->tsum);
    }

    /* Retreive the solution (or closest solution) from the hash */
    stack *ss;
    int find_val = (int)list[length - 1];
    HASH_FIND_INT(stack_hash, &find_val, ss);

    /* Pack the solution into the result object */
    (*res)->rsum = list[length - 1];
    (*res)->lsum = ss->top;
    (*res)->sum = malloc((*res)->lsum * sizeof(list_t));
    for (size_t i = ss->top; i > 0; i--)
        (*res)->sum[i-1] = ss->memory[i] - ss->memory[i-1];

    /* Set the right return code */
    ret_t ret;
    if (job->appx == 0)
    {
        if ((*res)->rsum == job->tsum) ret = ESOL;
        else ret = ENOSOL;
    }
    else
    {
        double approx_bound = job->tsum - job->tsum * job->appx;
        if ((*res)->rsum >= approx_bound) ret = ESOL;
        else ret = ENOSOL;
    }
    free(list);
    free_hash();
    return ret;
}

/* Print the entire contents of the stack-hash object */
void print_hash(void)
{
    stack *s, *tmp;
    HASH_ITER(hh, stack_hash, s, tmp)
    {
        stack_print(s);
        putchar('\n');
    }
}

/* integer array printing function */
void print_list(const list_t array[], size_t len)
{
    printf(" | ");
    for(size_t i = 0; i < len; i++) printf("%" PRIu64 " | ", array[i]);
    putchar('\n');
}

/* Helper for releasing result_t objects */
void free_result(result_t *r)
{
    free(r->sum);
    free(r);
}
