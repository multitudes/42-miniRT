/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/27 12:02:39 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/24 11:35:32 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEXTURE_H
# define TEXTURE_H

# include "vec3.h"
# include "color.h"

/*	This will be overlayed (cast) on each instance of the bottom 3 structs.
	The cast will cause only the first (t_texture) member to be taken from the
	old struct. */
typedef struct s_texture
{
	t_color (*value)(const void *self, double u, double v, const t_point3 *p);
}			t_texture;

typedef struct s_solid_color
{
	t_texture	base;
	t_color 	color_albedo;
}               t_solid_color;

/* has a color for an even and for an odd coordinate */
typedef struct 		s_checker_texture
{
	t_texture 		base;
	double    		inv_scale;
	t_solid_color	even;
	t_solid_color	odd;
}               	t_checker_texture;

typedef struct		s_img_texture
{
	t_texture		base;
	char 			*filename;
    int				bytes_per_pixel;
    float			*fdata; 			// Linear floating point pixel data
    unsigned char	*bdata;				// Linear 8-bit pixel data
    int				image_width;		// Loaded image width
    int				image_height;		// Loaded image height
    int				bytes_per_scanline;
}					t_img_texture;

void 	solid_color_init(t_solid_color *solid_color_texture, t_color albedo);
t_color solid_color_value(const void *self, double u, double v, const t_point3 *p);

void	checker_texture_init(t_checker_texture *checker_texture, double scale, t_rgb even_rgb, t_rgb odd_rgb);
t_color checker_texture_value(const void *self, double u, double v, const t_point3 *p);

void	img_texture_init(t_img_texture *img_texture, char *filename);
t_color img_texture_value(const void *self, double u, double v, const t_point3 *p);

/* for image handling */
unsigned char	float_to_byte(float value);
void			convert_to_bytes(t_img_texture *img);
unsigned char	*pixel_data(const t_img_texture *img, int x, int y);
int				width(const t_img_texture *img);
int				height(const t_img_texture *img);
int				load(t_img_texture *img, const char* filename);
int				clamp_rtw(int x, int low, int high);
void			free_rtw_image(t_img_texture *img);

#endif
