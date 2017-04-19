OBJ_DIR=obj
SRC_DIR=src
LIB_DIR=lib
INC_DIR=include
CXXFLAGS=-g -fPIC -std=c++11 -Iinclude -Wunused-variable -Wall -Wpedantic
CXX=g++

OBJS=ConfReader.o Brew.o LaTeXExporter.o Constants.o
DEPS=$(addprefix $(INC_DIR)/,Components.hpp Constants.hpp)

all: mbrew

$(LIB_DIR):
	mkdir -p $(LIB_DIR)
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(INC_DIR)/%.hpp $(DEPS) | $(OBJ_DIR)
	$(CXX) -c -o $@ $< $(CXXFLAGS) 

$(LIB_DIR)/libmbrew.so: $(addprefix $(OBJ_DIR)/,$(OBJS)) | $(LIB_DIR)
	$(CXX) -shared $^ -o $@ $(CXXFLAGS)

mbrew: $(SRC_DIR)/mbrew.cpp $(LIB_DIR)/libmbrew.so
	$(CXX) -o $@ $^ $(CXXFLAGS)

clean:
	rm -rf $(OBJ_DIR) $(LIB_DIR)
	rm -f mbrew
