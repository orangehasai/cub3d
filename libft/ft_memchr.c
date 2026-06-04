/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takenakatakeshiichirouta <takenakatakes    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 03:12:07 by takenakatak       #+#    #+#             */
/*   Updated: 2025/04/27 23:00:10 by takenakatak      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	const unsigned char		*str;
	unsigned char			uc;

	str = (const unsigned char *)s;
	uc = (unsigned char)c;
	while (n--)
	{
		if (*str == uc)
			return ((void *)str);
		str++;
	}
	return (NULL);
}

// #include <stdio.h>
// #include <string.h>
// int main(int argc, char ** argv)
// {
// 	char * result1;
// 	char * result2;
// 	if ( argc != 2 )
//     printf( "Usage: %s string\n", argv[0] );
// 	else
// 	{
// 	if ((result1 = (char *) memchr( argv[1], 'x', 3) ) != NULL
// 		&& (result2 = (char *) ft_memchr( argv[1], 'x', 3) ) != NULL)
// 	{
// 		printf( "本物\nThe string starting with x is %s\n", result1 );
// 		printf( "偽物\nThe string starting with x is %s\n", result2 );
// 	}
// 	else
// 		printf( "The letter x cannot be found in the string\n" );
// 	}
// }