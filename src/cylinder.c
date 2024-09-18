/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 15:07:07 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/18 15:23:37 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cylinder.h"
#include "vec3.h"
#include "ray.h"
#include "color.h"
#include <stdio.h>
#include "material.h"
#include "utils.h"
#include "debug.h"

/**
 * @brief: initializer for a cylinder uncapped standard material
 * 
 * the rt file will have the following format:
 * cy 	50.0,0.0,20.6     0.0,0.0,1.0    14.2 21.42 	10,0,255
 * x,y,z coordinates of the center of the cylinder
 * 3d normalized vector of axis of cylinder. In range [-1,1] for each x,y,z axis
 * the cylinder diameter
 * the cylinder height
 * R,G,B colors in range [0,255]
*/
void		cylinder_uncapped(t_cylinder *c, t_point3 center, t_vec3 axis, double diameter, double height, t_rgb rgbcolor)
{
	c->base.hit = hit_cylinder;
	c->base.pdf_value = obj_cylinder_pdf_value;
	c->base.random = obj_cylinder_random;
	c->center = center;
	c->axis = axis;
	c->radius = diameter / 2;
	c->height = height;
	c->min = -height / 2;
	c->max = height / 2;
	c->rgb = rgbcolor;
	c->color = rgb_to_color(rgbcolor);
	// i use the color to create a texture
	solid_color_init(&(c->texture), c->color);
	// i init the lambertian material with the texture
	lambertian_init_tex(&(c->lambertian_mat), (t_texture*)&(c->texture));
	// i assign the material to the sphere as a pointer
	// the pointer will contain the scatter function for the material
	// which will be passed to the t_record struct when hit
 	c->mat = (t_material*)&(c->lambertian_mat); 
	c->print = &print_cylinder;
}

/**
 * @brief: initializer for a cylinder uncapped with a material
 * 
 * the rt file will have the following format:
 * 	
 */
void		cylinder_mat_uncapped(t_cylinder *c, t_point3 center, t_vec3 axis, double diameter, double height, t_material *mat)
{
	c->base.hit = hit_cylinder;
	c->base.pdf_value = obj_cylinder_pdf_value;
	c->base.random = obj_cylinder_random;
	c->center = center;
	c->axis = axis;
	c->radius = diameter / 2;
	c->height = height;
	c->min = -height / 2;
	c->max = height / 2;
	c->mat = mat;
	c->rgb = rgb(0, 0, 0);
	c->color = color(0, 0, 0);
	c->print = &print_cylinder;
}

/**
 * @brief: initializer for a cylinder capped with a standard material
 * 
 * the rt file will have the following format:
 * 	
 */
void cylinder_capped(t_cylinder_capped *c, t_point3 center, t_vec3 axis, double diameter, double height, t_rgb rgbcolor)
{
	t_vec3 u, v;

	c->base.hit = hit_cylinder_capped;
	c->base.pdf_value = obj_pdf_value;
	c->base.random = obj_random;
	cylinder_uncapped(&c->cylinder_uncapped, center, axis, diameter, height, rgbcolor);
	// just need to create two vector parallel to the axis
	// make sure not parallel to the y axis
    u = vec3(1, 0, 0);
	if (fabs(axis.x) > fabs(axis.y)) {
        u = vec3(0, 1, 0);
    }
 	v = unit_vector(cross(axis, u)); 
    u = unit_vector(cross(v, axis)); 

    u = vec3multscalar(u, diameter);
    v = vec3multscalar(v, diameter);
  	
	// Calculate the positions of the top and bottom caps
    t_point3 top_center = vec3add(center, vec3multscalar(axis, height / 2));
    t_point3 bottom_center = vec3substr(center, vec3multscalar(axis, height / 2));

	disk_mat(&c->top, top_center, u, v, c->cylinder_uncapped.mat);
	disk_mat(&c->bottom, bottom_center, u, v, c->cylinder_uncapped.mat);
	c->print = &print_cylinder_capped;
}

void cylinder_mat_capped(t_cylinder_capped *c, t_point3 center, t_vec3 axis, double diameter, double height, t_material *mat)
{
	c->base.hit = hit_cylinder_capped;
	c->base.pdf_value = obj_pdf_value;
	c->base.random = obj_random;
	cylinder_mat_uncapped(&c->cylinder_uncapped, center, axis, diameter, height, mat);
	// just need to create two vector parallel to the axis
	t_vec3 u, v;
	// make sure not parallel to the y axis
    u = vec3(1, 0, 0);
	if (fabs(axis.x) > fabs(axis.y)) {
        u = vec3(0, 1, 0);
    }
 	v = unit_vector(cross(axis, u)); 
    u = unit_vector(cross(v, axis)); 

    u = vec3multscalar(u, diameter / 2);
    v = vec3multscalar(v, diameter / 2);
    t_point3 top_center = vec3add(center, vec3multscalar(axis, height / 2));
    t_point3 bottom_center = vec3add(center, vec3multscalar(axis, -height / 2));

	disk_mat(&c->top, top_center, u, v, c->cylinder_uncapped.mat);
	disk_mat(&c->bottom, bottom_center, u, v, c->cylinder_uncapped.mat);

	
}


/**
 * @brief print the cylinder capped object
 * 
 * The print will just print the cylinder which is a part of the capped cylinder
 */
void print_cylinder_capped(const void *self)
{
	const t_cylinder_capped *c = (const t_cylinder_capped *)self;
	c->cylinder_uncapped.print(&c->cylinder_uncapped);
}

/**
 * @brief print the cylinder object
 * cy 50.0,0.0,20.6 	 0.0,0.0,1.0  	14.2	 21.42  	10,0,255
 */
void		print_cylinder(const void *self)
{
	const t_cylinder *c = (const t_cylinder *)self;
	printf("cy\t%.f,%.f,%.f\t\t%.f,%.f,%.f\t\t%.f\t%.f\t%d,%d,%d\n", 
	c->center.x, c->center.y, c->center.z, c->axis.x, c->axis.y, c->axis.z, c->radius * 2, c->height, c->rgb.r, c->rgb.g, c->rgb.b);

}

/**
 * @brief: check if the ray hits the uncapped cylinder
 * 
 * The cylinder is an infinite object so we need to check if the intersection 
 * is within the height of the cylinder
 */
bool hit_cylinder(const void* self, const t_ray *r, t_interval ray_t, t_hit_record *rec) {
    const t_cylinder *cyl = (t_cylinder*)self;

    // Calculate coefficients for the quadratic equation
    double a = r->dir.x * r->dir.x + r->dir.z * r->dir.z;
	if (fabs(a) <= EPSILON)
		return false;
    double b = 2 * (r->dir.x * (r->orig.x - cyl->center.x) + r->dir.z * (r->orig.z - cyl->center.z));
    double c = (r->orig.x - cyl->center.x) * (r->orig.x - cyl->center.x) + (r->orig.z - cyl->center.z) * (r->orig.z - cyl->center.z) - cyl->radius * cyl->radius;

    // Solve the quadratic equation
    double discriminant = b * b - 4 * a * c;
    if (discriminant < 0) {
        return false;
    }

	rec->v = 0;
	rec->u = 0;	
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
		if (cyl->min < point.y && point.y < cyl->max) {
			// Check if this intersection point is closer to the origin
			if (closest_t < 0 || t0 < closest_t) {
				closest_t = t0;
				closest_point = point;
				normal = unit_vector(vec3(point.x - cyl->center.x, 0.0, point.z - cyl->center.z));
				hit = true;
			}
		}
	}
	if (surrounds(&ray_t, t1)) {
		t_vec3 point = point_at(r, t1);
		if (cyl->min < point.y && point.y < cyl->max) {
			// Check if this intersection point is closer to the origin
			if (closest_t < 0 || t1 < closest_t) {
				closest_t = t1;
				closest_point = point;
				normal = unit_vector(vec3(point.x - cyl->center.x, 0.0, point.z - cyl->center.z));
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
		get_cylinder_uncappedv(rec->normal, &rec->u, &rec->v);
        return true;
    }

    return false;
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
    double axis_distance_squared = length_squared(axis_vector);

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

void get_cylinder_uncappedv(t_vec3 normal, double* u, double* v) 
{
    double theta;
    double phi;

    // Calculate theta (angle around the cylinder's axis)
    theta = atan2(normal.x, normal.z);

    // Calculate phi (angle along the cylinder's height)
    phi = acos(normal.y);

    // Map theta and phi to UV coordinates
    *u = theta / (2 * PI);
    *v = phi / PI;
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