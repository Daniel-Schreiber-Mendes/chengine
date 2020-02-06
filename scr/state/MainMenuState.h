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
		//++p->x;
		//++p->y;
	}
}


void stateMachine_setup(void)
{
	systemManager_init(1, 0, 0, 0);
	entityManager_init();
	componentManager_init(2);

	componentManager_component_register(Position);
	componentManager_component_register(Transform);

	systemManager_system_register(sys, UPDATE, Position);


	for(uintEC i=0; i < 100; ++i)
	{
		entityManager_entity_generate(Position, Transform);
	}

	stateMachine_state_push(MainMenuState, mainMenuState_construct);
}


#endif