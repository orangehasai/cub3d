/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takenakatakeshiichirouta <takenakatakes    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 12:47:18 by takenakatak       #+#    #+#             */
/*   Updated: 2025/05/02 02:22:34 by takenakatak      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_words(char const *s, char c)
{
	int	cntwords;
	int	isnotc;

	cntwords = 0;
	while (*s)
	{
		isnotc = 0;
		while (*s == c)
			s++;
		while (*s != c && *s)
		{
			s++;
			isnotc = 1;
		}
		if (isnotc)
			cntwords++;
	}
	return (cntwords);
}

static int	*count_lens(char const *s, char c, int cntwords)
{
	int	*res;
	int	index;

	res = malloc(sizeof(int) * cntwords);
	if (!res)
		return (NULL);
	index = 0;
	while (*s)
	{
		res[index] = 0;
		while (*s == c)
			s++;
		while (*s != c && *s)
		{
			res[index]++;
			s++;
		}
		index++;
	}
	return (res);
}

static char	**set_strs(char const *s, char c, int cntwords, int *cntlens)
{
	int		index;
	int		instrindex;
	char	**strs;

	strs = malloc(sizeof(char *) * (cntwords + 1));
	if (!strs)
		return (NULL);
	index = -1;
	while (++index < cntwords)
	{
		instrindex = 0;
		strs[index] = ft_calloc(cntlens[index] + 1, sizeof(char));
		if (!strs[index])
		{
			while (--index >= 0)
				free(strs[index]);
			free(strs);
			return (NULL);
		}
		while (*s == c)
			s++;
		while (*s != c && *s)
			strs[index][instrindex++] = *s++;
	}
	return (strs);
}

char	**ft_split(char const *s, char c)
{
	int		cntwords;
	int		*cntlens;
	char	**res;

	if (!s)
		return (NULL);
	cntwords = count_words(s, c);
	cntlens = count_lens(s, c, cntwords);
	if (!cntlens)
		return (NULL);
	res = set_strs(s, c, cntwords, cntlens);
	free(cntlens);
	if (!res)
		return (NULL);
	res[cntwords] = NULL;
	return (res);
}

// int main(int argc, char const *argv[])
// {
// 	char	**tabstr;
// 	int		i;

// 	i = 0;
// 	if (!(tabstr = ft_split("lorem ipsum dolor sit "
//		"amet, consectetur adipiscing elit. Sed non risus. Suspendisse", ' ')))
// 		printf("NULL");
// 	else
// 	{
// 		while (tabstr[i] != NULL)
// 		{
// 			printf("%s\n", tabstr[i]);
// 			i++;
// 		}
// 	}
// 	return 0;
// }
