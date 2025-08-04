#include "MemoryManager.h"  // Include the header file for MemoryManager
#include "GlobalScheduler.h"

MemoryManager* MemoryManager::sharedInstance = nullptr;

void MemoryManager::initialize(int maxOverallMemory, int memFrameSize, int minMemPerProc, int maxMemPerProc) 
{
    if(sharedInstance == nullptr) {
        sharedInstance = new MemoryManager(maxOverallMemory, memFrameSize, minMemPerProc, maxMemPerProc);
	}
}

void MemoryManager::destroy() 
{
    if (sharedInstance != nullptr) {
        delete sharedInstance;
        sharedInstance = nullptr;
    }
}

MemoryManager* MemoryManager::getInstance() 
{
    return sharedInstance;
}

bool MemoryManager::loadPagesForProcess(std::shared_ptr<Process> process)
{
    int processID = process->getPID();
    int pageSize = memPerFrame;
    int totalPages = process->getNumPages();


    auto pageTable = process->getPageTable(); // Assumes getter for pageTable exists
    if (!pageTable) return false;

    for (int pageNumber = 0; pageNumber < totalPages; ++pageNumber) {
        PageEntry& entry = (*pageTable)[pageNumber];

        if (!entry.isPageValid()) {
            // Find a free frame
            auto it = std::find_if(memory.begin(), memory.end(), [](const FrameEntry& f) {
                return !f.isFrameValid(); // find an invalid (free) frame
                });

            int frameNumber;

            if (it == memory.end()) {   
                // FIFO Page Replacement
                if (fifoQueue.empty()) return false;

                auto victim = fifoQueue.front(); // pair<processID, pageNumber>
                fifoQueue.pop();

                int victimPID = victim.first;
                int victimPage = victim.second;

                // Find the frame to evict
                int evictFrame = -1;
                for (int i = 0; i < memory.size(); ++i) {
                    if (memory[i].getProcessID() == victimPID && memory[i].getPageNumber() == victimPage) {
                        evictFrame = i;
                        break;
                    } 
                }

                if (evictFrame == -1) return false; // Should not happen

                // Write evicted page to backing store
                writeToBackingStore(victimPID, victimPage);

                auto victimProcess = GlobalScheduler::getInstance()->getProcessByPID(victimPID);
                if (victimProcess) {
                    auto victimPageTable = victimProcess->getPageTable();
                    if (victimPageTable && victimPage >= 0 && victimPage < victimPageTable->size()) {
                        (*victimPageTable)[victimPage].invalidatePage();
                    }
                }

                // Overwrite frame
                memory[evictFrame] = FrameEntry{ processID, pageNumber, true };
                frameNumber = evictFrame;
            }
            else {
                // Free frame found
                frameNumber = std::distance(memory.begin(), it);
                memory[frameNumber] = FrameEntry{ processID, pageNumber, true };
            }

            // Update page table
            entry.setFrameNumber(frameNumber);

			removeFromBackingStore(processID, pageNumber); // Remove from backing store if it exists

            // Add to FIFO queue
            fifoQueue.push({ processID, pageNumber });
        }
    }

    return true;
}


bool MemoryManager::deallocateMemory(int processID) 
{
    /*bool deallocated = false;
    for (int& frame : memory) {
        if (frame == processID) {
            frame = 0;
            deallocated = true;
        }
    }
    return deallocated;*/
    return 0;
}

void MemoryManager::writeToBackingStore(int processID, int pageNumber)
{
    std::ofstream file("csopesy-backing-store.txt", std::ios::app);
    if (!file.is_open()) return;
    file << processID << ":" << pageNumber << "\n";
    file.close();
}

void MemoryManager::removeFromBackingStore(int processID, int pageNumber)
{
    std::ifstream inFile("csopesy-backing-store.txt");
    if (!inFile.is_open()) return;

    std::vector<std::string> remainingEntries;
    std::string line;
    std::string target = std::to_string(processID) + ":" + std::to_string(pageNumber);

    while (std::getline(inFile, line)) {
        if (line != target) {
            remainingEntries.push_back(line);
        }
    }
    inFile.close();

    std::ofstream outFile("csopesy-backing-store.txt", std::ios::trunc); // Overwrite the file
    for (const auto& entry : remainingEntries) {
        outFile << entry << "\n";
    }
    outFile.close();
}



int MemoryManager::getExternalFragmentation() const 
{
    /*int freeSpace = std::count(memory.begin(), memory.end(), 0);*/
    return 0 /** memPerProc*/;
}

void MemoryManager::saveMemorySnapshot(int cycle) const 
{
    //std::string folder = "snapshots";
    //std::string filename = folder + "/memory_stamp_" + std::to_string(cycle) + ".txt";
    //std::ofstream file(filename);

    //if (!file.is_open()) {
    //    return;
    //}

    //// Get current time as string using std::chrono and localtime_s
    //auto now = std::chrono::system_clock::now();
    //std::time_t time_t_now = std::chrono::system_clock::to_time_t(now);
    //std::tm tm;
    //localtime_s(&tm, &time_t_now);

    //char timeStr[64];
    //std::strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", &tm);

    //file << "Timestamp: (" << timeStr << ") " << "\n";
    //file << "Processes in memory: " << getAllocatedProcessCount() << "\n";
    //file << "Total external fragmentation in KB: " << getExternalFragmentation() << "\n";
    //file << "Memory Snapshot:\n";

    //file << "---end--- = " << maxOverallMemory << "\n";
    //int addr = maxOverallMemory;
    //for (int i = memory.size() - 1; i >= 0; --i) {
    //    if (memory[i] != 0) {
    //        file << "\n";
    //        file << addr << "\n";
    //        file << "P" << memory[i] << "\n";
    //        addr -= memPerProc;
    //        file << addr << "\n";
    //    }
    //    else {
    //        addr -= memPerProc; // still decrease addr even if not printing
    //    }
    //}

    //file << "\n---start--- = 0\n";
}
int MemoryManager::getAllocatedProcessCount() const 
{
    /*std::unordered_set<int> allocatedProcesses;
    for (int i : memory) {
        if (i != 0) {
            allocatedProcesses.insert(i);
        }
    }
    return allocatedProcesses.size();*/
	return 0; // Placeholder, should be replaced with actual logic
}

void MemoryManager::setMaxOverallMemory(int maxOverallMemory) 
{
	this->maxOverallMemory = maxOverallMemory;
}

void MemoryManager::setMemPerFrame(int memFrameSize) 
{
    this->memPerFrame = memFrameSize;
}

void MemoryManager::setMinMemPerProc(int minMemPerProc) 
{
    this->minMemPerProc = minMemPerProc;
}

void MemoryManager::setMaxMemPerProc(int maxMemPerProc) 
{
    this->maxMemPerProc = maxMemPerProc;
}

MemoryManager::MemoryManager(int maxOverallMemory, int memPerFrame, int minMemPerProc, int maxMemPerProc)
    : maxOverallMemory(maxOverallMemory), memPerFrame(memPerFrame), minMemPerProc(minMemPerProc), maxMemPerProc(maxMemPerProc)
{
	this->numFrames = maxOverallMemory / memPerFrame; // Calculate number of frames
    this->memory.resize(numFrames);
}

