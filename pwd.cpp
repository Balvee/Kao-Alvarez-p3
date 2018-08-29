#include <cstdlib>
#include <unistd.h>
#include <stdio.h>

/**Executes pwd*/
int main (int argc, const char *argv[]){
	setbuf(stdout,NULL);
	char *pwd = get_current_dir_name();
	printf("%s\n",pwd);
	return EXIT_SUCCESS;
} // main