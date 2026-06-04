/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: takenakatakeshiichirouta <takenakatakes    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/27 03:11:53 by takenakatak       #+#    #+#             */
/*   Updated: 2025/04/27 03:11:53 by takenakatak      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	unsigned char	*str;

	str = (unsigned char *)s;
	while (n--)
		*str++ = 0;
}

// #include <strings.h>
// #include <stdio.h>
// int main()
// {
// 	char s[50] = "initial string is here";
// 	printf("%s\n",s);
// 	ft_bzero(NULL,2);
	// bzero(NULL,2);
// 	printf("%s\n",s);
// 	return 0;
// }
