CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11
LDFLAGS = -lSDL2 -lSDL2_ttf

SRC_DIR = src
OBJ_DIR = obj

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))
TARGET = tetris

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

clean:
	rm -rf $(OBJ_DIR) $(TARGET)

fclean:
	rm -rf $(OBJ_DIR) $(TARGET)
	mkdir -p $(OBJ_DIR)
	@echo "Cleaned up build files."

re: fclean all
	@echo "Rebuilt the project."

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run fclean re
