#ifndef _NTR_GRAPH_H_
#define _NTR_GRAPH_H_ 1
#include <SDL2/SDL.h>

typedef struct {
	int x,y;
} Pos;
#define POS(x, y) ((Pos) { x,y })

#define TILE_SZ (70)

extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern int width, height;

void graphInit(void);
void graphDestroy(void);
SDL_Texture *textureLoad(const char *fname);

#endif // !_NTR_GRAPH_H_

