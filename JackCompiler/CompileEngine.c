#include "JackCompiler.h"

SymbolEntry_t *class_symbol_table, *subroutine_symbol_table;
char className[50];
char *identifiers[] = { "array", "variable", "object" };

void compilationEngine(FILE *fp, FILE *outputFile){
	//initalize all varaibles to be used in compilation 
	char token[100] = { 0 }, type[100] = { 0 };


	tokenizer(fp, token, type);
	compileRec(fp, outputFile, token, type);
}




int compileRec(FILE *fp, FILE *outputFile, char *token, char *type){
	char *statements[10] = { "let", "do", "while", "if", "return" };
	char *procedures[3] = { "function", "constructor", "method" };
	char *classvardec[2] = { "static", "field" };

	if( strcmp(token, "class") == 0 ){ 
		compileClass(fp, outputFile, token, type);
		return 0;
	}
	else if( strcmp(token, "var") == 0 ){
		compileVarDec(fp, outputFile, token, type);
		return 0;
	}
	else if( searchStrings(classvardec, 2, token) ){
		compileClassVarDec(fp, outputFile, token, type);
		return 0;
	}
	else if( searchStrings(procedures, 3, token) ){
		compileSubroutine(fp, outputFile, token, type);
		return 0;
	}
	else if( searchStrings(statements, 5, token) ){
		compileStatements(fp, outputFile, token, type);
		return 0;
	}
	else return 1;
}




void compileStatements(FILE *fp, FILE *outputFile, char *token, char *type){
	//char headerTag[50] = "statements";

	do{
		if( strcmp( token, "return" ) == 0 )	
			compileReturnStatement(fp, outputFile, token, type);
		else if( strcmp( token, "if" ) == 0 )
			compileIfStatement(fp, outputFile, token, type);
		else if( strcmp( token, "while" ) == 0 )
			compileWhileStatement(fp, outputFile, token, type);
		else if( strcmp( token, "do" ) == 0 )  
			compileDoStatement(fp, outputFile, token, type);
		else if( strcmp( token, "let" ) == 0 ) 
			compileLetStatement(fp, outputFile, token, type);
		//compileToken(fp, outputFile, token, type, "keyword");
		if( strcmp( token, "}" ) != 0 )	
			tokenizer(fp, token, type);
	}
	while(strcmp( token, "}" ) != 0 );

}






void compileTerm(FILE *fp, FILE *outputFile, char *token, char *type){
	termOutputHandler(outputFile, token, type);

	//if the term is a function call 
	if( (strcmp(type, "className") == 0 || strcmp(type, "object") == 0) && checkPrevChar(fp, 0) == '.' ){

		callHandler(fp, outputFile, token, type);
		
	}

	else if( strcmp(type, "array") == 0 ){
		compileToken(fp, outputFile, token, type, "[");

		compileExpression(fp, outputFile, token, type);

		compileToken(fp, outputFile, token, type, "]");

		vmWriteOp(outputFile, '+', 0);
		vmPopToken(outputFile, "pointer 1");
		vmPushToken(outputFile, "that 0");
	}


	// if it's a nested expression or an array term whose index is an expression example: ((x/y)* (x+y+z[y*x]))
	else if(token[0] == '(' ) {// || checkPrevChar(fp, 0) == '('){
		
		compileExpression(fp, outputFile, token, type);

		compileToken(fp, outputFile, token, type, ")");

	}

}


void compileExpression(FILE *fp, FILE *outputFile, char *token, char *type){
	//char headerTag[50] = "expression";
	char opeartions[20] = "~+<>=/-*|&";
	char terminators[10] = ";)],";
	char op = '\0';
	uint8_t unary;

		do{
			tokenizer(fp, token, type);

			//if it's an operation and it's not a unary one output it else it's a term
			//if(findChar(token[0], opeartions) != -1 && checkPrevChar(fp, -2) != '=' && checkPrevChar(fp, -2) != '(' ) op = token[0]; //outputHandler(outputFile, token, type); 

			if(findChar(token[0], opeartions) != -1 ){ 
				unary = 0;
				if(checkPrevChar(fp, -2) == '=' || checkPrevChar(fp, -2) == '(' || checkPrevChar(fp, -2) == ',') unary = 1;
				op = token[0];
			}
			else { 
				compileTerm(fp, outputFile, token, type);
			}
		}
		while(findChar(checkPrevChar(fp, 0), terminators) == -1);

		//push the operation in the vm language to the stack 
		if( op )	vmWriteOp(outputFile, op, unary);

}



int compileExpressionList(FILE *fp, FILE *outputFile, char *token, char *type){
	//char headerTag[50] = "expressionList";
	int nArgs = 0;


	while(checkPrevChar(fp, 0) != ')'){

		compileExpression(fp, outputFile, token, type);
		nArgs++;

		if( checkPrevChar(fp, 0) == ',' )
			tokenizer(fp, token, type);

	}

	return nArgs;
}


void compileDoStatement(FILE *fp, FILE *outputFile, char *token, char *type){

	//outputHandler(outputFile, token, type);
	
	//compile the call statement and push it's argument to the stack
	tokenizer(fp, token, type);
	callHandler(fp, outputFile, token, type);

	compileToken(fp, outputFile, token, type, ";");
	//since do statement is for void function dump the return value off the stack
	vmPopToken(outputFile, "temp 0");
	
}





void compileReturnStatement(FILE *fp, FILE *outputFile, char *token, char *type){


	//if it's not a void return parse it's expression
	if( checkPrevChar(fp, 0) != ';')
		compileExpression(fp, outputFile, token, type);
	else
		fprintf(outputFile, "push constant 0\n");

	fprintf(outputFile,"return\n");

	compileToken(fp, outputFile, token, type, ";");
}





void compileToken(FILE *fp, FILE *outputFile, char *token, char *type, char *expected){
	tokenizer(fp, token, type);
	if( strcmp(token, expected) != 0 && strcmp(type, expected) != 0 && expected[0] != '\0'){
		printf("invalid token\n");
		exit(-1);
	}
}





void compileIfStatement(FILE *fp, FILE *outputFile, char *token, char *type){
	char ntoken[50], ntype[50];
	char Label1[100] = { 0 };
	char Label2[100] = { 0 };
	static int i = 0;

	sprintf(Label1, "IF_TRUE%d", i);
	sprintf(Label2, "IF_FALSE%d", i);
	i++;

	compileToken(fp, outputFile, token, type, "(");

	compileExpression(fp, outputFile, token, type);

	fprintf(outputFile, "if-goto %s\n", Label1);

	fprintf(outputFile, "goto %s\n", Label2);
	
	compileToken(fp, outputFile, token, type, ")");

	compileToken(fp, outputFile, token, type, "{");

	fprintf(outputFile, "label %s\n", Label1);

	tokenizer(fp, token, type);
 
	compileStatements(fp, outputFile, token, type);


	if( nextToken(fp, ntoken, ntype), strcmp(ntoken, "else") == 0 ){
		char Label3[100] = { 0 };
		sprintf(Label3, "IF_END%d", i);

		fprintf(outputFile, "goto %s\n", Label3);

		fprintf(outputFile, "label %s\n", Label2);

		compileToken(fp, outputFile, token, type, "else");

		compileToken(fp, outputFile, token, type, "{");

		tokenizer(fp, token, type);

		compileStatements(fp, outputFile, token, type);

		fprintf(outputFile, "label %s\n", Label3);
	}
	else 
		fprintf(outputFile, "label %s\n", Label2);
	

}




void compileWhileStatement(FILE *fp, FILE *outputFile, char *token, char *type){
	char Label1[100] = { 0 };
	char Label2[100] = { 0 };
	static int i = 0;

	sprintf(Label1, "WHILE_EXP%d", i);
	sprintf(Label2, "WHILE_END%d", i);
	i++;

	fprintf(outputFile, "label %s\n", Label1);

	compileToken(fp, outputFile, token, type, "(" );

	compileExpression(fp, outputFile, token, type);

	fprintf(outputFile, "not\n");

	fprintf(outputFile, "if-goto %s\n", Label2);
	
	compileToken(fp, outputFile, token, type, ")");

	compileToken(fp, outputFile, token, type, "{");

	tokenizer(fp, token, type);

	compileStatements(fp, outputFile, token, type);

	fprintf(outputFile, "goto %s\n", Label1);

	fprintf(outputFile, "label %s\n", Label2);

}
	





void compileLetStatement(FILE *fp, FILE *outputFile, char *token, char *type){
	char variableAddress[50] = { 0 };
	char destination[50] = { 0 };

	//compiling the destination part 
	do{	
		tokenizer(fp, token, type);
		if( strcmp(type, "variable") == 0 ){
			variableSearch(token, variableAddress);
			strcpy(destination, "variable");
		}
		else if( strcmp(type, "array") == 0 ){
			variableSearch(token, variableAddress);
			strcpy(destination, "array");
		}
		else if( token[0] == '[' ) { 
			vmPushToken(outputFile, variableAddress);
			compileExpression(fp, outputFile, token, type);
			vmWriteOp(outputFile, '+', 0);
			vmPopToken(outputFile, "temp 0");
		}
	
	}
	while(token[0] != '=');

	compileExpression(fp, outputFile, token, type);


	//fprintf(outputFile, "//outputting to the destination\n");
	if( strcmp(destination, "variable") == 0) vmPopToken(outputFile, variableAddress);
	else if( strcmp(destination, "array") == 0) {
		vmPushToken(outputFile, "temp 0");
		vmPopToken(outputFile, "pointer 1");
		vmPopToken(outputFile, "that 0");
	}

	compileToken(fp, outputFile, token, type, ";");
}


	

	
void compileClass(FILE *fp, FILE *outputFile, char *token, char *type){
	class_symbol_table = (SymbolEntry_t *) malloc(sizeof(SymbolEntry_t));
	entryi = 0;
	fieldi = 0;
	statici = 0;

	do{
		tokenizer(fp, token, type);
		if( strcmp( type, "className" ) == 0 ) strcpy(className, token);
		if( compileRec(fp, outputFile, token, type) ) outputHandler(outputFile, token, type);
	}
	while( strcmp(token, "}") != 0 );

	free(class_symbol_table);
}

		

void compileClassVarDec(FILE *fp, FILE *outputFile, char *token, char *type){
	char kind[100], identifyerType[100], name[100];

	strcpy(kind, token);

	//compile the type keyword ex. int
	compileToken(fp, outputFile, token, type, "\0");
	strcpy(identifyerType, token);

	do{
		tokenizer(fp, token, type);
		if( searchStrings(identifiers, 3, type) ) {
			strcpy(name, token);
			class_symbol_table = symbolTable(class_symbol_table, name, identifyerType, kind);
			classTableSize = entryi;
		}
		outputHandler(outputFile, token, type);
	}
	while( strcmp(token, ";") != 0 );

}



void compileVarDec(FILE *fp, FILE *outputFile, char *token, char *type){
	//char headerTag[50] = "varDec";
	char kind[100], identifyerType[100], name[100];

	strcpy(kind, "local");

	//compile the type keyword ex. int
	compileToken(fp, outputFile, token, type, "\0");
	strcpy(identifyerType, token);


	do{
		tokenizer(fp, token, type);
		if( searchStrings(identifiers, 3, type) ) {
			strcpy(name, token);
			subroutine_symbol_table = symbolTable(subroutine_symbol_table, name, identifyerType, kind);
			subTableSize = entryi;
		}
		outputHandler(outputFile, token, type);
	}
	while( strcmp(token, ";") != 0 );

}









void compileSubroutine(FILE *fp, FILE *outputFile, char *token, char *type){
	char subName[100] = { 0 };
	char subType[100] = { 0 };
	vari = 0;
	argumenti = 0;
	entryi = 0;
	subroutine_symbol_table = (SymbolEntry_t *) malloc(sizeof(SymbolEntry_t));
	
	strcpy(subType, token);
	//write the name of the function
	compileToken(fp, outputFile, token, type, "\0");
	compileToken(fp, outputFile, token, type, "function");
	strcpy(subName, token);


	//treat each function type differently
	if( strcmp( subType, "method" ) == 0 )
		methodHandler(fp, outputFile, token, type, subName);
	else if( strcmp( subType, "constructor" ) == 0 )
		constructorHandler(fp, outputFile, token, type, subName);
	else if( strcmp( subType, "function" ) == 0 )
		functionHandler(fp, outputFile, token, type, subName);
	
	free(subroutine_symbol_table);
	token[0] = '\0';

}

void compileVarDeclerations(FILE *fp, FILE *outputFile, char *token, char *type){
	tokenizer(fp, token, type);
	if(strcmp(token, "var") == 0){
		do{
			compileVarDec(fp, outputFile, token, type);
			tokenizer(fp, token, type);
		}
		while( strcmp(token, "var") == 0);
	}
}



void compileArgument(FILE *fp, char *token, char *type){
	char kind[100], identifyerType[100];
	strcpy(kind, "argument");

	//compile argument type
	tokenizer(fp, token, type);
	if( strcmp( type, "keyword" ) == 0 || (token[0] >= 'A' && token[0] <= 'Z') ) 
		strcpy( identifyerType, token );
	else {
		printf("didn't find a proper identifyerType\n");
		exit(-1);
	}

	//compile Argument itself
	tokenizer(fp, token, type);
	if( searchStrings(identifiers, 3, type) ) {
		subroutine_symbol_table = symbolTable( subroutine_symbol_table, token, identifyerType, kind );
		subTableSize = entryi;
	}
	else{
		printf("didn't find a proper argument name\n");
		exit(-1);
	}
}




void parameterListHandler(FILE *fp, FILE *outputFile, char *token, char *type){ 


	while(checkPrevChar(fp, 0) != ')' ){
		compileArgument(fp, token, type);
		if( checkPrevChar(fp , 0) != ')' )	compileToken(fp, outputFile, token, type, ",");
		//tokenizer(fp, token, type);
		//if( strcmp( type, "keyword" ) == 0 ) strcpy( identifyerType, token );
		//if( searchStrings(identifiers, 3, type) ) {
		//	strcpy(name, token);
		//	subroutine_symbol_table = symbolTable( subroutine_symbol_table, name, identifyerType, kind );
		//	subTableSize = entryi;
		//}
	}

}









void methodHandler(FILE *fp, FILE *outputFile, char *token, char *type, char *subName){
	compileToken(fp, outputFile, token, type, "(");

	//push the this command to the symbol table
	subroutine_symbol_table = symbolTable(subroutine_symbol_table, "this", className, "argument");
	subTableSize = entryi;

	parameterListHandler(fp, outputFile, token, type);

	compileToken(fp, outputFile, token, type, ")");

	compileToken(fp, outputFile, token, type, "{");

	compileVarDeclerations(fp, outputFile, token, type);

	fprintf(outputFile, "function %s.%s %d\n", className, subName, vari);

	//pop what's in argument 0 to pointer 0
	vmPushToken(outputFile, "argument 0");
	vmPopToken(outputFile, "pointer 0");

	compileStatements(fp, outputFile, token, type);
}





void constructorHandler(FILE *fp, FILE *outputFile, char *token, char *type, char *subName){
	compileToken(fp, outputFile, token, type, "(");

	//push the this command to the symbol table
	subroutine_symbol_table = symbolTable(subroutine_symbol_table, "this", className, "pointer");
	subTableSize = entryi;

	parameterListHandler(fp, outputFile, token, type);

	compileToken(fp, outputFile, token, type, ")");

	compileToken(fp, outputFile, token, type, "{");

	compileVarDeclerations(fp, outputFile, token, type);

	fprintf(outputFile, "function %s.%s %d\n", className, subName, vari);

	//allocate memory for the new created object and return this
	//calculate the number of the field variables in the class Symbol Table pass it as the size to Memory.alloc(s) and pop the result to local 0 (this)
	//at the end of the constructor compilation the return statement will push the this (local 0) to the return value
	fprintf(outputFile, "push constant %d\n", fieldi);
	fprintf(outputFile, "call Memory.alloc 1\n");
	fprintf(outputFile, "pop pointer 0\n");


	compileStatements(fp, outputFile, token, type);
}




void functionHandler(FILE *fp, FILE *outputFile, char *token, char *type, char *subName){
	compileToken(fp, outputFile, token, type, "(");

	parameterListHandler(fp, outputFile, token, type);

	compileToken(fp, outputFile, token, type, ")");

	compileToken(fp, outputFile, token, type, "{");

	compileVarDeclerations(fp, outputFile, token, type);

	fprintf(outputFile, "function %s.%s %d\n", className, subName, vari);

	compileStatements(fp, outputFile, token, type);
}


void callHandler(FILE *fp, FILE *outputFile, char *token, char *type){
	char functionName[100] = { 0 };
	int nArgs = 0;
	int push = 0;

	//if it's a method or a class function make up it's name and push any standard argument
	if(checkPrevChar(fp, 0) == '.'){
		if( strcmp( type, "className" ) == 0 )
			strcat(functionName, token);
		else if( strcmp(type, "object") == 0) {
			int index;
			variableOutputHandler(outputFile, token, type);
			nArgs++;

			//name of the function class is the type of the object that called it 
			if( (index = searchSymbolTable(subroutine_symbol_table, subTableSize, token)) != -1 )
				strcat(functionName, subroutine_symbol_table[index].type);
			
			else if( (index = searchSymbolTable(class_symbol_table, classTableSize, token)) != -1 )
				strcat(functionName, class_symbol_table[index].type);
		}

		//concatenate the '.' to the name
		compileToken(fp, outputFile, token, type, ".");
		strcat(functionName, token);
		//concatenate the function name
		compileToken(fp, outputFile, token, type, "method");
		strcat(functionName, token);
	}
	else {
		push = 1;
		sprintf(functionName, "%s.%s", className, token);
	}
	

	compileToken(fp, outputFile, token, type, "(");

	if(push) {
		fprintf(outputFile, "push pointer 0\n");
		nArgs++;
	}

	nArgs += compileExpressionList(fp, outputFile, token, type);

	compileToken(fp, outputFile, token, type, ")");

	fprintf(outputFile, "call %s %d\n", functionName, nArgs);
}
