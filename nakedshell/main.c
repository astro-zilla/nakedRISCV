#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>


int main() {
    setvbuf(stdin, NULL, _IOLBF, BUFSIZ); // set stdin to be line buffered (_read checks for line buffering with a hack)
    setvbuf(stderr, NULL, _IOLBF, BUFSIZ);

    char *cd = "/home";
    assert(cd!=NULL);

    printf("\e[1;1H\e[2Jbuilt at %s on %s\n",__TIME__,__DATE__);

    char buf[64], command[64], arg1[64], arg2[64], arg3[64], arg4[64];
	while (errno==0) {
        memset(buf,0,strlen(buf));
        printf("(%s)$ ",cd);
        fflush(stdout);
        fgets(buf,sizeof(buf),stdin);

        sscanf(buf,"%s %s %s %s %s",command,arg1,arg2,arg3,arg4);

        if (strcmp(command,"exit")==0) { exit(EXIT_SUCCESS);}
        else if (strcmp(command,"echo")==0) {printf("%s\n",arg1);}
        else if (strcmp(command, "abort")==0) {abort();}
        else if (strcmp(command, "clear")==0) {printf("\e[1;1H\e[2J");}
        else if (strcmp(command, "die")==0) {errno=EOWNERDEAD;}
        else {printf("command not recognised: %s\n",command);}
    }
    char emsg[64];
    sprintf(emsg,"command %s failed",command);
    perror(emsg);
    return(errno);
}

