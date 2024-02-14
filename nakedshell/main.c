#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <assert.h>

int main() {
    setvbuf(stdin, NULL, _IOLBF, BUFSIZ); // set stdin to be line buffered (_read checks for line buffering with a hack)
//    char *cd = getcwd(NULL,0);
    char *cd = "/home";
    assert(cd!=NULL);
    char buf[80];
	while (1) {
        memset(buf,0,strlen(buf));
        printf("(%s)$ ",cd);
        fflush(stdout);
        fgets(buf,sizeof(buf),stdin);

        if (strcmp(buf,"exit\n")==0) {exit(0);}
        if (strcmp(buf,"echo\n")==0) {printf("%s",buf);}
        if (strcmp(buf, "abort\n")==0) {abort();}
    }
    exit(errno);
}

