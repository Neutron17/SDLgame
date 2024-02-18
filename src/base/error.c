#include "global.h"
#include "error.h"

#include "log.h"
#include <stdlib.h>

/* Dynamic variable problem
 * - Dynamic memory allocation is slow
 * - You can't cast a normal variable to a void *
 * - You shouldn't return a pointer to a local variable
 * - You have to dynamicaly allocate and that is slow 	*/

void _unwrap_err(Option ret, const char *restrict file, unsigned line, const char *restrict func) {
	if(ret.isErr)
		LOGF(L_ERR, "UNWRAP ERROR: %s, from %s/%s, line: %s\n", ret.msg, file, func, line);
}

// WARNING: if is actuall(not Error) Option -> needs to be freed
void *_unwrap(Option ret, const char *restrict file, unsigned line, const char *restrict func) {
	unwrap_err(ret);
	if(!ret.isntOpt) // is opt
		return ret.ret;
	return NULL;
}

// WARNING: if is actuall(not Error) Option -> needs to be freed
void *_except(Option ret, const char *msg, const char *restrict file, unsigned line, const char *restrict func) {
	unwrap_err(ret);
	if(!ret.isntOpt) // is opt
		return ret.ret;
	return NULL;
}

Option _Option_ComplexWrap(const void *ret, bool isErr, unsigned sz) {
	Option r = { 
		.isErr = isErr,
		.ret = malloc(sz)
	};
	if(!r.ret) {
		LOGF(L_ERR, "COMPLEX_UNWRAP ERROR: malloc failed\n");
		return ERROR_FAIL;
	}
	memcpy(r.ret, ret, sz);
	return r;
}

