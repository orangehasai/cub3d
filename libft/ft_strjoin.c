/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takenakatakeshiichirouta <takenakatakes    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 17:03:49 by takenakatak       #+#    #+#             */
/*   Updated: 2025/04/28 17:09:34 by takenakatak      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*res;
	size_t	s1len;
	size_t	s2len;

	s1len = ft_strlen(s1);
	s2len = ft_strlen(s2);
	res = malloc(sizeof(char) * (s1len + s2len + 1));
	if (!res)
		return (NULL);
	while (*s1 || *s2)
	{
		if (*s1)
			*res++ = *s1++;
		else
			*res++ = *s2++;
	}
	*res = '\0';
	return (res - (s1len + s2len));
}

// int main(int argc, char const *argv[])
// {
// 	char *s1 = "";
// 	char *s2 = "WORLD";
// 	printf("%s\n",ft_strjoin(s1,s2));
// 	return 0;
// }
