#ifndef _NTR_INPUT_H_
#define _NTR_INPUT_H_ 1
#include "graph.h"
#include <SDL2/SDL_events.h>

#define SPEED (15)

void input(SDL_Event event);
void mouseFollow(Pos dest);

#endif // !_NTR_INPUT_H_

