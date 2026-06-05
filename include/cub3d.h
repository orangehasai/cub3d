/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stonegaw <stonegaw@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 17:59:41 by stonegaw          #+#    #+#             */
/*   Updated: 2026/06/05 18:44:15 by stonegaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "libft.h"
# include "mlx.h"
# include <X11/X.h>
# include <X11/keysym.h>
# include <errno.h>
# include <fcntl.h>
# include <math.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

# define WIN_WIDTH 1280
# define WIN_HEIGHT 720
# define WIN_TITLE "cub3D"

# define FOV_SCALE 0.66
# define MOVE_SPEED 3.0
# define ROT_SPEED 2.0

typedef struct s_rgb
{
	int			r;
	int			g;
	int			b;
	int			value;
}				t_rgb;

typedef struct s_img
{
	void		*img;
	char		*addr;
	int			bpp;
	int			line_len;
	int			endian;
	int			width;
	int			height;
}				t_img;

typedef struct s_map
{
	char		**rows;
	char		**grid;
	int			width;
	int			height;
}				t_map;

typedef struct s_scene
{
	char		*no_path;
	char		*so_path;
	char		*we_path;
	char		*ea_path;
	t_rgb		floor;
	t_rgb		ceiling;
	t_map		map;
}				t_scene;

typedef struct s_player
{
	double		x;
	double		y;
	double		dir_x;
	double		dir_y;
	double		plane_x;
	double		plane_y;
}				t_player;

typedef struct s_keys
{
	int			w;
	int			a;
	int			s;
	int			d;
	int			left;
	int			right;
}				t_keys;

typedef struct s_time
{
	long		prev_usec;
	double		delta_sec;
}				t_time;

typedef struct s_ray
{
	int			x;
	int			map_x;
	int			map_y;
	int			step_x;
	int			step_y;
	int			side;
	double		camera_x;
	double		ray_dir_x;
	double		ray_dir_y;
	double		side_dist_x;
	double		side_dist_y;
	double		delta_dist_x;
	double		delta_dist_y;
	double		perp_wall_dist;
	double		wall_x;
	int			line_height;
	int			draw_start;
	int			draw_end;
	int			tex_x;
}				t_ray;

typedef struct s_game
{
	void		*mlx;
	void		*win;
	t_img		frame;
	t_img		no_tex;
	t_img		so_tex;
	t_img		we_tex;
	t_img		ea_tex;
	t_scene		scene;
	t_player	player;
	t_keys		keys;
	t_time		time;
	int			running;
}				t_game;

void			init_game(t_game *game);
int				init_mlx(t_game *game);
int				init_textures(t_game *game);
void			destroy_game(t_game *game);

int				read_file(const char *path, char ***lines);
int				parse_elements(t_scene *scene, char **lines, int *map_start);
int				parse_map(t_scene *scene, char **lines, int map_start);
int				validate_map(t_game *game);

int				render_frame(t_game *game);
void			raycast_column(t_game *game, int x);
int				get_texel(t_img *tex, int x, int y);

int				on_key_press(int keycode, t_game *game);
int				on_key_release(int keycode, t_game *game);
int				on_destroy(t_game *game);
int				on_expose(t_game *game);
int				on_loop(t_game *game);
void			update_player(t_game *game);
int				can_move_to(t_game *game, double x, double y);

int				print_error(const char *message);
void			free_strs(char **strs);
void			free_map(t_map *map);
const char		*skip_spaces(const char *str);

#endif
