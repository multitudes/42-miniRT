#include "debug.h"
#include <MLX42/MLX42.h>
#include "libft.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define ASPECT_RATIO (double)WINDOW_WIDTH / (double)WINDOW_HEIGHT
#define VIEWPORT_HEIGHT 2
#define FOCAL_LENGTH 1
#define WINDOW_TITLE "miniRT"
#define BPP sizeof(int32_t)

typedef struct 	s_mrt 
{

	void		*mlx;
	void		*win_ptr;
	mlx_image_t	*image;
	// t_camera	camera;
	// t_viewport	viewport;
	// t_pixel		pixel;
}				t_mrt;

void	_exit_gracefully(mlx_t *mlx)
{
	ft_printf("byebye!\n");
	mlx_close_window(mlx);
	mlx_terminate(mlx);
	exit(0);
}

void	_hook(void *param)
{
	mlx_t		*mlx;
	t_mrt		*mrt;

	mrt = (t_mrt *)param;
	mlx = mrt->mlx;
	if (mlx_is_key_down(mlx, MLX_KEY_ESCAPE))
		_exit_gracefully(mlx);
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
    offset = y * WINDOW_WIDTH + x;
    pixel = &image->pixels[offset * 4];
    *(pixel++) = (uint8_t)(color >> 24);
    *(pixel++) = (uint8_t)(color >> 16);
    *(pixel++) = (uint8_t)(color >> 8);
    *(pixel++) = (uint8_t)(color & 0xFF);
}

unsigned int    pix_color(int r, int g, int b)
{
    return ((r << 24) | (g << 16) | (b << 8) | 0xFF);
}

void    draw_gradient(t_mrt *data)
{
    int             x;
	int             y;
    unsigned int    color;
    int             r, g, b;
    
    y = 0;
    x = 0;
    while (y < WINDOW_HEIGHT)
    {	
		x = 0;
        while (x < WINDOW_WIDTH)
        {
            r = (x * 255) / (WINDOW_WIDTH - 1);
            g = (y * 255) / (WINDOW_HEIGHT - 1);
            b = 0;
            color = pix_color(r, g, b);
            draw_(data, x, y, color);
			x++;
        }
		y++;
    }
}

bool init_data(t_mrt *mrt)
{
    mrt->mlx = NULL;
    mrt->win_ptr = NULL;
    mrt->image = NULL;
    return (true);
}

int init_window(t_mrt *data)
{
    data->mlx = mlx_init(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, false);
	if (data->mlx == NULL)
		return (1);
	data->image = mlx_new_image(data->mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
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
    return (0);
}

void    draw_background(t_mrt *data)
{
    int             x;
	int             y;
    
	x = 0;
	y = 0;
    while (x < WINDOW_WIDTH)
    {	
		y = 0;
        while (y < WINDOW_HEIGHT)
        {
            draw_(data, x, y, 0xFF0000FF); // Drawing red color rgba
			y++;
        }
		x++;
    }
}

int main(int argc, char **argv)
{
    t_mrt data;
    (void)argv;

    if (!init_data(&data))
        return (1);
    debug("Start of minirt");
    if (argc >= 0)
    {
        if (init_window(&data))
            return (1);
        draw_background(&data);
        draw_gradient(&data);
    }
    else
        return (1);

    mlx_loop_hook(data.mlx, &_hook, (void *)&data);

    mlx_loop(data.mlx);
    ft_printf("\nbyebye!\n");
    mlx_terminate(data.mlx);

    return (0);
}