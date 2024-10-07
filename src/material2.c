/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   material2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 16:57:31 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/07 16:58:09 by lbrusa           ###   ########.fr       */
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
t_color	emitzero(void *self, t_hit_record rec, double uv[2], t_point3 p)
{
	(void)self;
	(void)rec;
	(void)uv;
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
	srec->attenuation = lamb->texture->value(lamb->texture, rec->uv, &rec->p);
	cosine_pdf_init(&srec->cosine_pdf, &rec->normal);
	srec->pdf_ptr = (t_pdf *)&(srec->cosine_pdf);
	srec->skip_pdf = false;
	return (true);
}
