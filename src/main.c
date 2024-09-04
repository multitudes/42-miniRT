#include "debug.h"
#include <MLX42/MLX42.h>
#include "libft.h"
#include "camera.h"
#include "vec3.h"
#include "ray.h"
#include "color.h"

#define IMAGE_WIDTH 800
#define IMAGE_HEIGHT 600
#define WINDOW_TITLE "miniRT"
#define BPP sizeof(int32_t)
#define TRUE 1
#define FALSE 0

typedef struct 	s_mrt 
{
	void		*mlx;
	void		*win_ptr;
	mlx_image_t	*image;
	t_camera cam;
	// t_viewport	viewport;
	// t_pixel		pixel;
}				t_mrt;

void	exit_gracefully(mlx_t *mlx)
{
	ft_printf("byebye!\n");
	mlx_close_window(mlx);
	mlx_terminate(mlx);
	exit(EXIT_SUCCESS);
}

void	hook(void *param)
{
	mlx_t		*mlx;
	t_mrt		*mrt;

	mrt = (t_mrt *)param;
	mlx = mrt->mlx;
	if (mlx_is_key_down(mlx, MLX_KEY_ESCAPE))
		exit_gracefully(mlx);
}

/*
this is my draw pixel function. I write directly to the buffer 
and the color is RGBA or 4 bytes. Code inspired from the MLX42 lib!
*/
void draw_(t_mrt *data, int x, int y, int color)
{
    int offset;
    mlx_image_t *image;
    uint8_t *pixel;

    image = data->image;
    offset = y * IMAGE_WIDTH + x;
    pixel = &image->pixels[offset * 4];
    *(pixel++) = (uint8_t)(color >> 24);
    *(pixel++) = (uint8_t)(color >> 16);
    *(pixel++) = (uint8_t)(color >> 8);
    *(pixel++) = (uint8_t)(color & 0xFF);
}

unsigned int    pix_color(t_color color)
{
	int r = color.r * 255;
	int g = color.g * 255;
	int b = color.b *255;
    return ((r << 24) | (g << 16) | (b << 8) | 0xFF);
}

void    draw_scene(t_mrt *data)
{
    int             x;
	int             y;
    unsigned int    color;

    
    y = 0;
    x = 0;
    while (y < IMAGE_HEIGHT)
    {	
		x = 0;
        while (x < IMAGE_WIDTH)
        {
			t_vec3 iu = vec3multscalar(data->cam.pixel_delta_u, x);
			t_vec3 ju = vec3multscalar(data->cam.pixel_delta_v, y);
			t_vec3 partial = vec3add(iu, ju); 
			t_point3 pixel_center = vec3add(data->cam.pixel00_loc, partial);

			t_vec3 ray_direction = vec3substr(pixel_center, data->cam.center); 
			t_ray r = ray(data->cam.center, ray_direction);

			t_color pixel_color = ray_color(&r);

            color = pix_color(pixel_color);
            draw_(data, x, y, color);
			x++;
			// add bar progress
        }
		
		y++;
    }
}


int init_window(t_mrt *data)
{
    data->mlx = mlx_init(IMAGE_WIDTH, IMAGE_HEIGHT, WINDOW_TITLE, false);
	if (data->mlx == NULL)
		return (FALSE);
	data->image = mlx_new_image(data->mlx, IMAGE_WIDTH, IMAGE_HEIGHT);
	if (!(data->image))
	{
		mlx_close_window(data->mlx);
		ft_printf("%s\n", mlx_strerror(mlx_errno));
		
	}
	if (mlx_image_to_window(data->mlx, data->image, 0, 0) == -1)
	{
		mlx_close_window(data->mlx);
		ft_printf("%s\n", mlx_strerror(mlx_errno));
    }
	debug("Window initialized");
    return (TRUE);
}



bool init_data(t_mrt *data)
{
    data->mlx = NULL;
    data->win_ptr = NULL;
    data->image = NULL;
    return (true);
}

double	deg_to_radians(double degrees)
{
	return (degrees / (180.0 / PI));
}

void init_cam(t_camera *cam, t_point3 center, t_vec3 direction, double fov) 
{
	(void)direction;
	cam->center = center;
	cam->hfov = fov;
	cam->image_width = IMAGE_WIDTH;
	cam->aspect_ratio = (double)16/9;
	cam->image_height = (int)(IMAGE_WIDTH/cam->aspect_ratio);
	double viewport_height = 2.0;
	double viewport_width = viewport_height * ((double)IMAGE_WIDTH / IMAGE_HEIGHT);

	t_vec3 viewport_u = vec3(viewport_width, 0, 0);
	t_vec3 viewport_v = vec3(0, -viewport_height, 0);

	cam->pixel_delta_u = vec3divscalar(viewport_u, IMAGE_WIDTH);
	cam->pixel_delta_v = vec3divscalar(viewport_v, IMAGE_HEIGHT);

	double focal_lenght = cos(degrees_to_radians(cam->hfov / 2)) * viewport_width / 2;
	t_point3 part1 = vec3substr(cam->center, vec3(0, 0, focal_lenght));
	t_point3 part2 = vec3substr(part1, vec3divscalar(viewport_u, 2));
	t_point3 viewport_upper_left = vec3substr(part2, vec3divscalar(viewport_v, 2));

	cam->pixel00_loc = vec3add(viewport_upper_left, vec3divscalar(vec3add(cam->pixel_delta_u, cam->pixel_delta_v), 2));
    
}

		

int main(int argc, char **argv)
{
    t_mrt data;
    (void)argv;
	(void)argc;
	if (!init_data(&data))
        return (1);

	t_camera cam;
	t_point3 center = point3(0,0,0);
	t_vec3 direction = vec3(0,0,0);
	init_cam(&cam, center, direction, 70);

	

	data.cam = cam;

	
    debug("Start of minirt %s", "helllo !! ");
	if (!init_window(&data))
		return (EXIT_FAILURE);
	
	draw_scene(&data);
	

    mlx_loop_hook(data.mlx, &hook, (void *)&data);

    mlx_loop(data.mlx);
    ft_printf("\nbyebye!\n");
    mlx_terminate(data.mlx);

    return (EXIT_SUCCESS);
}



// void    draw_background(t_mrt *data)
// {
//     int             x;
// 	int             y;
    
// 	x = 0;
// 	y = 0;
//     while (x < WINDOW_WIDTH)
//     {	
// 		y = 0;
//         while (y < WINDOW_HEIGHT)
//         {
//             draw_(data, x, y, 0xFF0000FF); // Drawing red color rgba
// 			y++;
//         }
// 		x++;
//     }
// }