/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rtw_stb_image2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 17:12:33 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/02 17:18:02 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "texture.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * @brief Clamp a value to a range and returns it. 
 *
 * This is the int version for the image mapping
 */
int	clamp_rtw(int x, int low, int high)
{
	if (x < low)
		return (low);
	if (x < high)
		return (x);
	return (high - 1);
}

/*
 * Convert a floating point value in the range [0.0, 1.0]
 * to an unsigned byte value in the range [0, 255].
 */
uint8_t	float_to_byte(float value)
{
	if (value <= 0.0)
		return (0);
	if (1.0 <= value)
		return (255);
	return ((unsigned char)(255.0 * value));
}

/*
 * @brief Convert the linear floating point pixel data to bytes,
 *
 * stores the resulting byte data in the `bdata` member.
 */
void	convert_to_bytes(t_img_texture *img)
{
	int		i;
	uint8_t	*bptr;
	float	*fptr;
	int		total_bytes;

	i = 0;
	total_bytes = img->image_width * img->image_height * img->bytes_per_pixel;
	printf("img width height and bytes per pixel = %d %d %d\n",
		img->image_width, img->image_height, img->bytes_per_pixel);
	printf("total_bytes = %d\n", total_bytes);
	img->bdata = malloc(total_bytes * sizeof(unsigned char));
	if (img->bdata == NULL)
	{
		fprintf(stderr, "Failed to allocate memory for image data\n");
		exit(1);
	}
	bptr = img->bdata;
	fptr = img->fdata;
	while (i < total_bytes)
	{
		*bptr = float_to_byte(*fptr);
		i++;
		fptr++;
		bptr++;
	}
}

/**
 * @brief Get the image width
 * @param img the image texture
 */
int	width(const t_img_texture *img)
{
	if (img->fdata == NULL)
		return (0);
	return (img->image_width);
}

/**
 * @brief Get the image height
 * @param img the image texture
 */
int	height(const t_img_texture *img)
{
	if (img->fdata == NULL)
		return (0);
	return (img->image_height);
}
