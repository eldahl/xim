OUT_DIR=build
INCLUDE=-I $(CMAKE_INCLUDE_PATH)
LIB=-L $(CMAKE_LIBRARY_PATH)
CPP_V=-std=c++11 -stdlib=libc++
LIB_ARGS=-lSDL2 -lX11
CC=clang
CXX=clang++

xim:
	mkdir -p build
	$(CXX) $(INCLUDE) $(CPP_V) -c -o $(OUT_DIR)/tfd.o src/tinyfiledialogs.c
	$(CXX) $(INCLUDE) $(CPP_V) -c -o $(OUT_DIR)/window.o src/xim_window.cpp
	$(CXX) $(INCLUDE) $(CPP_V) -c -o $(OUT_DIR)/xim.o src/xim.cpp
	
	cd $(OUT_DIR); $(CXX) $(CPP-V) tfd.o window.o xim.o -o xim $(LIB) $(LIB_ARGS) -fPIC
