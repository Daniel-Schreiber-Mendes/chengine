#ifndef CUSTOM_H
#define CUSTOM_H
#include <render/render.h>
#include <window/window.h>
#include <movement/movement.h>
#include <physics/physics.h>
#include <animation/animation.h>
#include <tilemap/tilemap.h>
#include <cglm/cglm.h>
#include <text/text.h>

void mainMenuState_construct(State *const s);
void mainMenuState_destruct(State *const state);
void mainMenuState_update(State *const state);
void mainMenuState_draw(State *const state);
void mainMenuState_events(State *const state);

extern Texture cobble, circle, player_tex, player1_tex, grass;
extern AnimationPattern player_animationPattern;

void game_state_construct(State *s);
void game_state_destruct(State *s);
void game_state_events(State *s);
void game_state_update(State *s);
void game_state_draw(State *s);


#endif