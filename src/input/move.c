/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stonegaw <stonegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 18:00:40 by stonegaw          #+#    #+#             */
/*   Updated: 2026/06/05 18:00:41 by stonegaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	register_hooks(t_game *game)
{
	mlx_hook(game->win, KeyPress, KeyPressMask, on_key_press, game);
	mlx_hook(game->win, KeyRelease, KeyReleaseMask, on_key_release, game);
	mlx_hook(game->win, DestroyNotify, 0, on_destroy, game);
	mlx_expose_hook(game->win, on_expose, game);
	mlx_loop_hook(game->mlx, on_loop, game);
	return (0);
}

int	on_loop(t_game *game)
{
	if (!game->running)
		mlx_loop_end(game->mlx);
	return (0);
}
