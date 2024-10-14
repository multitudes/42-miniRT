/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 09:38:33 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/14 16:41:23 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "camera.h"
#include "utils.h"
#include <stdbool.h>
#include <stdio.h>
#include "libft.h"

// Check if two floating-point numbers are approximately equal
bool	is_near_zero(double value)
{
	return (fabs(value) < EPSILON);
}
