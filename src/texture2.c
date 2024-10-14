/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 17:33:06 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/14 16:08:47 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "color.h"
#include "interval.h"
#include "texture.h"
#include "utils.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include "libft.h"

/**
 * @brief Initialize an image texture with the given image
 *
 * @param img_texture The image texture object
 * @param img The image to use as texture from the t_rtw_image struct
 */
void	img_texture_init(t_img_texture *img_texture, char *filename)
{
	img_texture->base.value = img_texture_value;
	img_texture->bytes_per_pixel = 3;
	img_texture->fdata = NULL;
	img_texture->bdata = NULL;
	img_texture->image_width = 0;
	img_texture->image_height = 0;
	img_texture->bytes_per_scanline = 0;
	ft_printf("filename = %s\n", filename);
	if (load(img_texture, filename) == 0)
	{
		write(2, "Failed to load image\n", 21);
		exit(1);
	}
	ft_printf("Image loaded\n");
}

/**
 * @brief Get the color of the texture at the given coordinates
 *
 * @param self The texture object
 * @param u The u texture coordinate
 * @param v The v texture coordinate
 * @param p The point in 3D space
 * @return t_color The color of the texture at the given coordinates
 *
 * If we have no texture data, then return solid cyan as a debugging aid.
 * The pixel is a pointer to the first byte of the RGB triplet.
 */
t_color	img_texture_value(const void *self, double uv[2], const t_point3 *p)
{
	t_img_texture	*image;
	int				i;
	int				j;
	unsigned char	*pixel;
	double			color_scale;

	(void)p;
	image = (t_img_texture *)self;
	if (height(image) <= 0)
		return (color(0, 1, 1));
	uv[0] = clamp(interval(0, 1), uv[0]);
	uv[1] = 1.0 - clamp(interval(0, 1), uv[1]);
	i = (int)(uv[0] * width(image));
	j = (int)(uv[1] * height(image));
	pixel = pixel_data(image, i, j);
	color_scale = 1.0 / 255.0;
	return (color((*pixel * color_scale), (*(pixel + 1) * color_scale), \
			(*(pixel + 2) * color_scale)));
}
