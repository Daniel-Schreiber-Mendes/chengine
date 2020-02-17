

#flags for linking
LFLAGS = -pg -o

#flags for compilation
CFLAGS = -c -pg -g -std=c99

make: OUTPUT
	./output

profile:
	gprof output gmon.out
	rm gmon.out

profileCache: output
	valgrind --tool=cachegrind make
	rm *.out.*
	rm gmon.out

OUTPUT: 		         main.o window.o stateMachine.o MainMenuState.o math.o utility.o vertexArray.o vertexBuffer.o indexBuffer.o program.o vertexBufferLayout.o renderer.o texture.o file.o commands.o
	gcc $(LFLAGS) output main.o window.o stateMachine.o MainMenuState.o math.o utility.o vertexArray.o vertexBuffer.o indexBuffer.o program.o vertexBufferLayout.o renderer.o texture.o file.o commands.o -lGL -lGLU -lglut -lGLEW -lglfw -lm -lchecl -lchecs

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

vertexArray.o: utility/vertexArray.c
	gcc $(CFLAGS) utility/vertexArray.c	

vertexBuffer.o: utility/vertexBuffer.c
	gcc $(CFLAGS) utility/vertexBuffer.c

indexBuffer.o: utility/indexBuffer.c
	gcc $(CFLAGS) utility/indexBuffer.c

program.o: utility/program.c
	gcc $(CFLAGS) utility/program.c

renderer.o: utility/renderer.c
	gcc $(CFLAGS) utility/renderer.c

vertexBufferLayout.o: utility/vertexBufferLayout.c	
	gcc $(CFLAGS) utility/vertexBufferLayout.c	

math.o: utility/math.c
	gcc $(CFLAGS) utility/math.c

utility.o: utility/utility.c
	gcc $(CFLAGS) utility/utility.c
	
MainMenuState.o: state/MainMenuState.c
	gcc $(CFLAGS) state/MainMenuState.c

commands.o: commands/commands.o
	gcc $(CFLAGS) commands/commands.c