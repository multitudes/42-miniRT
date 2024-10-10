/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hittable_list.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 08:21:39 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/04 14:38:56 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HITTABLE_LIST_H
# define HITTABLE_LIST_H

# include "hittable.h"
# include "interval.h"
# include "ray.h"
# include "vec3.h"

typedef struct s_hittable	t_hittable;
typedef struct s_hit_record	t_hit_record;
typedef struct s_ray		t_ray;

typedef struct s_hittablelist
{
	t_hittable		**list;
	int				size;
	bool			(*hit_objects)(const void *self, const t_ray * r, \
								t_interval ray_t, t_hit_record * rec);
	double			(*obj_pdf_value)(const void *self, \
								const t_point3 *o, const t_vec3 *v);
	t_vec3			(*obj_random)(const void *self, const t_vec3 * o);
}					t_hittablelist;

bool				hit_objects(const void *self, const t_ray *r,
						t_interval ray_t, t_hit_record *rec);
t_hittablelist		hittablelist(t_hittable **list, int size);
double				obj_pdf_value(const void *self, const t_point3 *o,
						const t_vec3 *v);
t_vec3				obj_random(const void *self, const t_vec3 *o);

#endif
