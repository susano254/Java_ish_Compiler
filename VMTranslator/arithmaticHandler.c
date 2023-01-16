#include "VMTranslator.h"


void arithmaticHandler(FILE* targetFile, char operation){
	static int n = 0;
	switch(operation) {
		case '\0':
			fprintf(targetFile, "\n // performing a negation operation\n");
			fprintf(targetFile,"@SP\nA=M-1\nM=-M\n");
			break;
		case '=':
			fprintf(targetFile, "\n // performing a sub operation and testing for EQ \n");
			fprintf(targetFile,"@SP\nA=M-1\nD=M\nA=A-1\nD=M-D\n@SP\nM=M-1\n@EQTRUE%d\nD;JEQ\n@SP\nA=M-1\nM=0\n@EXITEQ%d\n0;JMP\n(EQTRUE%d)\n@SP\nA=M-1\nM=-1\n(EXITEQ%d)\n",n,n,n,n);	
			n++;
			break;
		case '>':
			fprintf(targetFile, "\n // performing a sub operation and testing for GT \n");
			fprintf(targetFile,"@SP\nA=M-1\nD=M\nA=A-1\nD=M-D\n@SP\nM=M-1\n@GTTRUE%d\nD;JGT\n@SP\nA=M-1\nM=0\n@EXITGT%d\n0;JMP\n(GTTRUE%d)\n@SP\nA=M-1\nM=-1\n(EXITGT%d)\n",n,n,n,n);	
			n++;
			break;
		case '<':
			fprintf(targetFile, "\n // performing a sub operation and testing for LT \n");
			fprintf(targetFile,"@SP\nA=M-1\nD=M\nA=A-1\nD=M-D\n@SP\nM=M-1\n@LTTRUE%d\nD;JLT\n@SP\nA=M-1\nM=0\n@EXITLT%d\n0;JMP\n(LTTRUE%d)\n@SP\nA=M-1\nM=-1\n(EXITLT%d)\n",n,n,n,n);	
			n++;
			break;
		case '&':
			fprintf(targetFile, "\n // performing an AND '&' operation\n");
			fprintf(targetFile,"@SP\nA=M\nA=A-1\nD=M\nA=A-1\nD=M%cD\n@SP\nM=M-1\nA=M-1\nM=D\n",operation);
			break;
		case '|':
			fprintf(targetFile, "\n // performing an OR '|' operation\n");
			fprintf(targetFile,"@SP\nA=M\nA=A-1\nD=M\nA=A-1\nD=M%cD\n@SP\nM=M-1\nA=M-1\nM=D\n",operation);
			break;
		case '!':
			fprintf(targetFile, "\n // performing a NOT '!' operation\n");
			fprintf(targetFile,"@SP\nA=M-1\nM=!M\n");
			break;
		case '+':
			fprintf(targetFile, "\n // performing an ADD '+' operation\n");
			fprintf(targetFile,"@SP\nA=M\nA=A-1\nD=M\nA=A-1\nD=M%cD\n@SP\nM=M-1\nA=M-1\nM=D\n",operation);
			break;
		case '-':
			fprintf(targetFile, "\n // performing a SUB '-' operation\n");
			fprintf(targetFile,"@SP\nA=M\nA=A-1\nD=M\nA=A-1\nD=M%cD\n@SP\nM=M-1\nA=M-1\nM=D\n",operation);
			break;
	}
}


