#include <SDL2/SDL_render.h>
#ifndef _NTR_TILE_H_
#define _NTR_TILE_H_ 1

// Uniquely defined in each level
typedef int TileType;

typedef struct {
	SDL_Texture **textures;
	int len;
} TileEnv;

void tilemapSet(TileEnv _env, int *_tiles, int x, int y);

void tilemapDraw();
void distanceInit(int n);

#endif /* ifndef _NTR_TILE_H_ */

