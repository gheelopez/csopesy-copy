#pragma once
#include "ICommand.h"
class DeclareCommand : public ICommand {
public:
    DeclareCommand(const String& varName, uint16_t value, std::shared_ptr<std::unordered_map<std::string, uint16_t>> symbolTable);
    DeclareCommand(int processID, const String& varName, uint16_t value, std::shared_ptr<std::unordered_map<std::string, uint16_t>> symbolTable);
    std::shared_ptr<ICommand> clone() const;
    void execute() override;

    void declareVariable(const String varName);
    String getVariableName();
    uint16_t getValue() const;
    String getOutput() const override;
private:
    String newVar;
    uint16_t value;
    void getUniqueVariableName();
    std::shared_ptr<std::unordered_map<std::string, uint16_t>> symbolTable;
};
