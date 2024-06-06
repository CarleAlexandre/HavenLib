NAME		=	libHavenLib.a

BUILDDIR	=	build/

CC			=	gcc

CFLAGS		=	-std=c++20

OBJ			=	$(SRC:%.cpp=%.o)

DEPS		+=	$(wildcard include/*.hpp)
DEPS		+=	$(wildcard include/*.h)

SRC			+=	$(wildcard source/*.cpp)

INCLUDE		=	-I include


$(BUILDDIR)$(NAME)		:	$(OBJ) $(DEPS)
		mkdir -p $(BUILDDIR)
		ar rcs -o $@ $(OBJ)

$(OBJ)		:	%.o	:	%.cpp $(DEPS)
		$(CC) $(CFLAGS) ${INCLUDE} -lstdc++ -c $< -o $@

all		:	$(BUILDDIR)$(NAME)

clean		:
		rm -rf $(OBJ)

fclean		:	clean
		rm -rf $(BUILDDIR)

re		:	fclean all

