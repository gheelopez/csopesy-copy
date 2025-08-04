#include <iostream>
#include <string>
#include <sstream>
#include "IETThread.h"
#include "CommandPrinter.h"

CommandPrinter::CommandPrinter(int pid, String& printCommand) : ICommand(pid, PRINT) {
    this->printCommand = printCommand;
    this->pid = pid;
}

void CommandPrinter::execute() {
    ICommand::execute();
    std::stringstream msg;
    msg << "PID " << this->pid << ":" << this->printCommand << std::endl;
}