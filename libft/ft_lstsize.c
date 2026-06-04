/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takenakatakeshiichirouta <takenakatakes    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 01:19:03 by takenakatak       #+#    #+#             */
/*   Updated: 2025/05/01 23:28:21 by takenakatak      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	size_t	cnt;

	cnt = 0;
	while (lst)
	{
		lst = lst->next;
		cnt++;
	}
	return (cnt);
}

// int main(int argc, char const *argv[])
// {
// 	t_list *l = ((void *)0);
// 	int actual;
// 	int expected = 0;

// 	actual = ft_lstsize(l);
// 	if (actual == expected)
// 	{
// 		printf("SUCCESS");
// 		return 0;
// 	}
// 	printf("FAILED");
// 	return 0;
// }
