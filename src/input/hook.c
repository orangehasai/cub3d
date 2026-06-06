/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hook.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stonegaw <stonegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 18:00:35 by stonegaw          #+#    #+#             */
/*   Updated: 2026/06/05 18:00:58 by stonegaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	set_key_state(int keycode, t_game *game, int pressed)
{
	if (keycode == XK_w)
		game->keys.w = pressed;
	else if (keycode == XK_a)
		game->keys.a = pressed;
	else if (keycode == XK_s)
		game->keys.s = pressed;
	else if (keycode == XK_d)
		game->keys.d = pressed;
	else if (keycode == XK_Left)
		game->keys.left = pressed;
	else if (keycode == XK_Right)
		game->keys.right = pressed;
}

int	on_key_press(int keycode, t_game *game)
{
	if (keycode == XK_Escape)
		game->running = 0;
	set_key_state(keycode, game, 1);
	return (0);
}

int	on_key_release(int keycode, t_game *game)
{
	set_key_state(keycode, game, 0);
	return (0);
}

int	on_destroy(t_game *game)
{
	game->running = 0;
	return (0);
}

int	on_expose(t_game *game)
{
	mlx_put_image_to_window(game->mlx, game->win, game->frame.img, 0, 0);
	return (0);
}
