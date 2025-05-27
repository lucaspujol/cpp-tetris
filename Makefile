CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11
CXXFLAGS_DEBUG = $(CXXFLAGS) -g -fsanitize=address
LDFLAGS = -lSDL2 -lSDL2_ttf
LDFLAGS_DEBUG = $(LDFLAGS) -fsanitize=address

SRC_DIR = src
OBJ_DIR = obj
OBJ_DIR_DEBUG = obj/debug

SRCS = $(shell find $(SRC_DIR) -name '*.cpp')
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))
OBJS_DEBUG = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR_DEBUG)/%.o, $(SRCS))
NAME = tetris
NAME_DEBUG = tetris_debug

.PHONY: all clean run debug run_debug

all: $(NAME)

debug: $(NAME_DEBUG)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

$(NAME_DEBUG): $(OBJS_DEBUG)
	$(CXX) $(CXXFLAGS_DEBUG) $^ -o $@ $(LDFLAGS_DEBUG)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR_DEBUG)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR_DEBUG)
	$(CXX) $(CXXFLAGS_DEBUG) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

$(OBJ_DIR_DEBUG):
	mkdir -p $@

clean:
	rm -rf $(OBJ_DIR) $(OBJ_DIR_DEBUG)
	mkdir -p $(OBJ_DIR) $(OBJ_DIR_DEBUG)

fclean:
	rm -rf $(OBJ_DIR) $(OBJ_DIR_DEBUG) $(NAME) $(NAME_DEBUG)
	mkdir -p $(OBJ_DIR) $(OBJ_DIR_DEBUG)
	@echo "Cleaned up build files."

re: fclean all
	@echo "Rebuilt the project."

run: $(NAME)
	./$(NAME)

run_debug: $(NAME_DEBUG)
	./$(NAME_DEBUG)
