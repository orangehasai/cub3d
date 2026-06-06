/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stonegaw <stonegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 18:01:37 by stonegaw          #+#    #+#             */
/*   Updated: 2026/06/06 13:54:31 by stonegaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	has_cub_extension(const char *path)
{
	size_t	len;

	if (!path)
		return (0);
	len = ft_strlen(path);
	if (len <= 4)
		return (0);
	if (ft_strncmp(path + len - 4, ".cub", 4) != 0)
		return (0);
	return (1);
}

static int	validate_args(int argc, char **argv)
{
	if (argc != 2)
		return (print_error("usage: ./cub3D <map.cub>"));
	if (!has_cub_extension(argv[1]))
		return (print_error("map must use the .cub extension"));
	return (0);
}

static int	run_game(t_game *game, const char *path)
{
	char	**lines;
	int		map_start_index;

	init_game(game);
	if (read_file(path, &lines))
		return (1);
	if (parse_elements(&game->scene, lines, &map_start_index))
		return (free_strs(lines), 1);
	if (parse_map(&game->scene, lines, map_start_index))
		return (free_strs(lines), 1);
	free_strs(lines);
	if (validate_map(game))
		return (1);
	if (init_mlx(game))
		return (1);
	if (init_textures(game))
		return (1);
	if (render_frame(game))
		return (1);
	return (0);
}

int	main(int argc, char **argv)
{
	t_game	game;

	if (validate_args(argc, argv))
		return (1);
	if (run_game(&game, argv[1]))
	{
		destroy_game(&game);
		return (1);
	}
	destroy_game(&game);
	return (0);
}
