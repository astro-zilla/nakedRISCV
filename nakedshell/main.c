#include <stdio.h>
/** #include <stdio.h> */
/** #include <string.h> */
/**  */
/** int main() { */
/**   char str[80]; */
/**   while(1) { */
/**     putchar('$'); */
/**     putchar(' '); */
/**     gets(str); */
/**     if (!strcmp(str,"exit")) {puts("exiting..."); break;} */
/**     else if (!strcmp(str, "clear")) {putchar('');} */
/**     else {puts("command: \""); puts(str); puts("\" not recognised\navailable commands:\n  exit\n  clear");} */
/**   } */
/**   return(0); */
/** } */

int main() {
	char* b = "Hello, world!";
	puts(b);
	return b[5];
}

