#include "VMTranslator.h"

void funcDecHandler(FILE* assemblyFile, char* funcLabel, char* nLocals) {
	strcpy(returnLabel, funcLabel);
	fprintf(assemblyFile, "\n\n\n\t\t\t\t\t\t(%s)\n", funcLabel);
	for( int i = 0; i < atoi(nLocals); i++) pushHandler(assemblyFile, "constant", "0");
}
