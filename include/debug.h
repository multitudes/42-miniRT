/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/12 13:06:34 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/22 13:18:42 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*******************************/
/* To remove before evaluation */
/*******************************/

#ifndef DEBUG_H
# define DEBUG_H

# include <stdio.h>

/*
This allows us to use the debug macro to print debug messages but to 
compile them out when NDEBUG is defined.
If we define NDEBUG in the makefile or as a flag -DNDEBUG, 
the debug macro will be replaced with an empty macro.
during submission we will remove the debug macro from the code
including the macro below which is not allowed by norminette.
*/
# ifdef NDEBUG
# define debug(M, ...)
# else
# define debug(M, ...) fprintf(stderr, "\033[0;92mDEBUG %s:%s:%d: " M "\033[0m\n",\
        __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
# endif

#endif //DEBUG_H
