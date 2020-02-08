#ifndef MAINMENUSTATE
#define MAINMENUSTATE
#include "states.h"
#include "../utility/utility.h"
#include "../resources/resources.h"


typedef struct
{
	State b;

	GLfloat positions[5 * 4];
	GLuint indices[6];

	GLuint vbo, vao, program;
	VertexBufferLayout vbl;
	IndexBuffer ibo;
	Texture tex;

	mat4 view, model, mvp, proj;

}MainMenuState;


void mainMenuState_construct(MainMenuState *const s);
void mainMenuState_destruct(State *const state);
void mainMenuState_update(State *const state);
void mainMenuState_draw(State *const state);


void task(void)
{
	//printf("task\n");
}


#define PositionComponent 0
typedef struct
{
	int x;
	int y;
}Position;

#define TransformComponent 1
typedef struct
{
	int x;
	int y;
}Transform;



void sys(EntityId *const entitys, uintEC const entityCount)
{
	componentManager_component_use(Position, p);
	entityManager_foreach(entity)
	{
		componentManager_component_get(Position, p, entity);
		++p->x;
		++p->y;
	}
}


void stateMachine_setup(void)
{
	entityManager_hints_give(150, 5);
	checs_init(1, 0, 0, 0, 2);

	checs_component_register(Position);
	checs_component_register(Transform);
	
	checs_system_register(sys, UPDATE, Position);

	for(uintEC i=0; i < 150; ++i)
	{
		checs_entity_generate(Position);
	}

	stateMachine_state_push(MainMenuState, mainMenuState_construct);
}


#endif