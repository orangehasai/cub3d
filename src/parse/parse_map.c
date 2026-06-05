/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stonegaw <stonegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 18:00:51 by stonegaw          #+#    #+#             */
/*   Updated: 2026/06/05 23:56:29 by stonegaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	set_map_height(t_map *map, char **lines, int map_start_index)
{
	int	i;

	if (!lines[map_start_index])
		return (print_error("missing map"));
	i = map_start_index;
	while (lines[i])
	{
		if (lines[i][0] == '\0')
			return (print_error("empty line inside map"));
		i++;
	}
	map->height = i - map_start_index;
	return (0);
}

static int	get_map_width(char **rows, int height)
{
	int	i;
	int	len;
	int	width;

	i = 0;
	width = 0;
	while (i < height)
	{
		len = ft_strlen(rows[i]);
		if (len > width)
			width = len;
		i++;
	}
	return (width);
}

static int	copy_map_rows(t_map *map, char **lines, int map_start_index)
{
	int	i;

	map->rows = ft_calloc(map->height + 1, sizeof(char *));
	if (!map->rows)
		return (print_error("failed to allocate map rows"));
	i = 0;
	while (i < map->height)
	{
		map->rows[i] = ft_strdup(lines[map_start_index + i]);
		if (!map->rows[i])
		{
			free_map(map);
			return (print_error("failed to allocate map row"));
		}
		i++;
	}
	return (0);
}

int	parse_map(t_scene *scene, char **lines, int map_start_index)
{
	t_map	*map;

	map = &scene->map;
	if (set_map_height(map, lines, map_start_index))
		return (1);
	if (copy_map_rows(map, lines, map_start_index))
		return (1);
	map->width = get_map_width(map->rows, map->height);
	return (0);
}
