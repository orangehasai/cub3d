/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_mlx.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stonegaw <stonegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 18:00:05 by stonegaw          #+#    #+#             */
/*   Updated: 2026/06/05 18:00:06 by stonegaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	init_frame_image(t_game *game)
{
	t_img	*frame;

	frame = &game->frame;
	frame->img = mlx_new_image(game->mlx, WIN_WIDTH, WIN_HEIGHT);
	if (!frame->img)
		return (print_error("failed to create frame image"));
	frame->addr = mlx_get_data_addr(frame->img, &frame->bpp, &frame->line_len,
			&frame->endian);
	if (!frame->addr)
		return (print_error("failed to get frame buffer address"));
	frame->width = WIN_WIDTH;
	frame->height = WIN_HEIGHT;
	return (0);
}

int	init_mlx(t_game *game)
{
	game->mlx = mlx_init();
	if (!game->mlx)
		return (print_error("failed to initialize mlx"));
	game->win = mlx_new_window(game->mlx, WIN_WIDTH, WIN_HEIGHT, WIN_TITLE);
	if (!game->win)
		return (print_error("failed to create window"));
	if (init_frame_image(game))
		return (1);
	return (0);
}
