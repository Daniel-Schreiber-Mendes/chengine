SRC = modules/mod/file1.c
OBJ = $(SRC:.c=.o)
LIBS =
EXE = test
VER = -std=c99
MODE = -g
OPT = -O0
ERR = -Wall -Wuninitialized -Werror=implicit-function-declaration -Wextra -Wno-unused-parameter -Wno-incompatible-pointer-types -Werror=int-conversion
LFLAGS = -o
CFLAGS = $(ERR) $(VER) $(OPT) -c $(MODE)

run: $(EXE)
	./$(EXE)

$(EXE): $(OBJ)
	gcc $(LFLAGS) $(EXE) $(OBJ) $(LIBS)

%.o: %.c
	gcc -c $(CFLAGS) $*.c
