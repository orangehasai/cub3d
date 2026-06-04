# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: stonegaw <stonegaw@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/27 03:13:30 by takenakatak       #+#    #+#              #
#    Updated: 2025/05/02 18:48:08 by stonegaw         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a
CC = cc
CFLAGS = -Wall -Wextra -Werror
SRC = ft_atoi.c ft_bzero.c ft_calloc.c ft_isalnum.c ft_isalpha.c ft_isascii.c ft_isdigit.c\
	ft_isprint.c ft_itoa.c ft_memchr.c ft_memcmp.c ft_memcpy.c ft_memmove.c ft_memset.c\
	ft_putchar_fd.c ft_putendl_fd.c ft_putnbr_fd.c ft_putstr_fd.c ft_split.c ft_strchr.c\
	ft_strdup.c ft_striteri.c ft_strjoin.c ft_strlcat.c ft_strlcpy.c ft_strlen.c ft_strmapi.c\
	ft_strncmp.c ft_strnstr.c ft_strrchr.c ft_strtrim.c ft_substr.c ft_tolower.c ft_toupper.c

BSRC = ft_lstadd_back.c ft_lstadd_front.c ft_lstclear.c ft_lstdelone.c ft_lstiter.c ft_lstlast.c\
	ft_lstmap.c ft_lstnew.c ft_lstsize.c
OBJ = $(SRC:%.c=%.o)
ifdef WITH_BONUS
OBJ += $(BSRC:%.c=%.o)
endif

.PHONY:all
all:$(NAME)

$(NAME): $(OBJ)
	@echo "DEBUG: --- Archive Rule ---"
	@echo "DEBUG: SRC = $(SRC)" # DEBUG: Show the value of SRC
	@echo "DEBUG: OBJ = $(OBJ)" # DEBUG: Show the value of OBJ
	@echo "DEBUG: Prerequisites ($^): $^" # DEBUG: Show actual prerequisites passed to ar
	@echo "DEBUG: Building archive $(NAME) from $(OBJ)" # DEBUG: Confirm ar command inputs
	ar -csr $@ $^
	@echo "DEBUG: Archive rule finished."

%.o: %.c
	@echo "DEBUG: --- Compile Rule ---"
	@echo "DEBUG: Target ($@): $@" # DEBUG: Show the target .o file
	@echo "DEBUG: Source ($<): $<" # DEBUG: Show the source .c file
	$(CC) $(CFLAGS) -c $< -o $@ # CFLAGS 変数を使用
	@echo "DEBUG: Compile rule finished for $@"

.PHONY:clean
clean:
	@echo "DEBUG: --- Clean Rule ---"
	@echo "DEBUG: Cleaning object files: $(OBJ)" # DEBUG: Show files being cleaned
	rm -f $(OBJ) $(BSRC:%.c=%.o)
	@echo "DEBUG: Clean rule finished."

.PHONY:fclean
fclean:clean
	@echo "DEBUG: --- Fclean Rule ---"
	@echo "DEBUG: Cleaning library file: $(NAME)" # DEBUG: Show file being cleaned
	rm -f $(NAME)
	@echo "DEBUG: Fclean rule finished."

.PHONY:re
re:fclean all

.PHONY:bonus
bonus:
	make WITH_BONUS=1