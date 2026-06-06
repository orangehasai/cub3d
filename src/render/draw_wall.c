/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_wall.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stonegaw <stonegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/06 16:30:00 by stonegaw          #+#    #+#             */
/*   Updated: 2026/06/06 16:30:00 by stonegaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	draw_wall_column(t_game *game, t_ray *ray)
{
	double	step;
	double	tex_pos;
	int		y;

	step = (double)ray->tex->height / ray->line_height;
	tex_pos = (ray->draw_start - WIN_HEIGHT / 2 + ray->line_height / 2) * step;
	y = ray->draw_start;
	while (y <= ray->draw_end)
	{
		put_pixel(&game->frame, ray->x, y,
			get_texel(ray->tex, ray->tex_x, (int)tex_pos));
		tex_pos += step;
		y++;
	}
}
