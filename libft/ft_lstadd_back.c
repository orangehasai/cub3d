/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stonegaw <stonegaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 01:34:50 by takenakatak       #+#    #+#             */
/*   Updated: 2025/05/04 17:13:26 by stonegaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*tmp;

	if (!lst || !new)
		return ;
	tmp = ft_lstlast(*lst);
	if (!tmp)
		*lst = new;
	else
		tmp -> next = new;
}

// #include <string.h>

// int main(int argc, char const *argv[])
// {
// 	t_list *l = ((void *)0);
//     t_list *n = ft_lstnew(strdup("OK"));

//     ft_lstadd_back(&l, n);
//     if (l == n && !strcmp(l->content, "OK"))
//     {
// 		free(l->next);
// 		free(l);
//         printf("TEST_SUCCESS");
// 		return 0;
//     }
//     free(l->next);
//     free(l);
//     printf("TEST_FAILED");
// 	return 0;
// }
