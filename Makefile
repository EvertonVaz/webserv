parser := ./srcs/parse_config
server := ./srcs/server
aux := ./srcs/aux.cpp
main := ./srcs/main.cpp

all:
	c++ -g3 -std=c++98 -Wall -Wextra -Werror -o webserv $(main) $(aux) $(parser)/*.cpp $(server)/*.cpp

tests: clean
	c++ -g3 -std=c++98 -Wall -Wextra -Werror -o test_executable ./tests/*.cpp $(src)/*.cpp -D TEST && ./test_executable

clean:
	rm -f webserv test_executable

fclean: clean

re: fclean all


