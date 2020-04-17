VERSION = -std=c99
LIBS = 
MODE = -g
OPTIMIZATIONS = -O0
ERRORFLAGS = -Wall -Wuninitialized -Werror=implicit-function-declaration -Wextra -Wno-unused-parameter -Wno-incompatible-pointer-types -Werror=int-conversion
LFLAGS = -o
CFLAGS = $(ERRORFLAGS) $(VERSION) $(OPTIMIZATIONS) -c $(MODE)
OBJFILES = mainMenuState.o input_task.o physics_system.o chunk_loading_system.o render_system.o movement_task.o camera.o


compile: $(OBJFILES)
	gcc $(LFLAGS) application $(OBJFILES) -lchengine-dev -lGL -lGLEW -lglfw -lm -lchecl -lchecs -lopenal $(LIBS)

run:
	./application

profile:
	gprof application gmon.out > profile.txt
	gedit profile.txt

cachecheck: application
	valgrind --tool=cachegrind make

memcheck: application
	valgrind --tool=memcheck --leak-check=full make

	
mainMenuState.o: mainMenuState.c
	gcc $(CFLAGS) mainMenuState.c

input_task.o: input_task.c
	gcc $(CFLAGS) input_task.c

########################### extensions ##################################################################################################

chunk_loading_system.o: tiles_ext/chunk_loading_system.c
	gcc $(CFLAGS) tiles_ext/chunk_loading_system.c

physics_system.o: physics_ext/physics_system.c
	gcc $(CFLAGS) physics_ext/physics_system.c

render_system.o: render_ext/render_system.c
	gcc $(CFLAGS) render_ext/render_system.c

movement_task.o: movement_ext/movement_task.c
	gcc $(CFLAGS) movement_ext/movement_task.c

camera.o: camera_ext/camera.c
	gcc $(CFLAGS) camera_ext/camera.c