#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

/**Executes the Program*/
int main(int argc, char *argv[]){
	char outstr[200];
	time_t t;
	struct tm *tmp;

	setvbuf(stdout,outstr,_IONBF,0);
	
	if(argc == 1){
		char time[] = "+%a %b %-_d %X %Z %Y";
		argv[1] = time;
	}

	memmove(argv[1],argv[1]+1,25);
	
	t = time(NULL);
	tmp = localtime(&t);
	if(tmp == NULL){
		perror("localtime");
		return EXIT_SUCCESS;
	}
	if (strftime(outstr, sizeof(outstr), argv[1], tmp) == 0) {
		fprintf(stderr, "strftime returned 0\n");
		exit(EXIT_FAILURE);
	}	
	printf("%s\n", outstr);
	
	return EXIT_SUCCESS;
}