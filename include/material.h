/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   material.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 15:32:29 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/06 15:57:27 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATERIAL_H
# define MATERIAL_H

#include "texture.h"
#include "ray.h"
#include "color.h"


//forward declaration
typedef struct 		s_hit_record t_hit_record;
typedef struct 		s_texture t_texture;

typedef struct 		s_material
{
	bool 			(*scatter)(void *self, const t_ray *r_in, const t_hit_record *rec, t_color *attenuation, t_ray *scattered, double *pdf);
	t_color			(*emit)(void *self, const t_hit_record *rec, double u, double v, t_point3);
	double 			(*scattering_pdf)(void *self, const t_ray *r_in, const t_hit_record *rec, const t_ray *scattered);

}					t_material;

// Define a structure for Lambertian material
typedef struct 		s_lambertian
{
    t_material 		base; // Base material structure
	t_texture	 	*texture;
    t_color 		albedo;  // Albedo specific to Lambertian material
}               	t_lambertian;

typedef struct 		s_diffuse_light
{
	t_material		base;
	t_texture		*texture;
	
}					t_diffuse_light;


#endif