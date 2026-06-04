/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takenakatakeshiichirouta <takenakatakes    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 03:13:09 by takenakatak       #+#    #+#             */
/*   Updated: 2025/05/02 02:21:41 by takenakatak      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	const char		*start;
	unsigned char	uc;

	start = s;
	uc = (unsigned char) c;
	while (*s)
		s++;
	while (1)
	{
		if (*s == uc)
			return ((char *)s);
		if (s == start)
			break ;
		s--;
	}
	return (NULL);
}

// #include <stdio.h>
// #include <string.h>
// #define SIZE 40
// int main(void)
// {
// 	char buf[SIZE] = "computer program";
// 	char * ptr;
// 	int    ch = 'p';
//   /* This illustrates strchr */
// 	ptr = strchr( buf, ch );
// 	printf( "The first occurrence of %c in '%s' is '%s'\n", ch, buf, ptr );
//   /* This illustrates strrchr */
// 	ptr = strrchr( buf, ch );
// 	printf( "本物\nThe last occurrence of %c in '%s' is '%s'\n", ch, buf, ptr );
// 	ptr = ft_strrchr( buf, ch );
// 	printf( "偽物\nThe last occurrence of %c in '%s' is '%s'\n", ch, buf, ptr );
// }

// int main()
// {
// 	char *src = "abbbbbbbb";
// 	char *d1 = strrchr(src, 'a');
// 	char *d2 = ft_strrchr(src, 'a');

// 	printf("d1 : %s\nd2 : %s\n",d1,d2);
// 	return 0;
// }
