/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hittable_list.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 12:23:23 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/26 16:43:20 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hittable.h"
#include "hittable_list.h"
#include "interval.h"
#include "ray.h"
#include <stdbool.h>
#include "utils.h"
#include "debug.h"

/*
 * This function is just for a single ray.
 *
 * Loops through all objects and checks if the ray hits any object from the world
 * hittable list. The ray might hit multiple objects, but we keep track of the
 * closest value on the ray, which is populated into the hit record rec.
*/
bool hit_objects(const void *self, const t_ray* r, t_interval ray_t, t_hit_record* rec)
{
	int i;
	bool hit_anything;
	t_hit_record temp_rec;
	t_hittablelist *objects;
	double closest_so_far;

	i = 0;
	hit_anything = false;
	objects = (t_hittablelist *)self;
	closest_so_far = ray_t.max;
	if (!objects || !objects->size || !objects->list || !objects->list[0])
			return (false);
	while (i < objects->size)
	{
		t_interval closest = interval(ray_t.min, closest_so_far);
		if (objects->list[i]->hit(objects->list[i], r, closest, &temp_rec))
		{
			hit_anything = true;
			closest_so_far = temp_rec.t;
			*rec = temp_rec;
		}
		i++;
	}
	return (hit_anything);
}

/* Init function for a hittable list. */
t_hittablelist hittablelist(t_hittable **list, int size)
{
	t_hittablelist hittablelist;

	hittablelist.list = list;
	hittablelist.size = size;
	hittablelist.hit_objects = hit_objects;
	hittablelist.obj_pdf_value = obj_pdf_value;
	hittablelist.obj_random = obj_random;
	return (hittablelist);
}

double obj_pdf_value(const void *self, const t_point3 *o, const t_vec3 *v)
{
	int i;
	double sum;
	t_hittablelist *objects;

	objects = (t_hittablelist *)self;
	if (!objects || !objects->size || !objects->list || !objects->list[0])
	{
		// debug("no objects in the list\n");
		return (0.0);
	}
	sum = 0.0;
	i = 0;
	while (i < objects->size)
	{
		sum += (1.0 / objects->size) * objects->list[i]->pdf_value(objects->list[i], o, v);
		i++;
	}
	return (sum);
}

t_vec3 obj_random(const void *self, const t_vec3 *o)
{
	int random_i;
	t_hittablelist *objects;

	objects = (t_hittablelist *)self;
	if (!objects || !objects->size || !objects->list || !objects->list[0])
	{
		// debug("no objects in the list\n");
		return (vec3(0,0,0));
	}
	random_i = random_int(0, objects->size);
	return (objects->list[random_i]->random(objects->list[random_i], o));
}
