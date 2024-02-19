#include "tilemap.h"
#include "base/log.h"
#include "base/exitCodes.h"
#include "global.h"
#include "graph.h"
#include "entitysystem.h"

/** [x,y] 
 * 1x1 example:
 * [[[-1,-1],[0,-1],[1,-1]],[[-1,0],[0,0],[1,0]],[[-1,1],[0,1],[1,1]]] */
Pos *distance;

void tilemapDraw(int n, SDL_Texture *tile) {
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

void distanceInit(int n) {
	const int dim = (2*n+1)*(2*n+1);
	distance = malloc(sizeof(Pos) * dim);
	if(!distance) {
		LOGF(L_ERR, "Couldn't allocate for distance");
		cleanUp(E_ALLOC);
	}
	for(int i = -n; i <= n; i++)
		for(int j = -n; j <= n; j++)
			distance[(2*n+1)*(i+n) + j+n] = POS(TILE_SZ*j, TILE_SZ*i);
}


