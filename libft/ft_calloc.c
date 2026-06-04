/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takenakatakeshiichirouta <takenakatakes    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 03:11:55 by takenakatak       #+#    #+#             */
/*   Updated: 2025/04/27 03:11:55 by takenakatak      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	void	*res;

	if (size && count > __SIZE_MAX__ * size)
		return (NULL);
	res = malloc(size * count);
	if (!res)
		return (NULL);
	ft_bzero(res, size * count);
	return (res);
}

// int main()
// {
// 	char *kaku;
// 	//"Hello world"を入れたい
// 	kaku = (char*)ft_calloc(12, sizeof(char));
// 	kaku = ft_strcpy();
// 	printf("%s",kaku);
// 	return 0;
// }
// int main(int argc, char const *argv[])
// {
// 	int size = 8539;

// 	char *d1 = (char *)ft_calloc(size, sizeof(int));
//   	char *d2 = (char *)calloc(size, sizeof(int));
// 	printf("%#x\n",*d1);
// 	printf("%#x\n",*d2);
//     free(d1);
//     free(d2);
// 	return 0;
// }
