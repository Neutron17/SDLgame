#include "global.h"
#include "arr.h"

#include "error.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// default compare function
static bool defaultCmp(void *B, void *a) {
	return B==a;
}

/** pointer to the Nth element of void *M with members of size MSZ 
 * M + (N*MSZ) */
#define NTH(M, N, MSZ) PTR_ARITH_WARN(M, (N)*(MSZ)
#define NTH_A(A, N) PTR_ARITH_WARN((A).members, (N)*(A).mem_sz)
#define NTH_AP(A, N) PTR_ARITH_WARN((A)->members, (N)*(A)->mem_sz)

// Option: complex Array_t
Option _array_init(unsigned mem_sz, unsigned def_sz) {
	Option ret = {0};
	Array_t arr = {
		.mem_sz = mem_sz,
		.size = def_sz,
		.used = 0
	};
	arr.members = malloc(def_sz * mem_sz);
	if(!arr.members)
		return Option_WRAP_ERR("malloc failed");
	arr.cmp = defaultCmp;
	arr.isValid = 1;
	//array_print(arr);
	ret = Option_COMPLEX_WRAP_OK(&arr, Array_t);
	//array_print(*((Array_t*)ret.ret));
	return ret;
}
void array_destroy(Array_t *arr) {
	free(arr->members);
	*arr = (Array_t){0};
}

void array_set_cmp_fn(Array_t *arr, ArrayCmp fn) {
	arr->cmp = fn;
}

// Option: void *
Option array_index(Array_t arr, unsigned n) {
	if(!arr.isValid)
		return ERROR_FAIL;
	if(n >= arr.used)
		return Option_WRAP_ERR("n >= arr.used");
	return Option_WRAP_OK(NTH_A(arr, n));
}

Option array_last(Array_t arr) {
	if(arr.used == 0)
		return Option_WRAP_ERR("array is empty");
	return array_index(arr, arr.used-1);
}

Error array_resize(Array_t *arr, unsigned nsz) {
	if(nsz < arr->used)
		return ERROR_FAIL;
	if(nsz == arr->size)
		return ERROR_SUCC;
	arr->members = realloc(arr->members, nsz*arr->mem_sz);
	if(!arr->members)
		return ERROR_FAIL;
	return ERROR_SUCC;
}

Error array_push(Array_t *arr, const void *data) {
	if(!arr->isValid)
		return ERROR_FAIL;
	if(arr->used >= arr->size) 
		if(array_resize2(arr).isErr) 
			return ERROR_FAIL;
	memcpy(NTH_AP(arr, arr->used), data, arr->mem_sz);
	arr->used++;
	return ERROR_SUCC;
}

Error array_add(Array_t *arr, unsigned n, void *data) {
	if(!arr->isValid || n > arr->used)
		return ERROR_FAIL;
	if(n == arr->used)
		return array_push(arr, data);
	if(arr->used >= arr->size) 
		if(array_resize2(arr).isErr) 
			return ERROR_FAIL;

	void *old = NTH_AP(arr, n);
	void *new = NTH_AP(arr, n+1);

	memmove(new,
		old,
	      	(arr->used - n)*arr->mem_sz);
	memmove(NTH_AP(arr, n), data, arr->mem_sz);
	arr->used++;
	return ERROR_SUCC;
}

Error array_append(Array_t *dest, Array_t other) {
	if(!dest->isValid || !other.isValid || dest->mem_sz != other.mem_sz)
		return ERROR_FAIL;
	if(dest->used + other.used >= dest->size) 
		if(array_resize(dest, dest->size + other.used + 2).isErr) 
			return ERROR_FAIL;

	memmove(PTR_ARITH_WARN(dest->members, other.used*other.mem_sz),
		other.members, 
		other.used * other.mem_sz);
	dest->used += other.used;

	return ERROR_SUCC;
}

Option array_pop(Array_t *arr) {
	if(!arr->isValid || arr->used == 0)
		return ERROR_FAIL;
	Option last = array_last(*arr);
	arr->used--;
	return last;
}
Error array_pops(Array_t *arr, unsigned n) {
	for(int i = 0; i < n; i++)
		if(array_pop(arr).isErr)
			return ERROR_FAIL;
	return ERROR_SUCC;
}

Error array_remove(Array_t *arr, unsigned n) {
	if(!arr->isValid)
		return ERROR_FAIL;
	if(n == arr->used)
		return array_pop(arr);

	void *old = NTH_AP(arr, n);
	void *new = NTH_AP(arr, n+1);

	memmove(old,
		new,
		(arr->used - n - 1)*arr->mem_sz);
	arr->used--;
	return ERROR_SUCC;
}

Error array_remove_first(Array_t *arr, ArrayCondIter iter) {
	for(int i = 0; i < arr->used; i++)
		if(iter(NTH_AP(arr, i)))
			return ERROR_SUCC;
	return ERROR_FAIL;
}

// Option: int
Option array_remove_if(Array_t *arr, ArrayCondIter iter) {
	int count = 0;
	for(int i = 0; i < arr->used; i++)
		if(iter(NTH_AP(arr, i)))
			count++;
	if(!count)
		return ERROR_FAIL;
	return Option_WRAP_OK(count);
}

Error array_clear(Array_t *arr) { 
	if(!arr->isValid)
		return ERROR_FAIL;
	arr->used = 0;
	return ERROR_SUCC;
}
Error array_clone(Array_t src, Array_t *dest) {
	Option opt = array_init(src.mem_sz, src.used);
	if(opt.isErr)
		return opt;
	else
		UNWRAP_TO_COMPLEX_(opt, dest, Array_t)

	memcpy(dest->members, src.members, src.used*src.mem_sz);
	return ERROR_SUCC;
}

Error array_set(Array_t *arr, unsigned n, void *data) {
	if(!arr->isValid || arr->used <= n)
		return ERROR_FAIL;
	memcpy(NTH_AP(arr, n), data, arr->mem_sz);
	return ERROR_SUCC;
}

// Option: unsigned
Option array_index_of(Array_t arr, void *data) {
	for(int i = 0; i < arr.used; i++)
		if(arr.cmp(NTH_A(arr, i), data))
			return Option_WRAP_OK(i);
	return Option_WRAP_ERR("data not found");
}
// Option: bool
Option array_contains(Array_t arr, void *data) {
	for(int i = 0; i < arr.used; i++)
		if(arr.cmp(NTH_A(arr, i), data))
			return Option_WRAP_OK(1);
	return Option_WRAP_ERR("data not found");
}
// Option: bool
Option array_is_empty(Array_t arr) {
	if(!arr.isValid)
		return Option_WRAP_ERR("not valid");
	return Option_WRAP_OK(arr.used == 0);
}
// Option: complex Array_t
Option array_sub_array(Array_t arr, unsigned from, unsigned to) {
	if(!arr.isValid || to < from || to >= arr.used)
		return ERROR_FAIL;
	const int len = to - (from - 1);
	Option opt = _array_init(arr.mem_sz, len);
	Array_t ret;
	if(opt.isErr) 
		return opt;
	else
		UNWRAP_TO_COMPLEX_(opt, &ret, Array_t);

	for(int i = 0; i < len; i++)
		unwrap(array_push(&ret, unwrap(array_get(arr, from+i-1))));
	return Option_COMPLEX_WRAP_OK(&ret, Array_t);
}

Error array_for_each(Array_t arr, ArrayForEach fn) {
	if(!arr.isValid)
		return ERROR_FAIL;
	for(int i = 0; i < arr.used; i++)
		fn(NTH_A(arr, i));
	return ERROR_SUCC;
}

void array_print(Array_t arr) {
	printf("(Array_t(mem_sz=%d, size=%d, used=%d)) [", arr.mem_sz, arr.size, arr.used);
	for(int i = 0; i < arr.used; i++) {
		printf("%u", *(unsigned *)NTH_A(arr, i));
		if(i != arr.used-1)
			printf(", ");
	}
	puts("]");
}

