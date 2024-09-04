/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hittable.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 16:59:14 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/04 15:21:42 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include <stdbool.h>

typedef struct 	s_hit_record {
    t_point3 	p;
    t_vec3 		normal;
    double 		t;
	bool 		front_face;
} 				t_hit_record;

void 		set_face_normal(t_hit_record *rec, const t_ray *r, const t_vec3 outward_normal);


/* translated from cpp - remains to see if I will incorporate this into every object*/
typedef struct 	s_hittable{
    bool 		(*hit)(const void* self, const t_ray* r, double ray_tmin, double ray_tmax, t_hit_record* rec);
} 				t_hittable;



#endif


