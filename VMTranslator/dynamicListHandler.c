#include "VMTranslator.h"


void dynamicListHandler(char ***list, char* buffer){
	//printf("the error is  in dynamicListHandler\n");
	static int listSize = sizeof(char*);
	static int i = 0;
	if ( i == 0 ) *list = (char**) malloc(sizeof(char*));

	// deal with *list as if it's fileNames variable
	
	(*list)[i] = (char*) malloc(strlen(buffer) + 1);
	strcpy((*list)[i], buffer);	
	if((*list)[i] == NULL){ 
		printf("*list[%d] is pointing to null pointer\n",i);
		exit(-1);
	}
	listSize += sizeof(char*);
	*list =  (char**) realloc(*list, listSize); 
	if(*list == NULL){ 
		printf("couldn't allocate the requested memory\n");
		exit(-1);
	}

	i++;
	//printf("the error is not in dynamicListHandler\n");
}


