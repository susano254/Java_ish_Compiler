#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
extern char staticSymbol[20], returnLabel[30];
extern uint8_t directory;
extern int numberOfFiles;


#define compare(a,b) strcmp(a,b) == 0 ? 1 : 0


FILE* OpenStreamFile (FILE** ,char* , char* );

int findCharacter (char* , char );

char checkCharacter(FILE* );

void parser(FILE* , char *, char* , char* );

void symbolIdentifier(char *, char* , char* );

void popHandler(FILE* , char *, char*);

void pushHandler(FILE* , char *, char* );

void arithmaticHandler(FILE* , char );

void instructionHandler(FILE* , char *, char *, char *);

void directoryParser(char ***, const char *);

void dynamicListHandler(char ***, char* );

void branchHandler(FILE* , char* , char* );

void procedureHandler(FILE* , char* , char* , char* );

void funcDecHandler(FILE* , char* , char*);

void returnHandler(FILE*);

void callHandler(FILE *, char* , char* );
