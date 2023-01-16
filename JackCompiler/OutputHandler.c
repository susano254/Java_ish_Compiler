#include "JackCompiler.h"

void outputHandler(FILE* fp, char* token, char* type) {
	return;
}



void termOutputHandler(FILE *outputFile, char *token, char *type){
	if(strcmp(type, "integerConstant") == 0) 
		fprintf(outputFile, "push constant %s\n", token); 
	else if( searchStrings(identifiers, 3, type) || strcmp(token, "this") == 0)
		variableOutputHandler(outputFile, token, type);
	else if(strcmp(type, "stringConstant") == 0) {
		fprintf(outputFile, "push constant %ld\n", strlen(token));
		fprintf(outputFile, "call String.new 1\n");

		for( int i = 0; i < strlen(token); i++)
			fprintf(outputFile, "push constant %d\ncall String.appendChar 2\n", token[i]);
	}
	else if(strcmp(token, "null") == 0 || strcmp(token, "false") == 0) fprintf(outputFile, "push constant 0\n"); 
	else if(strcmp(token, "true") == 0 ) fprintf(outputFile, "push constant 0\nnot\n"); 


}



void variableSearch(char *variableName, char *variableAddress){
	char variableSymbols[110];
	char segment[20] = { 0 };
	int index;


	if( (index = searchSymbolTable(subroutine_symbol_table, subTableSize, variableName)) != -1 ){
		sprintf(variableSymbols, "%s %u", subroutine_symbol_table[index].kind, subroutine_symbol_table[index].id);
	}
	else if( (index = searchSymbolTable(class_symbol_table, classTableSize, variableName)) != -1 ){
		if(strcmp(class_symbol_table[index].kind, "field") == 0)
			strcpy(segment, "this");
		else 
			strcpy(segment, class_symbol_table[index].kind);

		sprintf(variableSymbols, "%s %u", segment, class_symbol_table[index].id);
	}
	else {
		printf("didn't found the variable in any scope\n");
		exit(-1);
	}

	strcpy(variableAddress, variableSymbols);
}



void variableOutputHandler(FILE *outputFile, char *variableName, char *variableType){
	char variableSymbols[110];
	int index;

	if( (index = searchSymbolTable(class_symbol_table, classTableSize, variableName)) != -1 ){
		char segment[20] = { 0 };
		if(strcmp(class_symbol_table[index].kind, "field") == 0)
			strcpy(segment, "this");
		else 
			strcpy(segment, class_symbol_table[index].kind);

		sprintf(variableSymbols, "%s %u", segment, class_symbol_table[index].id);
		vmPushToken(outputFile, variableSymbols);
	}

	else if( (index = searchSymbolTable(subroutine_symbol_table, subTableSize, variableName)) != -1 ){
		sprintf(variableSymbols, "%s %u", subroutine_symbol_table[index].kind, subroutine_symbol_table[index].id);
		vmPushToken(outputFile, variableSymbols);
	}
	//else if( strcmp(variableName, "this") == 0) fprintf(outputFile, "push pointer 0");
	else {
		printf("didn't found the variable in any scopre\n");
		exit(-1);
	}
}





int searchSymbolTable(SymbolEntry_t *symbolTable, unsigned char tableSize, char* variableName){
	//search for the kind and the id of the given variable name 
	for( int i = 0; i < tableSize; i++){
		if(strcmp(variableName, symbolTable[i].name) == 0) return i;
	}
	return -1;
}


void printSymbolTable(SymbolEntry_t *table, unsigned char size){
	printf("\tname\ttype\tkind\tid\n");

	if( table == 0 ){
		printf("invalid pointer the table pointer is null\n");
		return;
	}
	for( int i = 0; i < size; i++)
		printf("\t%s\t%s\t%s\t%d\n", table[i].name, table[i].type, table[i].kind, table[i].id);
}
