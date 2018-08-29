#include <iostream>
#include <string>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <cstdlib>
#include <errno.h>
#include <iomanip>

using namespace std;
using std::setw;
using std::setfill;


/**
* make - creates the directory tree. If the parent directory does not exist,
* path becomes the name of the directory before the first /, and this method
* will attempt to create that directory in the current current directory.
*/
void make(const char * path, mode_t mode, bool p) {
	
	mkdir(path, mode);
	chmod(path, mode);

	//if parent directory does not exist
	if(errno == ENOENT) {
		//if user did not specify -p option
		if (p == false) {
			printf("mkdir: cannot create directory `%s': No such file or directory\n", path);
		}
		//if user used -p option
		else if (p) {
			char* p = const_cast<char*>(path);

		  	//finds the next / and creates the path while not at the end of the path yet
		  	while (*p != '\0') {
			    	// skips the /
			    	p++;
			    // finds the next / or the end
			    while(*p != '\0' && *p != '/') {
			    	p++;
			    }
			    // stores the original path
			    char v = *p;
			    
			    *p = '\0';
			    // creates directory 
			    if(mkdir(path, mode) != 0) {
			      *p = v;
			    }
			    chmod(path, mode);
		    	// resets the original path
		    	*p = v;
			} //while
		} //else if
	} //if
} //make

int main(int argc, char *argv[]) {
	
	//default mode
	mode_t mode = 0755;
	//if user is using the -p option
	bool p = false;
	//start index for directory paths list
	int start = 1;

	if (argc > 2) {
		if(strcmp(argv[1],"-p") == 0) {
			p = true;
			start++;
		} //if
	} // if

	//if number of arguments > 3, and argv at 1 or 2 is -m
	if (argc > 3) {
		if(strcmp(argv[1],"-m") == 0 || strcmp(argv[2],"-m") == 0) {
			if(strcmp(argv[1], "-m") == 0) {
				stringstream iss(argv[2]); //converts argv to int

				//iss << setfill('0') << setw(4) << iss;
				if (!(iss >> mode)) {
				perror("Invalid Mode");
				return EXIT_FAILURE;
				} //if

			} //if
			else if(strcmp(argv[2],"-m") == 0) {
				stringstream iss(argv[3]); //converts argv to int

				//iss << setfill('0') << setw(4) << iss;
				if (!(iss >> mode)) {
				perror("Invalid Mode");
				return EXIT_FAILURE;
				} //if
			} //else if
		//increment start twice - once for the -m and once for the mode number
		start=start+2;
		} //if
	} //if

	//call make function for every directory in the argument list
	for (int i = start; i < argc; i++) {
		make(argv[i], mode, p);
	} //for
	exit(EXIT_SUCCESS);
} //main