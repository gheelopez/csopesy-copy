#pragma once
#include "ICommand.h"
class AddCommand : public ICommand
{
public:
	AddCommand(std::shared_ptr<std::unordered_map<std::string, uint16_t>> symbolTable);
	std::shared_ptr<ICommand> clone() const override;
	AddCommand(String target, String op1, String op2, std::shared_ptr<std::unordered_map<std::string, uint16_t>> symbolTable);
	void execute() override;
	String getOutput() const override;

private:
	String var1; // variable name for result
	uint16_t var2; // value 1
	uint16_t var3; // value 2
	std::shared_ptr<std::unordered_map<std::string, uint16_t>> symbolTable;
	void setVariablesForAddition();
	void getUniqueVariableName();
	void performAddition();
	uint16_t getResult() const;
};

