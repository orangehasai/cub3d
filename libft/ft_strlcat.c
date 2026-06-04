/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takenakatakeshiichirouta <takenakatakes    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 03:12:51 by takenakatak       #+#    #+#             */
/*   Updated: 2025/04/27 03:12:52 by takenakatak      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *restrict dst, const char *restrict src, size_t dstsize)
{
	size_t	srclen;
	size_t	dstlen;
	size_t	i;

	srclen = 0;
	dstlen = 0;
	while (src[srclen])
		srclen++;
	while (dst[dstlen])
		dstlen++;
	if (dstlen >= dstsize)
		return (dstsize + srclen);
	i = 0;
	while (dstlen + i < dstsize - 1 && src[i] != '\0')
	{
		dst[dstlen + i] = src[i];
		i++;
	}
	dst[dstlen + i] = '\0';
	return (dstlen + srclen);
}

// //負数がオーバーフローしない問題
// #include <string.h>
// int main()
// {
// 	char dst1[100] = "HELLO WOLD, GOODBYE WORLD";//25
// 	char src1[] = "SOSTASUKETE";//11
// 	printf("本物%ld\n",strlcat(dst1,src1,-2));
// 	char dst2[100] = "HELLO WOLD, GOODBYE WORLD";//25
// 	char src2[] = "SOSTASUKETE";//11
// 	printf("偽物%ld\n",ft_strlcat(dst2,src2,-2));
// 	return 0;
// }