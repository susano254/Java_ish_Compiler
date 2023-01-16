#ifndef Compiler_H
#define Compiler_H
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>

extern char** files;
extern const char* keywords[21];
extern char *identifiers[10];
extern unsigned char statici, fieldi, argumenti, vari, entryi, classTableSize, subTableSize;

typedef struct {
	char name[100];
	char type[100];
	char kind[100];
	char id;
}SymbolEntry_t;

extern SymbolEntry_t *class_symbol_table, *subroutine_symbol_table;



//file and string pasrers 
void outputHandler(FILE*, char*, char*); 
void tokenTypeHandler(FILE*, char*, char*); 
void symbolHandler(FILE *, char *);
void integerConstHandler(char*);
void identifierHandler(FILE*, char*, char*);
void stringConstHandler(FILE*, char*);
void addToFiles(char*);
char checkChar(FILE*, int);
char checkPrevChar(FILE *, int);
int commentHandler(FILE*);
int directoryParser(char*);
int findChar(char, char*);
void inputChecker(int, char*, const char*);
void myScanf(FILE*, char*);
int	symbol(char);
void skipLine(FILE*);
void skipUntilRegEx(FILE*, char*, int);
void tokenizer(FILE*, char*, char*);
void extModifier(char*, char*, char*);

// recursive compilation subroutines 
void compilationEngine(FILE *, FILE *);
int searchStrings(char **, int , char *);
void compileToken(FILE *, FILE *, char *, char *, char *);
void nextToken(FILE *, char *, char *);
int compileRec(FILE *, FILE *, char *, char *);
void compileClass(FILE*, FILE*, char*, char*);
void compileClassVarDec(FILE *, FILE *, char *, char *);
void compileVarDec(FILE *, FILE *, char *, char *);
void compileVarDeclerations(FILE *, FILE *, char *, char *);
void compileSubroutine(FILE*, FILE*, char*, char*);
void parameterListHandler(FILE* , FILE* , char* , char* ); 
void compileSubroutineBody(FILE *, FILE *, char *, char *);
void compileStatements(FILE*, FILE*, char*, char*);
void compileReturnStatement(FILE *, FILE *, char *, char *);
void compileIfStatement(FILE *, FILE *, char *, char *);
void compileWhileStatement(FILE *, FILE *, char *, char *);
void compileDoStatement(FILE *, FILE *, char *, char *);
void compileLetStatement(FILE *, FILE *, char *, char *);
void compileTerm(FILE *, FILE *, char *, char *);
void compileExpression(FILE *, FILE *, char *, char *);
int compileExpressionList(FILE *, FILE *, char *, char *);
SymbolEntry_t *symbolTable(SymbolEntry_t *, char *, char *, char *);


//handlers
void methodHandler(FILE *, FILE *, char *, char *, char *);
void constructorHandler(FILE *, FILE *, char *, char *, char *);
void functionHandler(FILE *, FILE *, char *, char *, char *);
void callHandler(FILE *, FILE *, char *, char *);


// vm code writers 
void vmWriteOp(FILE *, char, uint8_t);
void termOutputHandler(FILE *, char *, char *);
void vmPushToken(FILE *, char *);
void vmPopToken(FILE *, char *);

//extra one for debugging 
void printSymbolTable(SymbolEntry_t *, unsigned char);
int searchSymbolTable(SymbolEntry_t *, unsigned char , char* );
void variableOutputHandler(FILE *, char *, char *);
void variableSearch(char *, char *);

#endif
