#include "VMTranslator.h"




int findCharacter (char* string, char character){
	for( uint8_t i = 0; i < strlen(string); i++) {
		if( string[i] == character ) return i;
	}
	return -1;
}



char checkCharacter(FILE* filePointer){
	char c;
	c = fgetc(filePointer);
	fseek(filePointer, -1, SEEK_CUR);
	return c;
}

/*
int findOccurencies(FILE* vmPointer, char* word){
	int functionLocation, numOfReturns = 0;
	char temp[20];
	functionLocation = ftell(vmPointer);
	while(1) { 
		fscanf(vmPointer, "%s", temp);
		if ( compare(temp, "//") ) fscanf(vmPointer, "%*[^\n]\n");
		if( compare(temp, "return") ) { 
			numOfReturns++;
			fscanf(vmPointer, "%s", temp);
			printf("found the %d return instruction that comes before the word %s\n", numOfReturns, temp);
		}
		if( compare(temp, "function") || feof(vmPointer) ) {
			fseek(vmPointer, functionLocation, SEEK_SET);
			return numOfReturns;
		}
	}


}
*/
void parser(FILE* vmPointer, char *instruction, char* segment, char* index){
	if ( checkCharacter(vmPointer) == '/' ){
		fscanf(vmPointer, "%*[^\n]\n");
	}
	else {
		fscanf(vmPointer,"%s", instruction);
		if ( feof(vmPointer) ){
			printf("Reached end of file no more instructions\n");
			return;
		}
		if ( compare(instruction,"push") || compare(instruction,"pop") || compare(instruction,"call") ){
			fscanf(vmPointer, "%s %s", segment, index);
			printf(" Instruction is: %s\t segement is: %s\t index is: %s\n", instruction, segment, index);
		}
		else if ( compare(instruction, "function") ) {
			//returnNum = findOccurencies(vmPointer, "return");
			fscanf(vmPointer, "%s %s", segment, index);
			printf(" Instruction is: %s\t segement is: %s\t index is: %s\n", instruction, segment, index);
		}
		else if ( compare(instruction, "label") ) fscanf(vmPointer, "%s", segment);
		else if ( compare(instruction, "goto") ) fscanf(vmPointer, "%s", segment);
		else if ( compare(instruction, "if-goto") ) fscanf(vmPointer, "%s", segment);
		else if ( compare(instruction, "//") ) fscanf(vmPointer, "%*[^\n]\n");
		else										 printf(" Instruction is: %s\n", instruction);
		
	}
	
}


