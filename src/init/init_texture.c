/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_texture.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stonegaw <stonegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 18:00:10 by stonegaw          #+#    #+#             */
/*   Updated: 2026/06/05 18:00:15 by stonegaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	load_texture(t_game *game, t_img *tex, char *path)
{
	int	width;
	int	height;

	tex->img = mlx_xpm_file_to_image(game->mlx, path, &width, &height);
	if (!tex->img)
		return (print_error("failed to load texture image"));
	tex->addr = mlx_get_data_addr(tex->img, &tex->bpp, &tex->line_len,
			&tex->endian);
	if (!tex->addr)
		return (print_error("failed to get texture buffer address"));
	tex->width = width;
	tex->height = height;
	return (0);
}

int	init_textures(t_game *game)
{
	if (load_texture(game, &game->no_tex, game->scene.no_path))
		return (1);
	if (load_texture(game, &game->so_tex, game->scene.so_path))
		return (1);
	if (load_texture(game, &game->we_tex, game->scene.we_path))
		return (1);
	if (load_texture(game, &game->ea_tex, game->scene.ea_path))
		return (1);
	return (0);
}
