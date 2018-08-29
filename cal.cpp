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
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

long totalBlocks(const char * path, bool a) {
	DIR *dir;
	struct dirent *sd;
	char buff[1024];
    setvbuf(stdout, buff, _IONBF, 0); //disable output buffering
    struct stat sb;
    long tBlocks = 0;

    if (stat(path, &sb) == -1) {
        perror("stat");
        exit(EXIT_FAILURE);
    }


	if ((dir = opendir (path)) != NULL) {
		// print all the files and directories within directory
		while ((sd = readdir (dir)) != NULL) {
			
			if(a) {
				tBlocks += (long)sb.st_blocks;
			}
			else {
				if(sd->d_name[0] != '.') {
					tBlocks += (long)sb.st_blocks;
				} //if
			}
			//printf("%s: %ld \n", sd->d_name, (long)sb.st_blocks);
		} //while
		closedir (dir);
		return tBlocks;
	}
	else {
		/* could not open directory */
		perror ("Error. Could not open directory");
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

void printColor(const char * name, char type, bool lf) {

	struct stat sb;


	if(lf) {
		if (stat(name, &sb) == -1) {
        	perror("stat");
        	exit(EXIT_FAILURE);
    	}

    	//permissions
    	printf( (S_ISDIR(sb.st_mode)) ? "d" : "-");
	    printf( (sb.st_mode & S_IRUSR) ? "r" : "-");
	    printf( (sb.st_mode & S_IWUSR) ? "w" : "-");
	    printf( (sb.st_mode & S_IXUSR) ? "x" : "-");
	    printf( (sb.st_mode & S_IRGRP) ? "r" : "-");
	    printf( (sb.st_mode & S_IWGRP) ? "w" : "-");
	    printf( (sb.st_mode & S_IXGRP) ? "x" : "-");
	    printf( (sb.st_mode & S_IROTH) ? "r" : "-");
	    printf( (sb.st_mode & S_IWOTH) ? "w" : "-");
	    printf( (sb.st_mode & S_IXOTH) ? "x" : "-");
	    printf(". ");

	    //links
	    printf("%ld ", (long) sb.st_nlink);

	    //uid
	    printf("%s ", (getpwuid(sb.st_uid))->pw_name);

	    //gid
	    printf("%s ", (getgrgid(sb.st_gid))->gr_name);

	    //size

	    printf("%6ld ", (long) sb.st_size);

	    time_t t = sb.st_mtime;
		struct tm lt;
		localtime_r(&t, &lt);
	    char time [80];
	    strftime(time, 80, "%b %e %R", &lt);
	    printf("%s ", time);

	}

	if (type == 'r') {
		printf ("%s\n", name);
	}
	else if (type == 'd') {
		printf ("\033[0;34m%s\033[0m\n", name);
	}
	else if (type == 'x') {
		printf ("\033[0;32m%s\033[0m\n", name);
	}
	
}

void printDir(const char * path, bool a, bool l, bool multiple) {
	DIR *dir;
	struct dirent *sd;
	char buff[1024];
    setvbuf(stdout, buff, _IONBF, 0); //disable output buffering
    struct stat sb;
    char type = 'r'; //type of file(d = directory, r = regular, x = executable)
    bool lf = false; //long format - is long format used or not
    long tBlocks = 0;

    if (stat(path, &sb) == -1) {
        perror("stat");
        exit(EXIT_FAILURE);
    }

    //if not a directory, just print file name
    if (!S_ISDIR(sb.st_mode)) {
    	if (sb.st_mode & S_IXUSR) {
			type = 'x';
		}
		printColor(path, type, lf);
    }
    //if path is a directory
	else {
		if ((dir = opendir (path)) != NULL) {
			// print all the files and directories within directory
			
			if(l) {
				tBlocks = totalBlocks(path, a);
				printf("total %ld\n", tBlocks);
			}

			while ((sd = readdir (dir)) != NULL) {
				
				if (stat(sd->d_name, &sb) == -1) {
	        		perror("stat");
	        		exit(EXIT_FAILURE);
	    		}
	    		
	    		type = 'r';
	    		if (S_ISDIR(sb.st_mode)) {
	    			type = 'd';
	    		}
	    		else if (sb.st_mode & S_IXUSR) {
			    	type = 'x';
	    		}

				if(!a && !l) {
					if(sd->d_name[0] != '.') {
						printColor(sd->d_name, type, lf);
					} //if
				} //if
				else if (a && !l) {
					printColor(sd->d_name, type, lf);
				}
				else if (!a && l) {
					lf = true; //long format
					if(sd->d_name[0] != '.') {
						printColor(sd->d_name, type, lf);
					} //if
				}
				else if (a && l) {
					lf = true; //long format
					printColor(sd->d_name, type, lf);
				}

			} //while
			closedir (dir);
		}
		else {
			/* could not open directory */
			perror ("Error. Could not open directory");
			//return EXIT_FAILURE;
		}
	}
}


int main(int argc, char *argv[]) {

	char buff[1024];
    setvbuf(stdout, buff, _IONBF, 0); //disable output buffering
	int argvStart = 1; //the beginning of the list of files/directories
	bool a = false; //if the a option is used
	bool l = false; //if the l option is used
	bool multiple = false; //if there are multiple files
	char *path = NULL;

	if(argc == 1) {
		//print out normal files regularly
		//get current working directory
		path = getcwd(NULL, 0);
		printDir(path, a, l, multiple);

	}

	else {

		if(strcmp(argv[1],"-a") == 0 || strcmp(argv[1],"-l") == 0 || strcmp(argv[1],"-al") == 0) {

			if(strcmp(argv[1],"-a") == 0) {
				//print out hidden files
				argvStart = 2;
				a = true;
			}

			else if(strcmp(argv[1],"-l") == 0) {
				//print out regular files with information
				argvStart = 2;
				l = true;
			}

			else if(strcmp(argv[1],"-al") == 0 || strcmp(argv[1],"-la") == 0) {
				//print out hidden files AND information
				argvStart = 2;
				a = true;
				l = true;
			}

			if(argc == 2) {
				char *path = NULL;
				path = getcwd(NULL, 0);
				//if l option, print out the total number of blocks before listing files
				if (l && !multiple) {
				}
				printDir(path, a, l, multiple);
				return EXIT_SUCCESS; 
			}

			//if there are more than 3 arguments with options,
			//there must be multiple files in the initializer list
			if(argc > 3) {
				multiple = true;
			}
		}
		//if there are more than 2 arguments without options,
		//there must be multiple files in the initializer list
		if(argc > 2) {
			multiple = true;
		}
		
		//print list each file in the 
		for(int i = argvStart; i < argc; i++) {
			printDir(argv[i], a, l, multiple);
			
		}
	}
	return EXIT_SUCCESS; 
}