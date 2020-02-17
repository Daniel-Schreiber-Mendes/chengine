#ifndef COMMANDS_H
#define COMMANDS_H

#include "../state/states.h"



#define WindowCloseCommand 0


#define FramebufferSizeCommand 1
typedef struct
{
	int const width, height;
}
FramebufferSizeData;


#define KeyCommand 2
typedef struct
{
	int const key, scancode, action, mods;
}
KeyData;


#define MouseButtonCommand 3
typedef struct
{
	int const button, action, mods;
}
MouseButtonData;


#define JoystickCommand 4
typedef struct
{
	int const jid, event;
}
JoystickData;

#define COMMAND_COUNT 5


void commands_init(void);

#endif