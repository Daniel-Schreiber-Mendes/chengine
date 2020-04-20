VERSION = -std=c99
LIBS = 
MODE = -g
OPTIMIZATIONS = -O0
ERRORFLAGS = -Wall -Wuninitialized -Werror=implicit-function-declaration -Wextra -Wno-unused-parameter -Wno-incompatible-pointer-types -Werror=int-conversion
LFLAGS = -o
CFLAGS = $(ERRORFLAGS) $(VERSION) $(OPTIMIZATIONS) -c $(MODE)
OBJFILES = mainMenuState.o input_task.o physics_system.o chunk_loading_system.o render_system.o movement_task.o camera.o soundSource.o audio.o wav.o window.o vertexArray.o buffer.o program.o texture.o


compile: $(OBJFILES)
	gcc $(LFLAGS) test $(OBJFILES) -lchengine-dev -lGL -lGLEW -lglfw -lm -lchecl -lchecs -lcheio -lopenal $(LIBS)

run:
	./test

	
mainMenuState.o: mainMenuState.c
	gcc $(CFLAGS) mainMenuState.c

input_task.o: input_task.c
	gcc $(CFLAGS) input_task.c

########################### modules ##################################################################################################

chunk_loading_system.o: modules/std_tiles/chunk_loading_system.c
	gcc $(CFLAGS) modules/std_tiles/chunk_loading_system.c

physics_system.o: modules/std_physics/physics_system.c
	gcc $(CFLAGS) modules/std_physics/physics_system.c

render_system.o: modules/std_render/render_system.c
	gcc $(CFLAGS) modules/std_render/render_system.c

movement_task.o: modules/std_movement/movement_task.c
	gcc $(CFLAGS) modules/std_movement/movement_task.c

camera.o: modules/std_camera/camera.c
	gcc $(CFLAGS) modules/std_camera/camera.c

soundSource.o: modules/std_audio/soundSource.c
	gcc $(CFLAGS) modules/std_audio/soundSource.c

audio.o: modules/std_audio/audio.c
	gcc $(CFLAGS) modules/std_audio/audio.c

wav.o: modules/std_audio/wav.c
	gcc $(CFLAGS) modules/std_audio/wav.c

window.o: modules/std_window/window.c
	gcc $(CFLAGS) modules/std_window/window.c

texture.o: modules/std_gl/texture.c
	gcc $(CFLAGS) modules/std_gl/texture.c

vertexArray.o: modules/std_gl/vertexArray.c
	gcc $(CFLAGS) modules/std_gl/vertexArray.c	

buffer.o: modules/std_gl/buffer.c
	gcc $(CFLAGS) modules/std_gl/buffer.c

program.o: modules/std_gl/program.c
	gcc $(CFLAGS) modules/std_gl/program.c	