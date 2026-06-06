/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collision.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stonegaw <stonegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/06 17:45:00 by stonegaw          #+#    #+#             */
/*   Updated: 2026/06/06 17:45:00 by stonegaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	is_open_cell(t_game *game, double x, double y)
{
	int	map_x;
	int	map_y;

	if (x < 0.0 || y < 0.0 || x >= game->scene.map.width
		|| y >= game->scene.map.height)
		return (0);
	map_x = (int)x;
	map_y = (int)y;
	return (game->scene.map.grid[map_y][map_x] == '0');
}

int	can_move_to(t_game *game, double x, double y)
{
	return (is_open_cell(game, x - PLAYER_RADIUS, y - PLAYER_RADIUS)
		&& is_open_cell(game, x + PLAYER_RADIUS, y - PLAYER_RADIUS)
		&& is_open_cell(game, x - PLAYER_RADIUS, y + PLAYER_RADIUS)
		&& is_open_cell(game, x + PLAYER_RADIUS, y + PLAYER_RADIUS));
}
