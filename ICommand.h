#pragma once
#include "IETThread.h"
#include "ConfigReader.h"
#include <unordered_map>
#include <string>
#include <random>

typedef std::string String;

class ICommand
{
public:
	enum CommandType
	{
		PRINT,
		DECLARE,
		ADD,
		SUBTRACT,
		SLEEP,
		READ,
		WRITE,
		FOR
	};
	
	ICommand(int processID, CommandType commandtype);
	virtual ~ICommand() = default;
	virtual std::shared_ptr<ICommand> clone() const = 0;
	CommandType	getCommandType();
	virtual void execute();
	virtual String getOutput() const { return ""; }  // default empty

protected:
	int processID;
	CommandType commandType;

};