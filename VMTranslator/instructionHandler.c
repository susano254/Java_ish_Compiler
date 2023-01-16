#include "VMTranslator.h"

void instructionHandler(FILE* targetFile, char *instruction, char *segment, char *index) {
	if ( compare(instruction,"push") ) pushHandler(targetFile, segment, index);
	else if ( compare(instruction,"pop") ) popHandler(targetFile, segment, index);
	else if ( compare(instruction,"add" ) ) arithmaticHandler(targetFile,'+');
	else if ( compare(instruction, "sub") ) arithmaticHandler(targetFile,'-');
	else if ( compare(instruction, "neg") ) arithmaticHandler(targetFile,'\0');
	else if ( compare(instruction, "eq" ) ) arithmaticHandler(targetFile,'=');
	else if ( compare(instruction, "gt" ) ) arithmaticHandler(targetFile,'>');
	else if ( compare(instruction, "lt" ) ) arithmaticHandler(targetFile,'<');
	else if ( compare(instruction, "and") ) arithmaticHandler(targetFile,'&');
	else if ( compare(instruction, "or" ) ) arithmaticHandler(targetFile,'|');
	else if ( compare(instruction, "not") ) arithmaticHandler(targetFile,'!');
	else if ( compare(instruction, "label") ) branchHandler(targetFile, instruction, segment);
	else if ( compare(instruction, "goto") ) branchHandler(targetFile, instruction, segment);
	else if ( compare(instruction, "if-goto") ) branchHandler(targetFile, instruction, segment);
	else if ( compare(instruction,"call") ) procedureHandler(targetFile, instruction, segment, index);
	else if ( compare(instruction,"function") ) procedureHandler(targetFile, instruction, segment, index);
	else if ( compare(instruction,"return" ) ) procedureHandler(targetFile,instruction, "null", "null");
	
}



