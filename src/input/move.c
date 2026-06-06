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

static void	rotate_player(t_game *game, double angle)
{
	double	old_dir_x;
	double	old_plane_x;

	old_dir_x = game->player.dir_x;
	old_plane_x = game->player.plane_x;
	game->player.dir_x = old_dir_x * cos(angle) - game->player.dir_y
		* sin(angle);
	game->player.dir_y = old_dir_x * sin(angle) + game->player.dir_y
		* cos(angle);
	game->player.plane_x = old_plane_x * cos(angle) - game->player.plane_y
		* sin(angle);
	game->player.plane_y = old_plane_x * sin(angle) + game->player.plane_y
		* cos(angle);
}

static void	add_move(double *move, double x, double y, double step)
{
	move[0] += x * step;
	move[1] += y * step;
}

static void	move_player(t_game *game, double step)
{
	double	move[2];

	move[0] = 0.0;
	move[1] = 0.0;
	if (game->keys.w)
		add_move(move, game->player.dir_x, game->player.dir_y, step);
	if (game->keys.s)
		add_move(move, -game->player.dir_x, -game->player.dir_y, step);
	if (game->keys.a)
		add_move(move, game->player.dir_y, -game->player.dir_x, step);
	if (game->keys.d)
		add_move(move, -game->player.dir_y, game->player.dir_x, step);
	if (can_move_to(game, game->player.x + move[0], game->player.y))
		game->player.x += move[0];
	if (can_move_to(game, game->player.x, game->player.y + move[1]))
		game->player.y += move[1];
}

void	update_player(t_game *game)
{
	struct timeval	tv;
	double			rot_step;

	gettimeofday(&tv, NULL);
	if (game->time.prev_usec == 0)
	{
		game->time.prev_usec = tv.tv_sec * 1000000L + tv.tv_usec;
		game->time.delta_sec = 0.0;
		return ;
	}
	game->time.delta_sec = (tv.tv_sec * 1000000L + tv.tv_usec
			- game->time.prev_usec) / 1000000.0;
	game->time.prev_usec = tv.tv_sec * 1000000L + tv.tv_usec;
	move_player(game, MOVE_SPEED * game->time.delta_sec);
	rot_step = ROT_SPEED * game->time.delta_sec;
	if (game->keys.left)
		rotate_player(game, -rot_step);
	if (game->keys.right)
		rotate_player(game, rot_step);
}
