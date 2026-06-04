/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stonegaw <stonegaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 18:12:12 by takenakatak       #+#    #+#             */
/*   Updated: 2025/05/04 15:30:03 by stonegaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
//計算量もメモリも最適な方法求

static int	first_isnotset(char const *s1, char const *set)
{
	int		left;
	int		j;

	left = 0;
	while (s1[left])
	{
		j = 0;
		while (set[j] && s1[left] != set[j])
			j++;
		if (!set[j])
			break ;
		left++;
	}
	return (left);
}

static int	last_isnotset(char const *s1, char const *set)
{
	int		right;
	int		j;

	right = ft_strlen(s1) - 1;
	while (right >= 0)
	{
		j = 0;
		while (set[j] && s1[right] != set[j])
			j++;
		if (!set[j])
			break ;
		right--;
	}
	return (right);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*res;
	int		left;
	int		right;

	left = first_isnotset(s1, set);
	right = last_isnotset(s1, set);
	if (left > right)
	{
		res = malloc(sizeof(char) * 1);
		if (!res)
			return (NULL);
		res[0] = '\0';
		return (res);
	}
	res = malloc(sizeof(char) * (right - left + 2));
	if (!res)
		return (NULL);
	ft_strlcpy(res, s1 + left, right - left + 2);
	return (res);
}

// int main(int argc, char const *argv[])
// {
// 	char s1[] = "1234A213A2121A241223GS1122";
// 	char set[] = "1234";
// 	printf("%s\n",ft_strtrim(s1, set));
// 	return 0;
// }
