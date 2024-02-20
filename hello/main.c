#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "include/test.h"

int main() {
    setvbuf(stdin,NULL,_IOLBF,BUFSIZ);
    char str[200];
    fputs("$ ",stdout);
    fflush(stdout);
    gets(str);
    printf("\nreceived: %s\n",str);
    exit(0);
    char ret;
    puts("test_fgets()");
    ret = (char) test_fgets() + '0';
    puts(&ret);

    puts("test_SUM_I()");
    ret = (char) test_SUM_I() + '0';
    puts(&ret);

    puts("test_fputs()");
    ret = (char) test_fputs() + '0';
    puts(&ret);

    puts("test_printf()");
    ret = (char) test_printf() + '0';
    puts(&ret);

    return 0;
}

int test_SUM_I() {
    int ret;
    asm ("li %0, 0x55 \n\t"
        "addi %0, %0, 0xaa"
        : "=r" (ret)
        :
        );
    return ret!=0xff;
}
int test_COMP_I() {
    return 0;
}

int test_LOGIC_I() {
    return 0;
}
int test_SHIFT_I();

int test_LUI();
int test_AUIPC();

int test_SUM();
int test_COMP();
int test_LOGIC();
int test_SHIFT();

int test_JUMP();

int test_BRANCH();

int test_LOAD_STORE();

int test_ECALL_EBREAK();


int test_fputs() {
    int ret = fputs("fputs call 1 | ",stdout);
    ret |= fputs("fputs call 2\n",stdout);
	return(ret);
}

int test_printf() {
    char* name = "printf arg";
    int ret = printf("printf body [%s]\n", name);
    return (ret==strlen("printf body [printf arg]\n")) ? 0 : -1;
}

int test_fgets() {
    char buf[80];
    return (fgets(buf, sizeof(buf), stdin)!=NULL) ? 0 : -1;
}
