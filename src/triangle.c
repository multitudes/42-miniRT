/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   triangle.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 18:04:25 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/13 14:11:42 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "color.h"
#include "material.h"
#include "texture.h"
#include "triangle.h"
#include "utils.h"
#include "vec3.h"
#include <stdio.h>
#include "libft.h"

void	triangle(t_triangle *tri, t_init_params params)
{
	tri->base.hit = hit_triangle;
	tri->base.pdf_value = triangle_pdf_value;
	tri->base.random = triangle_random;
	tri->a = params.a;
	tri->b = params.b;
	tri->c = params.c;
	tri->edge1 = vec3substr(params.b, params.a);
	tri->edge2 = vec3substr(params.c, params.a);
	tri->rgb = params.rgbcolor;
	tri->color = rgb_to_color(params.rgbcolor);
	tri->normal = unit_vector(cross(tri->edge2, tri->edge1));
	tri->d = dot(tri->normal, params.a);
	tri->area = 0.5 * length(cross(vec3substr(params.b, params.a),
				vec3substr(params.c, params.a)));
	solid_color_init(&(tri->texture), tri->color);
	lambertian_init_tex(&(tri->lambertian_mat), (t_texture *)&(tri->texture));
	tri->mat = (t_material *)&(tri->lambertian_mat);
	tri->print = print_triangle;
}

void	triangle_mat(t_triangle *tri, t_init_params params)
{
	tri->base.hit = hit_triangle;
	tri->base.pdf_value = triangle_pdf_value;
	tri->base.random = triangle_random;
	tri->a = params.a;
	tri->b = params.b;
	tri->c = params.c;
	tri->edge1 = vec3substr(params.b, params.a);
	tri->edge2 = vec3substr(params.c, params.a);
	tri->normal = unit_vector(cross(tri->edge2, tri->edge1));
	tri->d = dot(tri->normal, params.a);
	tri->area = 0.5 * length(cross(vec3substr(params.b, params.a),
				vec3substr(params.c, params.a)));
	tri->mat = params.mat;
	tri->print = print_triangle;
}

void	print_triangle(const void *self)
{
	t_triangle	*tri;

	tri = (t_triangle *)self;
	ft_printf("tri %.f,%.f,%.f %.f,%.f,%.f %.f,%.f,%.f %d,%d,%d\n", tri->a.x,
		tri->a.y, tri->a.z, tri->b.x, tri->b.y, tri->b.z, tri->c.x, tri->c.y,
		tri->c.z, tri->rgb.r, tri->rgb.g, tri->rgb.b);
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
bool	hit_triangle(const void *self, const t_ray *r, t_interval ray_t,
		t_hit_record *rec)
{
	t_vec3	dirxe2;
	double	det;
	double	f;
	t_vec3	p1;
	t_vec3	oxe1;

	dirxe2 = cross(r->dir, ((t_triangle *)self)->edge2);
	det = dot(((t_triangle *)self)->edge1, dirxe2);
	if (fabs(det) < EPSILON)
		return (false);
	f = 1.0 / det;
	p1 = vec3substr(r->orig, ((t_triangle *)self)->a);
	if ((f * dot(p1, dirxe2)) < 0 || (f * dot(p1, dirxe2)) > 1)
		return (false);
	oxe1 = cross(p1, ((t_triangle *)self)->edge1);
	if ((f * dot(r->dir, oxe1)) < 0 || (f * dot(p1, dirxe2)) + (f * dot(r->dir,
				oxe1)) > 1)
		return (false);
	rec->t = f * dot(((t_triangle *)self)->edge2, oxe1);
	if (!contains(&ray_t, rec->t))
		return (false);
	rec->p = point_at(r, rec->t);
	rec->mat = ((t_triangle *)self)->mat;
	set_face_normal(rec, r, ((t_triangle *)self)->normal);
	return (true);
}
