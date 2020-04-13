VERSION = -std=c99
ERRORFLAGS = -Wall -Wuninitialized -Werror=implicit-function-declaration -Wextra -Werror=int-conversion -Wno-unused-function -Wno-unused-parameter
LFLAGS = -o
LIBS =  -lcheio -lchecl `pkg-config --libs gtksourceview-3.0`
CFLAGS = $(ERRORFLAGS) $(VERSION) -c -g `pkg-config --cflags gtk+-3.0` `pkg-config --cflags gtksourceview-3.0`
OBJFILES = main.o settings.o components.o



all: CHENGINE
	./chengine

CHENGINE: $(OBJFILES)
	gcc $(LFLAGS) chengine $(OBJFILES) $(LIBS)

main.o: main.c
	gcc $(CFLAGS) main.c

settings.o: settings.c
	gcc $(CFLAGS) settings.c

components.o: components.c
	gcc $(CFLAGS) components.c


setup:
	sudo mv chengine /usr/bin
	sudo cp install/chengine.desktop /usr/share/applications
	gio set /usr/share/applications/chengine.desktop "metadata::trusted" yes
	sudo cp install/chengine_icon.png /usr/share/pixmaps