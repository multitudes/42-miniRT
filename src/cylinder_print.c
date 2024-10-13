/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder_print.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/09 10:55:34 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/13 14:05:35 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cylinder.h"
#include "libft.h"

/**
 * @brief print the cylinder capped object
 *
 * The print will just print the cylinder which is a part of the capped cylinder
 */
void	print_cylinder_capped(const void *self)
{
	const t_cylinder_capped	*c = (const t_cylinder_capped *)self;

	c->cylinder_uncapped.print(&c->cylinder_uncapped);
}

/**
 * @brief print the cylinder object
 * cy 50.0,0.0,20.6 		0.0,0.0,1.0  	14.2		21.42  	10,0,255
 */
void	print_cylinder(const void *self)
{
	const t_cylinder	*c = (const t_cylinder *)self;

	ft_printf("cy\t%.f,%.f,%.f\t\t%.f,%.f,%.f\t\t%.f\t%.f\t%d,%d,%d\n",
		c->center.x, c->center.y, c->center.z, c->axis.x, c->axis.y, c->axis.z,
		c->radius * 2, c->height, c->rgb.r, c->rgb.g, c->rgb.b);
}
