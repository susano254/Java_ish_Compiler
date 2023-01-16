

char staticSymbol[20];
uint8_t directory = 0;
int numberOfFiles= 0;



	
int main (int argc, char** argument) {
	FILE *pVmFile, *pAssemblyFile;
	int slashIndex;
	char **files, instruction[20], segment[20], index[20], arg1[30];
	printf("\n\t\t\t\t\t\targument taken is %s\n", argument[1]);


	if( argc != 2 ) {
		printf("You supposed to specify one argument as a .vm file or directory containging a number of .vm files\n");
		return 1;
	}


	if ( ( findCharacter(argument[1], '.' ) )  ==  -1) {
		strcpy(arg1, argument[1]);
		if( (slashIndex = findCharacter(arg1, '/') ) == -1 ) { 
			printf("concatenating '/' to arg1\n");
			strcat(arg1, "/"); 
			slashIndex = findCharacter(arg1, '/');
		}

		printf("\t\t\t\t\t\t\tdirecotry to be opened is %s\n", arg1);
		directory = 1;
		//printf("\t\t\t\t\t\t\tyour directory is: %s\n", arg1);

		directoryParser(&files, arg1);

		printf("\n\t\tNUMBER OF FILES IN FILES IS %d\n", numberOfFiles);
		for ( int i = 0; i < numberOfFiles; i++) { 
			printf("file path is %s\n", files[i]);
		}			
		for( int i = 0; i < numberOfFiles; i++) {
			printf("%s\n", &(files[i])[slashIndex+1]);
			if ( compare(&(files[i])[slashIndex+1], "Sys.vm") ) { 
				pAssemblyFile = OpenStreamFile(&pVmFile, files[i], arg1);
				//fprintf(pAssemblyFile, "@256\nD=A\n@SP\nM=D\n@Sys.init\n0;JMP\n\n");
				fprintf(pAssemblyFile, "@256\nD=A\n@SP\nM=D\n\n\n");
				callHandler(pAssemblyFile, "Sys.init", "0");
				fclose(pVmFile);
				fclose(pAssemblyFile);
			}
		}

		for( int counter = 0; counter < numberOfFiles; counter++) {
			pAssemblyFile = OpenStreamFile(&pVmFile, files[counter], arg1);
			fprintf(pAssemblyFile, "\n\n\t\t\t\t\t\t\t\t\t\t//currently in the %d file which is %s\n", counter, files[counter]);
			while(1) {
				parser(pVmFile, instruction, segment, index);
				if( feof(pVmFile) ){
					fprintf(pAssemblyFile, "\n\t\t\t\t\t\t\t\t\t// Finished the %d file\n",counter );
					break;
				}
				instructionHandler(pAssemblyFile, instruction, segment, index);
			}
			strcpy(instruction, "null");
			fclose(pVmFile);
			fclose(pAssemblyFile);
		}
	}
	else {
						
		pAssemblyFile = OpenStreamFile(&pVmFile,argument[1], argument[1]);
		//printf("%p\n", pAssemblyFile);
		//printf("%p\n", pVmFile);
		while(1) {
			parser(pVmFile, instruction, segment, index);
			if ( feof(pVmFile) ) return 0;
			instructionHandler(pAssemblyFile, instruction, segment, index);
		}
		return 0;
	}

}
