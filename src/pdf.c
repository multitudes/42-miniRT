/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 15:08:47 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/25 14:53:21 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pdf.h"
#include "vec3.h"
#include "hittable.h"
#include "utils.h"
#include "sphere.h"
#include "hittable_list.h"
#include "debug.h"

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

double sphere_pdf_value(const void *self, const t_vec3 *dir)
{
	(void)self;
	(void)dir;
	return 1 / (4 * PI);
}

/** 
 * @brief Initializes the cosine_pdf structure.
 * 
 * The cosine PDF is used to generate random directions for flat materials.
*/
void	cosine_pdf_init(t_cosine_pdf *cos_pdf, const t_vec3 *w)
{
	cos_pdf->base.value = cosine_pdf_value;
	cos_pdf->base.generate = cosine_pdf_generate;
	onb_build_from_w(&cos_pdf->uvw, w);
}

/** 
 * @brief Computes the PDF value for a given direction.
 * 
 * This function computes the probability density function (PDF) value for a 
 * given direction using the cosine of the angle between the direction and the
 * normal vector of the surface.
*/
double cosine_pdf_value(const void *self, const t_vec3 *direction)
{
	t_cosine_pdf *cos_pdf = (t_cosine_pdf *)self;
	double cosine_theta = dot(unit_vector(*direction), cos_pdf->uvw.w);
	return (fmax(0, cosine_theta / PI));
}

/**
 * @brief Generates a random direction using the cosine PDF.
 * 
*/
t_vec3 cosine_pdf_generate(void *self)
{
	t_cosine_pdf *cos_pdf = (t_cosine_pdf *)self;
	return onb_transform(&cos_pdf->uvw, random_cosine_direction());
}


void hittable_pdf_init(t_hittable_pdf *hittable_pdf, const t_hittablelist *objects, const t_vec3 *origin)
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
	return hittable_pdf->objects->obj_pdf_value(hittable_pdf->objects, &hittable_pdf->origin, direction);
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
	return hittable_pdf->objects->obj_random(hittable_pdf->objects, &hittable_pdf->origin);
}

void mixture_pdf_init(t_mixture_pdf *mixture_pdf, t_pdf *p0, t_pdf *p1)
{	
	mixture_pdf->base.value = mixture_pdf_value;
	mixture_pdf->base.generate = mixture_pdf_generate;
	mixture_pdf->p1 = p0;
	mixture_pdf->p2 = p1;
}

double	mixture_pdf_value(const void *self, const t_vec3 *direction)
{
	// debug("mixture_pdf_value");
	t_mixture_pdf *mixture_pdf = (t_mixture_pdf *)self;
	double first_pdf_value = mixture_pdf->p1->value(mixture_pdf->p1, direction);
	double second_pdf_value = mixture_pdf->p2->value(mixture_pdf->p2, direction);
	return 0.5 * first_pdf_value + 0.5 * second_pdf_value;
}

t_vec3	mixture_pdf_generate(void *self)
{
	t_mixture_pdf *mixture_pdf = (t_mixture_pdf *)self;
    if (random_d() < 0.5) {
		// debug("mixture_pdf_generate");
		t_pdf *pdff = mixture_pdf->p1;
		// debug("pdff: %p", pdff);	
		t_vec3 v = pdff->generate(mixture_pdf->p1);
		// debug("v: %f %f %f", v.x, v.y, v.z);
        return v;
    } else {
		// debug("mixture_pdf_generate2");
		t_pdf *pdff = mixture_pdf->p2;
		// debug("pdff: %p", pdff);
		t_vec3 v = pdff->generate(mixture_pdf->p2);
        return v;
    }
}