/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hittable_list.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 08:21:39 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/11 11:36:11 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"
#include "interval.h"

typedef struct 	s_hittable t_hittable;
typedef struct 	s_hit_record t_hit_record;

typedef struct 	s_hittablelist {
	t_hittable 	**list;
	int 		size;
} 				t_hittablelist; 

bool 	hit_world(const t_hittablelist *world, const t_ray* r, t_interval ray_t, t_hit_record* rec);
t_hittablelist hittablelist(t_hittable **list, int size);
double hittablelist_pdf_value(const void *self, const t_point3 *o, const t_vec3 *v);
t_vec3 hittablelist_random(const void *self, const t_vec3 *o);

#endif
