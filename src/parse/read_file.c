/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stonegaw <stonegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 18:00:55 by stonegaw          #+#    #+#             */
/*   Updated: 2026/06/05 21:25:53 by stonegaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	reserve_buf(char **buf, size_t need, size_t *cap)
{
	char	*new_buf;
	size_t	new_cap;

	if (need < *cap)
		return (0);
	new_cap = *cap;
	while (need >= new_cap)
		new_cap *= 2;
	new_buf = malloc(new_cap);
	if (!new_buf)
		return (1);
	ft_memcpy(new_buf, *buf, *cap);
	free(*buf);
	*buf = new_buf;
	*cap = new_cap;
	return (0);
}

static char	*read_all(int fd)
{
	char	buf[1024];
	char	*text;
	size_t	len;
	size_t	cap;
	ssize_t	bytes;

	cap = 1024;
	len = 0;
	text = ft_calloc(cap, sizeof(char));
	if (!text)
		return (NULL);
	while (1)
	{
		bytes = read(fd, buf, sizeof(buf));
		if (bytes < 0)
			return (free(text), NULL);
		if (bytes == 0)
			break ;
		if (reserve_buf(&text, len + bytes + 1, &cap))
			return (free(text), NULL);
		ft_memcpy(text + len, buf, bytes);
		len += bytes;
	}
	text[len] = '\0';
	return (text);
}

static char	**split_lines(const char *text)
{
	char	**lines;
	size_t	i;
	size_t	j;
	size_t	start;

	lines = ft_calloc(ft_strlen(text) + 1, sizeof(char *));
	if (!lines)
		return (NULL);
	i = 0;
	j = 0;
	while (text[i])
	{
		start = i;
		while (text[i] != '\0' && text[i] != '\n')
			i++;
		lines[j] = ft_substr(text, start, i - start);
		if (!lines[j++])
			return (free_strs(lines), NULL);
		if (text[i] == '\n')
			i++;
	}
	return (lines);
}

int	read_file(const char *path, char ***lines)
{
	int		fd;
	char	*text;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (print_error(strerror(errno)));
	text = read_all(fd);
	close(fd);
	if (!text)
		return (print_error("failed to read file"));
	*lines = split_lines(text);
	free(text);
	if (!*lines)
		return (print_error("failed to split file lines"));
	return (0);
}
