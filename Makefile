NAME = webserv

SRCS 		=	WebServer/WebServer.cpp \
				Parser/Parser.cpp \
				Server/Server.cpp \
				Location/Location.cpp \
				Utils/Utils.cpp \
				Client/Client.cpp \
				ServerInfo/ServerInfo.cpp \
				Request/Request.cpp \
				Response/Response.cpp \
				Get/Get.cpp \
				Delete/Delete.cpp \
				Post/Post.cpp \
				ServerNetworkInfo/ServerNetworkInfo.cpp \
				Cgi/Cgi.cpp

HEADERS 	=	WebServer/WebServer.Hpp \
				Parser/Parser.Hpp \
				Server/Server.Hpp \
				Location/Location.Hpp \
				Utils/Utils.Hpp \
				Client/Client.Hpp \
				ServerInfo/ServerInfo.Hpp \
				Request/Request.Hpp \
				Response/Response.Hpp \
				Get/Get.Hpp \
				Delete/Delete.Hpp \
				Post/Post.Hpp \
				ServerNetworkInfo/ServerNetworkInfo.Hpp \
				Cgi/Cgi.Hpp

MAIN 		= main.cpp

MAIN_O 		= main.o

OBJS 		= $(SRCS:.cpp=.o)

FLAGS 		= -Wextra -Wall -Werror -std=c++98 #-fsanitize=address

COMPILER	= c++

all : $(NAME)

$(NAME) : $(OBJS)  $(MAIN_O)
	$(COMPILER) $(FLAGS) $(OBJS) $(MAIN_O) -o $(NAME)

%.o: %.cpp %.hpp
	$(COMPILER) $(FLAGS) -c $< -o $@

$(MAIN_O) : $(MAIN)
	$(COMPILER) $(FLAGS) -c $< -o $@

clean :
	rm -rf $(OBJS) $(MAIN_O)

fclean : clean
	rm -rf $(NAME)

re : fclean all

.PHONY : all clean fclean re
