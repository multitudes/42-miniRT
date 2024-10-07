/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mersenne_twister.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 13:06:58 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/07 17:09:30 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// done with copilot as test

#include <pthread.h>
#include <stdint.h>
#include <stdio.h>

// #define MT_N 624
// #define MT_M 397
// #define MT_MATRIX_A 0x9908b0dfUL
// #define MT_UPPER_MASK 0x80000000UL
// #define MT_LOWER_MASK 0x7fffffffUL

// typedef struct s_mt_state
// {
// 	uint32_t	mt[MT_N];
// 	int			mti;
// }				t_mt_state;

// static __thread mt_state state;

/**
 * @brief Initialize the generator with a seed
 * This is only for a demonstration of different 
 * random generators
 * 
 * @param s seed
 */
void	mt_init_genrand(uint32_t s)
{
	(void)s;
}

// void	mt_init_genrand(uint32_t s)
// {
// 	state.mt[0] = s & 0xffffffffUL;
// 	for (state.mti = 1; state.mti < MT_N; state.mti++)
// 	{
// 		state.mt[state.mti] = (1812433253UL * (state.mt[state.mti
// 					- 1] ^ (state.mt[state.mti - 1] >> 30)) + state.mti);
// 		state.mt[state.mti] &= 0xffffffffUL;
// 	}
// }

// uint32_t	mt_genrand_int32(void)
// {
// 	uint32_t y;
// 	static uint32_t mag01[2] = {0x0UL, MT_MATRIX_A};

// 	if (state.mti >= MT_N)
// 	{
// 		int kk;

// 		if (state.mti == MT_N + 1)
// 			mt_init_genrand(5489UL);

// 		for (kk = 0; kk < MT_N - MT_M; kk++)
// 		{
// 			y = (state.mt[kk] & MT_UPPER_MASK) | (state.mt[kk
// 					+ 1] & MT_LOWER_MASK);
// 			state.mt[kk] = state.mt[kk + MT_M] ^ (y >> 1) ^ mag01[y & 0x1UL];
// 		}
// 		for (; kk < MT_N - 1; kk++)
// 		{
// 			y = (state.mt[kk] & MT_UPPER_MASK) | (state.mt[kk
// 					+ 1] & MT_LOWER_MASK);
// 			state.mt[kk] = state.mt[kk + (MT_M
// 					- MT_N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
// 		}
// 		y = (state.mt[MT_N
// 				- 1] & MT_UPPER_MASK) | (state.mt[0] & MT_LOWER_MASK);
// 		state.mt[MT_N - 1] = state.mt[MT_M - 1] ^ (y >> 1) ^ mag01[y & 0x1UL];

// 		state.mti = 0;
// 	}

// 	y = state.mt[state.mti++];

// 	y ^= (y >> 11);
// 	y ^= (y << 7) & 0x9d2c5680UL;
// 	y ^= (y << 15) & 0xefc60000UL;
// 	y ^= (y >> 18);

// 	return (y);
// }