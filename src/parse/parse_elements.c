/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_elements.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stonegaw <stonegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 18:00:46 by stonegaw          #+#    #+#             */
/*   Updated: 2026/06/05 23:29:33 by stonegaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	parse_color_value(const char **str, int *value)
{
	int	num;
	int	digits;

	num = 0;
	digits = 0;
	while (**str >= '0' && **str <= '9')
	{
		num = num * 10 + (**str - '0');
		digits++;
		(*str)++;
	}
	if (digits == 0 || num > 255)
		return (1);
	*value = num;
	*str = skip_spaces(*str);
	return (0);
}

static int	parse_color(const char *value, t_rgb *color)
{
	const char	*str;
	int			rgb[3];

	if (color->value != -1)
		return (print_error("duplicate color identifier"));
	str = skip_spaces(value);
	if (parse_color_value(&str, &rgb[0]) || *str++ != ',')
		return (print_error("invalid color"));
	str = skip_spaces(str);
	if (parse_color_value(&str, &rgb[1]) || *str++ != ',')
		return (print_error("invalid color"));
	str = skip_spaces(str);
	if (parse_color_value(&str, &rgb[2]) || *str != '\0')
		return (print_error("invalid color"));
	color->r = rgb[0];
	color->g = rgb[1];
	color->b = rgb[2];
	color->value = (rgb[0] << 16) | (rgb[1] << 8) | rgb[2];
	return (0);
}

static int	parse_texture(char **dst, const char *value)
{
	char	*path;
	size_t	len;

	if (*dst)
		return (print_error("duplicate texture identifier"));
	path = ft_strtrim(value, " \t");
	if (!path)
		return (print_error("invalid texture path"));
	if (*path == '\0')
		return (free(path), print_error("invalid texture path"));
	len = ft_strlen(path);
	if (len <= 4 || ft_strncmp(path + len - 4, ".xpm", 4) != 0)
		return (free(path), print_error("texture must use .xpm"));
	if (access(path, R_OK) != 0)
		return (free(path), print_error(strerror(errno)));
	*dst = path;
	return (0);
}

static int	parse_element(t_scene *scene, const char *line, int *matched)
{
	*matched = 1;
	if (ft_strncmp(line, "NO ", 3) == 0 || ft_strncmp(line, "NO\t", 3) == 0)
		return (parse_texture(&scene->no_path, line + 3));
	if (ft_strncmp(line, "SO ", 3) == 0 || ft_strncmp(line, "SO\t", 3) == 0)
		return (parse_texture(&scene->so_path, line + 3));
	if (ft_strncmp(line, "WE ", 3) == 0 || ft_strncmp(line, "WE\t", 3) == 0)
		return (parse_texture(&scene->we_path, line + 3));
	if (ft_strncmp(line, "EA ", 3) == 0 || ft_strncmp(line, "EA\t", 3) == 0)
		return (parse_texture(&scene->ea_path, line + 3));
	if (ft_strncmp(line, "F ", 2) == 0 || ft_strncmp(line, "F\t", 2) == 0)
		return (parse_color(line + 2, &scene->floor));
	if (ft_strncmp(line, "C ", 2) == 0 || ft_strncmp(line, "C\t", 2) == 0)
		return (parse_color(line + 2, &scene->ceiling));
	*matched = 0;
	return (0);
}

int	parse_elements(t_scene *scene, char **lines, int *map_start)
{
	const char	*line;
	int			i;
	int			matched;

	i = 0;
	while (lines[i])
	{
		line = skip_spaces(lines[i]);
		if (*line == '\0')
			i++;
		else if (parse_element(scene, line, &matched))
			return (1);
		else if (!matched)
			break ;
		else
			i++;
	}
	*map_start = i;
	if (!scene->no_path || !scene->so_path || !scene->we_path
		|| !scene->ea_path)
		return (print_error("missing texture identifier"));
	if (scene->floor.value == -1 || scene->ceiling.value == -1)
		return (print_error("missing color identifier"));
	return (0);
}
