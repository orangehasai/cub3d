/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putchar_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takenakatakeshiichirouta <takenakatakes    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 03:42:07 by takenakatak       #+#    #+#             */
/*   Updated: 2025/04/30 03:42:13 by takenakatak      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}

// #include <fcntl.h>
// int main(int argc, char const *argv[])
// {
// 	size_t fd = open("./test.txt",O_RDWR);
// 	char c = 's';
// 	ft_putchar_fd(c, fd);
// 	return 0;
// }
