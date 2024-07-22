#include <stdlib.h>
#include <pthread.h>

#include "entitysystem.h"
#include "base/arr.h"
#include "base/exitCodes.h"
#include "base/log.h"
#include "base/lt.h"

static Array_t entities;
static int id_counter;
static pthread_mutex_t id_mutex;

void entitysystemInit() {
	UNWRAP_TO_COMPLEX(array_init(Entity, 3), entities, Array_t);
	if(pthread_mutex_init(&id_mutex, NULL)) {
		LOG(L_ERR, "Failed to create entity system mutex");
		cleanUp(E_ALLOC);
	}
}
void entitysystemDestroy() {
	array_destroy(&entities);
	pthread_mutex_destroy(&id_mutex);
}
void entitysystemAdd(Entity *e) {
	pthread_mutex_lock(&id_mutex);
	e->_id = id_counter;
	id_counter++;
	pthread_mutex_unlock(&id_mutex);
	array_push(&entities, e);
}
Entity _entity(Pos p, int w, int h, enum EntityType type, struct EntityState state, SDL_Texture *texture) {
	Entity ret = { p.x, p.y, w,h, type, state,texture };
	entitysystemAdd(&ret);
	return ret;
}

static int remover; // TODO: thread safety
static bool _remove_iter(const void *data) {
	Entity it = *(Entity *)data;
	if(it._id == remover)
		return true;
	return false;
}

void entitysystemRemove(Entity *e) {
	remover = e->_id;
	memset(e, 0, sizeof(*e));
	array_remove_first(&entities, _remove_iter);
}
