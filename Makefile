src := ./srcs/parse_config
main := ./srcs/main.cpp

all:
	c++ -g3 -std=c++98 -Wall -Wextra -Werror -o webserv $(main) $(src)/*.cpp

tests: clean
	c++ -g3 -std=c++98 -Wall -Wextra -Werror -o test_executable ./tests/*.cpp $(src)/*.cpp -D TEST && ./test_executable

clean:
	rm -f webserv test_executable

fclean: clean

re: fclean all


