NAME	=	webserv

CC			=	clang++
INCLUDES	=	srcs/Kernel/*.hpp

CPPFLAGS	=	-MD -std=c++98 -std=c++0x -I$(INCLUDES)
RM			=	rm -rf

SRCS		=	srcs/main.cpp \
				srcs/Kernel/Engine.cpp \
				srcs/Kernel/Kernel.cpp \
				srcs/Parser/Config.cpp \
				srcs/Parser/Server.cpp \
				srcs/Parser/Location.cpp


OBJS		=	$(SRCS:%.cpp=%.o)
DEPS		=	$(SRCS:%.cpp=%.d)

all:		$(NAME)

$(NAME):	$(OBJS)
			@$(CC) $(CPPFLAGS) $(OBJS) -o $(NAME)
			@echo "Linked into executable $(NAME)"

.cpp.o:
			@$(CC) $(CPPFLAGS) -c $< -o $(<:.cpp=.o)
			@echo "Compiling $<."

clean:
			@$(RM) $(OBJS)
			@$(RM) $(DEPS)
			@echo "Removed object files." 

fclean:		clean
			@$(RM) $(NAME)
			@echo "Removed executable."

re:			fclean all

-include $(DEPS)

.PHONY:		fclean, all, re
