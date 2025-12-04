CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude

SRC_DIR  = src
OBJ_DIR  = obj
BIN      = sistema

SOURCES  = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS  = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))

all: $(BIN)

$(BIN): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(BIN)
	./$(BIN)

clean:
	rm -rf $(OBJ_DIR) $(BIN)
