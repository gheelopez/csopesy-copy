#include "AddCommand.h"

AddCommand::AddCommand(std::shared_ptr<std::unordered_map<std::string, uint16_t>> symbolTable) 
    : ICommand(processID, ADD), symbolTable(symbolTable) 
{
	this->var2 = 0;
	this->var3 = 0;
}

// make another constructor here for mo2
AddCommand::AddCommand(String target, String op1, String op2, std::shared_ptr<std::unordered_map<std::string, uint16_t>> symbolTable)
	: ICommand(processID, ADD), symbolTable(symbolTable)
{
	this->var2 = (*symbolTable)[op1];
    this->var3 = (*symbolTable)[op2];
}

void AddCommand::execute()
{
	ICommand::execute(); // Call base class execute for common behavior
	setVariablesForAddition();
	getUniqueVariableName();
	performAddition();
}

std::shared_ptr<ICommand> AddCommand::clone() const {
	return std::make_shared<AddCommand>(*this);
}

void AddCommand::setVariablesForAddition()
{
    if (symbolTable->size() >= 2) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(0, symbolTable->size() - 1);

        int idx1 = distrib(gen);
        int idx2;
        do {
            idx2 = distrib(gen);
        } while (idx2 == idx1);

        auto it = symbolTable->begin();
        std::advance(it, idx1);
        var2 = it->second;

        it = symbolTable->begin();
        std::advance(it, idx2);
        var3 = it->second;
    }
    else if (symbolTable->size() == 1) {
        auto it = symbolTable->begin();
        var2 = it->second;
        var3 = 0;
    }
}

void AddCommand::getUniqueVariableName() { // generate a new variable name for the result
	int varCounter = 0;
	String varName = "var";

	do {
		var1 = varName + std::to_string(varCounter);
		varCounter++;
	} while (symbolTable->find(var1) != symbolTable->end());
}

void AddCommand::performAddition()
{
	(*symbolTable)[var1] = this->var2 + this->var3;
}

uint16_t AddCommand::getResult() const
{
	return (*symbolTable)[var1];
}

String AddCommand::getOutput() const
{
	return "Addition: " + std::to_string(var2) + " + " + std::to_string(var3) + " = " + std::to_string((*symbolTable)[var1]);
}