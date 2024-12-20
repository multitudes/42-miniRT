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

CFLAGS 			:= -Wall -Wextra -Werror
CFLAGS 			+= -Iinclude
CFLAGS			+= -Isrc -Ilib/external
CFLAGS			+= -O3 -Ofast -march=native -funroll-loops -Wunreachable-code
CFLAGS 			+= -finline-functions -fno-rtti -fno-exceptions -fno-stack-protector
CFLAGS 			+= -Wno-overlength-strings -fno-signed-zeros -fno-trapping-math
CFLAGS 			+= -fomit-frame-pointer -ffast-math -fno-math-errno -funsafe-math-optimizations
CFLAGS 			+= -fassociative-math -freciprocal-math -ffinite-math-only -frounding-math
CFLAGS 			+=  -g
# CFLAGS 			+=  -DNDEBUG


# directories
OBJ_DIR			= 	obj/
SRC_DIR			= 	src/
LIBFTDIR 		= 	./lib/libft
LIBMLX			= 	./lib/MLX42

LIBS			=  	$(LIBMLX)/build/libmlx42.a -ldl -lglfw -pthread -lm
LIBS 			+= 	$(LIBFTDIR)/libft.a

# this is used in production to optimize the linking and remove unnecessary code aking the binary smaller
#LDFLAGS			= 	-Wl,--gc-sections -Wl,-O2 -Wl,--as-needed -Wl,--strip-all

INCLUDES		=  	-I./include -I./lib/external -I$(LIBMLX)/include -I$(LIBFTDIR)
#INCLUDES		=  	-I./include -I$(LIBMLX)/include -I$(LIBFTDIR) -I/opt/homebrew/opt/glfw/include #petras mac

SRCS 			= $(addprefix $(SRC_DIR), main.c camera.c sphere.c sphere_utils.c \
						color.c color_utils.c ray.c rtw_stb_image.c \
						rtw_stb_image2.c vec3.c vec3_utils.c vec3_utils2.c vec3_utils3.c vec3_utils4.c \
						interval.c interval2.c utils.c utils2.c ambient.c plane.c plane_utils.c \
						cylinder.c cylinder_hit.c cylinder_utils.c \
						texture.c texture2.c material.c material2.c material3.c onb.c pdf.c pdf2.c pdf3.c \
						quad.c quad2.c hittable_list.c \
						parse.c parse_obj1.c parse_obj2.c parse_obj3.c parse_obj4.c parse_obj5.c parse_obj6.c \
						parse_utils1.c parse_utils2.c \
						hook_utils.c hook_utils2.c hook_utils3.c hook_utils4.c hook_utils5.c \
						disk.c disk_utils.c box.c triangle.c triangle2.c mersenne_twister.c \
						bilateral_filter.c bilateral_filter2.c camera_ray_color.c camera_ray_color_utils.c \
						render.c render_utils.c cone_init.c cone_hit.c cone_utils.c)

OBJS 			= $(patsubst $(SRC_DIR)%.c,$(OBJ_DIR)%.o,$(SRCS))
HDRS 			= $(addprefix include/, debug.h camera.h vec3.h sphere.h ray.h interval.h \
						hittable.h hittable_list.h minirt.h color.h hook_utils.h \
						utils.h ambient.h plane.h cylinder.h texture.h \
						material.h onb.h pdf.h quad.h disk.h bilateral_filter.h\
						box.h triangle.h mersenne_twister.h parse.h)
HDRS			+= $(addprefix lib/, external/stb_image.h external/stb_image_write.h)

LIBFT 			= $(LIBFTDIR)/libft.a
LIBFT_LIB 		= -Llibft -lft

# OS specific flags
UNAME 			= $(shell uname -s)
ifeq ($(UNAME), Linux)
	LIBS 		+=  -lbsd
else ifeq ($(UNAME), Darwin)
	LIBS 		+=  -L/opt/homebrew/lib
endif

# to look up later - should detect the changes in libft and recompile
# MAKEFILES := lib/libft/Makefile
# include $(MAKEFILES)

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

bonus: CFLAGS += -DDOBONUS
bonus: re

clean:
	rm -f $(OBJS)
	rm -rf $(OBJ_DIR)
	$(MAKE) -C $(LIBFTDIR) clean

fclean: clean
	rm -rf $(NAME)
	$(MAKE) -C $(LIBFTDIR) fclean
	@rm -rf $(LIBMLX)/build

re: fclean all

# The idea is for this project to use run for production with extra flags to speed it up
# and optimize the binary size
ARGS = z_scenes/billard.rt

run:
	@echo
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJS) $(LIBS) $(LDFLAGS) -o $(NAME)
	@PATH=".$${PATH:+:$${PATH}}" && $(NAME) $(ARGS)

valrun:
	@echo
	$(CC) $(CFLAGS) $(INCLUDES) $(OBJS) $(LIBS) $(LDFLAGS) -o $(NAME)
	@PATH=".$${PATH:+:$${PATH}}" && valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-out.txt ./$(NAME) $(ARGS)

.PHONY: all clean fclean re libmlx $(LIBFT)
