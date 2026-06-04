/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takenakatakeshiichirouta <takenakatakes    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 00:34:19 by takenakatak       #+#    #+#             */
/*   Updated: 2025/04/30 04:41:53 by takenakatak      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_digit(long int num)
{
	int	cnt;

	cnt = 1;
	while (num >= 10)
	{
		num /= 10;
		cnt++;
	}
	return (cnt);
}

static char	*set_nbr(long int num, char *res, int i)
{
	if (num >= 10)
		set_nbr(num / 10, res, i - 1);
	res[i] = num % 10 + '0';
	return (res);
}

char	*ft_itoa(int n)
{
	char		*res;
	long int	num;
	int			numdigit;
	int			isnegative;

	num = n;
	isnegative = 0;
	if (num < 0)
	{
		isnegative = 1;
		num *= -1;
	}
	numdigit = count_digit(num);
	res = malloc(sizeof(char) * (numdigit + isnegative + 1));
	if (!res)
		return (NULL);
	res = set_nbr(num, res, numdigit + isnegative - 1);
	if (isnegative)
		res[0] = '-';
	res[numdigit + isnegative] = '\0';
	return (res);
}

// int main(int argc, char const *argv[])
// {
// 	int	n = -0;
// 	printf("%s\n",ft_itoa(n));
// 	return 0;
// }
