/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takenakatakeshiichirouta <takenakatakes    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 03:12:34 by takenakatak       #+#    #+#             */
/*   Updated: 2025/04/28 18:43:22 by takenakatak      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char		*dest;
	const unsigned char	*source;

	if (dst == NULL && src == NULL)
		return (NULL);
	dest = (unsigned char *)dst;
	source = (const unsigned char *)src;
	while (n--)
		*dest++ = *source++;
	return (dst);
}

// int main()
// {
// 	int src[] ={0,256,2,3,4};
// 	int dst[]= {9,8,7,6,5};
// 	int *res = ft_memcpy(dst,src,5);
// 	for (size_t i = 0; i < 5; i++)
// 	{
// 		printf("%#010x\n",*res);
// 		res++;
// 	}
// 	return 0;
// }

// #include <string.h>
// #include <stdio.h>
// #define SIZE    21
// char target[SIZE] = "a shiny white sphere";
// int main( void )
// {
//   char * p = target + 8;
//   char * source = target + 2; /* start of "shiny" */
//   printf( "Before memmove, target is \"%s\"\n", target );
//   memcpy( p, source, 12);
//   printf( "After memmove, target becomes \"%s\"\n", target );
// }

// #include <string.h>
// #include <stdio.h>

// int main()
// {
// 	char src[] = "sss";
// 	printf("%p\n",ft_memcpy(NULL,NULL,4));
// 	printf("%p\n",memmove(NULL,NULL,4));
// 	return 0;
// }
