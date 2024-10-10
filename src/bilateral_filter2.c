/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bilateral_filter2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 16:29:16 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/07 13:08:19 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MLX42/MLX42.h"
#include "bilateral_filter.h"
#include "debug.h"

void	write_filtered_px(t_mrt *data, uint32_t xy[2], t_sigmas sigmas)
{
	uint32_t	filt_px;
	uint32_t	offset;

	filt_px = bilateral_filter_pixel(data->image, xy, sigmas);
	offset = (xy[1] * data->image->width + xy[0]) * 4;
	data->filt_img->pixels[offset + 0] = (uint8_t)(filt_px >> 24);
	data->filt_img->pixels[offset + 1] = (uint8_t)((filt_px >> 16));
	data->filt_img->pixels[offset + 2] = (uint8_t)((filt_px >> 8));
	data->filt_img->pixels[offset + 3] = data->image->pixels[offset + 3];
}

/**
 * @brief apply the bilateral filter to the image
 *
 * sigma_s (Spatial Sigma): Controls the spatial extent of the filter.
 * Increasing sigma_s will make the filter consider a larger neighborhood,
 * resulting in stronger smoothing.
 * sigma_r (Range Sigma): Controls the intensity difference that the
 * filter considers.
 * Increasing sigma_r will make the filter less sensitive to intensity
 * differences, resulting in stronger smoothing.
 */
void	apply_bilateral_filter(t_mrt *data)
{
	t_sigmas	sigmas;
	double		start;
	uint32_t	xy[2];

	sigmas = (t_sigmas){0.5, 0.5};
	start = mlx_get_time();
	data->filt_img = mlx_new_image(data->mlx, data->image->width,
			data->image->height);
	xy[1] = 0;
	while (xy[1] < data->image->height)
	{
		xy[0] = 0;
		while (xy[0] < data->image->width)
		{
			write_filtered_px(data, xy, sigmas);
			++xy[0];
		}
		++xy[1];
	}
	mlx_image_to_window(data->mlx, data->filt_img, 0, 0);
	mlx_delete_image(data->mlx, data->image);
	data->image = data->filt_img;
	debug("Bilateral filter applied in %.2f seconds\n", mlx_get_time() - start);
}
