/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putendl_fd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takenakatakeshiichirouta <takenakatakes    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 16:55:51 by takenakatak       #+#    #+#             */
/*   Updated: 2025/05/01 16:55:51 by takenakatak      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putendl_fd(char *s, int fd)
{
	write(fd, s, ft_strlen(s));
	write(fd, "\n", 1);
}

// #include <fcntl.h>
// int main(int argc, char const *argv[])
// {
// 	size_t fd = open("./test.txt",O_RDWR);
// 	char *s = "FKSKLSLA";
// 	ft_putendl_fd(s, fd);
// 	ft_putendl_fd(s, fd);
// 	return 0;
// }
