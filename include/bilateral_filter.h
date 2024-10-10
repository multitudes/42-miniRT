/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bilateral_filter.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 08:24:40 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/10 19:40:59 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BILATERAL_FILTER_H
# define BILATERAL_FILTER_H

# include "MLX42/MLX42.h"
# include "minirt.h"

typedef struct s_sigmas
{
	double		s;
	double		r;
}				t_sigmas;

typedef struct s_bil_filter
{
	t_sigmas	sigmas[2];
	t_color		acc;
	double		w_sum;
	int			radius;
	uint8_t		*neighbor_pixel;
	uint8_t		*center_pixel;
	t_color		neighbor;
	t_color		center;
	double		spatial_weight;
	double		range_weight;
	double		weight;
}				t_bil_filter;

void			apply_bilateral_filter(t_mrt *data);
uint32_t		bilateral_filter_pixel(mlx_image_t *image, uint32_t xy[2],
					t_sigmas sigmas);
double			gaussian(double x, double sigma);
uint32_t		pixel_value(t_bil_filter bf);
void			write_filtered_px(t_mrt *data, uint32_t xy[2], t_sigmas sigmas);
void			get_bil_weight(t_bil_filter *bf, mlx_image_t *image, \
								uint32_t xy[2], int i[2]);
void			init_bilateral_filter(t_bil_filter *bf, t_sigmas *sigmas);

#endif