/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stonegaw <stonegaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 14:50:11 by takenakatak       #+#    #+#             */
/*   Updated: 2025/05/04 16:20:22 by stonegaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	lstfree(t_list *left, void *con, void (*del)(void *))
{
	t_list	*tmp;
	t_list	*back;

	back = left;
	del(con);
	while (back)
	{
		del(back->content);
		tmp = back->next;
		free(back);
		back = tmp;
	}
}

t_list	*leftfree(t_list *left, void (*del)(void *))
{
	t_list	*res;

	res = left->next;
	del(left->content);
	free(left);
	return (res);
}

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*tmp;
	t_list	*left;
	t_list	*back;
	void	*con;

	left = ft_lstnew(0);
	if (!left)
		return (NULL);
	back = left;
	while (lst)
	{
		con = f(lst->content);
		tmp = ft_lstnew(con);
		back->next = tmp;
		if (!tmp)
		{
			lstfree(left, con, del);
			return (NULL);
		}
		back = tmp;
		lst = lst->next;
	}
	return (leftfree(left, del));
}

// #include <stdio.h>

// void	*f(void *content)
// {
// 	(void ) content;
// 	return ("OK !");
// }

// void	del(void *content)
// {
// 	content = NULL;
// }

// #include <string.h>

// int main(int argc, char const *argv[])
// {
// 	t_list *l = ft_lstnew(strdup(" 1 2 3 "));
//     t_list *ret;

//     l->next = ft_lstnew(strdup("ss"));
//     l->next->next = ft_lstnew(strdup("-_-"));
//     ret = ft_lstmap(l, f, del);
//     if (!strcmp(ret->content, "OK !") && !strcmp(ret->next->content, "OK !")
// 	&&!strcmp(ret->next->next->content,"OK !")&&!strcmp(l->content," 1 2 3 ")
// 	&&!strcmp(l->next->content,"ss")&&!strcmp(l->next->next->content,"-_-"))
//     {
// 		printf("TEST_SUCCESS");
// 		return 0;
// 	}
//     printf("TEST_FAILED");
// 	return 0;
// }
