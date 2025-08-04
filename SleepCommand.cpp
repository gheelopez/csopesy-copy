#include "SleepCommand.h"

void SleepCommand::execute()
{
	ICommand::execute();
	IETThread::sleep(this->sleepTicks);
}

std::shared_ptr<ICommand> SleepCommand::clone() const {
	return std::make_shared<SleepCommand>(*this);
}

String SleepCommand::getOutput() const
{
	return "CPU slept for: " + std::to_string(this->sleepTicks);
}