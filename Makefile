parser := ./srcs/parser
server := ./srcs/server
conn := ./srcs/connection
http := ./srcs/http
router := ./srcs/router
aux := ./srcs/aux.cpp
main := ./srcs/main.cpp
test_src := ./tests/src
main_test := ./tests/main.cpp

all: clean
	c++ -g3 -std=c++98 -Wall -Wextra -Werror -o webserv -I ./interfaces \
	$(main) $(aux) $(parser)/*.cpp $(server)/*.cpp $(conn)/*.cpp $(http)/*.cpp $(router)/*.cpp

tests: clean
	c++ -g3 -o test_executable $(main_test) $(test_src)/*.cpp -D TEST \
	$(parser)/*.cpp $(server)/*.cpp $(aux) $(conn)/*.cpp $(http)/*.cpp $(router)/*.cpp

clean:
	rm -f webserv test_executable

fclean: clean

re: fclean all


