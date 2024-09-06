/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hittable.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 16:59:14 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/06 19:40:39 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include <stdbool.h>
#include "interval.h"
#include "material.h"

typedef struct s_material t_material;

typedef struct 	s_hit_record {
	t_material 	*mat;
    t_point3 	p;
    t_vec3 		normal;
    double 		t;
	bool 		front_face;
	double		u;
	double		v;
	
} 				t_hit_record;

typedef struct 	s_hittable{
    bool 		(*hit)(const void* self, const t_ray* r, t_interval closest, t_hit_record* rec);
} 				t_hittable;

void 		set_face_normal(t_hit_record *rec, const t_ray *r, const t_vec3 outward_normal);

#endif


