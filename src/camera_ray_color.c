/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera_ray_color.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 17:07:38 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/04 16:56:47 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "debug.h"
#include "minirt.h"
#include "vec3.h"
#include "color.h"
#include "ray.h"
#include "hittable_list.h"
#include "interval.h"
#include "utils.h"
#include "camera.h"
#include "pdf.h"
#include <stdbool.h>

/**
 * @brief one of the most important functions in the raytracer
 * this function will return the color of the pixel for a given camera 
 * position sending rays into the scene.
 * @param cam the camera object
 * @param r the ray object
 * @param depth the maximum number of bounces
 * @param world the list of objects in the scene
 * @param lights the list of light sources in the scene
 * @return t_color the color of the pixel
 *
 * If there are no hits the functions returns a space grey color.
 * Then first I check if the material is a light source and return the color 
 * of the light source or what the emit function fort the material returns. 
 * Materials like diffuse lambertians and metals will have no emission and 
 * the emit function will return a black color.
 * So the next check if the material has a scatter function this will populate
 * the scatter record with the attenuation and the scattered ray. 
 * If the material is a light source the scatter function will return 
 * false and the function will return the color of the light source.
 * if the material is metal the scatter function will return the scattered 
 * ray and the attenuation and the function will call itself recursively 
 * with the scattered ray and return the color of the object.
 * At this stage i mix the ambient light with the color returned by the 
 * recursive call otherwise the metal object will be black with only specular 
 * reflections.
 * If it is not a metal object the function will continue and do a 
 * pdf calculation for the light sources.
 * In the hittable_pdf struct I have a pointer to the light source list and 
 * the point of intersection.
 * I will do a mix of the pdf values of the light sources and the material 
 * and then I will randomly choose a light source or the material to scatter 
 * the ray. I will calculate the pdf value for the chosen light source
 * and the material and then I will calculate the scattering pdf for the 
 * material. I will call the function recursively with the scattered ray 
 * and return the color of the object.
 */
t_color	ray_color(t_scene scene, t_ray *r, int depth)
{
	t_rcparams params;

	init_rcparams(&params);
	if (depth <= 0)
        return color(0,0,0);
	if (!scene.world->hit_objects(scene.world, r, interval(0.001, 100000), \
									&params.rec))
		return color(0.001,0.001,0.001);
	params.color_from_emission = params.rec.mat->emit(params.rec.mat, \
								 params.rec, params.rec.uv, params.rec.p);
	init_scatter_record(&params.srec);
	if (!params.rec.mat->scatter(params.rec.mat, r, &params.rec, &params.srec))
		return params.color_from_emission;
	params.scattered = params.srec.skip_pdf_ray;
	if (params.srec.skip_pdf)
		return (metal_color_mix(scene, params.rec, params.srec, depth));
	return get_color_from_scatter(params, scene, r, depth);
}

/**
 * @brief gamma correction
 * @param c the color 
 * @return t_color the gamma corrected color
 *
 * 
 * gamma corrects the colorvector.  
 * squareroots the color values. 
 */
uint32_t    color_gamma_corrected(t_color color)
{
	t_interval intensity;

	intensity = interval(0.0,0.999);
	t_rgb a;
	a = color_to_rgb((t_color){
		.r = clamp(intensity, linear_to_gamma(color.r)),
		.g = clamp(intensity, linear_to_gamma(color.g)),
		.b = clamp(intensity, linear_to_gamma(color.b))});
    return (rgb_to_uint(a));
}

/*
this is my draw pixel function. I write directly to the buffer
and the color is RGBA or 4 bytes. Code inspired from the MLX42 lib!
*/
void write_color(t_mrt *data, int x, int y, t_color colorvector)
{
    unsigned int color = color_gamma_corrected(colorvector);
    int offset;
    mlx_image_t *image;
    uint8_t *pixel;

    image = data->image;
    offset = y * data->cam.img_width + x;
    pixel = &image->pixels[offset * 4];
    *(pixel++) = (uint8_t)(color >> 24);
    *(pixel++) = (uint8_t)(color >> 16);
    *(pixel++) = (uint8_t)(color >> 8);
    *(pixel++) = (uint8_t)(color & 0xFF);
}

/**
 * @brief get a ray from the camera through a pixel
 * 
 * Creates a ray that goes from the cam through (i, j) on the viewport
 *  
*/ 
t_ray	get_ray(t_camera cam, int i, int j)
{
	t_vec3 offset;
	
	offset = sample_square();
	t_vec3 iu = vec3multscalar(cam.pixel_delta_u, i + offset.x);
	t_vec3 ju = vec3multscalar(cam.pixel_delta_v, j + offset.y);
	t_vec3 partial = vec3add(iu, ju);
	t_point3 pixel_sample = vec3add(cam.pixel00_loc, partial);
	t_point3 ray_origin = cam.orig;
	t_vec3 ray_direction = vec3substr(pixel_sample, ray_origin);
	return ray(ray_origin, ray_direction);
}
