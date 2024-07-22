#include "entitysystem.h"
#ifndef _NTR_DMG_H_
#define _NTR_DMG_H_ 1

enum DmgType {
	DMG_MELEE,
	DMG_RANGED,
	DMG_TILE,
	DMG_EFFECT
};

typedef struct {
	enum DmgType type;
	int value;
} Dmg;

// TODO: call when collision accours
void dmgResolve(Entity *src, Entity *target, Dmg dmg);

#endif /* ifndef _NTR_DMG_H_ */

