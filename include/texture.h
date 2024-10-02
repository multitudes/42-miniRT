/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 12:02:39 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/02 16:18:13 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEXTURE_H
# define TEXTURE_H

# include "color.h"
# include "vec3.h"

/**
 * @brief Texture structure
 * @param value: function pointer to get the color of the texture at
 * a specific point. for example a solid color texture will return
 * the same color for every point, a checker texture will return
 * different colors for different points. An image texture will
 * return the color of the pixel at those coordinates
 */
typedef struct s_texture
{
	t_color			(*value)(const void *self, double uv[2], const t_point3 *p);
}					t_texture;

/**
 * @brief Solid color texture
 * @param base: the base texture which is a pointer to the texture struct
 * containing the value function pointer
 * @param color_albedo: the color of the texture
 */
typedef struct s_solid_color
{
	t_texture		base;
	t_color			color_albedo;
}					t_solid_color;

/**
 * @brief Checker texture
 * @param base: the base texture which is a pointer to the texture struct
 * containing the value function pointer
 * @param inv_scale: the inverse scale of the checker texture, the higher
 * the scale the smaller the squares
 * @param even: the even color of the checker texture
 * @param odd: the odd color of the checker texture
 */
typedef struct s_checker_texture
{
	t_texture		base;
	double			inv_scale;
	t_solid_color	even;
	t_solid_color	odd;
}					t_checker_texture;

/**
 * @brief Image texture
 * @param base: the base texture which is a pointer to the texture struct
 * containing the value function pointer
 * @param filename: the filename of the image
 * The following fields are used to store the image data:
 * @param bytes_per_pixel: the number of bytes per pixel
 * @param fdata: the linear floating point pixel data
 * @param bdata: the linear 8-bit pixel data
 * @param image_width: the loaded image width
 * @param image_height: the loaded image height
 * @param bytes_per_scanline: the number of bytes per scanline
 *
 */
typedef struct s_img_texture
{
	t_texture		base;
	char			*filename;
	int				bytes_per_pixel;
	float			*fdata;
	unsigned char	*bdata;
	int				image_width;
	int				image_height;
	int				bytes_per_scanline;
}					t_img_texture;

void				solid_color_init(t_solid_color *solid_color_texture,
						t_color albedo);
t_color				solid_color_value(const void *self, double u, double v,
						const t_point3 *p);

void				checker_texture_init(t_checker_texture *checker_texture,
						double scale, t_rgb even_rgb, t_rgb odd_rgb);
t_color				checker_texture_value(const void *self, double u, double v,
						const t_point3 *p);

void				img_texture_init(t_img_texture *img_texture,
						char *filename);
t_color				img_texture_value(const void *self, double u, double v,
						const t_point3 *p);

/* for image handling */
unsigned char		float_to_byte(float value);
void				convert_to_bytes(t_img_texture *img);
unsigned char		*pixel_data(const t_img_texture *img, int x, int y);
int					width(const t_img_texture *img);
int					height(const t_img_texture *img);
int					load(t_img_texture *img, const char *filename);
int					clamp_rtw(int x, int low, int high);
void				free_rtw_image(t_img_texture *img);

#endif
