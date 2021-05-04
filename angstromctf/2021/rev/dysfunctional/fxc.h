#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
unsigned long func_size;
unsigned char* func_start;
unsigned long func_rzsize;
char *func_buf;
void *mmap_ptr;
#define LAMBDA_GUTS(varname, expr, xtype)\
	asm volatile goto("call %l0" : : : : after1_ ## varname);\
	asm volatile("movq %%rdi, %0" : "=r"(x));\
	asm volatile("movq %0, %%rax" : : "X"((xtype)(expr)));\
	asm volatile("pop %rbp");\
	asm volatile("ret");\
	after1_ ## varname: ;\
	asm volatile goto ("call %l0" : : : : after2_ ## varname);\
	after2_ ## varname: ;\
	asm volatile ("pop %%rdi; pop %%rsi; mov %%rsi, %0" : "=r"(func_start));\
	asm volatile ("sub %%rsi, %%rdi; mov %%rdi, %0" : "=r"(func_size));\
	func_size -= 5;\
	asm volatile ("mov %%rbp, %%rdi; sub %%rsp, %%rdi; mov %%rdi, %0" : "=r"(func_rzsize));\
	func_buf = malloc(func_size+func_rzsize+11);\
	memcpy(func_buf+11, func_start, func_size);\
	*(unsigned int*)func_buf = 0x00bd4855;\
	*(unsigned long*)(func_buf+3) = (unsigned long)(func_buf + func_size + func_rzsize + 11);\
	asm volatile ("mov %%rbp, %0" : "=r"(func_start));\
	func_start -= func_rzsize;\
	memcpy(func_buf + func_size + 11, func_start, func_rzsize);\
	mmap_ptr = mmap(0, func_size + func_rzsize + 11, PROT_EXEC | PROT_WRITE | PROT_READ, MAP_ANON | MAP_PRIVATE, -1, 0);\
	memcpy(mmap_ptr, func_buf, func_size + func_rzsize + 11);

#define LAMBDA(varname, expr)\
	LAMBDA_GUTS(varname, expr, unsigned long)\
	int (*varname)(int) = mmap_ptr;

int *map(int(*func)(int), int* array, int size) {
	int *ret = malloc(size*sizeof(int));
	for (int i = 0; i < size; i++) {
		ret[i] = func(array[i]);
	}
	return ret;
}
int *filter(int(*func)(int), int* array, int size, int* newSize) {
	int *ret = malloc(size*sizeof(int));
	int j = 0;
	for (int i = 0; i < size; i++) {
		if (func(array[i])) {
			ret[j++] = array[i];
		}
	}
	ret = realloc(ret, j*sizeof(int));
	*newSize = j;
	return ret;
}

int (*compose(int (*first)(int), int (*second)(int)))(int) {
	unsigned long x;
	LAMBDA(third, second(first(x)));
	return third;
}
