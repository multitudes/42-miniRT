/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera_ray_color_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 16:47:49 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/10 15:31:20 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "camera.h"
#include "debug.h"
#include "pdf.h"
#include "material.h"

t_color	metal_color_mix(t_scene scene, t_hit_record rec, t_scatter_record srec,
		int depth)
{
	t_color	ambient_light;
	t_metal	*metal;
	t_color	ambient_material;
	t_color	reflected_color;

	ambient_light = scene.cam->ambient.color;
	metal = (t_metal *)rec.mat;
	ambient_material = vec3multscalar(vec3add(metal->albedo,
				vec3multscalar(ambient_light, 0.1)), 0.01);
	reflected_color = vec3mult(srec.attenuation, ray_color(scene,
				&srec.skip_pdf_ray, depth - 1));
	return (vec3add(ambient_material, reflected_color));
}

void	init_rcparams(t_rcparams *params)
{
	params->rec = (t_hit_record){0};
	params->srec = (t_scatter_record){0};
	params->light_pdf = (t_hittable_pdf){0};
	params->mix_pdf = (t_mixture_pdf){0};
	params->recorded_pdf = NULL;
	params->color_from_emission = color(0, 0, 0);
	params->scattered = (t_ray){0};
	params->pdf_value = 0;
	params->scattering_pdf = 0;
	params->sample_color = color(0, 0, 0);
}

t_color	get_color_from_scatter(t_rcparams params, t_scene scene, t_ray *r,
		int depth)
{
	t_color	ambient;
	t_color	ambient_samplecolor;
	t_color	attenuationxscattering_pdf;
	t_color	color_from_scatter_partial;

	hittable_pdf_init(&params.light_pdf, scene.lights, &params.rec.p);
	mixture_pdf_init(&params.mix_pdf, params.srec.pdf_ptr,
		(t_pdf *)&params.light_pdf);
	params.scattered = ray(params.rec.p, mixture_pdf_generate(&params.mix_pdf));
	params.pdf_value = mixture_pdf_value(&params.mix_pdf,
			&params.scattered.dir);
	params.scattering_pdf = params.rec.mat->scattering_pdf(params.rec.mat, r,
			&params.rec, &params.scattered);
	params.sample_color = ray_color(scene, &params.scattered, depth - 1);
	ambient = scene.cam->ambient.color;
	ambient_samplecolor = vec3add(ambient, params.sample_color);
	attenuationxscattering_pdf = vec3multscalar(params.srec.attenuation,
			params.scattering_pdf);
	color_from_scatter_partial = vec3mult(attenuationxscattering_pdf,
			ambient_samplecolor);
	return (vec3divscalar(color_from_scatter_partial, params.pdf_value));
}
