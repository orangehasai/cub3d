/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stonegaw <stonegaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 02:52:03 by takenakatak       #+#    #+#             */
/*   Updated: 2025/05/02 18:34:10 by stonegaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

// void	f(unsigned int i, char* c)
// {
/*   *c += 1;*/
// }

void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	unsigned int	i;

	i = 0;
	while (s[i])
	{
		f(i, &s[i]);
		i++;
	}
}

// int main(int argc, char const *argv[])
// {
// 	char s[] = "HEEEEEELLOOO";
// 	printf("%s\n",s);
// 	ft_striteri(s,f);
// 	printf("%s\n",s);
// 	return 0;
// }
