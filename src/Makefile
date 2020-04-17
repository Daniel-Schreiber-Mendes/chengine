

VERSION = -std=c99
ERRORFLAGS = -Wall -Wuninitialized -Werror=implicit-function-declaration -Wextra -Wno-unused-parameter -Wno-incompatible-pointer-types -Werror=int-conversion
LFLAGS = -shared -o
LIBS = -lGL -lGLEW -lglfw -lm -lchecl -lchecs -lcheio
CFLAGS = $(ERRORFLAGS) $(VERSION) -c -g -fpic
OBJFILES = main.o stateMachine.o math.o utility.o vertexArray.o buffer.o program.o texture.o


make: $(OBJFILES)
	gcc $(LFLAGS) libchengine-dev.so $(OBJFILES) $(LIBS)
	mv libchengine-dev.so /usr/local/lib/
	cp ../*.h /usr/local/include/chengine-dev/

profile:
	gprof output gmon.out > profile.txt
	gedit profile.txt

cachecheck: output
	valgrind --tool=cachegrind make

memcheck: output
	valgrind --tool=memcheck --leak-check=full make
	
texture.o: resources/texture.c
	gcc $(CFLAGS) resources/texture.c
	
stateMachine.o: stateMachine.c 
	gcc $(CFLAGS) stateMachine.c

vertexArray.o: render/vertexArray.c
	gcc $(CFLAGS) render/vertexArray.c	

buffer.o: render/buffer.c
	gcc $(CFLAGS) render/buffer.c

program.o: render/program.c
	gcc $(CFLAGS) render/program.c	

math.o: utility/math.c
	gcc $(CFLAGS) utility/math.c

utility.o: utility/utility.c
	gcc $(CFLAGS) utility/utility.c

main.o: main.c
	gcc $(CFLAGS) main.c
