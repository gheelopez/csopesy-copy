#pragma once
#include "ICommand.h"

class PrintCommand : public ICommand
{
public:
	PrintCommand(int processID, String processName, std::shared_ptr<std::unordered_map<std::string, uint16_t>> symbolTable);
	PrintCommand(int processID, String processName, std::shared_ptr<std::unordered_map<std::string, uint16_t>> symbolTable, String remainingLine);
	std::shared_ptr<ICommand> clone() const override;
	void execute() override;
	void setString();
	String getOutput() const override;
private:
	String expression;
	String toPrint;
	String processName;
	std::shared_ptr<std::unordered_map<std::string, uint16_t>> symbolTable;
};

