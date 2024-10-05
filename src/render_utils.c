/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 09:38:33 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/05 09:43:36 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "camera.h"
#include "utils.h"
#include <stdbool.h>

/**
 * @brief print the camera information
 * in the rt file format
 * like C -50,0,20 		0,0,1		70
 */
void	print_camera(const void *self)
{
	const t_camera	*c;

	c = (const t_camera *)self;
	printf("C\t%.f,%.f,%.f\t\t%.f,%.f,%.f\t\t%.f\n", c->orig.x, c->orig.y,
		c->orig.z, c->dir.x, c->dir.y, c->dir.z, c->hfov);
}

// Check if two floating-point numbers are approximately equal
bool	is_near_zero(double value)
{
	return (fabs(value) < EPSILON);
}
