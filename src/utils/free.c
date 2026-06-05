/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stonegaw <stonegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 18:01:29 by stonegaw          #+#    #+#             */
/*   Updated: 2026/06/05 18:01:29 by stonegaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	free_strs(char **strs)
{
	int	i;

	if (!strs)
		return ;
	i = 0;
	while (strs[i])
	{
		free(strs[i]);
		i++;
	}
	free(strs);
}

void	free_map(t_map *map)
{
	if (!map)
		return ;
	free_strs(map->rows);
	free_strs(map->grid);
	map->rows = NULL;
	map->grid = NULL;
	map->width = 0;
	map->height = 0;
}
