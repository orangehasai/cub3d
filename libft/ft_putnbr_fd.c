/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takenakatakeshiichirouta <takenakatakes    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 04:12:47 by takenakatak       #+#    #+#             */
/*   Updated: 2025/04/30 04:12:48 by takenakatak      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	long int	num;
	char		tmp;

	num = n;
	if (n < 0)
	{
		write(fd, "-", 1);
		num *= -1;
	}
	if (num >= 10)
		ft_putnbr_fd(num / 10, fd);
	tmp = num % 10 + '0';
	write(fd, &tmp, 1);
}

// #include <fcntl.h>
// int main(int argc, char const *argv[])
// {
// 	size_t fd = open("./test.txt",O_RDWR);
// 	int n = 0;
// 	ft_putnbr_fd(n, fd);
// 	return 0;
// }
