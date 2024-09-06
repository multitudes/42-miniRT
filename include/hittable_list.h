/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hittable_list.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 08:21:39 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/06 15:42:46 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"
#include "interval.h"

typedef struct 	s_hittable t_hittable;
typedef struct 	s_hit_record t_hit_record;

typedef struct 	s_hittablelist {
	t_hittable 	**list;
	int 		size;
} 				t_hittablelist; 

bool 	hit_world(const t_hittablelist *world, const t_ray* r, t_interval ray_t, t_hit_record* rec);
t_hittablelist hittablelist(t_hittable **list, int size);


#endif
