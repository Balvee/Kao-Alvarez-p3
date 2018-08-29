#include <fcntl.h>
#include <unistd.h>
#include <cstdlib>
#include <stdio.h>
#include <string>
#include <sys/types.h>
#include <dirent.h>

using namespace std;

string option;

/**Executes the Program*/
int main (int argc, const char *argv[]){
	setbuf(stdout,NULL);
	if(argc == 3){
		int error = link(argv[1],argv[2]);
		if(error == -1){
			perror("Newpath already exist");
			return EXIT_SUCCESS;
		}
	} // files only
	else if(argc == 4){
		option = argv[1];
		if(option == "-s"){
			int error = symlink(argv[2],argv[3]);
			if(error == -1){
				perror("Newpath already exist");
				return EXIT_SUCCESS;
			}
		}
		else{
			perror("Incorrect option");
			return EXIT_SUCCESS;
		}
	} // option -s
	else{
		perror("Incorrect Input");
		return EXIT_SUCCESS;
	}
} // main