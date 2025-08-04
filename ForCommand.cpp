#include "ForCommand.h"
#include "Process.h"

void ForCommand::execute()
{
	ICommand::execute(); // Call base class execute for common behavior
	performForCommand();
}

void ForCommand::performForCommand()
{
	// Loop for the number of repeats
	for (int i = 0; i < this->repeats; i++)
	{
		// Execute each instruction in the instructions list
		for (ICommand* instruction : instructions)
		{
			if (instruction != nullptr)
			{
				instruction->execute();
			}
		}
	}
}

void ForCommand::performWithLogging(Process* process, int coreId, int currentDepth)
{
	process->logInstruction(coreId, "[FOR] Enter depth " + std::to_string(currentDepth) + " with " + std::to_string(repeats) + " repeats");

	for (int i = 0; i < this->repeats; i++)
	{
		for (ICommand* instruction : instructions)
		{
			if (instruction == nullptr) continue;

			// If instruction is another FOR, recurse
			if (auto* nestedFor = dynamic_cast<ForCommand*>(instruction)) {
				nestedFor->performWithLogging(process, coreId, currentDepth + 1);
			}
			else {
				instruction->execute();
				std::string output = instruction->getOutput();
				if (!output.empty())
					process->logInstruction(coreId, output);
			}
		}
	}

	process->logInstruction(coreId, "[FOR] Exit depth " + std::to_string(currentDepth));
}

String ForCommand::getOutput() const {
	String msg = "[FOR] Executed loop with " + std::to_string(repeats) + " repeats";
	return msg;
}
