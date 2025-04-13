CXX = g++
TARGET = vmfoptimizer
SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:src/%.cpp=build/%.o)
CXXFLAGS = -Wall -Wextra -O2

$(TARGET): build $(OBJ)
	$(CXX) $(OBJ) -o $@

build/%.o: src/%.cpp | build
	$(CXX) $(CXXFLAGS) -c $< -o $@ -Iincludes

build:
	mkdir -p build

clean:
	rm -rf build $(TARGET)

.PHONY: clean build
