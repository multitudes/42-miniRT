/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   material.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 15:43:42 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/30 09:59:40 by lbrusa           ###   ########.fr       */
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
		// Assign the scatter function
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
	lambertian_material->base.scatter = lambertian_scatter;
	lambertian_material->base.emit = emitzero;
	lambertian_material->base.scattering_pdf = lambertian_scattering_pdf;
	lambertian_material->texture = tex;
	lambertian_material->albedo = tex->value(tex, 0, 0, &(t_point3){.x = 0,
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

/*
 * Scatter record init function
 */
void	init_scatter_record(t_scatter_record *srec)
{
	srec->attenuation = color(0, 0, 0);
	srec->pdf_ptr = NULL;
	srec->skip_pdf = false;
	srec->skip_pdf_ray = ray(point3(0, 0, 0), vec3(0, 0, 0));
	srec->cosine_pdf.base.value = cosine_pdf_value;
	srec->cosine_pdf.base.generate = cosine_pdf_generate;
	srec->sphere_pdf.base.value = sphere_pdf_value;
	srec->sphere_pdf.base.generate = sphere_pdf_generate;
	srec->hittable_pdf.base.value = hittable_pdf_value;
	srec->hittable_pdf.base.generate = hittable_pdf_generate;
	srec->mixture_pdf.base.value = mixture_pdf_value;
	srec->mixture_pdf.base.generate = mixture_pdf_generate;
}

/**
 * No scatter as default for light and lambertian materials
 */
bool	noscatter(void *self, t_ray *r_in, t_hit_record *rec,
		t_scatter_record *srec)
{
	(void)self;
	(void)r_in;
	(void)rec;
	init_scatter_record(srec);
	return (false);
}

/**
 * No emisssion as default for lambertian and metal materials
 */
t_color	emitzero(void *self, t_hit_record rec, double u, double v, t_point3 p)
{
	(void)self;
	(void)rec;
	(void)u;
	(void)v;
	(void)p;
	return (color(0, 0, 0));
}

void	empty_material_init(t_empty_material *empty_material)
{
	empty_material->base.scatter = noscatter;
	empty_material->base.emit = emitzero;
	empty_material->base.scattering_pdf = scattering_pdf_zero;
}

/*
 * scatter function for a lambertian material
 */
bool	lambertian_scatter(void *self, t_ray *r_in, t_hit_record *rec,
		t_scatter_record *srec)
{
	t_lambertian	*lamb;

	(void)r_in;
	lamb = (t_lambertian *)self;
	srec->attenuation = lamb->texture->value(lamb->texture, rec->u, rec->v,
			&rec->p);
	cosine_pdf_init(&srec->cosine_pdf, &rec->normal);
	srec->pdf_ptr = (t_pdf *)&(srec->cosine_pdf);
	srec->skip_pdf = false;
	return (true);
}

/*
 * scatter function for a lambertian material
 */
double	lambertian_scattering_pdf(void *self, const t_ray *r_in,
		const t_hit_record *rec, const t_ray *scattered)
{
	double	cos_theta;

	(void)r_in;
	(void)self;
	cos_theta = dot(rec->normal, unit_vector(scattered->dir));
	if (cos_theta < 0)
		return (0);
	return (cos_theta / PI);
}

/**
 * scatter function for a metal material
 */
bool	metal_scatter(void *self, t_ray *r_in, t_hit_record *rec,
		t_scatter_record *srec)
{
	t_metal	*metal;
	t_vec3	reflected;

	metal = (t_metal *)self;
	reflected = vec3add(unit_vector(reflect(r_in->dir, rec->normal)),
			vec3multscalar(random_unit_vector(), metal->fuzz));
	srec->attenuation = metal->albedo;
	srec->pdf_ptr = NULL;
	srec->skip_pdf = true;
	srec->skip_pdf_ray = ray(rec->p, reflected);
	return (true);
}

t_color	emitlight(void *self, t_hit_record rec, double u, double v, t_point3 p)
{
	t_diffuse_light	*light;

	light = (t_diffuse_light *)self;
	if (!rec.front_face)
		return (color(0, 0, 0));
	return (light->texture->value(light->texture, u, v, &p));
}
