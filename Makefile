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

<<<<<<< HEAD
CFLAGS 			= 	-Wextra -Wall -Werror -g
CFLAGS 			+= 	-Iinclude -Isrc -Wunreachable-code 
=======
CFLAGS 			= 	-Wextra -Wall -Werror
CFLAGS 			+= 	-Iinclude -Isrc -O3 -Wunreachable-code -Ofast
>>>>>>> dev
# CFLAGS += -DDEBUG=1

# directories
OBJ_DIR			= 	obj/
SRC_DIR			= 	src/
LIBFTDIR 		= 	./lib/libft
LIBMLX			= 	./lib/MLX42

LIBS			=  	$(LIBMLX)/build/libmlx42.a -ldl -lglfw -pthread -lm 
LIBS 			+= 	$(LIBFTDIR)/libft.a

# this is used in production to optimize the linking and remove unnecessary code aking the binary smaller
LDFLAGS			= 	-Wl,--gc-sections -Wl,-O2 -Wl,--as-needed -Wl,--strip-all

INCLUDES		=  	-I./include -I$(LIBMLX)/include -I$(LIBFTDIR)
#INCLUDES		=  	-I./include -I$(LIBMLX)/include -I$(LIBFTDIR) -I/opt/homebrew/opt/glfw/include #petras mac

<<<<<<< HEAD
SRCS 			= $(addprefix $(SRC_DIR), main.c camera.c sphere.c color.c ray.c \
											vec3.c hittable.c interval.c utils.c ambient.c plane.c cylinder.c parse.c)
=======
SRCS 			= $(addprefix $(SRC_DIR), main.c camera.c sphere.c color.c ray.c rtw_stb_image.c \
											vec3.c hittable.c interval.c utils.c ambient.c plane.c cylinder.c \
											texture.c material.c onb.c)
>>>>>>> dev
OBJS 			= $(patsubst $(SRC_DIR)%.c,$(OBJ_DIR)%.o,$(SRCS))
HDRS 			= $(addprefix include/, debug.h camera.h vec3.h sphere.h ray.h interval.h \
									hittable.h hittable_list.h minirt.h color.h \
									utils.h ambient.h plane.h cylinder.h texture.h \
									rtw_stb_image.h material.h onb.h external/stb_image.h external/stb_image_write.h)

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
	$(CFLAGS) += -g
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJS) $(LIBS) -o $(NAME)

clean:
	rm -f $(OBJS)
	rm -rf $(OBJ_DIR)
#$(MAKE) -C $(LIBFTDIR) clean

fclean: clean
	@rm -rf $(NAME)
#$(MAKE) -C $(LIBFTDIR) fclean
#@rm -rf $(LIBMLX)/build

re: fclean all

# The idea is for this project to use run for production with extra flags to speed it up
# and optimize the binary size
run:
	@echo
	@$(CFLAGS) += -O3 -Ofast -march=native -funroll-loops -Wunreachable-code
	@$(CFLAGS) += -finline-functions -fno-rtti -fno-exceptions -fno-stack-protector
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJS) $(LIBS) $(LDFLAGS) -o $(NAME)
	@PATH=".$${PATH:+:$${PATH}}" && $(NAME) $(ARGS)

.PHONY: all clean fclean re libmlx $(LIBFT)


