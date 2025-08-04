#include "ICommand.h"
class WriteCommand : public ICommand
{
public:
	WriteCommand(int address, uint16_t value, std::shared_ptr<std::unordered_map<std::string, uint16_t>> symbolTable);
	std::shared_ptr<ICommand> clone() const override;
	void execute() override;

	void reWrite();
	String getOutput() const override;

private:
	int address;
	uint16_t value;
	std::shared_ptr<std::unordered_map<std::string, uint16_t>> symbolTable;
};
