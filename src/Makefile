

#flags for linking
LFLAGS = -pg -o

#flags for compilation
CFLAGS = -c -pg -g -std=c99

make: OUTPUT
	./output

profile:
	gprof output gmon.out > profile.txt
	gedit profile.txt

profileCache: output
	valgrind --tool=cachegrind make

OUTPUT: 		         main.o window.o stateMachine.o mainMenuState.o math.o utility.o vertexArray.o vertexBuffer.o elementBuffer.o program.o vertexBufferLayout.o texture.o file.o render_system.o inputManager.o input_task.o wav_loader.o
	gcc $(LFLAGS) output main.o window.o stateMachine.o mainMenuState.o math.o utility.o vertexArray.o vertexBuffer.o elementBuffer.o program.o vertexBufferLayout.o texture.o file.o render_system.o inputManager.o input_task.o wav_loader.o -lGL -lGLEW -lglfw -lm -lchecl -lchecs #-lcimgui

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

vertexBuffer.o: render/vertexBuffer.c
	gcc $(CFLAGS) render/vertexBuffer.c

elementBuffer.o: render/elementBuffer.c
	gcc $(CFLAGS) render/elementBuffer.c

program.o: render/program.c
	gcc $(CFLAGS) render/program.c

vertexBufferLayout.o: render/vertexBufferLayout.c	
	gcc $(CFLAGS) render/vertexBufferLayout.c	

render_system.o: render/render_system.c
	gcc $(CFLAGS) render/render_system.c

inputManager.o: input/inputManager.c
	gcc $(CFLAGS) input/inputManager.c

math.o: utility/math.c
	gcc $(CFLAGS) utility/math.c

utility.o: utility/utility.c
	gcc $(CFLAGS) utility/utility.c

wav_loader.o: audio/wav_loader.c
	gcc $(CFLAGS) audio/wav_loader.c

############################# custom ###############################################################################################

	
mainMenuState.o: custom/mainMenuState.c
	gcc $(CFLAGS) custom/mainMenuState.c

main.o: custom/main.c
	gcc $(CFLAGS) custom/main.c

input_task.o: custom/input_task.c
	gcc $(CFLAGS) custom/input_task.c