/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 15:08:47 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/11 14:33:27 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pdf.h"
#include "vec3.h"
#include "hittable.h"
#include "utils.h"
#include "sphere.h"
#include "hittable_list.h"


/**
 * sphere_pdf_generate - Generates a random direction uniformly distributed over a sphere.
 * @self: Pointer to the object (unused in this function).
 *
 * This function generates a random direction vector that is uniformly distributed
 * over the surface of a sphere. It uses the random_unit_vector function to produce
 * a unit vector with a random direction.
 *
 * Return: A t_vec3 vector representing a random direction on the unit sphere.
 */
t_vec3 sphere_pdf_generate(void *self)
{
	(void)self;
	return random_unit_vector();
}

void	cosine_pdf_init(t_cosine_pdf *cos_pdf, const t_vec3 *w)
{
	cos_pdf->base.value = cosine_pdf_value;
	cos_pdf->base.generate = cosine_pdf_generate;
	onb_build_from_w(&cos_pdf->uvw, w);
}

double cosine_pdf_value(const void *self, const t_vec3 *direction)
{
	t_cosine_pdf *cos_pdf = (t_cosine_pdf *)self;
	double cosine_theta = dot(unit_vector(*direction), cos_pdf->uvw.w);
	return (fmax(0, cosine_theta / PI));
}

t_vec3 cosine_pdf_generate(void *self)
{
	t_cosine_pdf *cos_pdf = (t_cosine_pdf *)self;
	return onb_transform(&cos_pdf->uvw, random_cosine_direction());
}


void hittable_pdf_init(t_hittable_pdf *hittable_pdf, t_hittablelist *objects, const t_vec3 *origin)
{
	hittable_pdf->base.value = hittable_pdf_value;
	hittable_pdf->base.generate = hittable_pdf_generate;
	hittable_pdf->objects = objects;
	hittable_pdf->origin = *origin;

}

/**
 * hittable_pdf_value - Computes the PDF value for a given direction.
 * @self: Pointer to the hittable_pdf structure.
 * @direction: Pointer to the t_vec3 direction vector.
 *
 * This function computes the probability density function (PDF) value for a
 * given direction using the PDF value function of the associated hittable object.
 *
 * Return: A double representing the PDF value for the given direction.
 */
double hittable_pdf_value(const void *self, const t_vec3 *direction)
{
	t_hittable_pdf *hittable_pdf = (t_hittable_pdf *)self;
	return hittablelist_pdf_value(hittable_pdf->objects, &hittable_pdf->origin, direction);
}

/**
 * hittable_pdf_generate - Generates a random direction using the PDF of the hittable objects.
 * @self: Pointer to the hittable_pdf structure.
 * 
 * Return: Choosing a random object and generating a random direction using its PDF
 */
t_vec3 hittable_pdf_generate(void *self)
{
	t_hittable_pdf *hittable_pdf = (t_hittable_pdf *)self;
	return hittablelist_random(hittable_pdf->objects, &hittable_pdf->origin);
}

void mixture_pdf_init(t_mixture_pdf *mixture_pdf, t_pdf *p0, t_pdf *p1)
{
	mixture_pdf->p[0] = *p0;
	mixture_pdf->p[1] = *p1;
}

double	mixture_pdf_value(const void *self, const t_vec3 *direction)
{
	t_mixture_pdf *mixture_pdf = (t_mixture_pdf *)self;
	double first_pdf_value = mixture_pdf->p[0].value(&mixture_pdf->p[0], direction);
	double second_pdf_value = mixture_pdf->p[1].value(&mixture_pdf->p[1], direction);
	return 0.5 * first_pdf_value + 0.5 * second_pdf_value;
}

t_vec3	mixture_pdf_generate(void *self)
{
	t_mixture_pdf *mixture_pdf = (t_mixture_pdf *)self;
	if (random_d() < 0.5)
		return mixture_pdf->p[0].generate(&mixture_pdf->p[0]);
	else
		return mixture_pdf->p[1].generate(&mixture_pdf->p[1]);
}