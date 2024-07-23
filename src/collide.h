#ifndef _NTR_COLLIDE_H_
#define _NTR_COLLIDE_H_ 1
#include "graph.h"
#include "entitysystem.h"

bool doIntesectRect(Pos a, Pos b, Pos a_dim, Pos b_dim);
bool doIntesectCircle(Pos a, Pos b, int a_r, int b_rad);

/** both 'a' and 'b' experience 
 * knockback  of equal magnitude 
 * (mass is not a property of the entitysystem) */
void collisionEllastic(Entity *a, Entity *b);
/** 'a' collides with 'b' but 'b' is super massive
 * so a will snap to edge of b */
void collisionStatic(Entity *a, Entity b);

#endif /* ifndef _NTR_COLLIDE_H_ */

