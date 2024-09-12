/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hittable_list.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 12:23:23 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/12 15:58:58 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hittable.h"
#include "hittable_list.h"
#include "interval.h"
#include "ray.h"
#include <stdbool.h>
#include "utils.h"

bool hit_objects(const void *self, const t_ray* r, t_interval ray_t, t_hit_record* rec)
{
	t_hittablelist *hittablelist = (t_hittablelist *)self;
	t_hit_record temp_rec;
	bool hit_anything = false;
	double closest_so_far = ray_t.max;
	int i = 0;

	if (!hittablelist)
			return (false);
	while (i < hittablelist->size)
	{	
		t_interval closest = interval(ray_t.min, closest_so_far);
		if (hittablelist->list[i]->hit(hittablelist->list[i], r, closest, &temp_rec))
		{
			hit_anything = true;
			closest_so_far = temp_rec.t;
			*rec = temp_rec;
		}
		i++;
	} 
	return (hit_anything);
}

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
	t_hittablelist *objects = (t_hittablelist *)self;
	double weight = 1.0 / objects->size;
	double sum = 0.0;
	int i = 0;

	while (i < objects->size)
	{
		sum += weight * objects->list[i]->pdf_value(objects->list[i], o, v);
		i++;
	}
	return (sum);
}



// vec3 random(const point3& origin) const override {
//     auto int_size = int(objects.size());
//     return objects[random_int(0, int_size-1)]->random(origin);
// }
t_vec3 obj_random(const void *self, const t_vec3 *o)
{
	t_hittablelist *objects = (t_hittablelist *)self;
	int int_size = objects->size;
	int random_i = random_int(0, int_size); // int_size is excluded so effectively it is random_int(0, int_size - 1)
	return (objects->list[random_i]->random(objects->list[random_i], o));
	

}