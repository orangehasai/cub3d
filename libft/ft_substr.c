/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takenakatakeshiichirouta <takenakatakes    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 03:13:14 by takenakatak       #+#    #+#             */
/*   Updated: 2025/04/30 13:53:37 by takenakatak      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	reslen;
	size_t	substrlen;
	char	*res;
	int		i;

	reslen = 0;
	if (start < ft_strlen(s))
	{
		substrlen = ft_strlen((s + start));
		if (substrlen < len)
			reslen = substrlen;
		else
			reslen = len;
	}
	res = malloc(sizeof(char) * (reslen + 1));
	if (!res)
		return (NULL);
	i = 0;
	while (reslen--)
		res[i++] = s[start++];
	res[i] = '\0';
	return (res);
}

// int main()
// {
// 	char s[] = "HELL,HELL,HELLO,WORLD";
// 	char *ans = ft_substr(s,100,20);
// 	if(ans != NULL)
// 		printf("%s\n", ans);
// 	return 0;
// }
// #include <string.h>

// int main()
// {
// 	char *str = "";
//     size_t size = 0;
// 	char	*ret = ft_substr(str, 5, size);
// 	if(ret == NULL)
// 	{
// 		printf("mallloc failed");
// 		return 0;
// 	}
// 	if (!strncmp(ret, str + 5, size))
// 	{
// 			free(ret);
// 			printf("SUCCESS");
// 			return 0;
// 	}
// 	printf("%s\n",str + 5);
// }