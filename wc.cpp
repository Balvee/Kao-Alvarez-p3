#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <termios.h>
#include <string.h>
#include <iostream>

using namespace std;

string opt;
char letter;
int bytes,newLines,characters,words,i;
int tBytes = 0,tNewLines = 0,tCharacters = 0,tWords = 0;
bool final = false;

/**
 * Adds file variable to total 
 * and sets variables to zero.
*/
void resetVar(){
	tBytes = tBytes + bytes;
	tNewLines = tNewLines + newLines;
	tCharacters = tCharacters + characters;
	tWords = tWords + words;
	bytes = 0;
	newLines = 0;
	characters = 0;
	words = 0;
} // resetVar

/**
 * Goes through each file and calculates 
 * each variable.
*/
void setVar(FILE *fileName){
	while((letter = getc(fileName)) != EOF){
			if(letter == '\n'){
				newLines++;
				words++;
				characters++;
			}
			else if(letter == ' '){
				words++;
				characters++;
			}
			else{characters++;}
			bytes++;
	} // while
	words++;
} // setVar

/**
 * Prints the output. 
*/ 
void optPrint(char *argv[]){
	if(opt == "-c" && final == false){
		printf(" %d %s\n",bytes,argv[i]);
	}
	else if(opt == "-l" && final == false){
		printf(" %d %s\n",newLines,argv[i]);
	}
	else if(opt == "-m" && final == false){
		printf(" %d %s\n",characters,argv[i]);
	}
	else if(opt == "-w" && final == false){
		printf(" %d %s\n",words,argv[i]);
	}
	else if(final == true){
		if(opt == "-c"){printf(" %d %s\n",tBytes,"total");}
		else if(opt == "-l"){printf(" %d %s\n",tNewLines,"total");}
		else if(opt == "-m"){printf(" %d %s\n",tCharacters,"total");}
		else if(opt == "-w"){printf(" %d %s\n",tWords,"total");}
	}
} // optPrint

/**Executes the Program*/
int main (int argc, char * argv[]){
	opt = argv[1];
	setbuf(stdout,NULL);
	
	if(opt != "-" && argc == 2){
		FILE *fileName = fopen(argv[1],"r");
		setVar(fileName);
		fclose(fileName);
		printf("  %d  %d %d %s\n",newLines,words,bytes,argv[1]);
	} // Standard output for one file.
	else if(opt != "-" && argc > 2){
		if(opt == "-c" || opt == "-l" || opt == "-m" || opt == "-w"){
			for(i = 2; i < argc; i++){
				FILE *fileName = fopen(argv[i],"r");
				setVar(fileName);
				fclose(fileName);
				optPrint(argv);
				resetVar();
			} // for
			if(argc > 3){
				final = true;
				optPrint(argv);
			}
		} // option for multiple files
		else{
			for(i = 1; i < argc; i++){
				FILE *fileName = fopen(argv[i],"r");
				setVar(fileName);
				fclose(fileName);
				printf("  %d  %d %d %s\n",newLines,words,bytes,argv[i]);
				resetVar();
			} // for
			printf(" %d  %d %d %s\n",tNewLines,tWords,tBytes,"total");
		} // no option for multiple files
	} // argc > 2
	else{
		while((letter = getc(stdin)) != EOF){}
	}
} // main