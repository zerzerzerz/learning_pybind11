# MODULE=cpp_module_basic
# MODULE=cpp_module_stl
# MODULE=cpp_module_numpy
# MODULE=cpp_module_tree
MODULE=cpp_module_inheritance

SRC_DIR=src
SOURCES=$(wildcard $(SRC_DIR)/$(MODULE)/*.cpp)
HEADER=$(wildcard $(SRC_DIR)/$(MODULE)/*.h)
SUFFIX=$(shell python3-config --extension-suffix)
TARGET=$(MODULE)$(SUFFIX)
INCLUDES=$(shell (python3 -m pybind11 --includes))
LDFLAGS=-O3 -Wall -shared -std=c++11 -fPIC

CC=g++

all: $(TARGET)


$(TARGET): $(SOURCES) $(HEADER)
	$(CC) -o $(TARGET) $(LDFLAGS) $(INCLUDES) $(SOURCES) 


.PHONY: clean
clean:
	if ls *$(SUFFIX) > /dev/null 2>&1; then rm *$(SUFFIX); fi