#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**Execute Program*/
int main(int argc, char **argv, char** envp)
{
  	char buff[1024];
    setvbuf(stdout, buff, _IONBF, 0); //setvbuf is used to disable output buffering
  	char** env;
  	
  	for (env = envp; (*env != nullptr); env++) {
   		printf("%s\n", *env);    
  	}
  	return EXIT_SUCCESS;;
}