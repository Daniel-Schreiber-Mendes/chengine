#!/bin/bash
#version 1.0

param_check () 
{
	if [ -z "$1" ] 
	then
		echo "Not enough arguments given"
		exit
	fi
}


module_update ()
{
	read -r firstline < Makefile
	for file in $1/*.c
	do
		if [[ firstline != *"$file"* ]]
		then
			sed -i "1s|.*|& $file|" Makefile
		fi
	done
	##TODO: remove files that are no longer in module
}


module_remove()
{
	for file in modules/"$1"/*.c
	do 
		sed -e "s|$file||g" -i Makefile
		#replaces all occurances of $file with nothing
	done
	rm -r modules/$1
}


module_header_create()
{
	echo -e "#ifndef ${1^^}_MOD_H\n#define ${1^^}_MOD_H\n\n\n\n#endif" > modules/$1/$1.h 
	#-e makes echo interpret \n as a newline character, otherwise it would just print it into the file
	#^^ makes the string before that fully uppercase
	cp Makefile Makefile_new
	awk -v header=$1.h 
	'/HDR/ { $0 = $0 header " "}
		{print $0 > "Makefile_new"}' Makefile
	rm Makefile
	mv Makefile_new Makefile
}


create_makefile ()
{
	cat << EOF > Makefile
SRC =
HDR = 
OBJ = \$(SRC:.c=.o)
LIBS =
EXE = ${1}
VER = -std=c99
MODE = -g
OPT = -O0 
ERR = -Wall -Wuninitialized -Werror=implicit-function-declaration -Wextra -Wno-unused-parameter -Wno-incompatible-pointer-types -Werror=int-conversion -Wduplicated-cond -Wlogical-op -Wrestrict -Wnull-dereference -Wjump-misses-init -Wdouble-promotion -Wshadow -Wformat=2
LFLAGS = -o
CFLAGS = \$(ERR) \$(VER) \$(OPT) -c \$(MODE)

run: \$(EXE)
	./\$(EXE)

\$(EXE): \$(OBJ)
	gcc \$(LFLAGS) \$(EXE) \$(OBJ) \$(LIBS)

%.o: %.c
	gcc -c \$(CFLAGS) \$*.c
	mv "\$\$(basename \$*.o)" "\$\$(dirname \$*.)"

cleanall:
	rm \$(OBJ)
EOF
}

param_check $2
if [ "$1" = "install" ]
then
	if [ "$2" = "module" ]
	then
		param_check $3
		for file in modules/"$3"/*.c
		do 
			files+=" $file"
		done
		sed -i "1s|.*|&$files|" Makefile
		#| seperators instead of / were used because the variable files contains /
	elif [ "$2" = "che" ]
	then
		sudo cp $0 /usr/bin/che #only used for developement, in end user version file wont copy itself back
		sudo chmod 777 /usr/bin/che
	else
		echo "Unkown install argument: ${2}"
	fi
elif [ "$1" = "remove" ]
then
	if [ "$2" = "module" ]
	then
		param_check $3
		module_remove $3
	elif [ "$2" = "project" ]
	then
		rm -r modules states
		rm Makefile
	elif [ "$2" = "che" ]
	then
		sudo rm /usr/bin/che
	else
		echo "Unkown remove argument: ${2}"
	fi
elif [ "$1" = "create" ]
then
	if [ "$2" = "module" ]
	then
		param_check $3
		if [ -d "modules/$3" ] 
		then
			echo "Module ${3} already exists"
		else
			mkdir modules/$3
			module_header_create $3
		fi

	elif [ "$2" = "project" ]
	then
		param_check $3
		mkdir modules
		mkdir states
		create_makefile $3
	elif [ "$2" = "makefile" ]
	then
		create_makefile $3
	else
		echo "Unkown create argument: ${2}"
	fi
elif [ "$1" = "update" ]
then
	if [ "$2" = "module" ]
	then
		param_check $3
		module_update $3
	elif [ "$2" = "states" ]
	then
		read -r firstline < Makefile
		for file in states/*.c
		do
			if [[ firstline != *"$file"* ]]
			then
				sed -i "1s|.*|& $file|" Makefile
			fi
		done
	elif [ "$2" = "project" ]
	then
		for mod in ./modules/*
		do
			module_update $mod
		done
		che update states
	elif [ "$2" = "cglm" ]
	then
		mkdir build
		cd build
		git clone https://github.com/recp/cglm.git
		sh autogen.sh
		./configure
		make
		make check
		sudo make install
		cd ..
		sudo rm -r build
	else
		echo "Unkown update argument: ${2}"
	fi
#elif [ "$1" = "rename" ]
#then
#	param_check $2
#	if [ "$2" = "project" ]
#	then
#		#TODO: implement it: change EXE in makefile to the given argument
#	else
#		echo "Unknown rename argument: ${$2}"
#	fi
elif [ "$1" = "compile" ]
then
	if [ "$2" = "project" ]
	then
		#ectract filenames from Makefile
		files=$(awk '
		BEGIN {files = ""}
		/SRC = / {
			for (i=3; i<=NF; i++) {
				files = files $i " "
			}
		}
		END { print files }
		' Makefile) 
		echo $files
		#copy files but with ComponentSignatures instead of components
		awk 'FNR == 1 {
		    close(out)
		    out = FILENAME ".c"
		}
		{
		    head = ""
		    tail = $0
		    while ( match(tail,/<[^>.]+>/) ) {
		        tgt = substr(tail,RSTART+1,RLENGTH-2)
		        if ( !(tgt in map) ) {
		            map[tgt] = cnt++
		        }
		        head = head substr(tail,1,RSTART-1) map[tgt]
		        tail = substr(tail,RSTART+RLENGTH)
		    }
		    print head tail > out
		}' states/test.c
		make
	else
		echo "Unknown compile argument: ${2}"
	fi
else
	echo "Unkown command: ${1}"
fi