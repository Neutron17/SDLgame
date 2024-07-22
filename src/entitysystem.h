#ifndef _NTR_ENTITY_SYSTEM_H_
#define _NTR_ENTITY_SYSTEM_H_ 1
#include <SDL2/SDL.h>

enum EnityType {
	ET_HAS_HEALTH 	= 1 << 0,
	ET_ENEMY	= 1 << 1, // follows player
	ET_COLLIDE 	= 1 << 2,
	ET_STATIC 	= 1 << 3,

	ET_DEFAULT = ET_HAS_HEALTH | ET_COLLIDE
};

struct EntityState {
	enum EntityStateType {
		EST_PLAYER
	} type;
	union {
		struct PlayerState {
			int health;
		} player;
	} state;
};

typedef struct {
	int x,y;
	int w,h;
	unsigned type; // EntityType
	SDL_Texture *texture;
	int _id;
} Entity;

void entitysystemInit();
void entitysystemDestroy();
void entitysystemAdd(Entity *e);
void entitysystemRemove(Entity *e);
Entity _entity(int x, int y, int w, int h, int type, SDL_Texture *texture);
#define entity(X, Y, W, H, T) _entity(X,Y,W,H,ET_DEFAULT,T)

#endif // _NTR_ENTITY_SYSTEM_H_

