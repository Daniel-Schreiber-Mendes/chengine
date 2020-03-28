VERSION = -std=c99
ERRORFLAGS = -Wall -Wuninitialized -Werror=implicit-function-declaration -Wextra -Werror=int-conversion
LFLAGS = -o
LIBS = `pkg-config --libs gtk+-3.0`
CFLAGS = $(ERRORFLAGS) $(VERSION) -c -g `pkg-config --cflags gtk+-3.0`
OBJFILES = main.o 


make: OUTPUT
	./output

profile:
	gprof output gmon.out > profile.txt
	gedit profile.txt

cachecheck: output
	valgrind --tool=cachegrind make

memcheck: output
	valgrind --tool=memcheck --leak-check=full make

OUTPUT: 		         main.o
	gcc $(LFLAGS) output main.o $(LIBS)

main.o: main.c
	gcc $(CFLAGS) main.c