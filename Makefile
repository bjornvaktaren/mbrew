OBJ_DIR=obj
SRC_DIR=src
LIB_DIR=lib
CXX_FLAGS=-std=c++11 -Iinclude -Wunused-variable -Wall -Wpedantic
CXX=g++

OBJS=ConfReader.o mbrew.o

all: mbrew

$(LIB_DIR):
	mkdir -p $(LIB_DIR)
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) -fPIC -c -g $(CXX_FLAGS) -o $@ $<

$(LIB_DIR)/libmbrew.so: $(addprefix $(OBJ_DIR)/,$(OBJS))  | $(LIB_DIR)
	$(CXX) -shared $^ $(LIBS) -o $@

mbrew: $(LIB_DIR)/libmbrew.so
	$(CXX) -g -O0 -o $@ $^ $(LIBS)
# g++ $(SRC_DIR)/mbrew.cpp -o mbrew $(CXX_FLAGS)


clean:
	rm -f $(OBJ_DIR)/*.o $(LIB_DIR)/*.so mbrew
