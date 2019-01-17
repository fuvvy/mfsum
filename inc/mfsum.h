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

#ifndef MFSUM_H_INCLUDE
#define MFSUM_H_INCLUDE

#include <stdint.h>

typedef uint64_t list_t;
typedef enum {ENOSOL, ESOL, ENEGTAR, ENEGSET, EAPPXOOR} ret_t;

typedef struct
{
	list_t	*set;	/* pointer to input set */
	size_t	lset;	/* length of input set */
	list_t	tsum;	/* target sum */
	double	appx;	/* approximation parameter - must be in range 0 =< appx < 1 */
} job_t;

typedef struct
{
	list_t *sum;	/* pointer to resulting sum */
	size_t lsum;	/* length of the sum array */
	list_t rsum;	/* resulting sum - may not equal the target sum in approximation mode */
} result_t;

ret_t mfsum(result_t **, job_t *);
void print_list(const list_t[], size_t);
void print_hash(void);
void free_result(result_t *);

#endif /* MFSUM_H_INCLUDE */
