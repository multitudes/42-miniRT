/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ambient.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 11:52:19 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/30 09:17:16 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AMBIENT_H
# define AMBIENT_H

# include "color.h"

typedef struct s_ambient
{
	double	ratio;
	t_rgb	rgbcolor;
	t_color	color;
	void	(*print)(const void *self);
}			t_ambient;

void		ambient(t_ambient *result, double ratio, t_rgb rgb);
void		print_ambient(const void *self);

#endif
