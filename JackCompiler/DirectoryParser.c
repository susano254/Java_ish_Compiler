#include "JackCompiler.h"

int directoryParser(char* d_path){
	DIR *pDir;
	struct dirent *pDirEnt;
 	int pointIndex, slashIndex, numOfFiles = 0;
	char currentFile[200], directoryString[100];

	//testing first if it's not a directory by searching for a '.' in the d_path [NOTE: might not work with hidden directories starting with '.']
	pointIndex = findChar('.', d_path);
	if( pointIndex != -1 ) {
		printf("This is not a directory\n");
		return 0;
	}

	//storing the path in a temporary variable for further modification
	strcpy(directoryString, d_path);

	// testing if the last char in the d_path is '/' or not if not add it 
	slashIndex = strlen(directoryString) - 1;
	if( directoryString[slashIndex] != '/') strcat(directoryString,"/");

	// opening the direcoty with opendir
	pDir = opendir(d_path);
	
	//looping through the content of the directory
	while( (pDirEnt = readdir(pDir)) != NULL ) { 
		//if the current entry isn't a file skip, and proceed with the loop otherwise
		pointIndex = findChar('.', pDirEnt->d_name);	
		if( pointIndex == -1) continue;
		
		//compare the extension with the ".jack" one if so add it to the files array
		if( (strcmp(&(pDirEnt->d_name)[pointIndex], ".jack") == 0 )) {
			//concatenate the directory path with the file path and it to the files array
			strcpy(currentFile, directoryString);
			strcat(currentFile, pDirEnt->d_name);
			addToFiles(currentFile);
			numOfFiles++;
		}
	}
	closedir(pDir);
	return numOfFiles;

}
