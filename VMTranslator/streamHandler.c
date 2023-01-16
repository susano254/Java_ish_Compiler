#include "VMTranslator.h"


void staticHandler(char *filePath, char *dirName, int pointIndex, int slashIndex) { 
	char string[10] = "Class";
	char test[30] = { 0 };
	int boolClass = 0;
	int beginingIndex = 1 + slashIndex;
	while ( ( 5 + beginingIndex ) < pointIndex ) { 
		strncpy(test, &filePath[beginingIndex], 5);
		printf("test is %s\n", test);
		if ( compare(test, string) ) { 
			boolClass = 1;
			break;
		}
		beginingIndex++;
	}
	if ( boolClass == 1 ){  
		strncpy(test, dirName, slashIndex);
		strcat(test, "$");
		strncat(test, &filePath[1+slashIndex], (pointIndex - slashIndex) - 1);
	}
	else if ( boolClass == 0 ) { 
		strncpy(test, dirName, slashIndex);
	}
	strcpy(staticSymbol, test);
	printf("test is %s\n", test);
}
	

FILE* OpenStreamFile (FILE** pointer,char* filePath, char* dirName){
	uint8_t index1, index2;
	char outputFile[100] = { 0 };
	static int i = 0;


	printf("\n\t\t\tfile that is currently being read is: %s\n\n",filePath);
	*pointer = fopen(filePath, "r");
	if (*pointer == NULL ) {
		printf("File specified Not found\n");
		exit(-1);
	}

	if ( directory ) {
		index1 = findCharacter(filePath,'.');
		index2 = findCharacter(filePath, '/');
		staticHandler(filePath, dirName, index1, index2);
		index1 = index1 - index2;
		
		strncpy(outputFile, dirName, index2);

		index1 = findCharacter(dirName, '/');
		strcpy(outputFile, dirName);
		strncat(outputFile, dirName, index1);
		strcat(outputFile,".asm");
		if ( i == 0 ) { 
			printf("this the %d time %s is opened\n", i+1, outputFile);
			i++;
			return fopen(outputFile, "w+");
		}
		else {
			printf("this the %d time %s is opened\n", i+1, outputFile);
			i++;
			return fopen(outputFile,"a+");
		}
	}
	else {
		index1 = findCharacter(filePath, '/');
		index2 = findCharacter(filePath, '.');
		//index3 = index2 - index1;
		if( index1 != -1 )  strncpy(staticSymbol, filePath, index1);
		else 				strncpy(staticSymbol, filePath, index2);
		//printf("static symbol contain %s\n", staticSymbol);
		strncpy(outputFile, filePath, index2);
		strcat(outputFile,".asm");
		//printf("the output file is %s\n", outputFile);
		return fopen(outputFile,"w+");
	}
}
