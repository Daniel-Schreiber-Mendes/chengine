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
	read -r sourcefiles < Makefile
	rm -f modules/$1/*.c.c 
	rm -f modules/$1/*.c.o 
	for file in modules/$1/*.c
	do
		if [[ sourcefiles != *"$file"* ]]
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
	cat << EOF > $1/Makefile
SRC = src/setup.c
HDR = 
OBJ = \$(SRC:.c=.c.o)
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
		rm -r $3
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
		mkdir $3
		mkdir $3/modules
		mkdir $3/states
		mkdir $3/src
		mkdir $3/resources
		mkdir $3/resources/error
		mkdir $3/resources/texture
		mkdir $3/resources/sound
		mkdir $3/resources/shader
		touch $3/src/setup.c
		touch $3/states/states.h
		touch $3/$3.h
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
				sed -i "1s|.*|& $file|" Makefile #deletes first line, should not do that
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
elif [ "$1" = "rename" ]
then
	param_check $2
	if [ "$2" = "project" ]
	then
		awk '/EXE = / {$1 = ${3}} {print $0 > Makefile.tmp}'
		rm Makefile
		mv Makefile.tmp Makfefile
	else
		echo "Unknown rename argument: ${$2}"
	fi
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

		#for file in $files
		#do
		#	if [ $file -ot $file.c ] 
		#	then
				#$files = ${files#${file}}
				#$files = $files
				#echo ${files:s/${file}/}
		#	fi
		#done
		
		#if component is found, write to file its signature otherwise its name and signature
		gawk -v RS='<[^>. ]+>' '{ ORS="" }  
		RT {                                       
		   switch (RT)
		   {
		    	case /E:/:
		    		if (!(RT in events))                    
				    	events[RT] = eventCount++   
				    name=RT    
				    sub(/<E:/, "", name)
				    sub(/>/, "", name)
				    ORS=name ", " events[RT]
				    break
				case /C:/:
		    		if (!(RT in commands))                    
				    	commands[RT] = commandcount++   
				    commandName=RT    
				    sub(/<C:/, "", commandName)
				    sub(/>/, "", commandName)
				    ORS=commandName ", " commands[RT]
				    break
				case /A:/:
		    		if (!(RT in attributes))                    
				    	attributes[RT] = attributecount++   
				    ORS=attributes[RT]
				    break
				case /T:/:
					if (!(RT in templates))     
				    	templates[RT] = templatecount++   
				    ORS=templates[RT]
				    break
				default:
					if (!(RT in components))                    
			      		components[RT] = componentCount++                       
			   		ORS=components[RT] 
		   }                
		}
		{
		   print $0 > (FILENAME ".c")
		}' $files
		make
	else
		echo "Unknown compile argument: ${2}"
	fi
else
	echo "Unkown command: ${1}"
fi