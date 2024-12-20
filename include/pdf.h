/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/28 15:05:52 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/30 09:49:26 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PDF_H
# define PDF_H

# include "hittable_list.h"
# include "onb.h"
# include "vec3.h"

typedef struct s_hittablelist	t_hittablelist;

typedef struct s_pdf
{
	double						(*value)(const void *self, const t_vec3 *dir);
	t_vec3						(*generate)(void *self);
}								t_pdf;

typedef struct s_sphere_pdf
{
	t_pdf						base;
}								t_sphere_pdf;

typedef struct s_cosine_pdf
{
	t_pdf						base;
	t_onb						uvw;
}								t_cosine_pdf;

typedef struct s_hittable_pdf
{
	t_pdf						base;
	t_vec3						origin;
	const t_hittablelist		*objects;
}								t_hittable_pdf;

typedef struct s_mixture_pdf
{
	t_pdf						base;
	t_pdf						*p1;
	t_pdf						*p2;
}								t_mixture_pdf;

t_vec3							sphere_pdf_generate(void *self);
double							sphere_pdf_value(const void *self,
									const t_vec3 *dir);

void							cosine_pdf_init(t_cosine_pdf *cos_pdf,
									const t_vec3 *w);
double							cosine_pdf_value(const void *self,
									const t_vec3 *direction);
t_vec3							cosine_pdf_generate(void *self);

void							hittable_pdf_init(t_hittable_pdf *hittable_pdf,
									const t_hittablelist *objects,
									const t_vec3 *origin);
double							hittable_pdf_value(const void *self,
									const t_vec3 *direction);
t_vec3							hittable_pdf_generate(void *self);

void							mixture_pdf_init(t_mixture_pdf *mixture_pdf,
									t_pdf *p0, t_pdf *p1);
double							mixture_pdf_value(const void *self,
									const t_vec3 *direction);
t_vec3							mixture_pdf_generate(void *self);

#endif
