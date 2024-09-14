/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 14:59:39 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/14 16:46:11 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "plane.h"
#include <stdio.h>
#include "utils.h"

t_plane plane(t_point3 point, t_vec3 normal, t_rgb rgbcol) 
{
    t_plane pl;

    pl.base.hit = hit_plane;
    pl.base.pdf_value = plane_pdf_value;
    pl.base.random = plane_random;
    pl.q = point;
    pl.normal = unit_vector(normal);
    pl.d = -dot(pl.normal, point); // Calculate D for the plane equation
	pl.u = vec3(PLANE_MAX, PLANE_MAX, PLANE_MAX);
	pl.v = vec3(PLANE_MAX, PLANE_MAX, PLANE_MAX);
    pl.rgbcolor = rgbcol;
    pl.color = rgb_to_color(rgbcol);

    // Initialize texture and material as I did for the quad
    solid_color_init(&(pl.texture), pl.color);
    lambertian_init_tex(&(pl.lambertian_mat), (t_texture*)&(pl.texture));
    pl.mat = (t_material*)&(pl.lambertian_mat);
	// print plane for the rt file
	pl.print = print_plane;
    return pl;
}



/**
 * @brief print the plane object
 * 
 * format is like this 
 * pl 0.0,0.0,-10.0 0.0,1.0,0.0 0,0,225
 */
void		print_plane(const void *self)
{
	const t_plane *p = (const t_plane *)self;
	printf("pl\t%.f,%.f,%.f\t\t%.f,%.f,%.f\t\t\t%d,%d,%d\n", 
	p->q.x, p->q.y, p->q.z, p->normal.x, p->normal.y, p->normal.z, p->rgb.r, p->rgb.g, p->rgb.b);

}



bool hit_plane(const void* self, const t_ray *r, t_interval ray_t,  t_hit_record *rec)
{
	// printf("hit_quad ----------------------********\n");
	const t_plane *pl = (t_plane *)self;
	double denom = dot(pl->normal, r->dir);
	// no hit if ray is parallel to the quad
	if (fabs(denom) < 1e-8)
		return false;

	// Return false if the hit point parameter t is outside the ray interval.
	double t = (pl->d - dot(pl->normal, r->orig)) / denom;
	if (!contains(&ray_t, t))
		return false;

	rec->t = t;
	rec->p = point_at(r, t);
	rec->mat = pl->mat;
	set_face_normal(rec, r, pl->normal);

	return true;
}

double plane_pdf_value(const void *self, const t_point3 *orig, const t_vec3 *dir) 
{
    const t_plane *pl = (t_plane *)self;
    t_hit_record rec;
    const t_ray r = ray(*orig, *dir);
    if (!hit_plane(pl, &r, interval(0.001, INFINITY), &rec))
        return 0;

    // Calculate the distance squared from the origin to the hit point
    double distance_squared = length_squared(vec3substr(rec.p, *orig));

    // Calculate the cosine of the angle between the ray and the plane normal
    double cosine = fabs(dot(*dir, pl->normal));

    // Calculate the area of the infinite plane (which is infinite)
    double area = INFINITY;

    return distance_squared / (cosine * area);
}

t_vec3 plane_random(const void *self, const t_point3 *orig)
{
	const t_plane *pl = (t_plane *)self;
	double max_u = random_double(0, 1) * PLANE_MAX;
    double max_v = random_double(0, 1) * PLANE_MAX;
	t_vec3 p = vec3add(pl->q, vec3add(vec3multscalar(pl->u, max_u), vec3multscalar(pl->v, max_v)));
	return vec3substr(p, *orig);
}

