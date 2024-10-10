/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   material.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 15:43:42 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/07 16:57:25 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "color.h"
#include "debug.h"
#include "hittable.h"
#include "material.h"
#include "minirt.h"
#include "onb.h"
#include "ray.h"
#include "texture.h"
#include "utils.h"
#include "vec3.h"

/**
 * @brief Initialize a lambertian material with albedo
 *
 * @param lambertian_material
 * @param tex
 * lambertian has two initializer functions, one takes the albedo as
 * initializer, the other takes a texture.
 * In the albedo initializer, the albedo is used to set the texture
 * which is used by the scatter function to get the color
 */
void	lambertian_init(t_lambertian *lambertian_material, t_color albedo)
{
	lambertian_material->base.scatter = lambertian_scatter;
	lambertian_material->base.emit = emitzero;
	lambertian_material->base.scattering_pdf = lambertian_scattering_pdf;
	lambertian_material->albedo = albedo;
}

/**
 * @brief Initialize a lambertian material with a texture
 *
 * @param lambertian_material
 * @param tex
 * lambertian has two initializer functions, one takes the albedo as
 * initializer, the other takes a texture. In the texture
 * initializer, the albedo is set to the value of the texture
 * but it will not be used directly since the scatter function
 * will call the value function of the texture to get the color
 */
void	lambertian_init_tex(t_lambertian *lambertian_material, t_texture *tex)
{
	double	uv[2];

	uv[0] = 0;
	uv[1] = 0;
	lambertian_material->base.scatter = lambertian_scatter;
	lambertian_material->base.emit = emitzero;
	lambertian_material->base.scattering_pdf = lambertian_scattering_pdf;
	lambertian_material->texture = tex;
	lambertian_material->albedo = tex->value(tex, uv, &(t_point3){.x = 0,
			.y = 0, .z = 0});
}

void	metal_init(t_metal *metal, t_rgb albedo, double fuzz)
{
	metal->base.scatter = metal_scatter;
	metal->base.emit = emitzero;
	metal->base.scattering_pdf = scattering_pdf_zero;
	metal->albedo = rgb_to_color(albedo);
	metal->fuzz = clamp(interval(0, 1), fuzz);
}

void	diffuse_light_init(t_diffuse_light *light, t_texture *texture)
{
	light->base.scatter = noscatter;
	light->base.emit = emitlight;
	light->base.scattering_pdf = scattering_pdf_zero;
	light->texture = texture;
}

/**
 * No scatter as default for light materials
 */
double	scattering_pdf_zero(void *self, const t_ray *r_in,
		const t_hit_record *rec, const t_ray *scattered)
{
	(void)r_in;
	(void)self;
	(void)rec;
	(void)scattered;
	return (0);
}
