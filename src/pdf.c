/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pdf.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 15:08:47 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/07 17:17:47 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "debug.h"
#include "hittable.h"
#include "hittable_list.h"
#include "pdf.h"
#include "sphere.h"
#include "utils.h"
#include "vec3.h"

/**
 * @brief Generates a random direction uniformly distributed over a sphere.
 * @self: Pointer to the object (unused in this function).
 * This function generates a random direction vector that is uniformly 
 * distributed over the surface of a sphere. It uses the random_unit_vector 
 * function to produce a unit vector with a random direction.
 *
 * Return: A t_vec3 vector representing a random direction on the unit sphere.
 */
t_vec3	sphere_pdf_generate(void *self)
{
	(void)self;
	return (random_unit_vector());
}

/**
 * @brief Computes the PDF value for a given direction.
 */
double	sphere_pdf_value(const void *self, const t_vec3 *dir)
{
	(void)self;
	(void)dir;
	return (1 / (4 * PI));
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
double	cosine_pdf_value(const void *self, const t_vec3 *direction)
{
	t_cosine_pdf	*cos_pdf;
	double			cosine_theta;

	cos_pdf = (t_cosine_pdf *)self;
	cosine_theta = dot(unit_vector(*direction), cos_pdf->uvw.w);
	return (fmax(0, cosine_theta / PI));
}

/**
 * @brief Generates a random direction using the cosine PDF.
 *
 */
t_vec3	cosine_pdf_generate(void *self)
{
	t_cosine_pdf	*cos_pdf;

	cos_pdf = (t_cosine_pdf *)self;
	return (onb_transform(&cos_pdf->uvw, random_cosine_direction()));
}
