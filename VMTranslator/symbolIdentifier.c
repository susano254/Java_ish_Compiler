#include "VMTranslator.h"


void symbolIdentifier(char *SEGB, char* segment, char* index) {
	//char operand = 'M';
	//char *Asegments[] = { "pointer", "temp" };
	char extenstion[20] = ".";

	/*for ( uint8_t i = 0; i < sizeof(Asegments) / sizeof(Asegments[1]); i++){
		if ( compare(segment, Asegments[i]) ) operand = 'A';
	}
	*/

	if	   ( compare(segment, "argument") ) strcpy(SEGB,"ARG");
	/*
	else if( compare(segment, "REG" ) ){ 
		strcpy(SEGB, index);
		operand = 'A';
	}
	*/
	else if( compare(segment, "local" )   ) strcpy(SEGB, "LCL");
	else if( compare(segment, "this" )   ) strcpy(SEGB, "THIS");
	else if( compare(segment, "that" )   ) strcpy(SEGB, "THAT");
	else if( compare(segment, "pointer") ) strcpy(SEGB, "3");
	else if( compare(segment, "temp" )  )   strcpy(SEGB, "5");
	else if( compare(segment, "static") ) { 
		strcpy(SEGB, staticSymbol);
		printf("SEGB is: %s\n", SEGB);
		strcat(extenstion, index);
		strcat(SEGB, extenstion);
		printf("SEGB is: %s\n", SEGB);
		strcpy(index, "0");
		printf("%s\n", SEGB);
	}
}


