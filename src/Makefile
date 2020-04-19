

VERSION = -std=c99
ERRORFLAGS = -Wall -Wuninitialized -Werror=implicit-function-declaration -Wextra -Wno-unused-parameter -Wno-incompatible-pointer-types -Werror=int-conversion
LFLAGS = -shared -o
CFLAGS = $(ERRORFLAGS) $(VERSION) -c -g -fpic
OBJFILES = main.o stateMachine.o math.o utility.o


make: $(OBJFILES)
	gcc $(LFLAGS) libchengine-dev.so $(OBJFILES)
	mv libchengine-dev.so /usr/local/lib/
	cp ../*.h /usr/local/include/chengine-dev/

stateMachine.o: stateMachine.c 
	gcc $(CFLAGS) stateMachine.c

math.o: math.c
	gcc $(CFLAGS) math.c

utility.o: utility.c
	gcc $(CFLAGS) utility.c

main.o: main.c
	gcc $(CFLAGS) main.c
