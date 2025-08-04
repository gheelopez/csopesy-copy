#pragma once
#include "ICommand.h"
class SubtractCommand : public ICommand
{
public:
	SubtractCommand(std::shared_ptr<std::unordered_map<std::string, uint16_t>> symbolTable);
	SubtractCommand(String var1, String var2, String var3, std::shared_ptr<std::unordered_map<std::string, uint16_t>> symbolTable);
	std::shared_ptr<ICommand> clone() const override;
	void execute() override;
	String getOutput() const override;

private:
	String var1; // variable name for result
	uint16_t var2; // value 1
	uint16_t var3; // value 2
	std::shared_ptr<std::unordered_map<std::string, uint16_t>> symbolTable;
	void setVariablesForSubtraction();
	void getUniqueVariableName();
	void performSubtraction();
	uint16_t getResult() const;
};

