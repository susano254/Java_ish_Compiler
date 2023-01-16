#include "JackCompiler.h"

void inputChecker(int argc, char* arg1, const char* ext){ 
	int pointIndex;
	pointIndex = findChar('.', arg1);
	if ( argc < 2) {
		printf("You must pass one argument either a .jack file or a directory containing one or more .jack files\n");
		exit(-1);
	} 
	else if( (pointIndex != -1) && (strcmp(&arg1[pointIndex], ext) != 0) ){
		printf("This is not a %s File You must enter a valid jack File with extenstion \"%s\"\n", ext, ext);
		exit(-1);
	}
}

