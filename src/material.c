/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   material.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/23 15:43:42 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/17 19:08:27 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hittable.h"
#include "ray.h"
#include "vec3.h"
#include "color.h"
#include "minirt.h"
#include "texture.h"
#include "onb.h"
#include "utils.h"
#include "debug.h"
#include "material.h"

void lambertian_init(t_lambertian *lambertian_material, t_color albedo)
{
    lambertian_material->base.scatter = lambertian_scatter; // Assign the scatter function
	lambertian_material->base.emit = emitzero;
    lambertian_material->albedo = albedo; // Set the albedo
	lambertian_material->base.scattering_pdf = lambertian_scattering_pdf;
}

void lambertian_init_tex(t_lambertian *lambertian_material, t_texture *tex)
{
    lambertian_material->base.scatter = lambertian_scatter; // Assign the scatter function
	lambertian_material->base.emit = emitzero;
	lambertian_material->base.scattering_pdf = lambertian_scattering_pdf;
    lambertian_material->albedo = color(0,0,0); // Set the albedo to null to experiment with lights
	lambertian_material->texture = tex;
	lambertian_material->base.scattering_pdf = lambertian_scattering_pdf;
}


void metal_init(t_metal *metal, t_color albedo, double fuzz)
{
	metal->base.scatter = metal_scatter;
	metal->base.emit = emitzero;
	metal->base.scattering_pdf = scattering_pdf_zero;
	metal->albedo = albedo;
	metal->fuzz = fuzz < 1 ? fuzz : 1;
}


void diffuse_light_init(t_diffuse_light *light, t_texture *texture)
{
	light->base.scatter = noscatter;
	light->base.emit = emitlight;
	light->base.scattering_pdf = scattering_pdf_zero;
	light->texture = texture;
}

/**
 * No scatter as default for light materials
 */
double scattering_pdf_zero(void* self, const t_ray *r_in, const t_hit_record *rec, const t_ray *scattered)
{
	(void)r_in;
	(void)self;
	(void)rec;
	(void)scattered;
    return 0;
}

/*
 * Scatter record init function
*/
void init_scatter_record(t_scatter_record *srec)
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
bool noscatter(void *self, t_ray *r_in, t_hit_record *rec, t_scatter_record *srec)
{
	(void)self;
	(void)r_in;
	(void)rec;
	init_scatter_record(srec);
	return false;
}

/**
 * No emisssion as default for lambertian and metal materials
 */
t_color		emitzero(void *self, t_hit_record rec, double u, double v, t_point3 p)
{
	(void)self;
	(void)rec;
	(void)u;
	(void)v;
	(void)p;
	return color(0, 0, 0);
}

void 		empty_material_init(t_empty_material *empty_material)
{
	empty_material->base.scatter = noscatter;
	empty_material->base.emit = emitzero;
	empty_material->base.scattering_pdf = scattering_pdf_zero;
}

/*
 * scatter function for a lambertian material
 */
bool lambertian_scatter(void* self, t_ray *r_in, t_hit_record *rec, t_scatter_record *srec)
{
	(void)r_in;
	t_lambertian *lamb = (t_lambertian *)self;

	srec->attenuation = lamb->texture->value(lamb->texture, rec->u, rec->v, &rec->p);

	cosine_pdf_init(&srec->cosine_pdf, &rec->normal);
	srec->pdf_ptr = (t_pdf *)&(srec->cosine_pdf);
	srec->skip_pdf = false;

    return true;
}

/*
 * scatter function for a lambertian material
 */
double lambertian_scattering_pdf(void* self, const t_ray *r_in, const t_hit_record *rec, const t_ray *scattered)
{
	(void)r_in;
	(void)self;
	double cos_theta = dot(rec->normal, unit_vector(scattered->dir));
        return cos_theta < 0 ? 0 : cos_theta/PI;
}

/**
 * scatter function for a metal material
 */
bool metal_scatter(void *self,  t_ray* r_in,  t_hit_record *rec, t_scatter_record *srec)
{
	t_metal *metal = (t_metal *)self;
	t_vec3 reflected = reflect(r_in->dir, rec->normal);
	reflected = vec3add(unit_vector(reflected), vec3multscalar(random_unit_vector(), metal->fuzz));
	srec->attenuation = metal->albedo;
	srec->pdf_ptr = NULL;
	srec->skip_pdf = true;
	srec->skip_pdf_ray = ray(rec->p, reflected);

	return (true);
}

t_color		emitlight(void *self,  t_hit_record rec, double u, double v, t_point3 p)
{
	// debug("emitting light");
	// (void)rec;
	// t_diffuse_light *light = (t_diffuse_light *)self;
	// return light->texture->value(light->texture ,u, v, &p);

	t_diffuse_light *light = (t_diffuse_light *)self;
	if (!rec.front_face)
		return color(0, 0, 0);
	return light->texture->value(light->texture ,u, v, &p);
}
