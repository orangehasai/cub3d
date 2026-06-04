/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takenakatakeshiichirouta <takenakatakes    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 03:12:39 by takenakatak       #+#    #+#             */
/*   Updated: 2025/05/01 16:55:41 by takenakatak      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	unsigned char		*dest;
	const unsigned char	*source;
	size_t				i;

	if (dst == NULL && src == NULL)
		return (NULL);
	dest = (unsigned char *)dst;
	source = (const unsigned char *)src;
	i = 0;
	if (dest < source)
	{
		while (i++ < len)
			dest[i - 1] = source[i - 1];
	}
	else
	{
		while (len > 0)
		{
			dest[len - 1] = source[len - 1];
			len--;
		}
	}
	return (dst);
}

// #include <string.h>
// #include <stdio.h>
// #define SIZE    21
// char target[SIZE] = "a shiny white sphere";
// int main( void )
// {
//   char * p = target + 8;
//   char * source = target + 2; /* start of "shiny" */
//   printf( "Before memmove, target is \"%s\"\n", target );
//   memmove(p, source, 13);
//   printf( "After memmove, target becomes \"%s\"\n", target );
// }
// #include <string.h>
// #include <stdio.h>
// int main()
// {
// 	char	dst1[0xF0];
// 	char	dst2[0xF0];
// 	char	*data = "thiß ß\xde\xad\xbe\xeftriñg will be øvérlapéd !\r\n";
// 	int		size = 0xF0 - 0xF;
// 	memset(dst1, 'A', sizeof(dst1));
// 	memset(dst2, 'A', sizeof(dst2));
// 	memcpy(dst1, data, strlen(data));
// 	memcpy(dst2, data, strlen(data));
// 	printf("本物:\n%s\n",(char *)memmove(dst1, dst1 + 3, size));
// 	printf("偽物:\n%s\n",(char *)ft_memmove(dst2, dst2 + 3, size));
// 	return 0;
// }
