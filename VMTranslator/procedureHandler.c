#include "VMTranslator.h"

char returnLabel[30] = "null";

void procedureHandler(FILE* assemblyFile, char* instruction, char* segment, char* index){
	if( compare(instruction, "call") ) {
		callHandler(assemblyFile, segment, index);
	}
	else if( compare(instruction, "function") ) {
		funcDecHandler(assemblyFile, segment, index);
	}
	else if ( compare(instruction, "return") ) { 
		returnHandler(assemblyFile);
	}
}
