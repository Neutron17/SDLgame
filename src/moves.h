/** -- moves.h --
 * definitions of movement callback functions
 * for the movement system see movement.h
 */
#ifndef _NTR_MOVES_H_
#define _NTR_MOVES_H_ 1

#include "graph.h"
#include "entitysystem.h"
#include "tile.h"

// self is not const, bc tiles can damage the entity
PosF movePlayer(Entity *self, const Uint8 *keystates, TileProp tile);
PosF moveGimp(Entity *self, const Uint8 *keystates, TileProp tile);
void mouseFollow(Pos dest);

#endif /* ifndef _NTR_MOVES_H_ */

