#include "ICommand.h"
class ReadCommand : public ICommand
{
public:
	ReadCommand(const String& varName, int address, std::shared_ptr<std::unordered_map<std::string, uint16_t>> symbolTable);
	std::shared_ptr<ICommand> clone() const override;
	void execute() override;

	uint16_t getValue() const;
	String getOutput() const override;

private:
	String varName;
	int address;
	uint16_t value; 
	std::shared_ptr<std::unordered_map<std::string, uint16_t>> symbolTable;
};
