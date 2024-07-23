/** -- tile.h --
 *  Tilesystem
 * The tilemap is made up of the identifiers of
 * each tile (int). The identifier -1 is not to 
 * be rendered. Identifiers less than -1 result
 * in undefined behaviour. 
 * The identifiers are not uniquely defined, they
 * correspond to the index of the 'textures' and 
 * 'properties' field in the 'TileEnv' struct.
 * 
 * */
#ifndef _NTR_TILE_H_
#define _NTR_TILE_H_ 1
#include <SDL2/SDL_render.h>
#include "graph.h"

/** Tile Properties 
 * represent the attributes of a single tile */
typedef struct {
	/** 0 < speed_mul <= 1
	 * for default, set to 1 */
	float speed_mul;
	// for default, set to 0
	int dmg_p_sec;
} TileProp;

/** Tile Environment
 * Describes the environment in which tiles are interpreted.
 * corresponds to the whole system, NOT a single tile */
typedef struct {
	// 'textures' are not freed by the tilesystem
	SDL_Texture **textures;
	// 'properties' are not freed by the tilesystem
	TileProp *properties;
	int len;
} TileEnv;

void tilemapSet(TileEnv _env, int *_tiles, int x, int y);
TileProp tilePropFromPos(Pos pos);

/** Copies the tile textures to the renderer
 * The tiles are defined with 'tilemapSet()'
 *
 * Note:
 *  'distanceInit()' and 'tilemapSet()' must be called beforehand */
void tilemapDraw();
/** Initializes a private variable required to perform 
 * other operations later on */
void distanceInit(int n);

#endif /* ifndef _NTR_TILE_H_ */

