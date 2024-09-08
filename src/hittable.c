/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hittable.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 10:49:28 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/07 20:54:16 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hittable.h"
#include "hittable_list.h"
#include "interval.h"
#include "ray.h"
#include <stdbool.h>

bool hit_world(const t_hittablelist *hittablelist, const t_ray* r, t_interval ray_t, t_hit_record* rec)
{
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
		// hittablelist.hit = hit;
	return (hittablelist);
}
