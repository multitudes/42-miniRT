# The @ symbol at the beginning of the line suppresses the echoing of the command to the terminal when it is executed.
# Without it, the command and its output would be printed to the terminal, which can make the output cleaner.
# -o $@:
# This specifies the output file to be produced. $@ is a special variable in Makefiles, representing
# the target of the rule. In this context, it specifies the output file name.
# -c $<:
# The -c flag indicates that the compiler should only produce an object file without linking.
# $< is another special variable, representing the first prerequisite (dependency) of the rule. It is the source file.

NAME 			= 	miniRT

CC 				= 	cc

CFLAGS 			= 	-Wextra -Wall -Werror -g
CFLAGS 			+= 	-Iinclude -Isrc -O3 -Wunreachable-code -Ofast
# CFLAGS += -DDEBUG=1

# directories
OBJ_DIR			= 	obj/
SRC_DIR			= 	src/
LIBFTDIR 		= 	./lib/libft
LIBMLX			= 	./lib/MLX42

LIBS			=  	$(LIBMLX)/build/libmlx42.a -ldl -lglfw -pthread -lm
LIBS 			+= 	$(LIBFTDIR)/libft.a

INCLUDES		=  	-I./include -I$(LIBMLX)/include -I$(LIBFTDIR)
#INCLUDES		=  	-I./include -I$(LIBMLX)/include -I$(LIBFTDIR) -I/opt/homebrew/opt/glfw/include #petras mac

SRCS 			= $(addprefix $(SRC_DIR), main.c camera.c color.c ray.c vec3.c)
OBJS 			= $(patsubst $(SRC_DIR)%.c,$(OBJ_DIR)%.o,$(SRCS))
HDRS 			= $(addprefix include/, debug.h vec3.h ray.h rtweekend.h color.h camera.h )

LIBFT 			= $(LIBFTDIR)/libft.a
LIBFT_LIB 		= -Llibft -lft

# OS specific flags
UNAME 			= 	$(shell uname -s)
ifeq ($(UNAME), Linux)
	LIBS 		+=  -lbsd
else ifeq ($(UNAME), Darwin)
	# LIBS 		+=  -L/opt/homebrew/lib
endif

all: libmlx $(LIBFT) $(NAME)

libmlx:
	@if [ "$(shell uname)" = "Linux" ]; then \
		if ! pkg-config --exists glfw3; then \
			echo "Error: GLFW is not installed. Please install GLFW to proceed."; \
			exit 1; \
		fi \
	elif [ "$(shell uname)" = "Darwin" ]; then \
		if [ ! -d "/opt/homebrew/include/GLFW" ] && [ ! -d "/usr/local/include/GLFW" ]; then \
			echo "Error: GLFW is not installed. Please install GLFW to proceed."; \
			exit 1; \
		fi \
	fi
	@if [ ! -d "lib/MLX42" ]; then \
		mkdir -p lib/MLX42; \
		git clone https://github.com/codam-coding-college/MLX42.git lib/MLX42; \
	fi
	@cmake $(LIBMLX) -B $(LIBMLX)/build && make -C $(LIBMLX)/build -j4

$(LIBFT):
	$(MAKE) -C $(LIBFTDIR) all

# Static pattern rule for compilation - adding the .o files in the obj folder
# with includes for the libft that will allow the "libft.h" notation
$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(NAME): $(OBJS) $(HDRS)
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJS) $(LIBS) -o $(NAME)
#$(CC) $(CFLAGS) $(INCLUDES) $(OBJS) $(LDFLAGS) $(LIBS) -o $(NAME)

clean:
	rm -f $(OBJS)
	rm -rf $(OBJ_DIR)
	$(MAKE) -C $(LIBFTDIR) clean

fclean: clean
	@rm -rf $(NAME)
#$(MAKE) -C $(LIBFTDIR) fclean
#@rm -rf $(LIBMLX)/build

re: fclean all

run: all
	@echo
	@PATH=".$${PATH:+:$${PATH}}" && $(NAME) $(ARGS)

.PHONY: all clean fclean re libmlx $(LIBFT)


