/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takenakatakeshiichirouta <takenakatakes    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 03:12:43 by takenakatak       #+#    #+#             */
/*   Updated: 2025/04/27 23:04:34 by takenakatak      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *b, int c, size_t len)
{
	unsigned char	*str;
	unsigned char	uc;

	str = (unsigned char *)b;
	uc = (unsigned char)c;
	while (len--)
	{
		*str = uc;
		str++;
	}
	return (b);
}

// #include <string.h>
// #include <stdio.h>
// #define  BUF_SIZE  20
// int main(void)
// {
//    char buffer[BUF_SIZE + 1];
//    char *string;
//    ft_memset(buffer, 0, sizeof(buffer));
//    string = (char *) ft_memset(buffer,'A', 10);
//    printf("\nBuffer contents: %s\n", string);
//    ft_memset(buffer+10, 'B', 10);
//    printf("\nBuffer contents: %s\n", buffer);
// }