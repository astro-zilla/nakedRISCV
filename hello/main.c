#include "include/test.h"

int main() {
	char ret = '0'+fputs("Hello, world",stdout);
	puts(&ret);
	char c = '0'+errno;
	puts(&c);
	/** test_fputs(); */
	/** printf("Hello, world"); */
	return 0; 
}

int test_fputs() {
	int ret;
	if ((ret=fputs("test: fputs\n",stdout))==EOF) { return -1; }
	if (ret<0) {return -1;}
	return 0;
}
