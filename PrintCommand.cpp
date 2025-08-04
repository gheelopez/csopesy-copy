#include "PrintCommand.h"
#include "IETThread.h"

PrintCommand::PrintCommand(int processID, String processName, std::shared_ptr<std::unordered_map<std::string, uint16_t>> symbolTable)
	: ICommand(processID, PRINT), processName(processName), symbolTable(symbolTable) {
}

PrintCommand::PrintCommand(int processID, String processName, std::shared_ptr<std::unordered_map<std::string, uint16_t>> symbolTable, String remainingLine)
    : ICommand(processID, PRINT), processName(processName), symbolTable(symbolTable), expression(remainingLine) {
}

std::shared_ptr<ICommand> PrintCommand::clone() const {
	return std::make_shared<PrintCommand>(*this);
}

void PrintCommand::execute()
{
	ICommand::execute();
	setString();
}

void PrintCommand::setString()
{
	// If no expression provided, fall back to default
	if (expression.empty()) {
		toPrint = "Hello world from " + processName;
		return;
	}

	// Handle PRINT("message" + varName) style
	size_t quoteStart = expression.find("\"");
	size_t quoteEnd = expression.find_last_of("\"");

	String result = "";

	if (quoteStart != std::string::npos && quoteEnd != std::string::npos && quoteEnd > quoteStart) {
		// Extract string before +
		String message = expression.substr(quoteStart + 1, quoteEnd - quoteStart - 1);
		result += message;

		// Check if there's a '+' and a variable after the quote
		size_t plusPos = expression.find('+', quoteEnd);
		if (plusPos != std::string::npos) {
			String varName = expression.substr(plusPos + 1);
			// Trim whitespace
			varName.erase(0, varName.find_first_not_of(" \t"));
			varName.erase(varName.find_last_not_of(" \t\r\n") + 1);

			// Look up varName
			if (symbolTable && symbolTable->count(varName)) {
				result += std::to_string(symbolTable->at(varName));
			}
			else {
				result += "[undefined:" + varName + "]";
			}
		}
		toPrint = result;
	}
	else {
		// Fallback if badly formatted
		toPrint = "Invalid PRINT expression: " + expression;
	}

}

String PrintCommand::getOutput() const
{

	return toPrint;
}