/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stonegaw <stonegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 18:01:25 by stonegaw          #+#    #+#             */
/*   Updated: 2026/06/05 18:01:26 by stonegaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void	put_err(const char *str)
{
	if (!str)
		return ;
	write(STDERR_FILENO, str, ft_strlen(str));
}

int	print_error(const char *message)
{
	put_err("Error\n");
	if (message)
	{
		put_err(message);
		put_err("\n");
	}
	return (1);
}
