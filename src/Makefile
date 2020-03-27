

VERSION = -std=c99

ERRORS = -Wall -Wuninitialized -Werror=implicit-function-declaration -Wextra -Wno-unused-parameter -Wno-incompatible-pointer-types -Werror=int-conversion

#flags for linking
LFLAGS = -o

#flags for compilation
CFLAGS = $(ERRORS) $(VERSION) -c -g

make: OUTPUT
	./output

profile:
	gprof output gmon.out > profile.txt
	gedit profile.txt

cachecheck: output
	valgrind --tool=cachegrind make

memcheck: output
	valgrind --tool=memcheck --leak-check=full make

OUTPUT: 		         main.o window.o stateMachine.o mainMenuState.o math.o utility.o vertexArray.o buffer.o program.o texture.o file.o render_system.o input_task.o wav_loader.o components.o chunk_loading_system.o physics_system.o config.o
	gcc $(LFLAGS) output main.o window.o stateMachine.o mainMenuState.o math.o utility.o vertexArray.o buffer.o program.o texture.o file.o render_system.o input_task.o wav_loader.o components.o chunk_loading_system.o physics_system.o config.o -lGL -lGLEW -lglfw -lm -lchecl -lchecs -lopenal #-lcimgui

window.o: window/window.c
	gcc $(CFLAGS) window/window.c
	
texture.o: resources/texture.c
	gcc $(CFLAGS) resources/texture.c

file.o: resources/file.c
	gcc $(CFLAGS) resources/file.c
	
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

config.o: config/config.c
	gcc $(CFLAGS) config/config.c

############################# custom ###############################################################################################

	
mainMenuState.o: custom/mainMenuState.c
	gcc $(CFLAGS) custom/mainMenuState.c

main.o: custom/main.c
	gcc $(CFLAGS) custom/main.c

input_task.o: custom/input_task.c
	gcc $(CFLAGS) custom/input_task.c


########################### extensions ##################################################################################################
chunk_loading_system.o: tiles_chext/chunk_loading_system.c
	gcc $(CFLAGS) tiles_chext/chunk_loading_system.c

physics_system.o: physics_chext/physics_system.c
	gcc $(CFLAGS) physics_chext/physics_system.c