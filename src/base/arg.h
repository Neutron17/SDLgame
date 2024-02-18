#ifndef _NTR_ARG_H_
#define _NTR_ARG_H_ 1
#include <stdbool.h>

struct Arg {
	bool isDebug;
};

struct Arg getArgs(int argc, char *argv[]);
void parseArgs(struct Arg arg);

#endif /* ifndef _NTR_ARG_H_ 1 */
