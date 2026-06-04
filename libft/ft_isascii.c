/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isascii.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takenakatakeshiichirouta <takenakatakes    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 03:12:01 by takenakatak       #+#    #+#             */
/*   Updated: 2025/04/27 03:12:21 by takenakatak      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isascii(int c)
{
	return (0 <= c && c <= 127);
}

// #include <ctype.h>

// int main(int argc, char const *argv[])
// {
// 	printf("本物%d\n",isascii(128));
// 	printf("偽物%d",ft_isascii(128));
// 	return 0;
// }
