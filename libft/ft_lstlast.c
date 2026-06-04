/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takenakatakeshiichirouta <takenakatakes    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 01:24:51 by takenakatak       #+#    #+#             */
/*   Updated: 2025/05/01 17:46:52 by takenakatak      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstlast(t_list *lst)
{
	if (!lst)
		return (lst);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

// int main(int argc, char const *argv[])
// {
// 	t_list *l = ((void *)0);
//     t_list *expected;
//     t_list *actual;

//     expected = ((void *)0);
//     actual = ft_lstlast(l);
//     if (actual == expected)
//     {
// 		printf("SUCCESS");
// 		return 0;
// 	}
//     printf("FAILED");
// 	return 0;
// }
