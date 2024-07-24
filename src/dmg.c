#include "dmg.h"
#include "entitysystem.h"

void dmgResolve(Entity *src, Entity *target, Dmg dmg) {
	//printf("Resolving %d\n", dmg.value);
	target->state.health -= dmg.value - dmg.value*target->state.shield_percent;
	if(target->state.health < 0)
		entitysystemRemove(target);
	// TODO: handle death
	if(!src)
		return;
	if(src->type & ET_THORNS)
		src->state.health -= dmg.value*target->state.shield_percent*src->state.shield_percent;
}

