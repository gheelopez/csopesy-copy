#pragma once
#include <vector>
#include <queue>
#include <unordered_set>
#include <fstream>
#include <iostream>
#include <algorithm> 
#include <string>   
#include <ctime>
#include <chrono>
#include <cstdio> 

#include "Process.h"
#include "FrameEntry.h"

class MemoryManager {
public:
	// Default Constructor and Destructor
    MemoryManager(int maxOverallMemory, int memFrameSize, int minMemPerProc, int maxMemPerProc);
    ~MemoryManager() = default;

    // Singleton access
    static MemoryManager* getInstance();
    static void initialize(int maxOverallMemory, int memFrameSize, int minMemPerProc, int maxMemPerProc);
    static void destroy();

    // Function to allocate memory for a process
    bool loadPagesForProcess(std::shared_ptr<Process> process);

    // Function to deallocate memory for a process
    bool deallocateMemory(int processID);

	// Function to check if a page is in the backing store
	void removeFromBackingStore(int processID, int pageNumber);
    void writeToBackingStore(int processID, int pageNumber);

    // Function to get the external fragmentation in KB
    int getExternalFragmentation() const;

    // Function to save a snapshot of memory to a file
    void saveMemorySnapshot(int cycle) const;

    void setMaxOverallMemory(int maxOverallMemory);
    void setMemPerFrame(int memPerFrame);
    void setMinMemPerProc(int minMemPerProc);
    void setMaxMemPerProc(int maxMemPerProc);

private:
    // Disallow copying
    MemoryManager(const MemoryManager&) = delete;
    MemoryManager& operator=(MemoryManager const&) {}

    // Singleton instance
    static MemoryManager* sharedInstance;

    int maxOverallMemory = 0;     // Total memory size (in bytes)
    int memPerFrame = 0;          // Size of each memory frame (in bytes)
	int numFrames = 0;            // Number of memory frames (THIS IS JUST FOR HOMEWORK 10. Should be removed in the future)
    int minMemPerProc = 0;        // Size of each process (in bytes)
	int maxMemPerProc = 0;        // Maximum size of each process (in bytes)
    std::vector<FrameEntry> memory;      // Vector representing the memory/frames 
    std::queue<std::pair<int, int>> fifoQueue; // PID, page number

    // Helper function to get the count of allocated processes
    int getAllocatedProcessCount() const;
};