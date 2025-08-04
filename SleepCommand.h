#pragma once
#include "ICommand.h"
class SleepCommand : public ICommand
{
public:
	SleepCommand(int processID, uint8_t sleepTicks) : ICommand(processID, CommandType::SLEEP) 
	{
		this->processID = processID;
		this->sleepTicks = sleepTicks;
	}
	std::shared_ptr<ICommand> clone() const override;
	void execute() override;
	String getOutput() const;

private:
	int processID; 
	uint8_t sleepTicks;
};

