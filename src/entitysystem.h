#ifndef _NTR_ENTITY_SYSTEM_H_
#define _NTR_ENTITY_SYSTEM_H_ 1
#include <SDL2/SDL.h>

typedef struct {
	int x,y;
} Pos;

enum EnityType {
	ET_HAS_HEALTH = 1,
	ET_ENEMY = 2, // follows player

	ET_DEFAULT = ET_HAS_HEALTH
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

