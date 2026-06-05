/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_map.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stonegaw <stonegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 18:01:04 by stonegaw          #+#    #+#             */
/*   Updated: 2026/06/05 18:01:05 by stonegaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	is_valid_map_char(char cell)
{
	return (cell == '0' || cell == '1' || cell == 'N' || cell == 'S'
		|| cell == 'E' || cell == 'W' || cell == ' ');
}

static void	set_player_direction(t_player *player, char spawn)
{
	if (spawn == 'N')
	{
		player->dir_y = -1.0;
		player->plane_x = FOV_SCALE;
	}
	else if (spawn == 'S')
	{
		player->dir_y = 1.0;
		player->plane_x = -FOV_SCALE;
	}
	else if (spawn == 'E')
	{
		player->dir_x = 1.0;
		player->plane_y = FOV_SCALE;
	}
	else if (spawn == 'W')
	{
		player->dir_x = -1.0;
		player->plane_y = -FOV_SCALE;
	}
}

static int	set_player_spawn(t_game *game, int x, int y, char spawn)
{
	game->player.x = x + 0.5;
	game->player.y = y + 0.5;
	game->player.dir_x = 0.0;
	game->player.dir_y = 0.0;
	game->player.plane_x = 0.0;
	game->player.plane_y = 0.0;
	set_player_direction(&game->player, spawn);
	return (0);
}

static int	validate_row(t_game *game, char *row, int y, int *spawn_count)
{
	int		x;
	char	cell;

	x = 0;
	while (row[x])
	{
		cell = row[x];
		if (!is_valid_map_char(cell))
			return (print_error("invalid map character"));
		if (cell == 'N' || cell == 'S' || cell == 'E' || cell == 'W')
		{
			(*spawn_count)++;
			if (*spawn_count > 1)
				return (print_error("multiple player spawns"));
			set_player_spawn(game, x, y, cell);
		}
		x++;
	}
	return (0);
}

int	validate_map(t_game *game)
{
	t_map	*map;
	int		y;
	int		spawn_count;

	map = &game->scene.map;
	y = 0;
	spawn_count = 0;
	while (y < map->height)
	{
		if (validate_row(game, map->rows[y], y, &spawn_count))
			return (1);
		y++;
	}
	if (spawn_count == 0)
		return (print_error("missing player spawn"));
	return (0);
}
