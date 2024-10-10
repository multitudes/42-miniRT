/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   material3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 16:58:17 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/10 19:54:13 by lbrusa           ###   ########.fr       */
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

t_color	emitlight(void *self, t_hit_record rec, double uv[2], t_point3 p)
{
	t_diffuse_light	*light;

	light = (t_diffuse_light *)self;
	if (!rec.front_face)
		return (color(0, 0, 0));
	return (light->texture->value(light->texture, uv, &p));
}
