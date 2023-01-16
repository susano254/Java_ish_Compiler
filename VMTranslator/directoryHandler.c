#include "VMTranslator.h"


void directoryParser(char ***files, const char *directoryString){
	DIR* directoryPointer;
	struct dirent *directoryEntry; 	
	uint8_t pointIndex;
	char currentFile[100];
	
	directoryPointer = opendir(directoryString);


	if ( directoryPointer == NULL ) {
		printf("couldn't open specified directory may be it doesn't exist\n");
		exit(1);
	}


	while( (directoryEntry = readdir(directoryPointer) ) != NULL ) {
		strcpy(currentFile, directoryString);

		if( (findCharacter(currentFile, '/') ) == -1 ) strcat(currentFile, "/");

		strcat(currentFile, directoryEntry->d_name);
		//printf("the current file is: %s\n", currentFile);

		pointIndex = findCharacter(currentFile, '.');
		if( compare(&currentFile[pointIndex], ".vm" ) ){ 
			//printf("found one .vm file\n");
			dynamicListHandler(files, currentFile);
			numberOfFiles++;
		}


	}
	
	closedir(directoryPointer);
}
		

