#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <termios.h>
#include <string.h>
#include <iostream>

#define BUFFSIZE 4096

using namespace std;

string opt;
char letter;
int bytes,lines,nLines,strtLine,curLine,i = 0,usrBytes,usrLine;
char buf[BUFFSIZE];
int c,count;

/**Sets variables from file*/
void setVar(FILE *fileName){
	i = 0;
	for(;i < bytes;i++){
		buf[i] = ' ';
	}
	while((letter = getc(fileName)) != EOF){
			if(letter == '\n'){	
				lines++;
				bytes++;
			}
			else{
				bytes++;
			}
			buf[i] = letter;
			i++;
	}
	if(buf[i-1] == '\n'){lines--;}
} // setVar

/**Read File*/
void readLines(){
	strtLine = lines - nLines;

	if(opt == "-c"){
		i = bytes - usrBytes;
		if(i < 0){i = 0;}
	}
	else{i = 0;}
	if(bytes != 0){
			for(; i<=bytes;i++){
				if(opt == "-c"){cout << buf[i];}
				else if(curLine > strtLine && opt != "-c"){
					cout << buf[i];
				}
				if(buf[i] == '\n'){curLine++;}
			}
	}
	cout << flush;
}

/**Recursive Check to update -f*/
bool checkChange(const char *argv[]){
	usrBytes = bytes;
	bytes = 0;
	lines = 0;
	FILE *fileName = fopen(argv[2],"r");
	setVar(fileName);
	if(count == 10){
		count = 0;
		fclose(fileName);
		return true;
	}
	else if(bytes != usrBytes){
		count = -1;
		fclose(fileName);
		return true;
	}
	else{
		count++;
		fclose(fileName);
		return checkChange(argv);
	}
}	

/**Executes Program*/
int main (int argc, const char *argv[]){
	opt = argv[1];
	if(argc == 2){
		FILE *fileName = fopen(argv[1],"r");
		setVar(fileName);
		nLines = 10;
		readLines();
		fclose(fileName);
	}
	else if(argc > 2){
		if(opt == "-f"){
				FILE *fileName = fopen(argv[2],"r");
				setVar(fileName);
				nLines = 10;
				readLines();
				fclose(fileName);
				while(checkChange(argv)){
						if(bytes != usrBytes && bytes != 0){
							cout << "tail: " << argv[2] << ": file truncated" << endl;
							readLines();
						}		
				}
		}
		else if(opt == "-n"){
			FILE *fileName = fopen(argv[3],"r");
			setVar(fileName);
			nLines = atoi(argv[2]);
			readLines();
			fclose(fileName);
		}
		else if(opt == "-c"){
			usrBytes = atoi(argv[2]);
			FILE *fileName = fopen(argv[3],"r");
			setVar(fileName);
			readLines();
			fclose(fileName);
		}
	} // argc > 2
} // main