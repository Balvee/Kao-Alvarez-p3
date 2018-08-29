#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <termios.h>
#include <string.h>
#include <iostream>

#define BUFFSIZE 4096

using namespace std;

/**Executes the Program*/
int main (int argc, char * argv[]){
	char buf[BUFFSIZE];
	int count = 0, i, r;
	int linesRead;

	if(argc == 3){
		memmove(argv[1],argv[1]+1,3);
		linesRead = atoi(argv[1]);
		r = 2;
	}
	else{
		linesRead = 10;
		r = 1;	
	}
	if(!(linesRead > 0)){
		perror("Invalid Input");
	}
	setvbuf(stdout,buf,_IONBF,0);

	int fd = open(argv[r], O_RDONLY);
	int byteRead = read(fd,buf,BUFFSIZE);
	for(i = 0; i < byteRead; i++){
		if(count == linesRead){
			break;
		}
		cout << buf[i];
		
		if(buf[i] == '\n'){	
			count++;
		}
	}
	close(fd);
	return 0;
}