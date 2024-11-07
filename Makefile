parser := ./srcs/parser
server := ./srcs/server
conn := ./srcs/connection
http := ./srcs/http
router := ./srcs/router
handlers := ./srcs/handlers
aux := ./srcs/aux.cpp
main := ./srcs/main.cpp
test_src := ./tests/src
main_test := ./tests/main.cpp

all_srcs = $(parser)/*.cpp $(server)/*.cpp $(conn)/*.cpp $(http)/*.cpp $(router)/*.cpp $(handlers)/*.cpp

all: clean
	c++ -g3 -std=c++98 -Wall -Wextra -Werror -o webserv -I ./interfaces $(main) $(aux)  \
	$(all_srcs)

tests: clean
	c++ -g3 -o test_executable $(main_test) $(test_src)/*.cpp $(aux) -D TEST \
	$(all_srcs)

clean:
	rm -f webserv test_executable

fclean: clean

re: fclean all


