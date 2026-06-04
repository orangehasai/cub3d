/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tolower.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takenakatakeshiichirouta <takenakatakes    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 03:16:34 by takenakatak       #+#    #+#             */
/*   Updated: 2025/04/27 03:16:35 by takenakatak      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_tolower(int c)
{
	if ('A' <= c && c <= 'Z')
		return (c + 32);
	return (c);
}

// #include <ctype.h>

// int main()
// {
// 	printf("本物%c\n",tolower(''));
// 	printf("偽物%c\n",ft_tolower(''));
// 	return 0;
// }
