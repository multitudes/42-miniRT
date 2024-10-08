/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder_hit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 18:11:42 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/08 18:22:17 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cylinder.h"
#include "vec3.h"
#include "ray.h"
#include "color.h"
#include <stdio.h>
#include "material.h"
#include "utils.h"

typedef struct s_cyl_utils
{
	t_cylinder		*cyl;
	const t_ray		*r;
	double			closest_t;
	t_vec3			closest_point;
	t_vec3			normal;
}			t_cyl_utils;

bool	solve_discriminant(double *t0, double *t1, t_cyl_utils utils)
{
    t_vec3 delta_p;
	t_vec3 cross_rd_cd;
	t_vec3 cross_dp_cd;
	double discr;

    delta_p = vec3substr(utils.r->orig, utils.cyl->center);
    cross_rd_cd = cross(utils.r->dir, utils.cyl->axis);
    cross_dp_cd = cross(delta_p, utils.cyl->axis);

    // Solve the quadratic equation
    double a = dot(cross_rd_cd, cross_rd_cd);
    double b = 2 * dot(cross_rd_cd, cross_dp_cd);
    double c = dot(cross_dp_cd, cross_dp_cd) - pow(utils.cyl->radius, 2);
  
    discr = b * b - 4 * a * c;
    if (discr < 0)
        return false;
	*t0 = (-b - sqrt(discr)) / (2 * a);
    *t1 = (-b + sqrt(discr)) / (2 * a);
	return true;
}

/**
 * @brief: check if the ray hits the uncapped cylinder
 *
 * The cylinder is an infinite object so we need to check if the intersection
 * is within the height of the cylinder
 */
bool hit_cylinder(const void* self, const t_ray *r, t_interval ray_t, t_hit_record *rec)
{
	t_cyl_utils utils; 
	double			t0;
	double			t1;

	utils.cyl = (t_cylinder *)self;
	utils.r = r;
	utils.closest_t = -1;
	if (!solve_discriminant(&t0, &t1, utils))
		return false;

    t_vec3 closest_point;
    t_vec3 normal;
    bool hit = false;
    if (surrounds(&ray_t, t0)) {
        t_vec3 point = point_at(r, t0);
        t_vec3 delta_point = vec3substr(point, cyl->center);
        double height = dot(delta_point, cyl->axis);
        if (cyl->min < height && height < cyl->max) {
            if (closest_t < 0 || t0 < closest_t) {
                closest_t = t0;
                closest_point = point;
                // changed this for correct shadows
                // normal = unit_vector(cross(cyl->axis, delta_point));
                normal = unit_vector(vec3substr(point, vec3add(cyl->center, vec3multscalar(cyl->axis, height))));
                hit = true;
            }
        }
    }
    if (surrounds(&ray_t, t1)) {
        t_vec3 point = point_at(r, t1);
        t_vec3 delta_point = vec3substr(point, cyl->center);
        double height = dot(delta_point, cyl->axis);
        if (cyl->min < height && height < cyl->max) {
            if (closest_t < 0 || t1 < closest_t) {
                closest_t = t1;
                closest_point = point;
				// changed this for correct shadows
                // normal = unit_vector(cross(cyl->axis, delta_point));
                normal = unit_vector(vec3substr(point, vec3add(cyl->center, vec3multscalar(cyl->axis, height))));
                hit = true;
            }
        }
    }


    if (closest_t >= 0 && hit) {
        rec->t = closest_t;
        rec->p = closest_point;
        rec->normal = normal;
        set_face_normal(rec, r, rec->normal);
        rec->mat = cyl->mat;
        get_cylinder_uncappedv(rec->normal, rec->uv);
        return true;
    }

    return false;
}

/**
 * @brief: check if the ray hits the cylinder capped object
 *
 * This is essentially creating a hittable list with the cylinder and the two disks
 * and using the hit function for the hittable list on them
 */
bool	hit_cylinder_capped(const void* self, const t_ray *r, t_interval closest, t_hit_record *rec)
{
	const t_cylinder_capped *c = (t_cylinder_capped *)self;
	t_hittablelist cylinder_hittable_list;
	t_hittable *list[3];

	list[0] = (t_hittable*)(&c->cylinder_uncapped);
	list[1] = (t_hittable*)(&c->top);
	list[2] = (t_hittable*)(&c->bottom);
	cylinder_hittable_list = hittablelist(list, 3);
	return hit_objects(&cylinder_hittable_list, r, closest, rec);
}

/**
 * @brief: Computes the PDF value for a uniform cylinder.
 * @param: self: Pointer to the object
 * @param: orig: Pointer to the t_point3 origin vector
 * @param: dir: Pointer to the t_vec3 direction vector
 * @return: A double representing the PDF value for a uniform cylinder.
 */
double obj_cylinder_pdf_value(const void *self, const t_point3 *orig, const t_vec3 *dir) {
    const t_cylinder *cyl = (t_cylinder *)self;

    // Calculate distance squared from origin to cylinder axis
    t_vec3 axis_vector =  vec3substr(cyl->center, *orig);
    double axis_distance_squared = len_sqrd(axis_vector);

    // Prevent division by zero
    if (axis_distance_squared == 0) {
        return 0;
    }

    // Project the ray direction onto the cylinder axis
    double cos_theta = dot(axis_vector, *dir) / sqrt(axis_distance_squared);

    // Calculate the solid angle of the cylinder's surface visible from the origin
    double solid_angle = 2 * PI * cyl->radius * (1.0 - sqrt(1.0 - cos_theta * cos_theta));

    // Return the PDF (inverse of the solid angle)
    return 1.0 / solid_angle;
}

/**
 * @brief: Generates a random point on the surface of a cylinder.
 * @param: self: Pointer to the object
 * @param: orig: Pointer to the t_point3 origin vector
 * @return: A t_vec3 representing a random point on the surface of a cylinder.
 */
t_vec3 obj_cylinder_random(const void *self, const t_point3 *orig) {
    const t_cylinder *cyl = (t_cylinder *)self;

    // Calculate distance squared from origin to cylinder axis
    // t_vec3 axis_vector = vec3substr(cyl->center, *orig);
    // double axis_distance_squared = length_squared(axis_vector);

    // Generate a random point on the cylinder's surface
    double r1 = ((double)rand()) / RAND_MAX;
    double r2 = ((double)rand()) / RAND_MAX;
    double phi = 2.0 * PI * r1;
    double y = cyl->radius * sin(r2);
    double x = cyl->radius * cos(phi) * cos(r2);
    double z = cyl->radius * sin(phi) * cos(r2);

    // Translate the point to the cylinder's center
    t_vec3 point_on_cylinder = vec3add(cyl->center, vec3(x, y, z));

    // Calculate the direction vector from the origin to the point on the cylinder
    t_vec3 direction = vec3add(point_on_cylinder, *orig);

    // Normalize the direction vector
    return unit_vector(direction);
}

void get_cylinder_uncappedv(t_vec3 normal, double uv[2])
{
    double theta;
    double phi;

    // Calculate theta (angle around the cylinder's axis)
    theta = atan2(normal.x, normal.z);

    // Calculate phi (angle along the cylinder's height)
    phi = acos(normal.y);

    // Map theta and phi to UV coordinates
    uv[0] = theta / (2 * PI);
    uv[1] = phi / PI;
}
