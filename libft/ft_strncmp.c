/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takenakatakeshiichirouta <takenakatakes    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 03:13:05 by takenakatak       #+#    #+#             */
/*   Updated: 2025/04/27 22:20:49 by takenakatak      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	while (n--)
	{
		if (*s1 != *s2 || *s1 == '\0' || *s2 == '\0')
			return ((int)((unsigned char)*s1 - (unsigned char)*s2));
		s1++;
		s2++;
	}
	return (0);
}

// #include <stdio.h>
// #include <string.h>
// int main()
// {
// 	char *s1 = "HELLO WORLD";
// 	char *s2 = "gELLO WORLD";
// 	printf("本物\n%d\n" , strncmp(s1,s2,5));
// 	printf("偽物\n%d\n" , ft_strncmp(s1,s2,5));
// 	return 0;
// }
