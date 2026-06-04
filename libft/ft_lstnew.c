/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takenakatakeshiichirouta <takenakatakes    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 00:17:31 by takenakatak       #+#    #+#             */
/*   Updated: 2025/05/01 16:55:18 by takenakatak      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*res;

	res = malloc(sizeof(t_list));
	if (!res)
		return (NULL);
	res->content = content;
	res->next = NULL;
	return (res);
}

// int main(int argc, char const *argv[])
// {
// 	t_list test;
// 	test.content = "aa";
// 	test.next = ft_lstnew("bb");

// 	printf("test.content:%s\ntest.next:%p\n"
// 		"test.next->content:%s\ntest.next->next:%p\n"
// 		 ,(char *)test.content,test.next,
// 		 (char *)test.next->content,test.next->next);
// 	return 0;
// }
