/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_sample.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stonegaw <stonegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 18:01:22 by stonegaw          #+#    #+#             */
/*   Updated: 2026/06/06 15:36:18 by stonegaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static double	get_perp_wall_dist(t_game *game, t_ray *ray)
{
	double	dist;

	if (ray->side == 0)
		dist = (ray->map_x - game->player.x + (1 - ray->step_x) * 0.5)
			/ ray->ray_dir_x;
	else
		dist = (ray->map_y - game->player.y + (1 - ray->step_y) * 0.5)
			/ ray->ray_dir_y;
	return (fabs(dist));
}

static void	set_draw_range(t_ray *ray)
{
	ray->line_height = (int)(WIN_HEIGHT / ray->perp_wall_dist);
	ray->draw_start = WIN_HEIGHT / 2 - ray->line_height / 2;
	if (ray->draw_start < 0)
		ray->draw_start = 0;
	ray->draw_end = WIN_HEIGHT / 2 + ray->line_height / 2;
	if (ray->draw_end >= WIN_HEIGHT)
		ray->draw_end = WIN_HEIGHT - 1;
}

static t_img	*select_wall_texture(t_game *game, t_ray *ray)
{
	if (ray->side == 0)
	{
		if (ray->ray_dir_x > 0.0)
			return (&game->we_tex);
		return (&game->ea_tex);
	}
	if (ray->ray_dir_y > 0.0)
		return (&game->no_tex);
	return (&game->so_tex);
}

void	prepare_texture_sample(t_game *game, t_ray *ray)
{
	ray->perp_wall_dist = get_perp_wall_dist(game, ray);
	set_draw_range(ray);
	if (ray->side == 0)
		ray->wall_x = game->player.y + ray->perp_wall_dist * ray->ray_dir_y;
	else
		ray->wall_x = game->player.x + ray->perp_wall_dist * ray->ray_dir_x;
	ray->wall_x -= floor(ray->wall_x);
	ray->tex = select_wall_texture(game, ray);
	ray->tex_x = (int)(ray->wall_x * ray->tex->width);
	if ((ray->side == 0 && ray->ray_dir_x > 0.0) || (ray->side == 1
			&& ray->ray_dir_y < 0.0))
		ray->tex_x = ray->tex->width - ray->tex_x - 1;
}

int	get_texel(t_img *tex, int x, int y)
{
	char	*pixel;
	int		bytes_per_pixel;
	int		color;

	if (x < 0 || y < 0 || x >= tex->width || y >= tex->height)
		return (0);
	bytes_per_pixel = tex->bpp / 8;
	pixel = tex->addr + y * tex->line_len + x * bytes_per_pixel;
	color = 0;
	ft_memcpy(&color, pixel, bytes_per_pixel);
	return (color);
}
