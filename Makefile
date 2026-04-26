NAME = cub3D

CC = cc
CFLAGS = -Wall -Wextra -Werror -O3 #-fsanitize=address

MLX_MACOS = mlx_macos/libmlx.a
MLX_LINUX = mlx_linux/libmlx.a

INCLUDES = -Iinc

OS = $(shell uname -s)
ifeq ($(OS), Darwin)
	INCLUDES += -Imlx_macos -Iinc/macos_inc
	MLX = $(MLX_MACOS)
	LIBS = -Lmlx_macos -lmlx -framework OpenGL -framework AppKit -O3
else ifeq ($(OS), Linux)
	INCLUDES += -Imlx_linux -Iinc/linux_inc
	MLX = $(MLX_LINUX)
	LIBS = -Lmlx_linux -lmlx -lXext -lX11 -lm
else
	$(error Unsupported OS. Only Darwin and Linux are supported.)
endif

SRCS_DIR = src
OBJS_DIR = obj/$(OS)
SRCS = $(shell find $(SRCS_DIR) -type f -name "*.c")
OBJS = $(patsubst $(SRCS_DIR)/%.c, $(OBJS_DIR)/%.o, $(SRCS))

all: $(NAME)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(MLX_MACOS):
	make -s -C mlx_macos

$(MLX_LINUX):
	make -s -C mlx_linux

$(NAME): $(OBJS) $(MLX)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME)

clean:
	rm -rf $(OBJS_DIR)
	# make -s clean mlx_linux
	# make -s clean mlx_macos

fclean: clean
	rm -rf $(NAME)

X:
	#clear

re: fclean all X

.PHONY: all clean fclean re
