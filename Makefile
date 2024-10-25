parser := ./srcs/parse_config
server := ./srcs/server
aux := ./srcs/aux.cpp
main := ./srcs/main.cpp
test_src := ./tests

all:
	c++ -g3 -std=c++98 -Wall -Wextra -Werror -o webserv $(main) $(aux) $(parser)/*.cpp $(server)/*.cpp -I ./interfaces

tests: clean
	c++ -g3 -std=c++98 -Wall -Wextra -Werror -o test_executable $(test_src)/*.cpp $(parser)/*.cpp $(server)/*.cpp $(aux) -D TEST

clean:
	rm -f webserv test_executable

fclean: clean

re: fclean all


