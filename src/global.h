#ifndef _NTR_GLOBALL_H_
#define _NTR_GLOBALL_H_ 1
#include "entitysystem.h"

#define NTH(ARR, N, Y, X) (ARR[N*Y + X])
extern _Noreturn void cleanUp(int status);
extern Entity player;

#endif

