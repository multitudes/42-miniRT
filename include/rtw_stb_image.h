/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rtw_stb_image.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 13:52:18 by lbrusa            #+#    #+#             */
/*   Updated: 2024/09/30 10:14:38 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RTW_STB_IMAGE_H
# define RTW_STB_IMAGE_H

/**
 * @brief Load an image from a file
 *
 * @param bytes_per_pixel The number of bytes per pixel
 * @param fdata Pointer to the linear floating point pixel data
 * @param bdata Pointer to the linear 8-bit pixel data
 * @param image_width
 * @param image_height
 * @param bytes_per_scanline
 */
typedef struct s_rtw_image
{
	int				bytes_per_pixel;
	float			*fdata;
	unsigned char	*bdata;
	int				image_width;
	int				image_height;
	int				bytes_per_scanline;
}					t_rtw_image;

#endif
