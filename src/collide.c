#include "collide.h"

/** both 'a' and 'b' experience 
 * knockback  of equal magnitude 
 * (mass is not a property of the entitysystem) */
void collisionEllastic(Entity *a, Entity *b) {

}
/** 'a' collides with 'b' but 'b' is super massive
 * so a will snap to edge of b */
void collisionStatic(Entity *a, Entity b) {
	a->x = b.x + ((float)b.w+a->w)/2;
	a->y = b.y + ((float)b.h+a->h)/2;
}

