parser := ./srcs/parser
server := ./srcs/server
conn := ./srcs/connection
http := ./srcs/HTTP
aux := ./srcs/aux.cpp
main := ./srcs/main.cpp
test_src := ./tests/src
main_test := ./tests/main.cpp

all:
	c++ -g3 -std=c++98 -Wall -Wextra -Werror -o webserv $(main) $(aux) $(parser)/*.cpp $(server)/*.cpp $(conn)/*.cpp $(http)/*.cpp -I ./interfaces

tests: clean
	c++ -g3 -o test_executable $(main_test) $(test_src)/*.cpp $(parser)/*.cpp $(server)/*.cpp $(aux) $(conn)/*.cpp -D TEST

clean:
	rm -f webserv test_executable

fclean: clean

re: fclean all


