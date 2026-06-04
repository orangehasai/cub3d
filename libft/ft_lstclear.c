/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stonegaw <stonegaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 01:40:57 by takenakatak       #+#    #+#             */
/*   Updated: 2025/05/04 17:18:33 by stonegaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	t_list	tmp;

	if (!lst || !del)
		return ;
	while (*lst)
	{
		tmp.next = (*lst)->next;
		del((*lst)->content);
		free(*lst);
		*lst = tmp.next;
	}
	*lst = NULL;
}
