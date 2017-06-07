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
#include <limits.h>
#include <inttypes.h>
#include <time.h>
#include "memory.h"
#include "mfsum.h"

#define SET_SIZE    15
#define TRIALS      100

void seed_rand(void)
{
    srand(time(NULL));    
}

int next_rand(int min, int max)
{
    return min + (rand() % (int)(max - min + 1));
}

int exists_array(list_t target, list_t *set, size_t len)
{
    for (size_t i = 0; i < len; i++)
        if (set[i] == target) return 1;
    return 0;
}

int main(int argc, char *argv[])
{
    unsigned int min, max;

    seed_rand();
    list_t *set = malloc(SET_SIZE * sizeof *set);

    for (size_t t = 0; t < TRIALS; t++)
    {
        // Fill up the search set
        min = 1;
        max = RAND_MAX;
        for (size_t i = 0; i < SET_SIZE; i++)
            set[i] = next_rand(min, max);

        // Allocate pseudo-random sized solution set
        min = 0;
        max = SET_SIZE;
        size_t subset_size = next_rand(min, max);
        list_t *subset = malloc(subset_size * sizeof *subset);

        // Fill the solution array while avoiding duplicate draws
        max = SET_SIZE - 1;
        list_t sum = 0;
        for (size_t i = 0; i < subset_size; i++)
        {
            int draw_index;
            do draw_index = next_rand(min, max);
            while (exists_array(set[draw_index], subset, subset_size));
            subset[i] = set[draw_index];
            sum += set[draw_index];
        }
        
        printf("++++++++++++++++++++++++++++++++++++++++\n");
        printf("Round %zu\n", t+1);
        printf("Set :\n");
        print_list(set, SET_SIZE);
        printf("Subset :\n");
        print_list(subset, subset_size);
        printf("Target sum : %" PRIu64 "\n", sum);

        // Run mfsum on the problem and make sure it finds a correct solution
        result_t *result = NULL;
        job_t test_job;
        test_job.set    = set;
        test_job.lset   = SET_SIZE;
        test_job.tsum   = sum;
        test_job.appx   = 0;
        ret_t ret = mfsum(&result, &test_job);
        printf("Result -> ");
        if (ret != ESOL)
        {
            printf("Fail\n");
            exit(EXIT_FAILURE);
        }
        else printf("Pass\n");
        free(subset);
        free(result);
    }
    printf("++++++++++++++++++++++++++++++++++++++++\n");
    exit(EXIT_SUCCESS);  
}
