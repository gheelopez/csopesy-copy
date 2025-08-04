#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
typedef std::string String;

class ConfigReader
{
public:
	static ConfigReader* getInstance();
	static void initialize();
	static void destroy();

	std::vector<String> readFileToVector(const String& filename);
	void setParams(const String& filename);
	void assignParams(int index);
	void testPrint(); // debug test to 

	int getNumCPU();
	String getSchedulerToUse();
	int getQuantum();
	int getBatchProcessFreq();
	int getMinIns();
	int getMaxIns();
	int getDelays();
	int getMaxOverallMem();
	int getMemPerFrame();
	int getMinMemPerProc();
	int getMaxMemPerProc();

private:
	ConfigReader();
	~ConfigReader() = default;
	static ConfigReader* sharedInstance;
	ConfigReader& operator=(ConfigReader const&) {}; // assignment operator is private

	std::vector<String> paramsList;
	int num_cpu;
	String scheduler_to_use;
	int quantum_cycles;
	int batch_process_freq;
	int min_ins;
	int max_ins;
	int delays;
	int max_overall_mem;
	int mem_per_frame;
	int min_mem_per_proc;
	int max_mem_per_proc;
};

