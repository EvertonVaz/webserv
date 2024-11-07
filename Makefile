NAME = webserv
TEST_NAME = test_executable
FLAGS = -Wall -Werror -Wextra -std=c++98
CC = c++ -g3

# Colors
BLUE = "\033[34;1m"
GREEN = "\033[32;1m"
RED = "\033[31;1m"
CYAN = "\033[36;1;3;208m"
WHITE = "\033[37;1;4m"
END_COLOR = "\033[0m"

# Directories
SRC_DIR = ./srcs
TEST_DIR = ./tests
OBJ_DIR = ./obj
TEST_OBJ_DIR = ./obj/test
INCLUDES = -I./interfaces

# Source files
SRC = $(wildcard $(SRC_DIR)/parser/*.cpp) \
      $(wildcard $(SRC_DIR)/server/*.cpp) \
      $(wildcard $(SRC_DIR)/connection/*.cpp) \
      $(wildcard $(SRC_DIR)/http/*.cpp) \
      $(wildcard $(SRC_DIR)/router/*.cpp) \
      $(wildcard $(SRC_DIR)/handlers/*.cpp) \
      $(SRC_DIR)/aux.cpp $(SRC_DIR)/main.cpp

TEST_SRC = $(wildcard $(TEST_DIR)/src/*.cpp) \
          $(TEST_DIR)/main.cpp \
          $(filter-out $(SRC_DIR)/main.cpp, $(SRC))

# Object files
OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
TEST_OBJ = $(TEST_SRC:$(TEST_DIR)/%.cpp=$(TEST_OBJ_DIR)/%.o)

all: $(NAME)

test: $(TEST_NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@echo $(BLUE)[Compiling]$(END_COLOR) $(WHITE)$(notdir $<)$(END_COLOR)
	@$(CC) $(FLAGS) $(INCLUDES) -c $< -o $@

$(TEST_OBJ_DIR)/%.o: $(TEST_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@echo $(BLUE)[Compiling Test]$(END_COLOR) $(WHITE)$(notdir $<)$(END_COLOR)
	@$(CC) $(INCLUDES) -D TEST -c $< -o $@

$(NAME): $(OBJ)
	@echo $(CYAN)"--------------------------------------------------"$(END_COLOR)
	@echo $(CYAN)"|       Webserver created successfully!!         |"$(END_COLOR)
	@echo $(CYAN)"--------------------------------------------------"$(END_COLOR)
	@$(CC) $(FLAGS) $^ -o $@

$(TEST_NAME): $(TEST_OBJ)
	@echo $(CYAN)"--------------------------------------------------"$(END_COLOR)
	@echo $(CYAN)"|       Test executable created successfully!!   |"$(END_COLOR)
	@echo $(CYAN)"--------------------------------------------------"$(END_COLOR)
	@$(CC) $^ -o $@

clean:
	@echo $(RED)[Removing Objects]$(END_COLOR)
	@rm -rf $(OBJ_DIR)

fclean: clean
	@echo $(RED)[Removing executables]$(END_COLOR)
	@rm -f $(NAME) $(TEST_NAME)

re: fclean all

.PHONY: all clean fclean re test