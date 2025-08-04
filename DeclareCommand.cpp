#include "DeclareCommand.h"

// Constructor for DECLARE: takes variable name and initial value
DeclareCommand::DeclareCommand(const String& varName, uint16_t value, std::shared_ptr<std::unordered_map<std::string, uint16_t>> symbolTable)
    : ICommand(processID, CommandType::DECLARE), newVar(varName), value(value), symbolTable(symbolTable) {
}

//DeclareCommand::DeclareCommand(int processID, const String& varName, uint16_t value,
//	std::shared_ptr<std::unordered_map<std::string, uint16_t>> symbolTable)
//	: ICommand(processID, CommandType::DECLARE), newVar(varName), value(value), symbolTable(symbolTable) {
//}

std::shared_ptr<ICommand> DeclareCommand::clone() const {
	return std::make_shared<DeclareCommand>(*this);
}

void DeclareCommand::execute()
{
	ICommand::execute();
	if (this->newVar.empty()) {
		// If no variable name is provided, generate a unique one
		getUniqueVariableName();
	}
	else {
		declareVariable(this->newVar);
	}
	declareVariable(this->newVar);
}

void DeclareCommand::getUniqueVariableName() {
	int varCounter = 0;
	String varName = "var";

	do {
		newVar = varName + std::to_string(varCounter);
		varCounter++;
	} while (symbolTable->find(newVar) != symbolTable->end());
}

void DeclareCommand::declareVariable(const String varName) {
	this->newVar = varName;
	// this->value = static_cast<uint16_t>(rand() % 65536); // clamp range of uint16_t from 0 to 65535
	(*symbolTable)[newVar] = value;
}

String DeclareCommand::getVariableName() {
	return this->newVar;
}

uint16_t DeclareCommand::getValue() const {
	return this->value;
}

String DeclareCommand::getOutput() const {  
	String msg = this->newVar + " declared with the value: " + std::to_string(this->value) + "!";
    return msg;  
}
