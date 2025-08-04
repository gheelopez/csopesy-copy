#pragma once

#include <string>
#include <memory>
#include <vector>
#include <chrono>
#include <cmath>
#include <ctime>
#include <format>
#include <sstream>
#include <unordered_map>
#include <cstdlib>  
#include <iterator> 
#include <filesystem>
#include <algorithm>
#include <random>  
#include <map>

#include "ICommand.h"
#include "ConfigReader.h"
#include "PageEntry.h"
#include "PrintCommand.h"
#include "DeclareCommand.h"
#include "AddCommand.h"
#include "SubtractCommand.h"
#include "SleepCommand.h"
#include "ForCommand.h"

typedef std::string String;
typedef std::vector<std::shared_ptr<ICommand>> CommandList;
typedef std::unordered_map<std::string, uint16_t> Symbol_Table; // stores results from process commands
typedef std::map<int, PageEntry> Page_Table; // stores page table for this process

class Process {
public:
    enum ProcessState {
        READY,
        RUNNING,
        WAITING,
        FINISHED
    };

    Process(int pid, String name);
	Process(int pid, String name, int memorySize, int numPages);

    void addCommand(std::shared_ptr<ICommand> command);
    void executeCurrentCommand(int coreId); // called by ScheduleWorker

    bool isFinished() const;

    int incrementCommandCounter();

    int getRemainingTime() const;
    int getCommandCounter() const;
    std::size_t getLinesOfCode() const;
    int getPID() const;
    int getCPUCoreID() const;

    void setState(ProcessState currentState);
    ProcessState getState() const;
    String getName() const;
	int getMemSize() const;
	int getNumPages() const; // returns the number of pages for this process
    std::shared_ptr<Page_Table> getPageTable() const;

    void generateRandomCommands();
    void printCommands() const;
    void logInstruction(int coreId, String message);

    void generateNestedForCommand(int currentDepth, int maxDepth, int repeats);

    CommandList getCommandList();
    String getRunningTimestamp();
    String getFinishedTimestamp();
    std::vector<String> getLogs();

    String stateToString(ProcessState state);

    void parseAndLoadInstructions(const std::string& instructionStr);

private:
    int pid;
    String name;
	int memorySize; // size in bytes
    int pages;
    CommandList commandList;
    std::shared_ptr<Symbol_Table> symbolTable;
	std::shared_ptr<Page_Table> pageTable; // page table for this process

    int commandCounter;
    int commandCounterIndex;
    int cpuCoreID;
    ProcessState currentState = WAITING;

    String runningTimestamp;
    String finishedTimestamp;

    std::vector<String> print_logs;

    std::vector<String> instructions;

    String getCurrentTimestamp();
};
