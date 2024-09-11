/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   material.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 15:32:29 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/11 14:35:58 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATERIAL_H
# define MATERIAL_H

#include "pdf.h"
#include "texture.h"
#include "ray.h"
#include "color.h"
#include "vec3.h"
#include "hittable_list.h"

typedef struct s_scatter_record t_scatter_record;
typedef struct s_hit_record t_hit_record;
typedef struct s_pdf t_pdf;
typedef struct s_cosine_pdf t_cosine_pdf;
typedef struct s_sphere_pdf t_sphere_pdf;
typedef struct s_hittable_pdf t_hittable_pdf;
typedef struct s_mixture_pdf t_mixture_pdf;
/**
 * @brief Material structure
 * 
 * The material struct as base struct for all materials
 * It uses basically three functions:
 * - scatter: to calculate the scattered ray for metals and dielectrics
 * - emit: to calculate the emitted light when the material is a light source
 * - scattering_pdf: to calculate the probability density function
 */
typedef struct 		s_material
{
	bool 			(*scatter)(void *self,  t_ray *r_in, t_hit_record *rec, t_scatter_record *srec);
	t_color			(*emit)(void *self, t_hit_record rec, double u, double v, t_point3);
	double 			(*scattering_pdf)(void *self, const t_ray *r_in, const t_hit_record *rec, const t_ray *scattered);
}					t_material;

//forward declaration
typedef struct 		s_hit_record t_hit_record;
typedef struct 		s_texture t_texture;

/**
 * @brief Hit record structure
 * 
 * Not every shape and functions will use all pdfs which are 
 * "inherited" from the t_pdf structure. However unfortunately
 * in the ray_color function I dont know which pdf will be used
 * and I initialize the pdf_ptr... therefore depending of the 
 * scatter function called, a different pdf will be created 
 * and then casted down to a pointer to the generate and value 
 * functions. Since we have all structs on the stack they have 
 * to reside somewhere therefore I need to keep them in the 
 * struct to be able to reference them.
 */
typedef struct 	s_scatter_record 
{
    t_color 		attenuation;
    t_pdf* 			pdf_ptr;
	t_cosine_pdf 	*cosine_pdf;
	t_sphere_pdf 	*sphere_pdf;
	t_hittable_pdf 	*hittable_pdf;
	t_mixture_pdf 	*mixture_pdf;
    bool 			skip_pdf;
    t_ray 			skip_pdf_ray;
} 				t_scatter_record;

typedef struct 		s_metal
{
	t_material 		base; // Base material structure
	t_color 		albedo;  // Albedo specific to Metal material
	double 			fuzz;     // Fuzziness of the metal
}               	t_metal;


/**
 * @brief Lambertian material structure
 * It will "inherit" base from the material structure
 * adding the albedo attribute and a pointer to a texture 
 */
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

void		set_face_normal(t_hit_record *rec, const t_ray *r, const t_vec3 outward_normal);

bool 		noscatter(void *self,  t_ray *r_in,  t_hit_record *rec, t_scatter_record *srec);
t_color		emitzero(void *self, t_hit_record rec, double u, double v, t_point3 p);
double 		scattering_pdf_zero(void* self, const t_ray *r_in, const t_hit_record *rec, const t_ray *scattered);

void 		lambertian_init(t_lambertian *lambertian_material, t_color albedo);
void		lambertian_init_tex(t_lambertian *lambertian_material, t_texture *tex);

void		diffuse_light_init(t_diffuse_light *diffuse_light, t_texture *tex);

bool		lambertian_scatter(void* self,  t_ray *r_in,  t_hit_record *rec, t_scatter_record *srec); ;
double 		lambertian_scattering_pdf(void* self, const t_ray *r_in, const t_hit_record *rec, const t_ray *scattered);

void		metal_init(t_metal *metal, t_color albedo, double fuzz);
bool		metal_scatter(void *self, t_ray* r_in, t_hit_record *rec, t_scatter_record *srec);
double		metal_scattering_pdf(void* self, const t_ray *r_in, const t_hit_record *rec, const t_ray *scattered);

t_color		emitlight(void *self, t_hit_record rec, double u, double v, t_point3 p);



#endif