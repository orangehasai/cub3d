/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takenakatakeshiichirouta <takenakatakes    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 03:43:00 by takenakatak       #+#    #+#             */
/*   Updated: 2025/04/30 03:44:27 by takenakatak      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putstr_fd(char *s, int fd)
{
	write(fd, s, ft_strlen(s));
}

// #include <fcntl.h>
// int main(int argc, char const *argv[])
// {
// 	size_t fd = open("./test.txt",O_RDWR);
// 	char *s = "dadaasdasfgga\ndsadas\nassa";
// 	ft_putstr_fd(s, fd);
// 	return 0;
// }.