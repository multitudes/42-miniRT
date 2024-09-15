/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 15:07:07 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/15 18:22:08 by lbrusa           ###   ########.fr       */
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
 * @brief: initializer for a cylinder
 * 
 * the rt file will have the following format:
 * cy 	50.0,0.0,20.6     0.0,0.0,1.0    14.2 21.42 	10,0,255
 * x,y,z coordinates of the center of the cylinder
 * 3d normalized vector of axis of cylinder. In range [-1,1] for each x,y,z axis
 * the cylinder diameter
 * the cylinder height
 * R,G,B colors in range [0,255]
*/
t_cylinder		cylinder(t_point3 center, t_vec3 axis, double diameter, double height, t_rgb rgbcolor)
{
	t_cylinder c;

	c.base.hit = hit_cylinder;
	c.base.pdf_value = obj_cylinder_pdf_value;
	c.base.random = obj_cylinder_random;
	c.center = center;
	c.axis = axis;
	c.radius = diameter / 2;
	c.height = height;
	c.rgb = rgbcolor;
	c.color = rgb_to_color(rgbcolor);
	// i use the color to create a texture
	solid_color_init(&(c.texture), c.color);
	// i init the lambertian material with the texture
	lambertian_init_tex(&(c.lambertian_mat), (t_texture*)&(c.texture));
	// i assign the material to the sphere as a pointer
	// the pointer will contain the scatter function for the material
	// which will be passed to the t_record struct when hit
 	c.mat = (t_material*)&(c.lambertian_mat); 

	c.print = &print_cylinder;
	return c;
}

t_cylinder		cylinder_mat(t_point3 center, t_vec3 axis, double diameter, double height, t_material *mat)
{
	t_cylinder c;

	c.base.hit = hit_cylinder;
	c.base.pdf_value = obj_cylinder_pdf_value;
	c.base.random = obj_cylinder_random;
	c.center = center;
	c.axis = axis;
	c.radius = diameter / 2;
	c.height = height;
	c.mat = mat;
	c.rgb = rgb(0, 0, 0);
	c.color = color(0, 0, 0);

	c.print = &print_cylinder;
	return c;
}


/**
 * @brief print the cylinder object
 * cy 50.0,0.0,20.6 	 0.0,0.0,1.0  	14.2	 21.42  	10,0,255
 */
void		print_cylinder(const void *self)
{
	const t_cylinder *c = (const t_cylinder *)self;
	printf("cy\t%.f,%.f,%.f\t\t%.f,%.f,%.f\t\t%f\t%f\t%d,%d,%d\n", 
	c->center.x, c->center.y, c->center.z, c->axis.x, c->axis.y, c->axis.z, c->radius * 2, c->height, c->rgb.r, c->rgb.g, c->rgb.b);

}

bool hit_cylinder(const void* self, const t_ray *r, t_interval ray_t, t_hit_record *rec) {
    const t_cylinder *cyl = (t_cylinder*)self;

	(void)cyl;
	(void)r;
	(void)ray_t;
	(void)rec;

    // Calculate coefficients for the quadratic equation
    double a = r->dir.x * r->dir.x + r->dir.z * r->dir.z;
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
	
    // Check if either hit point is within the cylinder's height range
    if (surrounds(&ray_t, t0) && fabs(point_at(r, t0).y - cyl->center.y) <= cyl->height / 2) {
        rec->t = t0;
		double y = point_at(r, t1).y;
		if (y > cyl->center.y + cyl->height / 2 || y < cyl->center.y - cyl->height / 2)
			return false;
        rec->p = point_at(r, t0);
        rec->normal = unit_vector(vec3((rec->p.x - cyl->center.x), 0.0, rec->p.z - cyl->center.z));
		set_face_normal(rec, r, rec->normal);
		rec->mat = cyl->mat;
        return true;
    } else if (surrounds(&ray_t, t1) && fabs(point_at(r, t1).y - cyl->center.y) <= cyl->height / 2) {
        rec->t = t1;
		double y = point_at(r, t1).y;
		if (y > cyl->center.y + cyl->height / 2 || y < cyl->center.y - cyl->height / 2)
			return false;
        rec->p = point_at(r, t1);
        rec->normal = unit_vector(vec3(rec->p.x - cyl->center.x, 0.0, rec->p.z - cyl->center.z));
        set_face_normal(rec, r, rec->normal);
		rec->mat = cyl->mat;
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