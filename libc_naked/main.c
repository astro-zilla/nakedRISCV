#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>
#include "io.h"

volatile struct NAKEDUART *uart = (volatile struct NAKEDUART*)IO_ADDR;



void _exit(int __status) {
	printf("exit with status: %d\n",__status);
	asm("ebreak");
    while(1);
}

void _kill(int pid, int sig) {
	errno = ENOSYS;
	return;
}

int _getpid() {
    // no process management
	errno = ENOSYS;
	return -1;
}

int _close(int __fd) {
	// no filesystem
	return -1;
}

int _fstat(int __fd, struct stat *__st) {
	if (__fd<3) {
        // std streams are char-only
		__st->st_mode = S_IFCHR;
		__st->st_blksize = 0;
		return 0;
	} else {
        // no file system implemented
		errno = EBADF;
		return -1;
	}
}

int _isatty(int __fd) {
	// check if file represents tty device
    struct stat s;
    int ret = fstat (__fd, &s);
    return ( ret == -1 || __fd == STDIN_FILENO ) ? 0 : !!(s.st_mode & S_IFCHR);
}

off_t _lseek(int __fd, off_t __offset, int __whence) {
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
    int i;
	switch (__fd) {
		case STDIN_FILENO: // char mode lbf: read until newline
            for (i=0; i<__nbyte; i++) {
                while (!uart->UART_CTRL_RX_NEMPTY) { ; }
                if((*(__buf+i) = uart->UART_DATA_RTX)=='\n'){i++; break;};
            }
			return i;
											
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
    int nbyte;
	switch (__fd) {
		case STDOUT_FILENO:
			for (nbyte=0; nbyte < __len; nbyte++) {
				uart->UART_DATA_RTX = *__ptr++;
			}
			return nbyte;
		default:
			return 0;
	}
}

//int puts(const char* __c) {
//	do { uart->UART_DATA_RTX=*__c++; } while (*__c!='\0');
//    uart->UART_DATA_RTX='\n';
//    return 0;
//}

