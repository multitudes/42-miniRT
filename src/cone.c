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
#include "debug.h"

/* forward declaration */
bool	hit_cone_cap(const void* self, const t_ray *r, t_interval closest, t_hit_record *rec);
double	obj_cone_pdf_value(const void *self, const t_point3 *orig, const t_vec3 *dir);
t_vec3	obj_cone_random(const void *self, const t_point3 *orig);

/*
 * Inits a capped cone with an rgb color.
*/
void	cone_rgb(t_cone *c, t_point3 center, t_vec3 axis, double angle, double height, \
	t_rgb rgbcolor)
{
	// this for the whole cylinder
	c->base.hit = hit_cone_cap;
	c->base.pdf_value = obj_pdf_value;
	c->base.random = obj_random;
	
	// this for the body only (uncapped)
	c->body.base.hit = hit_cone;
	c->body.base.random = obj_cone_random;
	c->body.base.pdf_value = obj_cone_pdf_value;
	
	c->body.center = center;				// center of the base
	c->body.axis = axis;
	c->body.angle = angle / 2;			// radius of the base
	c->body.height = height;
	c->body.color = rgb_to_color(rgbcolor);
	solid_color_init(&(c->body.texture), c->body.color);
	lambertian_init_tex(&(c->body.lambertian_mat), (t_texture*)&(c->body.texture));
	c->body.mat = (t_material*)&(c->body.lambertian_mat);
	disk(&c->bottom, center, axis, diam, rgbcolor);
}

/*
 * Inits a capped cone with a material (metal).
*/
void	cone_mat(t_cone *c, t_point3 center, t_vec3 axis, double angle, double height, \
	t_material *mat)
{
	c->base.hit = hit_cone_cap;
	c->base.random = obj_cone_random;
	c->base.pdf_value = obj_cone_pdf_value;
	
	// this for the body only (uncapped)
	c->body.base.hit = hit_cone;
	c->body.base.random = obj_cone_random;
	c->body.base.pdf_value = obj_cone_pdf_value;
	
	c->body.center = center;				// center of the base
	c->body.axis = axis;
	c->body.angle = angle / 2;			// radius of the base
	c->body.height = height;
	c->body.mat = mat;
	disk_mat(&c->bottom, center, axis, diam, mat);
}

bool	hit_cone(const void *self, const t_ray *r, t_interval ray_t, t_hit_record *rec)
{
	const t_cone_uncap *co = (t_cone_uncap*)self;
	
	
	
}



// bool	hit_cone(const void *self, const t_ray *r, t_interval ray_t, t_hit_record *rec)
// {
// 	const t_cone_uncap	*co = (t_cone_uncap*)self;
// 	t_vec3	delta_p, cross_rd_cd, cross_dp_cd;

// 	delta_p = vec3substr(r->orig, co->center);
// 	cross_rd_cd = cross(r->dir, co->axis);
// 	cross_dp_cd = cross(delta_p, co->axis);

// 	double a = dot(cross_rd_cd, cross_rd_cd);
// 	double b = 2 * dot(cross_rd_cd, cross_dp_cd);
// 	double c = dot(cross_dp_cd, cross_dp_cd) - pow(co->radius, 2) * pow(co->height, 2);

//     // Solve the quadratic equation
//     double discriminant = b * b - 4 * a * c;
//     if (discriminant < 0)
//     	return false;

//     double t0 = (-b - sqrt(discriminant)) / (2 * a);
//     double t1 = (-b + sqrt(discriminant)) / (2 * a);

//     // return false;
// 	// Initialize variables to track the closest intersection point
//     double closest_t = -1;
//     t_vec3 closest_point;
//     t_vec3 normal;

// 	bool hit = false;

//     // Check both intersections
// 	if (surrounds(&ray_t, t0)) {
// 		t_vec3 point = point_at(r, t0);
// 		t_vec3 delta_point = vec3substr(point, co->center);
// 		double height = dot(delta_point, co->axis);
// 		double radius_at_height = co->radius * (co->height - height) / co->height;
// 		if (height > 0 && height <= co->height && length(cross(delta_point, co->axis)) <= radius_at_height)
// 		{
// 			debug("t0 hit\n");
// 			// Check if this intersection point is closer to the origin
// 			if (closest_t < 0 || t0 < closest_t) {
// 				closest_t = t0;
// 				closest_point = point;
// 				normal = unit_vector(cross(co->axis, delta_point));
// 				hit = true;
// 			}
// 		}
// 	}
// 	if (surrounds(&ray_t, t1)) {
// 		t_vec3 point = point_at(r, t1);
// 		t_vec3 delta_point = vec3substr(point, co->center);
// 		double height = dot(delta_point, co->axis);
// 		double radius_at_height = co->radius * (co->height - height) / co->height;
// 		if (height > 0 && height <= co->height && length(cross(delta_point, co->axis)) <= radius_at_height)
// 		{
// 			debug("t1 hit\n");
// 			// Check if this intersection point is closer to the origin
// 			if (closest_t < 0 || t1 < closest_t) {
// 				closest_t = t1;
// 				closest_point = point;
// 				normal = unit_vector(cross(co->axis, delta_point));
// 				hit = true;
// 			}
// 		}
// 	}

//     if (closest_t >= 0 && hit) {
//         rec->t = closest_t;
//         rec->p = closest_point;
//         rec->normal = normal;
//         set_face_normal(rec, r, rec->normal);
//         rec->mat = co->mat;
//         return true;
//     }

//     return false;
// }

/*
 * Make a hittable list to pass into hit_objects.
 * The point is that hit_objects is going to call the hit function
 * of cylinder itself and the base.
*/
bool	hit_cone_cap(const void* self, const t_ray *r, t_interval closest, t_hit_record *rec)
{
	const t_cone	*c = (t_cone*)self;
	t_hittablelist	cone_hit_list;
	t_hittable		*list[2];
	
	list[0] = (t_hittable*)(&c->body);
	list[1] = (t_hittable*)(&c->bottom);
	cone_hit_list = hittablelist(list, 2);
	return (hit_objects(&cone_hit_list, r, closest, rec));
}

/*
 * Calculates the pdf value of a cone
*/
double obj_cone_pdf_value(const void *self, const t_point3 *orig, const t_vec3 *dir)
{
	(void)self;
	(void)orig;
	(void)dir;
	//TODO:
	return (0);
}

/*
 * Returns a random point on the surface of a cone.
*/
t_vec3 obj_cone_random(const void *self, const t_point3 *orig)
{
	(void)self;
	(void)orig;
	//TODO:
	// when would the be called ??
	return (vec3(0,0,0));
}
