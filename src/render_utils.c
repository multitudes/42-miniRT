/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/05 09:38:33 by lbrusa            #+#    #+#             */
/*   Updated: 2025/02/18 18:44:35 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "camera.h"
#include "utils.h"
#include <stdbool.h>
#include <stdio.h>
#include "libft.h"

/** 
 * Checks if two floating-point numbers are approximately equal
 */ 
bool	is_near_zero(double value)
{
	return (fabs(value) < EPSILON);
}
