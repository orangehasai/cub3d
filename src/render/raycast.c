/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stonegaw <stonegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 18:01:09 by stonegaw          #+#    #+#             */
/*   Updated: 2026/06/05 18:01:09 by stonegaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	init_ray_dir(t_game *game, t_ray *ray, int x)
{
	ray->x = x;
	ray->camera_x = 2.0 * x / (double)WIN_WIDTH - 1.0;
	ray->ray_dir_x = game->player.dir_x + game->player.plane_x * ray->camera_x;
	ray->ray_dir_y = game->player.dir_y + game->player.plane_y * ray->camera_x;
	ray->map_x = (int)game->player.x;
	ray->map_y = (int)game->player.y;
}

static void	init_ray_delta(t_ray *ray)
{
	if (ray->ray_dir_x == 0.0)
		ray->delta_dist_x = 1e30;
	else
		ray->delta_dist_x = fabs(1.0 / ray->ray_dir_x);
	if (ray->ray_dir_y == 0.0)
		ray->delta_dist_y = 1e30;
	else
		ray->delta_dist_y = fabs(1.0 / ray->ray_dir_y);
}

static void	init_ray_step(t_game *game, t_ray *ray)
{
	if (ray->ray_dir_x < 0.0)
	{
		ray->step_x = -1;
		ray->side_dist_x = (game->player.x - ray->map_x) * ray->delta_dist_x;
	}
	else
	{
		ray->step_x = 1;
		ray->side_dist_x = (ray->map_x + 1.0 - game->player.x)
			* ray->delta_dist_x;
	}
	if (ray->ray_dir_y < 0.0)
	{
		ray->step_y = -1;
		ray->side_dist_y = (game->player.y - ray->map_y) * ray->delta_dist_y;
	}
	else
	{
		ray->step_y = 1;
		ray->side_dist_y = (ray->map_y + 1.0 - game->player.y)
			* ray->delta_dist_y;
	}
}

static void	step_ray(t_ray *ray)
{
	if (ray->side_dist_x < ray->side_dist_y)
	{
		ray->side_dist_x += ray->delta_dist_x;
		ray->map_x += ray->step_x;
		ray->side = 0;
	}
	else
	{
		ray->side_dist_y += ray->delta_dist_y;
		ray->map_y += ray->step_y;
		ray->side = 1;
	}
}

void	raycast_column(t_game *game, int x)
{
	t_ray	ray;
	int		steps;

	ft_bzero(&ray, sizeof(ray));
	init_ray_dir(game, &ray, x);
	init_ray_delta(&ray);
	init_ray_step(game, &ray);
	steps = 0;
	while (steps++ < game->scene.map.width * game->scene.map.height)
	{
		step_ray(&ray);
		if (ray.map_x < 0 || ray.map_y < 0 || ray.map_x >= game->scene.map.width
			|| ray.map_y >= game->scene.map.height)
			break ;
		if (game->scene.map.grid[ray.map_y][ray.map_x] == '1')
			break ;
	}
}
