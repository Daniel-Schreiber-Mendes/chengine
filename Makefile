VERSION = -std=c99
ERRORFLAGS = -Wall -Wuninitialized -Werror=implicit-function-declaration -Wextra -Werror=int-conversion -Wno-unused-function -Wno-unused-parameter
LFLAGS = -o
LIBS =  -lcheio -lchecl `pkg-config --libs gtksourceview-3.0`
CFLAGS = $(ERRORFLAGS) $(VERSION) -c -g `pkg-config --cflags gtk+-3.0` `pkg-config --cflags gtksourceview-3.0`
OBJFILES = main.o settings.o components.o


make: OUTPUT
	./output

OUTPUT: $(OBJFILES)
	gcc $(LFLAGS) output $(OBJFILES) $(LIBS)

main.o: main.c
	gcc $(CFLAGS) main.c

settings.o: settings.c
	gcc $(CFLAGS) settings.c

components.o: components.c
	gcc $(CFLAGS) components.c