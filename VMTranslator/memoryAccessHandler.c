#include "VMTranslator.h"

	
void popHandler(FILE* targetFile, char *segment, char *index) {
	fprintf(targetFile,"\n\t\t\t// performing a pop %s %s operation\n", segment, index);
	char SEGB[20], operand = 'M';
	int RegNum;
	symbolIdentifier(SEGB, segment, index);

	if ( compare(segment, "static") ) fprintf(targetFile,"@SP\nAM=M-1\nD=M\n@%s\nM=D\n", SEGB);
	else if ( compare(segment, "pointer") || compare(segment, "temp") ) { 
		RegNum = atoi(SEGB) + atoi(index);
		printf("RegNum is %d\n", RegNum);
		fprintf(targetFile, "@SP\nAM=M-1\nD=M\n@%d\nM=D\n", RegNum);
	}
	else fprintf(targetFile, "@SP\nM=M-1\nA=M\nD=M\n@R13\nM=D\n@%s\nD=%c\n@%s\nD=D+A\n@R14\nM=D\n@R13\nD=M\n@R14\nA=M\nM=D\n", SEGB, operand, index);
}






void pushHandler(FILE* targetFile, char *segment, char *index) {
	fprintf(targetFile,"\n\t\t\t// performing a push %s %s operation\n", segment, index);
	char SEGB[20], operand = 'M';
	int RegNum;
	symbolIdentifier(SEGB, segment, index);

	if ( compare(segment,"constant") ) fprintf(targetFile,"@%s\nD=A\n@SP\nA=M\nM=D\n@SP\nM=M+1\n",index);
	else if ( compare(segment, "static") ) fprintf(targetFile,"@%s\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n", SEGB);
	else if ( compare(segment, "pointer") || compare(segment, "temp") ) { 
		RegNum = atoi(SEGB) + atoi(index);
		printf("RegNum is %d\n", RegNum);
		fprintf(targetFile, "@%d\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n", RegNum);
	}
	else fprintf(targetFile, "@%s\nD=%c\n@%s\nD=D+A\nA=D\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n", SEGB, operand, index);
}


