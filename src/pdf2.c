/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pdf2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 17:10:43 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/07 17:18:17 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "debug.h"
#include "hittable.h"
#include "hittable_list.h"
#include "pdf.h"
#include "sphere.h"
#include "utils.h"
#include "vec3.h"

void	hittable_pdf_init(t_hittable_pdf *hittable_pdf,
		const t_hittablelist *objects, const t_vec3 *origin)
{
	hittable_pdf->base.value = hittable_pdf_value;
	hittable_pdf->base.generate = hittable_pdf_generate;
	hittable_pdf->objects = objects;
	hittable_pdf->origin = *origin;
}

/**
 * @brief  - Computes the PDF value for a given direction.
 * @self: Pointer to the hittable_pdf structure.
 * @direction: Pointer to the t_vec3 direction vector.
 *
 * This function computes the probability density function (PDF) value for a
 * given direction using the PDF value function of the associated
 * hittable object.
 *
 * Return: A double representing the PDF value for the given direction.
 */
double	hittable_pdf_value(const void *self, const t_vec3 *direction)
{
	t_hittable_pdf	*hittable_pdf;

	hittable_pdf = (t_hittable_pdf *)self;
	return (hittable_pdf->objects->obj_pdf_value(hittable_pdf->objects,
			&hittable_pdf->origin, direction));
}

/**
 * @brief - Generates a random direction using the PDF of the hittable objects.
 * @self: Pointer to the hittable_pdf structure.
 *
 * Return: Choosing a random object and generating a random direction
 * using its PDF
 */
t_vec3	hittable_pdf_generate(void *self)
{
	t_hittable_pdf	*hittable_pdf;

	hittable_pdf = (t_hittable_pdf *)self;
	return (hittable_pdf->objects->obj_random(hittable_pdf->objects,
			&hittable_pdf->origin));
}
