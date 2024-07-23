/* -- entitysystem.h -- 
 * creates and destroys entities
 * all other tasks are delegated elsewhere
 * ex: entity damage is handled by dmg.h/.c
 * */
#ifndef _NTR_ENTITY_SYSTEM_H_
#define _NTR_ENTITY_SYSTEM_H_ 1
#include <SDL2/SDL.h>
#include "graph.h"

enum EntityType {
	ET_NO_STATE 	= 0,
	ET_HAS_HEALTH 	= 1 << 0,
	ET_ENEMY	= 1 << 1, // follows player
	ET_COLLIDE 	= 1 << 2,
	ET_STATIC 	= 1 << 3,
	ET_THORNS 	= 1 << 4,

	ET_DEFAULT = ET_HAS_HEALTH | ET_COLLIDE
};

/** Extends some types in 'enum EntityType' with values */
struct EntityState {
	int health;
	float thorn_percent;
	float shield_percent;
};

/**
 * Note: can be casted to 'Pos' */
typedef struct {
	int x,y;
	int w,h;
	enum EntityType type;
	struct EntityState state;
	SDL_Texture *texture;
	int _id;
} Entity;

void entitysystemInit();
void entitysystemDestroy();
void entitysystemAdd(Entity *e);
void entitysystemRemove(Entity *e);
Entity _entity(Pos pos, int w, int h, enum EntityType type, struct EntityState state, SDL_Texture *texture);
#define entity(P, W, H, S, T) _entity(P,W,H,ET_DEFAULT,S,T)

#endif // _NTR_ENTITY_SYSTEM_H_

