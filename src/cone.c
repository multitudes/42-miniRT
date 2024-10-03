/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 16:06:09 by ralgaran          #+#    #+#             */
/*   Updated: 2024/09/30 09:59:18 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cone.h"
#include "utils.h"
#include <math.h>

/* forward declaration */
bool	hit_cone_cap(const void* self, const t_ray *r, t_interval closest, t_hit_record *rec);
double	obj_cone_pdf_value(const void *self, const t_point3 *orig, const t_vec3 *dir);
t_vec3	obj_cone_random(const void *self, const t_point3 *orig);

/*
 * Inits a capped cone with an rgb color.
*/
void	cone_rgb(t_cone *c, t_point3 apex, t_vec3 axis, double diam, double height, \
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

	c->body.apex = apex;
	c->body.axis = unit_vector(vec3multscalar(axis, -1));
	c->body.radius = diam / 2;
	c->body.height = height;
	
	c->body.color = rgb_to_color(rgbcolor);
	solid_color_init(&(c->body.texture), c->body.color);
	lambertian_init_tex(&(c->body.lambertian_mat), (t_texture*)&(c->body.texture));
	c->body.mat = (t_material*)&(c->body.lambertian_mat);
	
	t_point3 bottom_center = vec3add(apex, vec3multscalar(c->body.axis, height));
	disk(&c->bottom, bottom_center, axis, diam, rgbcolor);
}

/*
 * Inits a capped cone with a material (metal).
*/
void	cone_mat(t_cone *c, t_point3 apex, t_vec3 axis, double diam, double height, \
	t_material *mat)
{
	// this for the whole cylinder
	c->base.hit = hit_cone_cap;
	c->base.pdf_value = obj_pdf_value;
	c->base.random = obj_random;
	
	// this for the body only (uncapped)
	c->body.base.hit = hit_cone;
	c->body.base.random = obj_cone_random;
	c->body.base.pdf_value = obj_cone_pdf_value;

	c->body.apex = apex;
	c->body.axis = unit_vector(vec3multscalar(axis, -1));
	c->body.radius = diam / 2;
	c->body.height = height;
	
	c->body.mat = mat;
	
	t_point3 bottom_center = vec3add(apex, vec3multscalar(c->body.axis, height));
	disk_mat(&c->bottom, bottom_center, axis, diam, mat);
}

bool hit_cone(const void* self, const t_ray *r, t_interval ray_t, t_hit_record *rec)
{
    t_cone_uncap *cone = (t_cone_uncap*)self;
    t_vec3 delta_p, cross_rd_ca, cross_dp_ca;

    delta_p = vec3substr(r->orig, cone->apex);
    cross_rd_ca = cross(r->dir, cone->axis);
    cross_dp_ca = cross(delta_p, cone->axis);

    double a = dot(cross_rd_ca, cross_rd_ca) - dot(r->dir, r->dir) * pow(cone->radius / cone->height, 2);
    double b = 2 * (dot(cross_rd_ca, cross_dp_ca) - dot(r->dir, delta_p) * pow(cone->radius / cone->height, 2));
    double c = dot(cross_dp_ca, cross_dp_ca) - dot(delta_p, delta_p) * pow(cone->radius / cone->height, 2);
    
    // Solve the quadratic equation
    double discriminant = b * b - 4 * a * c;
    if (discriminant < 0)
        return false;

    double t0 = (-b - sqrt(discriminant)) / (2 * a);
    double t1 = (-b + sqrt(discriminant)) / (2 * a);

    // Initialize variables to track the closest intersection point
    double closest_t = -1;
    t_vec3 closest_point;
    t_vec3 normal;

    bool hit = false;

    // Check both intersections
    if (surrounds(&ray_t, t0))
    {
        t_vec3 point = point_at(r, t0);
        t_vec3 delta_point = vec3substr(point, cone->apex);
        double height = dot(delta_point, cone->axis);
        if (0 <= height && height <= cone->height)
        {
            // Check if this intersection point is closer to the origin
            if (closest_t < 0 || t0 < closest_t)
            {
                closest_t = t0;
                closest_point = point;
                normal = unit_vector(cross(cone->axis, delta_point));
                hit = true;
            }
        }
    }
    if (surrounds(&ray_t, t1))
    {
        t_vec3 point = point_at(r, t1);
        t_vec3 delta_point = vec3substr(point, cone->apex);
        double height = dot(delta_point, cone->axis);
        if (0 <= height && height <= cone->height)
        {
            // Check if this intersection point is closer to the origin
            if (closest_t < 0 || t1 < closest_t)
            {
                closest_t = t1;
                closest_point = point;
                normal = unit_vector(cross(cone->axis, delta_point));
                hit = true;
            }
        }
    }

    if (closest_t >= 0 && hit)
    {
        rec->t = closest_t;
        rec->p = closest_point;
        rec->normal = normal;
        set_face_normal(rec, r, rec->normal);
        rec->mat = cone->mat;
        // get_cone_uncappedv(rec->normal, cone->radius, cone->height, &rec->u, &rec->v);
        return true;
    }

    return false;
}

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
	const t_cone *cone = (t_cone *)self;
	
    // Calculate distance squared from origin to cylinder axis
    t_vec3 apex_to_orig =  vec3substr(*orig, cone->body.apex);
    // double apex_to_orig_len = length_squared(apex_to_orig);

    t_vec3 apex_to_orig_norm = unit_vector(apex_to_orig);
    
    // Project the ray direction onto the cylinder axis
    double cos_theta = dot(cone->body.axis, apex_to_orig_norm);

    double half_angle_cos = cone->body.height / sqrt(pow(cone->body.height, 2) + pow(cone->body.radius, 2));
  
    if (cos_theta < half_angle_cos)
    	return (0);		// outside the cone
    
    // Calculate the solid angle of the cylinder's surface visible from the origin
    double solid_angle = 2 * PI * (1 - half_angle_cos);

    // Return the PDF (inverse of the solid angle)
    return 1.0 / solid_angle;
}

/*
 * Returns a random point on the surface of a cone.
*/
t_vec3 obj_cone_random(const void *self, const t_point3 *orig)
{
	const t_cone_uncap *cone = (const t_cone_uncap *)self;

    // Generate a random angle phi
    double r1 = ((double)rand() / RAND_MAX);
    double phi = 2.0 * PI * r1; // Random angle in [0, 2π]

    // Generate a random height along the cone's height
    double r2 = ((double)rand() / RAND_MAX);
    double h = r2 * cone->height; // Random height in [0, height]

    // Calculate the radius at this height
    double radius_at_height = (cone->height - h) * tan(cone->angle);

    // Calculate the x and z coordinates on the cone's surface
    double x = radius_at_height * cos(phi);
    double z = radius_at_height * sin(phi);

    // The y coordinate is the height
    double y = h;

    // Create the point on the cone's surface
    t_vec3 point_on_cone = vec3(x, y, z);

    // Translate the point to the origin
    point_on_cone = vec3add(point_on_cone, *orig);

    // Return the point on the cone's surface
    return point_on_cone;
}
