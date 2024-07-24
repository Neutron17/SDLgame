#include "movement.h"
#include "base/arr.h"
#include "entitysystem.h"
#include "tile.h"
#include <SDL2/SDL_stdinc.h>

struct MovePair {
	Entity *e;
	movementCallbackFn cb;
};
// array of MovePairs
static Array_t movableEntityMap;

void movementInit() {
	UNWRAP_TO_COMPLEX(array_init(struct MovePair, 3), movableEntityMap, Array_t);
}
void movementDestroy() {
	array_destroy(&movableEntityMap);
}
/** Binds callback to entity, not thread safe */
void movementBindCb(Entity *entity, movementCallbackFn cb) {
	const struct MovePair pair = { entity, cb };
	array_push(&movableEntityMap, &pair);
	entity->_moveID = movableEntityMap.used;
}

void movementUnbind(Entity entity) {
	if(!entity._moveID)
		return;
	array_remove(&movableEntityMap, entity._moveID-1);
}

static const Uint8 *states;
static void movementForEach(void *data) {
	struct MovePair pair = *(struct MovePair *)data;
	if(!pair.e)
		return;
	PosF *p = (PosF *)pair.e;
	const Pos pFloor = { floor(p->x), floor(p->y) };
	//printf("(%d %d)\n", p->x, p->y);
	*p = pair.cb(pair.e, states, tilePropFromPos(pFloor));
}

/** Calls the movement callback on each bound entity */
void movementMoveAll() {
	states = SDL_GetKeyboardState(NULL);
	array_for_each(movableEntityMap, movementForEach);
}

