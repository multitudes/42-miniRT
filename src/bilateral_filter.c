/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bilateral_filter.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 08:24:08 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/05 10:30:44 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MLX42/MLX42.h"
#include "bilateral_filter.h"

/**
 * @brief gaussian function
 *
 * @param x
 * @param sigma
 *
 * @return double
 *
 * implements the Gaussian probability density function (PDF),
 * often referred to as the normal distribution or bell curve.
 * It's a widely used statistical distribution in various fields,
 * including signal processing, image processing, and machine learning.
 * The Gaussian PDF describes a continuous probability distribution
 * that is symmetric around its mean (which is usually 0 in this context)
 * and has a characteristic bell shape.
 *
 */
double	gaussian(double x, double sigma)
{
	double	res;

	res = (1 / (sigma * sqrt(2 * PI))) * exp(-(x * x) / (2 * sigma * sigma));
	return (res);
}

uint32_t	pixel_value(t_bil_filter bf)
{
	uint32_t	result;

	result = 0;
	result |= (uint8_t)(fmax(0, fmin(bf.acc.r / bf.w_sum, 1.0)) * 255) << 24;
	result |= (uint8_t)(fmax(0, fmin(bf.acc.g / bf.w_sum, 1.0)) * 255) << 16;
	result |= (uint8_t)(fmax(0, fmin(bf.acc.b / bf.w_sum, 1.0)) * 255) << 8;
	result |= 0xFF;
	return (result);
}

void	get_bil_weight(t_bil_filter *bf, mlx_image_t *image, uint32_t xy[2],
		int i[2])
{
	bf->center_pixel = &image->pixels[(xy[1] * image->width + xy[0]) * 4];
	bf->neighbor = rgb_to_color(rgb(bf->neighbor_pixel[0],
				bf->neighbor_pixel[1], bf->neighbor_pixel[2]));
	bf->center = rgb_to_color(rgb(bf->center_pixel[0], bf->center_pixel[1],
				bf->center_pixel[2]));
	bf->spatial_weight = gaussian(sqrt(i[0] * i[0] + i[1] * i[1]),
			bf->sigmas->s);
	bf->range_weight = gaussian(sqrt((bf->neighbor.r - bf->center.r)
				* (bf->neighbor.r - bf->center.r) + (bf->neighbor.g
					- bf->center.g) * (bf->neighbor.g - bf->center.g)
				+ (bf->neighbor.b - bf->center.b) * (bf->neighbor.b
					- bf->center.b)), bf->sigmas->r);
	bf->weight = bf->spatial_weight * bf->range_weight;
	bf->acc.r += bf->neighbor.r * bf->weight;
	bf->acc.g += bf->neighbor.g * bf->weight;
	bf->acc.b += bf->neighbor.b * bf->weight;
	bf->w_sum += bf->weight;
}

void	init_bilateral_filter(t_bil_filter *bf, t_sigmas *sigmas)
{
	*bf = (t_bil_filter){0};
	bf->sigmas->s = sigmas->s;
	bf->sigmas->r = sigmas->r;
	bf->radius = (int)(2 * sigmas->s);
}

/**
 * @brief apply the bilateral filter to a single pixel
 *
 * @param image the image to apply the filter to
 * @param x the x coordinate of the pixel
 * @param y the y coordinate of the pixel
 * @param sigma_s the spatial sigma
 * @param sigma_r the range sigma
 *
 */
uint32_t	bilateral_filter_pixel(mlx_image_t *image, uint32_t xy[2],
		t_sigmas sigmas)
{
	uint32_t				nx;
	uint32_t				ny;
	int				i[2];
	t_bil_filter	bf;

	init_bilateral_filter(&bf, &sigmas);
	i[0] = -bf.radius;
	while (i[0] <= bf.radius)
	{
		i[1] = -bf.radius;
		while (i[1] <= bf.radius)
		{
			nx = xy[0] + i[0];
			ny = xy[1] + i[1];
			if (nx >= 0 && nx < image->width && ny >= 0 && ny < image->height)
			{
				bf.neighbor_pixel = &image->pixels[(ny * image->width + nx)
					* 4];
				get_bil_weight(&bf, image, xy, i);
			}
			++i[1];
		}
		++i[0];
	}
	return (pixel_value(bf));
}
