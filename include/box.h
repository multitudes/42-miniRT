/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   box.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 11:38:14 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/15 12:29:59 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef BOX_H
# define BOX_H

#include "vec3.h"
#include "ray.h"
#include "quad.h"
#include "material.h"
#include "hittable_list.h"
#include "color.h"

typedef struct	s_box {
	t_hittable      base;
    t_point3        a;
    t_point3        b;
    t_quad 		    q1;
    t_quad 		    q2;
    t_quad 		    q3;
    t_quad 		    q4;
    t_quad 		    q5;
    t_quad 		    q6;
    t_rgb           rgb;
    t_color			color;
	t_lambertian 	lambertian_mat;
	t_solid_color 	texture;
    t_material		*mat;
    void		    (*print)(const void* self);
}				t_box;

t_box   box(t_point3 a, t_point3 b, t_material *mat);
t_box   box_rgb(t_point3 a, t_point3 b, t_rgb rgb);
// void    create_box(t_box *box, t_point3 a, t_point3 b, t_material *mat);
void    print_box(const void *self);
bool    hit_box(const void* self, const t_ray *r, t_interval ray_t,  t_hit_record *rec);

#endif