/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ralgaran <ralgaran@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 16:06:09 by ralgaran          #+#    #+#             */
/*   Updated: 2024/09/16 16:06:10 by ralgaran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cone.h"

void	cone_rgb(t_cone *c, t_point3 center, t_vec3 axis, double diam, double height, \
	t_rgb rgbcolor)
{
	c->base.hit = hit_cone;
	// TODO:
	// c->base.random = ...
	// c->base.pdf_value = ...
	c->center = center;				// center of the base
	c->axis = axis;
	c->radius = diam / 2;			// radius of the base
	c->height = height;
	c->color = rgb_to_color(rgbcolor);
	solid_color_init(&(c->texture), c->color);
	lambertian_init_tex(&(c->lambertian_mat), (t_texture*)&(c->texture));
	c->mat = (t_material*)&(c->lambertian_mat);
	disk(&c->bottom, center, axis, diam, rgbcolor);
}

void	cone_mat(t_cone *c, t_point3 center, t_vec3 axis, double diam, double height, \
	t_material *mat)
{
	c->base.hit = hit_cone;
	// TODO:
	// c->base.random = ...
	// c->base.pdf_value = ...
	c->center = center;				// center of the base
	c->axis = axis;
	c->radius = diam / 2;			// radius of the base
	c->height = height;
	c->mat = mat;
	disk_mat(&c->bottom, center, axis, diam, mat);
}

// TODO: call the base hit function here as well
bool	hit_cone(const void *self, const t_ray *r, t_interval ray_t, t_hit_record *rec)
{
	const t_cone	*co = (t_cone*)self;
	t_vec3	delta_p, cross_rd_cd, cross_dp_cd;

	delta_p = vec3substr(r->orig, co->center);
	cross_rd_cd = cross(r->dir, co->axis);
	cross_dp_cd = cross(delta_p, co->axis);

	double a = dot(cross_rd_cd, cross_rd_cd);
	double b = 2 * dot(cross_rd_cd, cross_dp_cd);
	double c = dot(cross_dp_cd, cross_dp_cd) - pow(co->radius, 2) * pow(co->height, 2);

    // Solve the quadratic equation
    double discriminant = b * b - 4 * a * c;
    if (discriminant < 0)
    	return false;

    double t0 = (-b - sqrt(discriminant)) / (2 * a);
    double t1 = (-b + sqrt(discriminant)) / (2 * a);

    // return false;
	// Initialize variables to track the closest intersection point
    double closest_t = -1;
    t_vec3 closest_point;
    t_vec3 normal;

	bool hit = false;

    // Check both intersections
	if (surrounds(&ray_t, t0)) {
		t_vec3 point = point_at(r, t0);
		t_vec3 delta_point = vec3substr(point, co->center);
		double height = dot(delta_point, co->axis);
		double radius_at_height = co->radius * (co->height - height) / co->height;
		if (height > 0 && height <= co->height && dot(delta_point, delta_point) <= pow(radius_at_height, 2))
		{
			// Check if this intersection point is closer to the origin
			if (closest_t < 0 || t0 < closest_t) {
				closest_t = t0;
				closest_point = point;
				normal = unit_vector(cross(co->axis, delta_point));
				hit = true;
			}
		}
	}
	if (surrounds(&ray_t, t1)) {
		t_vec3 point = point_at(r, t1);
		t_vec3 delta_point = vec3substr(point, co->center);
		double height = dot(delta_point, co->axis);
		double radius_at_height = co->radius * (co->height - height) / co->height;
		if (height > 0 && height <= co->height && dot(delta_point, delta_point) <= pow(radius_at_height, 2))
		{
			// Check if this intersection point is closer to the origin
			if (closest_t < 0 || t1 < closest_t) {
				closest_t = t1;
				closest_point = point;
				normal = unit_vector(cross(co->axis, delta_point));
				hit = true;
			}
		}
	}

    if (closest_t >= 0 && hit) {
        rec->t = closest_t;
        rec->p = closest_point;
        rec->normal = normal;
        set_face_normal(rec, r, rec->normal);
        rec->mat = co->mat;
        return true;
    }

    return false;
}
