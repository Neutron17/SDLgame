#ifndef _NTR_TILE_H_
#define _NTR_TILE_H_ 1
#include <SDL2/SDL_render.h>
#include "graph.h"

// Uniquely defined in each level
typedef int TileType;

typedef struct {
	float speed_mul;
	int dmg_p_sec;
} TileProp;

typedef struct {
	SDL_Texture **textures;
	TileProp *properties;
	int len;
} TileEnv;

void tilemapSet(TileEnv _env, int *_tiles, int x, int y);
TileProp tilePropFromPos(Pos pos);

void tilemapDraw();
void distanceInit(int n);

#endif /* ifndef _NTR_TILE_H_ */

