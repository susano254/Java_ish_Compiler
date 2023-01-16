#include "JackCompiler.h"

char** files;
unsigned char statici, fieldi, entryi, argumenti, vari, classTableSize, subTableSize;

void addToFiles(char* filePath) { 
	static int i = 0;
	static int size = sizeof(char*);


	if ( i == 0 ) files = (char**) malloc(sizeof(char*));

	//allocate a space for a new char pointer
	files[i] = (char*) malloc(strlen(filePath) + 1 );

	//after allocating the space copy the desired string into the allocated space
	strcpy(files[i], filePath);
	i++;
	size += sizeof(char*);
	files = (char**) realloc(files,size);

}




SymbolEntry_t* symbolTable(SymbolEntry_t *symbol_table, char *name, char *type, char *kind){
	unsigned char *index;

	if( strcmp(kind, "static") == 0 )
		index = &statici;
	else if( strcmp( kind, "field" ) == 0 )
		index = &fieldi;
	else if( strcmp( kind, "argument" ) == 0 )
		index = &argumenti;
	else if( strcmp( kind, "local" ) == 0 )
		index = &vari;
	else if( strcmp( name, "this") == 0){
		uint8_t i = 0;
		index = &i;
	}
		

	strcpy(symbol_table[entryi].name, name);
	strcpy(symbol_table[entryi].kind, kind);
	strcpy(symbol_table[entryi].type, type);
	symbol_table[entryi].id = *index;
	(*index)++;
	entryi++;

	symbol_table = realloc( symbol_table, sizeof(SymbolEntry_t) * (entryi + 1) );
	return symbol_table;

}
