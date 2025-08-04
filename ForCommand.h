#pragma once
#include "ICommand.h"
#include <vector>

class Process;

class ForCommand : public ICommand
{
public:
	ForCommand(int processID, std::vector<ICommand*> instructions, int repeats) : ICommand(processID, CommandType::FOR)
	{
		this->processID = processID;
		this->instructions = instructions;
		this->repeats = repeats;
	}
	void performForCommand();
	void performWithLogging(Process* process, int coreId, int currentDepth=1);
	void execute() override;
	String getOutput() const override;
	//std::shared_ptr<ICommand> clone() const override;

private:
	int processID;
	std::vector<ICommand*> instructions;
	int repeats;
};

