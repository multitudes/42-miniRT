/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rtw_stb_image.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbrusa <lbrusa@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/04 15:17:08 by lbrusa            #+#    #+#             */
/*   Updated: 2024/10/14 16:09:12 by lbrusa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "texture.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// need to include only once and not in the header file
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

/**
 * The stbi_loadf function from the stb_image.h library is used to load an image
 * file and return the image data as floating-point values.
 * It takes several parameters, including the filename of the image,
 * pointers to variables that will store the image width
 * and height, a pointer to an integer that will store the number of channels
 * in the image, and the number of bytes per pixel.
 * The function returns a pointer to the loaded image data as an array of
 * floating-point values.
 * Each pixel in the image is represented by multiple floating-point values,
 * depending on the number of channels in the image. The exact format of the
 * returned data depends on the image file being loaded.
 * It's important to note that the returned pointer points to dynamically
 * allocated memory, so it's necessary to free this memory
 */

/**
 * @brief Load an image from a file,
 *
 * Loads the linear (gamma=1) image data from the given file name
 * storing the linear floating point pixel data in the `fdata` member.
 * Returns true if the load succeeded.
 * The resulting data buffer contains the three [0.0, 1.0]
 * floating-point values for the first pixel (red, green, blue).
 * Pixels are contiguous, going left to right for the width of the image,
 * followed by the next row below, for the full height of the image.
 * Load an image from a file.
 */
bool	load(t_img_texture *img, const char *filename)
{
	int	n;

	if (!filename)
		return (0);
	n = img->bytes_per_pixel;
	img->fdata = stbi_loadf(filename, &img->image_width, &img->image_height, &n,
			img->bytes_per_pixel);
	if (img->fdata == NULL)
		return (false);
	ft_printf("Image loaded - got fdata as array of float\n");
	img->bytes_per_scanline = img->image_width * img->bytes_per_pixel;
	convert_to_bytes(img);
	return (true);
}

/**
 * @brief Get the pixel data at the given coordinates
 *
 * Return the address of the three RGB bytes of the pixel at x,y.
 * If there is no image data, returns magenta.
 */
unsigned char	*pixel_data(const t_img_texture *img, int x, int y)
{
	static unsigned char	magenta[] = {255, 0, 255};

	if (img->bdata == NULL)
		return (magenta);
	x = clamp_rtw(x, 0, img->image_width);
	y = clamp_rtw(y, 0, img->image_height);
	return (img->bdata + y * img->bytes_per_scanline + x
		* img->bytes_per_pixel);
}

/**
 * @brief Function to free the image data
 *
 * Because the mapping uses the stb_image library to
 * load image, it provides a function to free the image data
 * as well.
 */
void	free_rtw_image(t_img_texture *img)
{
	if (img->fdata)
	{
		stbi_image_free(img->fdata);
		img->fdata = NULL;
	}
	if (img->bdata)
	{
		free(img->bdata);
		img->bdata = NULL;
	}
}
