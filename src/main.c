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

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>
#include "mfsum.h"
#include "stack.h"
#include "memory.h"
#include "config.h"

#define MAX_WIDTH	21
#define ERROR_NARG	"Wrong number of arguments\n\n"
#define ERROR_TARG	"Target value must be a positive integer\n\n"
#define	ERROR_APPX	"Approximation value must be the range 0 to < 1\n\n"
#define ERROR_SET	"Set must only contain positive integers\n\n"

void print_usage(void)
{
    fprintf(stderr, "%s : subset sum solver\n", PACKAGE_STRING);
    fprintf(stderr, "usage:\n");
    fprintf(stderr, "%s <target sum> <approx> <number 1> ... <number N>\n", PACKAGE_NAME);
}

void fail_msg(const char *msg)
{
    fprintf(stderr, "%s", msg);
    print_usage();
    exit(EXIT_FAILURE);
}

size_t read_line(char *buff, size_t max_len, FILE *stream)
{
    char *result = fgets(buff, max_len, stream);
    if (result == NULL) return 0;
    int len = strlen(buff);
    if (len == 0) return 0;
    if (buff[len-1] == '\n') buff[len-1] = 0;
    return len;
}

bool valid_int(char *buff)
{
    if (buff == NULL) return false;
    for (size_t i = 0; i < strlen(buff); i++)
    {
        if (buff[i] < 0x30 ||
            buff[i] > 0x39)
            return false;
    }
    return true;
}

bool valid_float(char *buff)
{
    if (buff == NULL) return false;
    for (size_t i = 0; i < strlen(buff); i++)
    {
        if ((buff[i] < 0x30 ||
             buff[i] > 0x39) &&
             buff[i] != 0x2E)
            return false;
    }
    return true;
}

int main(int argc, char *argv[])
{
    list_t target, *input;
    double approx;
    size_t length;
    bool stdin_mode = false;

    if (argc < 2) fail_msg(ERROR_NARG);

    /* Standard-in mode */
    if (*argv[1] == '-')
    {
        char temp[MAX_WIDTH];
        stdin_mode = true;

        if (read_line(temp, MAX_WIDTH, stdin) == 0)
            fail_msg(ERROR_NARG);
        if (!valid_int(temp))
            fail_msg(ERROR_TARG);
        target = atoll(temp);

        if (read_line(temp, MAX_WIDTH, stdin) == 0)
            fail_msg(ERROR_NARG);
        if (!valid_float(temp))
            fail_msg(ERROR_APPX);
        approx = atof(temp);

        length = 0;
        input = malloc(sizeof *input);
        while (read_line(temp, MAX_WIDTH, stdin) != 0)
        {
            if (!valid_int(temp))
                fail_msg(ERROR_SET);
            input[length++] = atoll(temp);
            input = realloc(input, (length + 1) * sizeof *input);
        }
        if (length == 0)
            fail_msg(ERROR_NARG);
    }
    /* Command line argument mode */
    else
    {
        stdin_mode = false;
        if (argc < 4)
            fail_msg(ERROR_NARG);

        if (!valid_int(argv[1]))
            fail_msg(ERROR_TARG);	
        target = atoll(argv[1]);

        if (!valid_float(argv[2]))
            fail_msg(ERROR_APPX);
        approx = atof(argv[2]);

        length = argc - 3;
        input = malloc(length * sizeof *input);
        for (size_t i = 0; i < length; i++)
        {
            if (!valid_int(argv[3 + i]))
                fail_msg(ERROR_SET);
            input[i] = atoll(argv[3 + i]);
        }
    }

    /* Set up the job & run the calculation */
    result_t *result = NULL;
    job_t job;
    job.set  = input;
    job.lset = length;
    job.tsum = target;
    job.appx = approx;
    ret_t ret = mfsum(&result, &job);
    switch (ret)
    {
    case ESOL:
        if (stdin_mode)
        {
            for (size_t i = 0; i < result->lsum; i++)
                printf("%" PRIu64 "\n", result->sum[i]);
        }
        else
        {
            printf("%" PRIu64 " = ", result->rsum);
            for (size_t i = 0; i < result->lsum; i++)
            {
                if (i == result->lsum-1) printf("%" PRIu64 , result->sum[i]);
                else printf("%" PRIu64 " + ", result->sum[i]);
            }
            putchar('\n');
        }
        break;
    case ENOSOL:
        printf("No solution.\n");
        break;
    case ENEGTAR:
        fail_msg(ERROR_TARG);
    case ENEGSET:
        fail_msg(ERROR_SET);
    case EAPPXOOR:
        fail_msg(ERROR_APPX);
    }

    free(job.set);
    free_result(result);
#ifdef DEBUG
    print_heap_usage();
#endif
}
