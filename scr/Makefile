

make: OUTPUT
	./output

profile: output
	gprof output gmon.out
	rm gmon.out

OUTPUT: 		  main.o window.o stateMachine.o MainMenuState.o math.o utility.o vertexArray.o vertexBuffer.o indexBuffer.o program.o vertexBufferLayout.o renderer.o texture.o file.o
	gcc -pg -o output main.o window.o stateMachine.o MainMenuState.o math.o utility.o vertexArray.o vertexBuffer.o indexBuffer.o program.o vertexBufferLayout.o renderer.o texture.o file.o -lGL -lGLU -lglut -lGLEW -lglfw -lm -lchecl -lchecs
	
main.o: main.c
	gcc -pg -c main.c

window.o: window/window.c
	gcc -pg -c window/window.c
	
texture.o: resources/texture.c
	gcc -pg -c resources/texture.c

file.o: resources/file.c
	gcc -pg -c resources/file.c
	
stateMachine.o: state/stateMachine.c 
	gcc -pg -c state/stateMachine.c

vertexArray.o: utility/vertexArray.c
	gcc -pg -c utility/vertexArray.c	

vertexBuffer.o: utility/vertexBuffer.c
	gcc -pg -c utility/vertexBuffer.c

indexBuffer.o: utility/indexBuffer.c
	gcc -pg -c utility/indexBuffer.c

program.o: utility/program.c
	gcc -pg -c utility/program.c

renderer.o: utility/renderer.c
	gcc -pg -c utility/renderer.c

vertexBufferLayout.o: utility/vertexBufferLayout.c	
	gcc -pg -c utility/vertexBufferLayout.c	

math.o: utility/math.c
	gcc -pg -c utility/math.c

utility.o: utility/utility.c
	gcc -pg -c utility/utility.c
	
MainMenuState.o: state/MainMenuState.c
	gcc -pg -c state/MainMenuState.c