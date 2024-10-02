/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_utils4.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 18:44:14 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/02 18:45:08 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include "vec3.h"

t_vec3	random_unit_vector(void)
{
	return unit_vector(random_in_unit_sphere());
}

t_vec3	reflect(const t_vec3 v, const t_vec3 n)
{
	return vec3substr(v, vec3multscalar(n, dot(v, n) * 2));
}

t_vec3	refract(const t_vec3 uv, const t_vec3 n, double etai_over_etat)
{
	double	cos_theta;
	t_vec3	r_out_perp;
	t_vec3	r_out_parallel;

	cos_theta = fmin(dot(vec3negate(uv), n), 1.0);
	r_out_perp = vec3multscalar(vec3add(uv, vec3multscalar(n, cos_theta)),
			etai_over_etat);
	r_out_parallel = vec3multscalar(n, -sqrt(fabs(1.0
					- length_squared(r_out_perp))));
	return vec3add(r_out_perp, r_out_parallel);
}

double	reflectance(double cosine, double refraction_index)
{
	double	r0;

	// Use Schlick's approximation for reflectance.
	r0 = (1 - refraction_index) / (1 + refraction_index);
	r0 = r0 * r0;
	return r0 + (1 - r0) * pow((1 - cosine), 5);
}

t_vec3	random_cosine_direction(void)
{
	double r1 = random_d();
	double r2 = random_d();

	double phi = 2 * PI * r1;
	double x = cos(phi) * sqrt(r2);
	double y = sin(phi) * sqrt(r2);
	double z = sqrt(1 - r2);

	return vec3(x, y, z);
}