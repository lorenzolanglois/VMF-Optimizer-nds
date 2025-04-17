CXX ?= g++
TARGET ?= vmfoptimizer
BUILD_DIR = build/$(TARGET)
SRC = $(wildcard src/*.cpp)
OBJ = $(patsubst src/%.cpp, $(BUILD_DIR)/%.o, $(SRC))
DEP = $(OBJ:.o=.d)

CXXFLAGS ?= -Wall -Wextra -O2 -flto -DNDEBUG -Iincludes -MMD -MP -std=c++20
LDPLATEFORMSFLAGS ?=
LDFLAGS ?= -flto

.PHONY: all clean windows

all: $(TARGET)

cross: $(TARGET) windows

$(TARGET): $(OBJ)
	@echo Linking $@
	$(CXX) $(OBJ) -o $@ $(LDFLAGS) $(LDPLATEFORMSFLAGS)

$(BUILD_DIR)/%.o: src/%.cpp
	@echo Compiling $<
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

-include $(DEP)

windows:
	@$(MAKE) --no-print-directory CXX="x86_64-w64-mingw32-g++" TARGET=$(TARGET).exe LDPLATEFORMSFLAGS="-static -s"

clean:
	rm -rf build $(TARGET) $(TARGET).exe
