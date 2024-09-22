/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 12:06:24 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/22 16:11:28 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include "texture.h"
#include "color.h"
#include "interval.h"
#include "rtw_stb_image.h"
#include <stdio.h>
#include "utils.h"
#include "interval.h"

void	solid_color_init(t_solid_color *solid_color_texture, t_color albedo)
{
	solid_color_texture->base.value = solid_color_value;
	solid_color_texture->color_albedo = albedo;
}	

t_color solid_color_value(const void *self, double u, double v, const t_point3 *p)
{
	(void)u;
	(void)v;
	(void)p;
	t_solid_color *solid_color = (t_solid_color *)self;
	return (solid_color->color_albedo);
}

/*
** Checker texture
* colors for the checkerboard ex
* even_color = color(0.5, 0.0, 0.5); // Purple
* odd_color = color(1.0, 1.0, 1.0); // White
*/
void	checker_texture_init(t_checker_texture *checker_texture, double scale, t_solid_color *even, t_solid_color *odd)
{
	checker_texture->base.value = checker_texture_value;
	checker_texture->inv_scale = 1.0 / scale;
	checker_texture->even = even;
	checker_texture->odd = odd;
}

#include <math.h>
#include <stdbool.h>

// Function to compute spherical coordinates (u, v) from point p
void get_spherical_uv(const t_point3 *p, double *u, double *v) {
    double phi = atan2(p->z, p->x);   // Azimuthal angle
    double theta = acos(p->y);        // Polar angle

    *u = (phi + PI) / (2 * PI);   // u is in the range [0, 1]
    *v = theta / PI;                // v is in the range [0, 1]
}

// // Modified checker texture function
// t_color checker_texture_value(const void *self, double u, double v, const t_point3 *p) {
//     // Compute spherical coordinates (u, v) based on the point p
//     get_spherical_uv(p, &u, &v);

//     // Scale the texture coordinates
//     double scaled_u = u * ((t_checker_texture*)self)->inv_scale;
//     double scaled_v = v * ((t_checker_texture*)self)->inv_scale;

//     // Determine if we are in an even or odd square of the checker pattern
//     int u_int = (int)floor(scaled_u);
//     int v_int = (int)floor(scaled_v);

//     bool is_even = (u_int + v_int) % 2 == 0;

//     // Return the corresponding color
//     if (is_even)
//         return ((t_checker_texture*)self)->even->color_albedo;
//     else
//         return ((t_checker_texture*)self)->odd->color_albedo;
// }



t_color checker_texture_value(const void *self, double u, double v, const t_point3 *p)
{
	(void)u;
	(void)v;
	int xint = (int)floor(p->x * ((t_checker_texture*)self)->inv_scale);	
	int yint = (int)floor(p->y * ((t_checker_texture*)self)->inv_scale);
	int zint = (int)floor(p->z * ((t_checker_texture*)self)->inv_scale);


	bool is_even = (xint + yint + zint) % 2 == 0;
	if (is_even)
		return (((t_checker_texture*)self)->even->color_albedo);
	else
		return (((t_checker_texture*)self)->odd->color_albedo);
}

void	img_texture_init(t_img_texture *img_texture, t_rtw_image *img)
{
	img_texture->base.value = img_texture_value;
	img_texture->img = img;
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
t_color img_texture_value(const void *self, double u, double v, const t_point3 *p)
{
	(void)p;
	t_img_texture *image;
	
	image = (t_img_texture *)self;
	if (height(image->img) <= 0) 
		return color(0, 1, 1);
	int i = (int)(clamp(interval(0, 1), u) * width(image->img));
	int j = (int)((1.0 - clamp(interval(0, 1), v)) * height(image->img));
	uint8_t *pixel = pixel_data(image->img, i, j);
	return rgb_to_color(rgb(*pixel, *(pixel + 1), *(pixel + 2)));
}