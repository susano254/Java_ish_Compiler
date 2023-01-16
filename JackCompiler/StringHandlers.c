#include "JackCompiler.h"



char checkChar(FILE* fp, int offset) { 
	char c;
	int pullBack;
	fseek(fp, offset, SEEK_CUR);
	c = fgetc(fp);
	pullBack = -(offset + 1);
	fseek(fp, pullBack, SEEK_CUR);
	return c;
}

char checkPrevChar(FILE *fp, int offset){
	char c;
	c = checkChar(fp, offset);
	while( c == ' ' || c == '\n' || c == '\t' || c == EOF){
		(offset < 0) ? offset-- : offset++;
		c = checkChar(fp, offset);
	}
	return c;
}

int findChar(char c, char* string){
	for(int i = 0; i < strlen(string); i++){
		if( c == string[i] ) return i;
	}
	return -1;
}





void extModifier(char* inFilePath, char* outFilePath, char* ext){
	char buffer[100] = { 0 };
	int pointIndex;
	
	pointIndex = findChar('.', inFilePath);

	// copy the string until it's point index is reached 
	for( int i = 0; i < pointIndex; i++) buffer[i] = inFilePath[i];
	strcat(buffer, ext);


	strcpy(outFilePath, buffer);
}




int commentHandler(FILE* fp) {
	if( ( checkChar(fp,0) == '/' ) && ( checkChar(fp,1) == '/' ) ){ 
		skipLine(fp);
		return 1;
	}
	else if( ( checkChar(fp,0) == '/' ) && ( checkChar(fp,1) == '*' ) ) { 
		skipUntilRegEx(fp, "*/", 1);
		return 1;
	}
	else return 0;

}





void skipLine(FILE* fp){
	printf("Found a comment so working on skiping the rest of the line\n");
	char c;
	while( c != '\n' ) c = fgetc(fp);
}





void skipUntilRegEx(FILE* fp, char* regex, int bora){			// bora is short for before or after
	printf("In function skipUntilRegEx\n");						// after = 1; and before = 0;
	int boolRegEx = 0;

	while( !(boolRegEx) ) {
		printf("following regEx is: ");
		for( int i = 0; i < strlen(regex); i++) {
			printf("%c", checkChar(fp, i) );
			boolRegEx = ( checkChar(fp, i) == regex[i] );
			if( !(boolRegEx) ) { 
				printf("\nboolRegEx is %d (no match) so loop breaked \n", boolRegEx);
				break;
			}
		}
		if ( boolRegEx ) printf("\nboolRegEx is %d RegEx found !!!! \n\n", boolRegEx);
		if( !(boolRegEx) ) fseek(fp, 1, SEEK_CUR);
	}
	
	if( bora == 1 ) fseek(fp, strlen(regex), SEEK_CUR);
}



int searchStrings(char **strings, int stringsSize, char *string){
	for( int i = 0; i < stringsSize; i++){
		if(strcmp(strings[i], string) == 0)
			return 1;
	}
	return 0;
}

