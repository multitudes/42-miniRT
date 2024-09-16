/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   triangle.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 18:05:35 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/16 14:26:43 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TRIANGLE_H
# define TRIANGLE_H

#include "vec3.h"
#include "interval.h"
#include "material.h"
#include "ray.h"
#include "hittable.h"

typedef struct s_triangle {
    t_hittable  base;
    t_point3    a;
    t_point3    b;
    t_point3    c;
    t_vec3      edge1;
    t_vec3      edge2;
    t_rgb           rgb;
    t_color         color;
    t_lambertian    lambertian_mat;
    t_solid_color   texture;
    t_material  *mat;
    double      d;
    t_vec3      normal;
    t_vec3      w;
    double      area;
    void        (*print)(const void* self);
}               t_triangle;

// create vertices struct

t_triangle  triangle(t_point3 a, t_point3 b, t_point3 c, t_rgb rgbcolor);
t_triangle  triangle_mat(t_point3 a, t_point3 b, t_point3 c, t_material *mat);
void        print_triangle(const void *self);
bool        hit_triangle(const void* self, const t_ray *r, t_interval ray_t,  t_hit_record *rec);
double      triangle_pdf_value(const void *self, const t_point3 *orig, const t_vec3 *dir);
t_vec3      triangle_random(const void *self, const t_point3 *orig);

#endif