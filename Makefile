VERSION = -std=c99
LIBS = 
MODE = -g
OPTIMIZATIONS = -O0
ERRORFLAGS = -Wall -Wuninitialized -Werror=implicit-function-declaration -Wextra -Wno-unused-parameter -Wno-incompatible-pointer-types -Werror=int-conversion
LFLAGS = -o
CFLAGS = $(ERRORFLAGS) $(VERSION) $(OPTIMIZATIONS) -c $(MODE)
OBJFILES = 


compile: $(OBJFILES)
gcc $(LFLAGS) test $(OBJFILES) $(LIBS)

run:
./test
