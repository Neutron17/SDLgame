#ifndef _NTR_GLOBAL_H_
#define _NTR_GLOBAL_H_ 1

#include <stdbool.h>

/** get rid of pointer arithmetic warnings
 *  A + B 			*/
#define PTR_ARITH_WARN(A, B) (void *)((long)A + (long)B)
/** A */
#define N_TO_PTR_WARN(A) ((void *)(long)(A))

#endif /* ifndef _NTR_GLOBAL_H_ */
