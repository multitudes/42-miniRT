/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pdf3.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 17:11:26 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/07 17:18:54 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "debug.h"
#include "hittable.h"
#include "hittable_list.h"
#include "pdf.h"
#include "sphere.h"
#include "utils.h"
#include "vec3.h"

void	mixture_pdf_init(t_mixture_pdf *mixture_pdf, t_pdf *p0, t_pdf *p1)
{
	mixture_pdf->base.value = mixture_pdf_value;
	mixture_pdf->base.generate = mixture_pdf_generate;
	mixture_pdf->p1 = p0;
	mixture_pdf->p2 = p1;
}

double	mixture_pdf_value(const void *self, const t_vec3 *direction)
{
	t_mixture_pdf	*mixture_pdf;
	double			first_pdf_value;
	double			second_pdf_value;

	mixture_pdf = (t_mixture_pdf *)self;
	first_pdf_value = mixture_pdf->p1->value(mixture_pdf->p1, direction);
	second_pdf_value = mixture_pdf->p2->value(mixture_pdf->p2, direction);
	return (0.5 * first_pdf_value + 0.5 * second_pdf_value);
}

t_vec3	mixture_pdf_generate(void *self)
{
	t_mixture_pdf	*mixture_pdf;
	t_pdf			*pdff;
	t_vec3			v;

	mixture_pdf = (t_mixture_pdf *)self;
	if (random_d() < 0.5)
	{
		pdff = mixture_pdf->p1;
		v = pdff->generate(mixture_pdf->p1);
		return (v);
	}
	else
	{
		pdff = mixture_pdf->p2;
		v = pdff->generate(mixture_pdf->p2);
		return (v);
	}
}
