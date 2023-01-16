#include "JackCompiler.h"

int main(int argc, char** args){
	int fileLen;
	FILE *fp, *outputFile;
	char  outputFilePath[100]; //token[100] = { 0 }, tokenType[100] = { 0 };

	inputChecker(argc, args[1], ".jack");
	fileLen = directoryParser(args[1]);
	if(fileLen == 0 ){
		printf("argument to be added to files is %s\n", args[1]);
		addToFiles(args[1]);
		fileLen++;
	}

	printf("fileLen is equal to %d\n", fileLen);
	for(int i = 0; i < fileLen; i++){

		fp = fopen(files[i],"r");
		extModifier(files[i], outputFilePath, ".vm");
		outputFile = fopen(outputFilePath, "w");
		printf("%s\n", files[i]); 

		compilationEngine(fp, outputFile);
		
		fclose(fp);
		fclose(outputFile);
	}
	free(files);
	return 0;
}




