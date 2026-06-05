/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_game.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stonegaw <stonegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 17:59:56 by stonegaw          #+#    #+#             */
/*   Updated: 2026/06/05 17:59:57 by stonegaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	init_game(t_game *game)
{
	if (!game)
		return ;
	ft_bzero(game, sizeof(*game));
	game->scene.floor.value = -1;
	game->scene.ceiling.value = -1;
	game->running = 1;
}
