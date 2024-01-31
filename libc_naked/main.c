#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stddef.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <malloc.h>
#include <stdbool.h>
#include "io.h"




volatile struct NAKEDUART *uart = (volatile struct NAKEDUART*)IO_ADDR;

void _exit(int __status) {
	char c = __status;
	puts(&c);
	while (true) {}
}

void _kill(int pid, int sig) {
	errno = ENOSYS;
	return;
}

int _getpid() {
	errno = ENOSYS;
	return -1;
}

int _close(int __fd) {
	// no filesystem
	return -1;
}

int _fstat(int __fd, struct stat *__st) {
	if (__fd<3) {
		__st->st_mode = S_IFCHR;
		__st->st_blksize = 0;
		return 0;
	} else {
		errno = EBADF;
		return -1;
	}
}

int _isatty(int __fd) {
	// should always return true
  struct stat s;
  int ret = _fstat (__fd, &s);
  return ret == -1 ? 0 : !!(s.st_mode & S_IFCHR);
}

off_t _lseek(int __fd, int *__ptr, int __dir) {
	// tty is non-seekable
	if (__fd<3) {
		errno = ESPIPE;
		return -1;
	} else {
		errno = ENOSYS;
		return -1;
	}
}

int _open(const char *name, int flags, int mode) {
	// no filesystem
	errno = EIO;
  return -1;
}

size_t _read(int __fd, char *__buf, size_t __nbyte){
	char *__base;
	switch (__fd) {
		case STDIN_FILENO:
			__base = __buf;
			for (; __nbyte>0; __nbyte--) {
				while (!uart->UART_CTRL_IRQ_RX_NEMPTY) {}
				*__buf++ = uart->UART_DATA_RTX;
			}
			return __buf-__base;
											
		default:
			errno = EBADF;
			return -1;
	}
}


char *_sbrk(int __inc) {
	extern char _end;
	extern char _eheap;		/* Defined by the linker */
	static char *heap_ptr;
	char *base;
	
	if (!heap_ptr) {
		heap_ptr = &_end;
	}
	base = heap_ptr;
	heap_ptr += __inc;
	if (heap_ptr>&_eheap) {
		errno = ENOMEM;
		return (char *)-1;
	}
	return base;
}

size_t _write(int __fd, char *__ptr, int __len) {
	switch (__fd) {
		case STDOUT_FILENO:
			for (; __len != 0; __len--) {
				uart->UART_DATA_RTX = *__ptr++;
			}
			return __len;
		default:
			return 0;
	}
}

int puts(const char* __c) {
	do { uart->UART_DATA_RTX=*__c++; } while (*__c!='\0');
	return(uart->UART_DATA_RTX='\n');
}

