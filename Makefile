SRC := $(shell find ./modules ./src -name '*.c.c') $(shell find ./libs -name '*.c')
OBJ = $(SRC:.c=.o)
LIBS = -lchecl -lchecs -lglfw -lm -lGL -lGLEW -lopenal -lfreetype -lezd  -lm
EXE = executable
VER = -std=c99
OPT = -O0
ERR = -Wall -Wuninitialized -Werror=implicit-function-declaration -Wextra -Wno-unused-parameter -Wno-incompatible-pointer-types -Werror=int-conversion -Wduplicated-cond -Wlogical-op -Wrestrict -Wnull-dereference -Wjump-misses-init -Wdouble-promotion -Wshadow -Wformat=2 -Wno-unused-function -Wno-discarded-qualifiers -Wno-parentheses
LFLAGS = -o
CFLAGS = $(ERR) $(VER) $(OPT) -c -g -lm -Iinclude -I/usr/include/freetype2 -Ilibs -Imodules -I../../modules `pkg-config --cflags freetype2`


run: $(EXE)
	./$(EXE)

$(EXE): $(OBJ)
	gcc $(LFLAGS) $(EXE) $(OBJ) $(LIBS)

%.o: %.c
	gcc $(CFLAGS) $<
	mv "$$(basename $@)" "$$(dirname $@)"
