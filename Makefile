NAME	=	webserv

CC			=	clang++
INCLUDES	=	srcs/*.hpp 

CPPFLAGS	=	-MD -g -fno-limit-debug-info -std=c++98 -I$(INCLUDES)
RM			=	rm -rf

SRCS		=	srcs/main.cpp \
				srcs/Kernel/Kernel.cpp \
				srcs/Request/Request.cpp \
				srcs/Response/Response.cpp \
				srcs/Response/ResponseBody.cpp \
				srcs/Response/autoindex.cpp \
				srcs/Response/Cgi.cpp \
				srcs/Client/Client.cpp \
				srcs/Config/Config.cpp \
				srcs/Server/Server.cpp \
				srcs/Parser/Utils.cpp \
				srcs/Parser/Parsing.cpp \
				srcs/Parser/ParseRequest.cpp \
				srcs/Parser/ParseResponse.cpp 
				 

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
