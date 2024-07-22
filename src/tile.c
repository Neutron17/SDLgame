#include "tile.h"
#include "base/log.h"
#include "base/exitCodes.h"
#include "global.h"
#include "graph.h"
#include <SDL2/SDL_render.h>

/** [x,y] 
 * 1x1 example:
 * [[[-1,-1],[0,-1],[1,-1]],
 *  [[-1, 0],[0, 0],[1, 0]],
 *  [[-1, 1],[0, 1],[1, 1]]] 
 * freed by graphDestroy */
Pos *distance;
static TileEnv env;
static int n;

// freed in graph.c
int *tiles;
static Pos tiles_dim;

TileProp tilePropFromPos(Pos pos) {
	return env.properties[tiles[tiles_dim.x*(pos.y/TILE_SZ) + pos.x/TILE_SZ]];;
}

void _old_tilemapDraw(int n, SDL_Texture *tile) {
	const Pos playertile = POS(width/2 - (player.x % TILE_SZ), 
				height/2 - (player.y % TILE_SZ));
	SDL_Rect rect = {0,0,TILE_SZ,TILE_SZ};
	for(int i = 0; i < (2*n+1); i++) {
		for(int j = 0; j < (2*n+1); j++) {
			const Pos offs = NTH(distance, (2*n+1), i, j);
			rect.x = playertile.x + offs.x;
			rect.y = playertile.y + offs.y;
			SDL_RenderCopy(renderer, tile, NULL, &rect);
		}
	}
}

void tilemapDraw() {
	for(int i = 0; i < tiles_dim.y; i++) {
		for(int j = 0; j < tiles_dim.x; j++) {
     			const int index = tiles[i*tiles_dim.x + j];
			if(index == -1)
				continue;
			SDL_Texture *tex = env.textures[index];
			SDL_Rect rect = { width/2 - player.x + TILE_SZ*j, height/2 - player.y + TILE_SZ*i, TILE_SZ, TILE_SZ};
			SDL_RenderCopy(renderer, tex, NULL, &rect);
		}
	}
	/*return;
	// top-left corner of the tile the player is on, relative to the screen
	const Pos playertile = POS(width/2 - (player.x % TILE_SZ), 
				height/2 - (player.y % TILE_SZ));
	SDL_Rect rect = {0,0,TILE_SZ,TILE_SZ};
	for(int i = 0; i < (2*n+1); i++) {
		for(int j = 0; j < (2*n+1); j++) {
			const Pos offs = NTH(distance, (2*n+1), i, j);
			rect.x = playertile.x + offs.x;
			rect.y = playertile.y + offs.y;

			printf("HERE\n");
			const Pos relsys = POS(player.x / TILE_SZ, player.y / TILE_SZ);
			const int foo = tiles[tiles_dim.x*relsys.y + relsys.x];
			SDL_Texture *textu = NULL;
			for(int l = 0; l < env.len; l++)
				if(env.types[l] == foo)
					textu = env.textures[l];
			if(!textu)
				continue;
			SDL_RenderCopy(renderer, textu, NULL, &rect);
		}
	}*/

}

void distanceInit(int _n) {
	n = _n;
	const int dim = (2*n+1)*(2*n+1);
	if(!distance)
		distance = malloc(sizeof(Pos) * dim);
	else
		distance = realloc(distance, sizeof(*distance) * dim);

	if(!distance) {
		LOGF(L_ERR, "Couldn't allocate for distance");
		cleanUp(E_ALLOC);
	}
	for(int i = -_n; i <= _n; i++)
		for(int j = -_n; j <= _n; j++)
			distance[(2*_n+1)*(i+_n) + j+_n] = POS(TILE_SZ*j, TILE_SZ*i);
}

void tilemapSet(TileEnv _env, int *_tiles, int x, int y) {
	env = _env;
	tiles = malloc(sizeof(*tiles) * x * y); // freed in graph.c
	memcpy(tiles, _tiles, sizeof(int) * x * y);
	tiles_dim = POS(x, y);
}

