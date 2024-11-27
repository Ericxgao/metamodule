#include <stdio.h>

void _init(void) {
}
void _fini(void) {
}

void *__dso_handle = (void *)&__dso_handle;

void _exit(int x) {
	puts("_exit\n");
	while (1)
		;
}
int _kill(int x, int sig) {
	return -1;
}
int _getpid() {
	return -1;
}
int *__errno() {
	static int errno = 0;
	return &errno;
}
