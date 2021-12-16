CPPC = g++
CPP = -std=c++17

build_dir = src

SRC = $(build_dir)/*.cpp
HDR = $(build_dir)/*.h
OBJ = *.o
GCH = $(build_dir)/*.gch

TARGET = main

WFLAGS = -Wall -Wextra -Wshadow -Wformat=2 -Wconversion -Wlogical-op -Wshift-overflow=2 -Wduplicated-cond -Wfloat-equal
DFLAGS = -D_GLIBCXX_ASSERTIONS -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -D_FORTIFY_SOURCE=2 -fno-sanitize-recover -fstack-protector -DDEBUG -ggdb3 -fsanitize=address,undefined -fmax-errors=2
OFLAGS = -Og -g -Ofast -pedantic

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CPPC) $(WFLAGS) $(DFLAGS) $(OFLAGS) $(CPP) $(OBJ) -o $(TARGET)

$(OBJ): $(SRC) $(HDR)
	$(CPPC) $(WFLAGS) $(DFLAGS) $(OFLAGS) $(CPP) -c $(SRC) $(HDR)

clean:
	rm -rf $(OBJ) $(GCH) *.gch
