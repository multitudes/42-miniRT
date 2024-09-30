/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mersenne_twister.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 13:08:22 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/30 09:44:31 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MERSENNE_TWISTER_H
# define MERSENNE_TWISTER_H

# include <stdint.h>

void		mt_init_genrand(uint32_t s);
uint32_t	mt_genrand_int32(void);

#endif
