#include "minirt.h"
#include "debug.h"
#include "error.h"
#include <MLX42/MLX42.h>
#include "hooks.h"
#include <stdio.h>

int main(int argc, char **argv)
{
    t_mrt data;
    (void)argv;

    if (!init_data(&data))
        return (panic(RT_ERR, &data));
    debug("Start of minirt");
    if (argc == 2)
    {
        // open scenes
        // parse scenes
        //hardcoded_parsing(&data);
        if (init_window(&data))
            return (panic(RT_ERR, &data));

        //draw_with_mlx(&data);
        draw_background(&data);
        //draw_gradient(&data);
    }
    else
        panic(RT_ERR, NULL);

    mlx_loop_hook(data.mlx, &_hook, (void *)&data);
    mlx_mouse_hook(data.mlx, &_mouse_cbk, (void *)&data);
    mlx_scroll_hook(data.mlx, &_scroll_hook, (void *)&data);
    mlx_loop(data.mlx);
    ft_printf("\nbyebye!\n");
    mlx_terminate(data.mlx);
    //free_data(data);

    return (EXIT_SUCCESS);
}