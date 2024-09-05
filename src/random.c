/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   random.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 12:25:08 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/05 12:26:27 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


// Define constants for the LCG
#define A 1664525
#define C 1013904223
#define M 4294967296 // 2^32

// Function to generate the next random number
unsigned int rand_rt() 
{
	unsigned int seed = 1;
    seed = (A * seed + C) % M;
    return seed;
}