/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takenakatakeshiichirouta <takenakatakes    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 03:12:48 by takenakatak       #+#    #+#             */
/*   Updated: 2025/04/27 23:17:21 by takenakatak      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	char	*dst;
	int		cnt;

	cnt = 0;
	while (s1[cnt] != '\0')
		cnt++;
	dst = malloc(sizeof(char) * (cnt + 1));
	if (!dst)
		return (NULL);
	dst[cnt] = '\0';
	while (--cnt >= 0)
		dst[cnt] = s1[cnt];
	return (dst);
}

// #include <stdio.h>
// #include <string.h>
// int main(void)
// {
//    char *string = "this is a copy";
//    char *newstr;
// 	if ((newstr = ft_strdup(string)) != NULL)
// 		printf("The new string is: %s\n", newstr);
// 	return 0;
// }
// int main()
// {
// 	char *tmp = "this is a normal test";
// 	int r_size = strlen(tmp);
// 	int size;
// 	printf("%s",ft_strdup(tmp));
// 	return 0;
// }
