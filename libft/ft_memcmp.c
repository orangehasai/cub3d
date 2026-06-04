/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takenakatakeshiichirouta <takenakatakes    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 03:12:31 by takenakatak       #+#    #+#             */
/*   Updated: 2025/04/27 23:02:31 by takenakatak      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	const unsigned char	*str1;
	const unsigned char	*str2;

	str1 = (unsigned const char *)s1;
	str2 = (unsigned const char *)s2;
	while (n--)
	{
		if (*str1 != *str2)
			return ((int)(*str1 - *str2));
		str1++;
		str2++;
	}
	return (0);
}

// #include <stdio.h>
// #include <string.h>
// int main(int argc, char ** argv)
// {
//   int  len;
//   int  result;

//   if ( argc != 3 )
//   {
//      printf( "Usage: %s string1 string2¥n", argv[0] );
//   }
//   else
//   {
//      /* Determine the length to be used for comparison */
//      if (strlen( argv[1] ) < strlen( argv[2] ))
//        len = strlen( argv[1] );
//      else
//        len = strlen( argv[2] );

//      result = ft_memcmp( argv[1], argv[2], len );

//      printf( "When the first %i characters are compared,\n", len );
//      if ( result == 0 )
//        printf( "\"%s\" is identical to \"%s\"\n", argv[1], argv[2] );
//      else if ( result < 0 )
//        printf( "\"%s\" is less than \"%s\"\n", argv[1], argv[2] );
//      else
//        printf( "\"%s\" is greater than \"%s\"\n", argv[1], argv[2] );
//    }
// }