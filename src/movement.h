/** -- movement.h --
 * handles movement of entities
 * (player too)
 * */
#ifndef _NTR_MOVE_H_
#define _NTR_MOVE_H_ 1
#include "entitysystem.h"
#include "graph.h"
#include "tile.h"
#include <SDL2/SDL_stdinc.h>

/** returns the new position */
typedef Pos (*movementCallbackFn)(Entity *self, const Uint8 *keyboardState, TileProp tile);

void movementInit();
void movementDestroy();
/** Binds callback to entity, not thread safe */
void movementBindCb(Entity *entity, movementCallbackFn cb);
/** Calls the movement callback on each bound entity */
void movementMoveAll();

#endif /* ifndef _NTR_MOVE_H_ */

