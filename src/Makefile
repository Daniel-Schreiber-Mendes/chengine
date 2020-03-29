

VERSION = -std=c99
ERRORFLAGS = -Wall -Wuninitialized -Werror=implicit-function-declaration -Wextra -Wno-unused-parameter -Wno-incompatible-pointer-types -Werror=int-conversion
LFLAGS = -o
LIBS = -lGL -lGLEW -lglfw -lm -lchecl -lchecs -lopenal
CFLAGS = $(ERRORFLAGS) $(VERSION) -c -g
OBJFILES = main.o window.o stateMachine.o mainMenuState.o math.o utility.o vertexArray.o buffer.o program.o texture.o io.o render_system.o input_task.o wav_loader.o components.o chunk_loading_system.o physics_system.o

make: OUTPUT
	./output

profile:
	gprof output gmon.out > profile.txt
	gedit profile.txt

cachecheck: output
	valgrind --tool=cachegrind make

memcheck: output
	valgrind --tool=memcheck --leak-check=full make

OUTPUT: 		         $(OBJFILES)
	gcc $(LFLAGS) output $(OBJFILES) $(LIBS)

window.o: window/window.c
	gcc $(CFLAGS) window/window.c
	
texture.o: resources/texture.c
	gcc $(CFLAGS) resources/texture.c

io.o: io/io.c
	gcc $(CFLAGS) io/io.c
	
stateMachine.o: state/stateMachine.c 
	gcc $(CFLAGS) state/stateMachine.c

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

wav_loader.o: audio/wav_loader.c
	gcc $(CFLAGS) audio/wav_loader.c

components.o: components/components.c
	gcc $(CFLAGS) components/components.c

main.o: main.c
	gcc $(CFLAGS) main.c

############################# custom ###############################################################################################

mainMenuState.o: custom/mainMenuState.c
	gcc $(CFLAGS) custom/mainMenuState.c

input_task.o: custom/input_task.c
	gcc $(CFLAGS) custom/input_task.c

########################### extensions ##################################################################################################
chunk_loading_system.o: tiles_chext/chunk_loading_system.c
	gcc $(CFLAGS) tiles_chext/chunk_loading_system.c

physics_system.o: physics_chext/physics_system.c
	gcc $(CFLAGS) physics_chext/physics_system.c