

VERSION = -std=c99
ERRORFLAGS = -Wall -Wuninitialized -Werror=implicit-function-declaration -Wextra -Wno-unused-parameter -Wno-incompatible-pointer-types -Werror=int-conversion
LFLAGS = -shared -o
LIBS = -lGL -lGLEW -lglfw -lm -lchecl -lchecs -lopenal -lcheio
CFLAGS = $(ERRORFLAGS) $(VERSION) -c -g -fpic
OBJFILES = main.o window.o stateMachine.o math.o utility.o vertexArray.o buffer.o program.o texture.o render_system.o wav_loader.o components.o


make: $(OBJFILES)
	gcc $(LFLAGS) libchengine-dev.so $(OBJFILES) $(LIBS)

lib-update:
	mv libchengine-dev.so /usr/local/lib/
	cp ../*.h /usr/local/include/chengine-dev/

profile:
	gprof output gmon.out > profile.txt
	gedit profile.txt

cachecheck: output
	valgrind --tool=cachegrind make

memcheck: output
	valgrind --tool=memcheck --leak-check=full make

window.o: window.c
	gcc $(CFLAGS) window.c
	
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

render_system.o: render/render_system.c
	gcc $(CFLAGS) render/render_system.c

math.o: utility/math.c
	gcc $(CFLAGS) utility/math.c

utility.o: utility/utility.c
	gcc $(CFLAGS) utility/utility.c

wav_loader.o: wav_loader.c
	gcc $(CFLAGS) wav_loader.c

components.o: components.c
	gcc $(CFLAGS) components.c

main.o: main.c
	gcc $(CFLAGS) main.c