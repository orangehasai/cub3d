/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_frame.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stonegaw <stonegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 18:01:15 by stonegaw          #+#    #+#             */
/*   Updated: 2026/06/06 13:59:57 by stonegaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	put_pixel(t_img *img, int x, int y, int color)
{
	char	*pixel;
	int		bytes_per_pixel;

	if (x < 0 || y < 0 || x >= img->width || y >= img->height)
		return ;
	bytes_per_pixel = img->bpp / 8;
	pixel = img->addr + y * img->line_len + x * bytes_per_pixel;
	ft_memcpy(pixel, &color, bytes_per_pixel);
}

static void	fill_background(t_game *game)
{
	int	x;
	int	y;
	int	ceiling_color;
	int	floor_color;

	ceiling_color = mlx_get_color_value(game->mlx, game->scene.ceiling.value);
	floor_color = mlx_get_color_value(game->mlx, game->scene.floor.value);
	y = 0;
	while (y < game->frame.height)
	{
		x = 0;
		while (x < game->frame.width)
		{
			if (y < game->frame.height / 2)
				put_pixel(&game->frame, x, y, ceiling_color);
			else
				put_pixel(&game->frame, x, y, floor_color);
			x++;
		}
		y++;
	}
}

int	render_frame(t_game *game)
{
	int	x;

	fill_background(game);
	x = 0;
	while (x < game->frame.width)
	{
		raycast_column(game, x);
		x++;
	}
	mlx_put_image_to_window(game->mlx, game->win, game->frame.img, 0, 0);
	return (0);
}
