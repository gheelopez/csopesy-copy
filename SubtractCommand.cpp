#include "SubtractCommand.h"

SubtractCommand::SubtractCommand(std::shared_ptr<std::unordered_map<std::string, uint16_t>> symbolTable)
    : ICommand(processID, SUBTRACT), symbolTable(symbolTable)
{
    this->var2 = 0;
    this->var3 = 0;
}

SubtractCommand::SubtractCommand(String target, String op1, String op2, std::shared_ptr<std::unordered_map<std::string, uint16_t>> symbolTable)
    : ICommand(processID, SUBTRACT), symbolTable(symbolTable)
{
    this->var2 = (*symbolTable)[op1];
    this->var3 = (*symbolTable)[op2];
}

void SubtractCommand::execute()
{
    ICommand::execute();
    setVariablesForSubtraction();
    getUniqueVariableName();
    performSubtraction();
}

std::shared_ptr<ICommand> SubtractCommand::clone() const {
    return std::make_shared<SubtractCommand>(*this);
}

void SubtractCommand::setVariablesForSubtraction()
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

void SubtractCommand::getUniqueVariableName()
{
    int varCounter = 0;
    std::string varName = "var";
    do {
        var1 = varName + std::to_string(varCounter);
        varCounter++;
    } while (symbolTable->find(var1) != symbolTable->end());
}

void SubtractCommand::performSubtraction()
{
    if (var2 >= var3) {
        (*symbolTable)[var1] = var2 - var3;
    }
    else {
        (*symbolTable)[var1] = 0;
    }
}

uint16_t SubtractCommand::getResult() const
{
    return (*symbolTable)[var1];
}

std::string SubtractCommand::getOutput() const
{
    return "Subtraction: " + std::to_string(var2) + " - " + std::to_string(var3) + " = " + std::to_string((*symbolTable)[var1]);
}