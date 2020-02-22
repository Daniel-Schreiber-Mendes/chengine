

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

OUTPUT: 		         main.o window.o stateMachine.o MainMenuState.o math.o utility.o vertexArray.o vertexBuffer.o elementBuffer.o program.o vertexBufferLayout.o texture.o file.o commands.o render_system.o
	gcc $(LFLAGS) output main.o window.o stateMachine.o MainMenuState.o math.o utility.o vertexArray.o vertexBuffer.o elementBuffer.o program.o vertexBufferLayout.o texture.o file.o commands.o render_system.o -lGL -lGLEW -lglfw -lm -lchecl -lchecs -lcimgui

main.o: main.c
	gcc $(CFLAGS) main.c

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

math.o: utility/math.c
	gcc $(CFLAGS) utility/math.c

utility.o: utility/utility.c
	gcc $(CFLAGS) utility/utility.c
	
MainMenuState.o: state/MainMenuState.c
	gcc $(CFLAGS) state/MainMenuState.c

commands.o: commands/commands.o
	gcc $(CFLAGS) commands/commands.c