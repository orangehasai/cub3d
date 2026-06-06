NAME = cub3D
CC = cc
RM = rm -f
CFLAGS = -Wall -Wextra -Werror
CPPFLAGS = -Iinclude -Ilibft -Iminilibx-linux
MLX_LIBS = -lXext -lX11 -lm -lbsd
OBJ_DIR = obj

LIBFT_DIR = libft
LIBFT_A = $(LIBFT_DIR)/libft.a
MLX_DIR = minilibx-linux
MLX_A = $(MLX_DIR)/libmlx.a

SRC = src/main.c \
	src/init/init_game.c \
	src/init/init_mlx.c \
	src/init/init_texture.c \
	src/init/destroy_game.c \
	src/parse/read_file.c \
	src/parse/parse_elements.c \
	src/parse/parse_map.c \
	src/parse/validate_map.c \
	src/parse/validate_grid.c \
	src/render/render_frame.c \
	src/render/draw_wall.c \
	src/render/raycast.c \
	src/render/texture_sample.c \
	src/input/hook.c \
	src/input/move.c \
	src/utils/error.c \
	src/utils/free.c \
	src/utils/string.c

OBJ = $(SRC:src/%.c=$(OBJ_DIR)/%.o)
HEADERS = include/cub3d.h

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(LIBFT_A) $(MLX_A) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT_A) $(MLX_A) $(MLX_LIBS) -o $(NAME)

$(LIBFT_A):
	$(MAKE) -C $(LIBFT_DIR)

$(MLX_A):
	$(MAKE) -C $(MLX_DIR)

$(OBJ_DIR)/%.o: src/%.c $(HEADERS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)
	$(MAKE) -C $(LIBFT_DIR) clean
	@if [ -f $(MLX_DIR)/Makefile.gen ]; then \
		$(MAKE) -C $(MLX_DIR) clean; \
	fi
	$(RM) $(MLX_DIR)/Makefile.gen $(MLX_DIR)/test/Makefile.gen

fclean: clean
	$(RM) $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all
