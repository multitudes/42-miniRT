/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   triangle.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 18:04:25 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/20 13:41:00 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "triangle.h"
#include "utils.h"
#include "color.h"
#include "material.h"
#include "texture.h"
#include "vec3.h"



void triangle(t_triangle *tri, t_point3 a, t_point3 b, t_point3 c, t_rgb rgbcolor)
{
    tri->base.hit = hit_triangle;
    tri->base.pdf_value = triangle_pdf_value;
    tri->base.random = triangle_random;
    tri->a = a;
    tri->b = b;
    tri->c = c;
    tri->edge1 = vec3substr(b, a);
    tri->edge2 = vec3substr(c, a);
    tri->rgb = rgbcolor;
    tri->color = rgb_to_color(rgbcolor);
    tri->normal = unit_vector(cross(tri->edge2, tri->edge1));
    tri->d = dot(tri->normal, a);
    tri->area = 0.5 * length(cross(vec3substr(b, a), vec3substr(c, a)));

    // Initialize texture and material as before
    solid_color_init(&(tri->texture), tri->color);
    lambertian_init_tex(&(tri->lambertian_mat), (t_texture*)&(tri->texture));
    tri->mat = (t_material*)&(tri->lambertian_mat);
    tri->print = print_triangle;
}

void triangle_mat(t_triangle *tri, t_point3 a, t_point3 b, t_point3 c, t_material *mat)
{
    tri->base.hit = hit_triangle;
    tri->base.pdf_value = triangle_pdf_value;
    tri->base.random = triangle_random;
    tri->a = a;
    tri->b = b;
    tri->c = c;
    tri->mat = mat;
    tri->normal = unit_vector(cross(vec3substr(b, a), vec3substr(c, a)));
    tri->d = dot(tri->normal, a);
    tri->area = 0.5 * length(cross(vec3substr(b, a), vec3substr(c, a)));
    tri->rgb = rgb(0, 0, 0);
    tri->color = color(0, 0, 0);
    tri->print = print_triangle;
}

void print_triangle(const void *self) {
    t_triangle *tri = (t_triangle *)self;
    printf("tri %.f,%.f,%.f %.f,%.f,%.f %.f,%.f,%.f %d,%d,%d\n",
    tri->a.x, tri->a.y, tri->a.z,
    tri->b.x, tri->b.y, tri->b.z,
    tri->c.x, tri->c.y, tri->c.z,
    tri->rgb.r, tri->rgb.g, tri->rgb.b);
}

/**
 * @brief Check if a ray intersects a triangle.
 * 
 * @param self Pointer to the triangle object.
 * @param r Pointer to the ray object.
 * @param ray_t Interval of valid t values for the ray.
 * @param rec Pointer to the hit record to update.
 * @return true if the ray intersects the triangle, false otherwise.
 * 
 * We use the Möller–Trumbore intersection algorithm to check if
 * the ray intersects the triangle. The algorithm starts by 
 * extracting the edges of the triangle and calculating the
 * determinant which is the dot product of the first edge  
 * with the cross product of the second edge and the ray direction vector. 
 * If the determinant is close to zero, the ray is parallel to the triangle 
 * and we return false. 
 * We then take the inverse of the determinant and calculate 
 * the Vector from Vertex A to the ray origin using this to 
 * calculate the barycerntric u parameter. 
 * If u is outside the range [0, 1], the intersection point is outside 
 * the triangle.
 * Calculate the Cross Product of p1_to_origin and e1 and use it for
 * the Barycentric Coordinate v. If v is outside the range [0, 1] or
 * u + v > 1, the intersection point is outside the triangle.
 * Calculate the t value for the intersection point and check if it is
 * within the valid interval. Then we store the hit record and return true.
*/ 
bool hit_triangle(const void* self, const t_ray *r, t_interval ray_t,  t_hit_record *rec) 
{
    const t_triangle *tri = (t_triangle *)self;
    t_vec3 e1 = tri->edge1;
    t_vec3 e2 = tri->edge2;
    t_vec3 dir_cross_e2 = cross(r->dir, e2);
    double det = dot(e1, dir_cross_e2);
    if (fabs(det) < EPSILON)
        return false;
    double f = 1.0 / det;
    t_vec3 p1_to_origin = vec3substr(r->orig, tri->a);
    double u = f * dot(p1_to_origin, dir_cross_e2);
    if (u < 0 || u > 1)
        return false;
    t_vec3 origin_cross_e1 = cross(p1_to_origin, e1);
    double v = f * dot(r->dir, origin_cross_e1);
    if (v < 0 || u + v > 1)
        return false;
    double t = f * dot(e2, origin_cross_e1);
    if (!contains(&ray_t, t))
        return false;
    rec->t = t;
    rec->p = point_at(r, t);
    rec->mat = tri->mat;
    set_face_normal(rec, r, tri->normal);
    return (true);
}


double triangle_pdf_value(const void *self, const t_point3 *orig, const t_vec3 *dir)
{
    const t_triangle *tri = (t_triangle *)self;
    t_hit_record rec;
    const t_ray r = ray(*orig, *dir);
    if (!hit_triangle(tri, &r, interval(0.001, 1e30), &rec))
        return 0;

    double distance_squared = length_squared(vec3substr(rec.p, *orig));
    double cosine = fabs(dot(*dir, tri->normal));

    // Calculate the area of the triangle
    t_vec3 e1 = vec3substr(tri->b, tri->a);
    t_vec3 e2 = vec3substr(tri->c, tri->a);
    double area = 0.5 * length(cross(e1, e2));

    return distance_squared / (cosine * area);
}

t_vec3 triangle_random(const void *self, const t_point3 *orig)
{
    const t_triangle *tri = (t_triangle *)self;

    // Generate random points within the unit triangle (0, 1) × (0, 1)
    double u = random_double(0, 1);
    double v = random_double(0, 1);
    if (u + v > 1) {
        u = 1 - u;
        v = 1 - v;
    }

    // Calculate the point on the triangle corresponding to the random coordinates
    t_vec3 p = vec3add(tri->a, vec3add(vec3multscalar(vec3substr(tri->b, tri->a), u), vec3multscalar(vec3substr(tri->c, tri->a), v)));

    return vec3substr(p, *orig);
}
