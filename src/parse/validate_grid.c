/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_grid.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stonegaw <stonegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/06 00:00:00 by stonegaw          #+#    #+#             */
/*   Updated: 2026/06/06 02:14:52 by stonegaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	alloc_map_grid(t_map *map)
{
	int	y;

	map->grid = ft_calloc(map->height + 1, sizeof(char *));
	if (!map->grid)
		return (print_error("failed to allocate map grid"));
	y = 0;
	while (y < map->height)
	{
		map->grid[y] = ft_calloc(map->width + 1, sizeof(char));
		if (!map->grid[y])
		{
			free_strs(map->grid);
			map->grid = NULL;
			return (print_error("failed to allocate map grid"));
		}
		ft_memset(map->grid[y], ' ', map->width);
		y++;
	}
	return (0);
}

static void	fill_map_grid(t_map *map)
{
	int		y;
	int		x;
	char	cell;

	y = 0;
	while (y < map->height)
	{
		x = 0;
		while (map->rows[y][x])
		{
			cell = map->rows[y][x];
			if (cell == 'N' || cell == 'S' || cell == 'E' || cell == 'W')
				cell = '0';
			map->grid[y][x] = cell;
			x++;
		}
		y++;
	}
}

static int	flood_space(t_map *map, int x, int y)
{
	char	cell;

	if (x < 0 || y < 0 || x >= map->width || y >= map->height)
		return (0);
	cell = map->grid[y][x];
	if (cell == '0')
		return (1);
	if (cell == '1' || cell == 'V')
		return (0);
	map->grid[y][x] = 'V';
	if (flood_space(map, x + 1, y) || flood_space(map, x - 1, y)
		|| flood_space(map, x, y + 1) || flood_space(map, x, y - 1))
		return (1);
	return (0);
}

static int	is_map_open(t_map *map)
{
	int	x;
	int	y;

	y = 0;
	while (y < map->height)
	{
		x = 0;
		while (x < map->width)
		{
			if ((y == 0 || y == map->height - 1 || x == 0 || x == map->width
					- 1) && map->grid[y][x] == '0')
				return (1);
			if (map->grid[y][x] == ' ' && flood_space(map, x, y))
				return (1);
			x++;
		}
		y++;
	}
	return (0);
}

int	validate_map_closed(t_map *map)
{
	int	x;
	int	y;

	if (alloc_map_grid(map))
		return (1);
	fill_map_grid(map);
	if (is_map_open(map))
		return (print_error("map is not closed"));
	y = 0;
	while (y < map->height)
	{
		x = 0;
		while (x < map->width)
		{
			if (map->grid[y][x] == 'V')
				map->grid[y][x] = ' ';
			x++;
		}
		y++;
	}
	return (0);
}
