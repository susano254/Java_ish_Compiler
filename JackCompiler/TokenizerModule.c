#include "JackCompiler.h"

const char* keywords[21] =
{ 
	"class", "constructor", "function", "method", "field",
	"static", "var", "int", "char", "boolean", "void", "true", 
	"false", "null", "this", "let", "do", "if", "else", "while", "return"
};

void nextToken(FILE *fp, char *token, char *tokenType){
	int pullBack;
	tokenizer(fp, token, tokenType);
	pullBack = -(strlen(token)) - 1;
	fseek(fp, pullBack, SEEK_CUR);
}




void tokenizer(FILE *fp, char* token, char* tokenType) { 
	char buffer[1000], type[30];
	int boolComment = 0;


	if( !(boolComment = commentHandler(fp)) ) { 
		myScanf(fp,buffer);
		tokenTypeHandler(fp, buffer, type);
	
		//for feedback into the console 
		if(buffer[0] != '\0') { 
			printf("\n===========> buffer is:- %s\n", buffer);
			printf("===========> type is:- %s\n\n", type);

		}
		strcpy(token, buffer);
		strcpy(tokenType, type);
	}
	if( token[0] == '\0' && checkChar(fp, 0) != EOF ) tokenizer(fp, token, tokenType);
}





void myScanf(FILE* fp, char* buffer){
	int i = 0;
	char token[100] = { 0 };
	strcpy(buffer, token);

	char c = fgetc(fp);

	while( (i < 100) && (c > 0) ) { 
		// if it's a symbol and the first one to be encountered in the current token iteration store it else ignore it
		if( symbol(c) && i == 0 ){
			token[i] = c;
			break;
		}
		//ignore the symbol 
		if( symbol(c) ) {
			fseek(fp, -1, SEEK_CUR);
			break;
		}
		//in case it's not a symbol it may be a non printable char like carriage return ,space or a newline etc...
		if( c <= 32 ) break;

		//if it's not a symbol or non char add it to token 
		token[i] = c;
		printf("  token is:- %s\n", token);
		
		//advance the token to be ready to store the next valid char
		i++;

		// get the next char and repeat
		c = fgetc(fp);
	}
	strcpy(buffer, token);
}





void tokenTypeHandler(FILE* fp, char* token, char* type) { 
	//if it's a symbol and not a double quot return symbol type 
	if( symbol(token[0]) && token[0] != '"'){
		//symbolHandler(fp, token);
		strcpy(type,"symbol");
		return;
	}
	// if it's a double quote ignore the double quote and call stringConstHandler to store the string without the quotes
	else if( token[0] == '"' ){ 
		printf("in tokenTypeHandler token[0] = %c\n", token[0]);
		stringConstHandler(fp, token);
		strcpy(type, "stringConstant");
		return;
	}
	// if it's between 48 and 57 ( i.e. between 1 and 9 in ascii ) then it's a string const
	else if( token[0] >= '0' && token[0] <= '9' ){
		strcpy(type, "integerConstant");
		return;
	}
	// search if it's a keyword
	for( int i = 0; i < 21; i++ ) {
		if( strcmp(token, keywords[i]) == 0 ) { 
			strcpy(type,"keyword");
			return;
		}
	}
	// if non of the above then it's an identifier
	if( token[0] != '\0' ){
		identifierHandler(fp, token, type);
	}
}





int symbol(char c){
	int found = 0;
	char symbols[] = "~()[]{}.,;+-*/&|<>=\""; 
	for( int i = 0; i < strlen(symbols); i++) { 
		if( c == symbols[i] ) { 
			found = 1;
			break;
		}
	}
	return found;
}







//void symbolHandler(FILE *fp, char *token){
//	char c;
//	c = token[0];
//
//	switch(c) {
//		case '>':
//			strcpy(token, "&gt;");
//			break;
//		case '<':
//			strcpy(token, "&lt;");
//			break;
//		case '&':
//			strcpy(token, "&amp;");
//			break;
//	}
//}


void stringConstHandler(FILE *fp, char *token){
	char c;
	char buffer[100] = { 0 };
	strcpy(token, "\0");


	for( int i = 0; (checkChar(fp, 0) != '"'); i++) { 
		c = fgetc(fp);
		printf("%c", c);
		buffer[i] = c;
	}
	fseek(fp, 1, SEEK_CUR); // advance the file pointer to not read the last double quote again 
	strcpy(token, buffer);
}





void identifierHandler(FILE *fp, char *token, char *type){
	char identifyerType[100];

	if( (token[0] > 64)&&(token[0] < 91) && (checkPrevChar(fp, 0) == '.' || checkPrevChar(fp, 0) == '{' ) )
		strcpy(identifyerType, "className"); 
	else if( checkChar(fp, 0) == '.' ) {
		strcpy(identifyerType, "object");
	}
	else if( (checkPrevChar(fp, -(strlen(token) + 1) ) == '.') && (checkChar(fp, 0) == '(') ) strcpy(identifyerType, "method");
	else if( checkPrevChar(fp, 0) == '(' ) strcpy(identifyerType, "function");
	else if( checkPrevChar(fp, 0) == '[' ) strcpy(identifyerType, "array");
	else strcpy(identifyerType, "variable");

	strcpy(type, identifyerType);
	printf("\n===========> identifyerType is:- %s\n", identifyerType);
}





