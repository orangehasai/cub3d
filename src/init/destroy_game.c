/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy_game.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takenakat <takenakat@student.42tokyo.jp>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 17:58:00 by takenakat         #+#    #+#             */
/*   Updated: 2026/06/05 17:58:00 by takenakat        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	destroy_img(t_game *game, t_img *img)
{
	if (!game->mlx || !img->img)
		return ;
	mlx_destroy_image(game->mlx, img->img);
	img->img = NULL;
	img->addr = NULL;
	img->bpp = 0;
	img->line_len = 0;
	img->endian = 0;
	img->width = 0;
	img->height = 0;
}

static void	free_scene(t_scene *scene)
{
	free(scene->no_path);
	free(scene->so_path);
	free(scene->we_path);
	free(scene->ea_path);
	scene->no_path = NULL;
	scene->so_path = NULL;
	scene->we_path = NULL;
	scene->ea_path = NULL;
	free_map(&scene->map);
}

static void	destroy_mlx(t_game *game)
{
	if (game->mlx && game->win)
		mlx_destroy_window(game->mlx, game->win);
	game->win = NULL;
	if (game->mlx)
		mlx_destroy_display(game->mlx);
	free(game->mlx);
	game->mlx = NULL;
}

void	destroy_game(t_game *game)
{
	if (!game)
		return ;
	destroy_img(game, &game->frame);
	destroy_img(game, &game->no_tex);
	destroy_img(game, &game->so_tex);
	destroy_img(game, &game->we_tex);
	destroy_img(game, &game->ea_tex);
	destroy_mlx(game);
	free_scene(&game->scene);
	game->running = 0;
}
