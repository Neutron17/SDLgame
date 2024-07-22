#ifndef _NTR_INPUT_H_
#define _NTR_INPUT_H_ 1
#include "graph.h"
#include <SDL2/SDL_events.h>
#include "entitysystem.h"
#include "tile.h"

#define SPEED (15)

// self is not const, bc tiles can damage the entity
Pos movePlayer(Entity *self, const Uint8 *keystates, TileProp tile);
void mouseFollow(Pos dest);

#endif // !_NTR_INPUT_H_

