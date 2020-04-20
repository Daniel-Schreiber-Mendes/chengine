VERSION = -std=c99
ERRORFLAGS = -Wall -Wuninitialized -Werror=implicit-function-declaration -Wextra -Werror=int-conversion -Wno-unused-function -Wno-unused-parameter
LFLAGS = -o
LIBS =
CFLAGS = $(ERRORFLAGS) $(VERSION) -c -g
OBJFILES = main.o

compile: $(OBJFILES)
	gcc $(LFLAGS) che $(OBJFILES) $(LIBS)

main.o: main.c
	gcc $(CFLAGS) main.c

setup:
	sudo mv che /usr/bin


