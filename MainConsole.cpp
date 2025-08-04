#include "MainConsole.h"

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
std::string userInput = "";
std::string commandMessage = "";
std::string outputArg2 = "";
int outputArg3 = 0;
bool isInitialized = false;

std::string instructionString = "";

MainConsole::MainConsole()
{
	this->name = "MainConsole";
}

MainConsole::MainConsole(String name)
{
	this->name = name;
}

void MainConsole::onEnabled() 
{
	system("cls");
	printHeader();
}

std::string MainConsole::separateCommands(const std::string& str)
{
	size_t first = str.find_first_not_of(" \t\r\n");
	if (first == std::string::npos) return "";
	size_t last = str.find_last_not_of(" \t\r\n");
	return str.substr(first, (last - first + 1));
}

void MainConsole::display() // handles what displayes after the process function handles the input
{
	if (commandMessage.empty())
	{
		onEnabled();
	}

	if (isInitialized) {
		if (commandMessage == "initialize") 
		{
			ConfigReader::initialize();
			ConfigReader::getInstance()->setParams("config.txt");
			//ConfigReader::getInstance()->testPrint(); // for debugging only
			GlobalScheduler::initialize();
			GlobalScheduler::getInstance()->setCoreCount(ConfigReader::getInstance()->getNumCPU());
			GlobalScheduler::getInstance()->selectScheduler(ConfigReader::getInstance()->getSchedulerToUse());
			MemoryManager::initialize(ConfigReader::getInstance()->getMaxOverallMem(), ConfigReader::getInstance()->getMemPerFrame(), ConfigReader::getInstance()->getMinMemPerProc(), ConfigReader::getInstance()->getMaxMemPerProc());
			GlobalScheduler::getInstance()->getScheduler()->init();
			GlobalScheduler::getInstance()->getScheduler()->run();
			
			commandMessage = "";
		}

		if (commandMessage == "clear") 
		{
			system("cls");
			printHeader();
			commandMessage = "";
		} 

		if (commandMessage == "exit")
		{
			commandMessage = "";
			ConsoleManager::getInstance()->exitApplication();
		}

		if (commandMessage == "screenR")
		{
			commandMessage = "";
			ConsoleManager::getInstance()->switchToScreen(outputArg2);
		}

		if (commandMessage == "screenS")
		{
			commandMessage = "";
			if (outputArg3 >= 64 && outputArg3 <= 65536)
			{
				int numPages = (outputArg3 + ConfigReader::getInstance()->getMemPerFrame() - 1) / outputArg3;
				std::shared_ptr<Process> newProcess = std::make_shared<Process>(
					ConsoleManager::getInstance()->getNumScreens(), outputArg2, outputArg3, numPages
				);
				ConsoleManager::getInstance()->createBaseScreen(newProcess, true);
				GlobalScheduler::getInstance()->addProcess(newProcess);
			} 
			else
			{
				std::cout << "Invalid memory size. Please enter a value between 64 and 65536." << std::endl;
			}
			
		}
		
		if (commandMessage == "screenLS")
		{
			this->screenLS = GlobalScheduler::getInstance()->getScheduler()->screenLS();
			std::cout << this->screenLS;
		}

		if (commandMessage == "screenC")
		{
			commandMessage = "";
			if (outputArg3 >= 64 && outputArg3 <= 65536)
			{
				/*int numPages = (outputArg3 + ConfigReader::getInstance()->getMemPerFrame() - 1) / outputArg3;
				std::shared_ptr<Process> newProcess = std::make_shared<Process>(
					ConsoleManager::getInstance()->getNumScreens(), outputArg2, outputArg3, numPages
				);
				ConsoleManager::getInstance()->createBaseScreen(newProcess, true);
				GlobalScheduler::getInstance()->addProcess(newProcess);*/
				
				// separating commands from the instruction string
				std::vector<std::string> instructions;
				std::stringstream ss(instructionString);
				std::string token;
				while (getline(ss, token, ';')) {
					if (!token.empty()) {
						instructions.push_back(separateCommands(token));
					}
				}

				//if (instructions.size() < 1 || instructions.size() > 50) {
				//	std::cout << "Invalid command: number of instructions must be between 1 and 50." << std::endl;
				//	return;
				//}

				//int numPages = (outputArg3 + ConfigReader::getInstance()->getMemPerFrame() - 1) / outputArg3;
				//std::shared_ptr<Process> newProcess = std::make_shared<Process>(
				//	ConsoleManager::getInstance()->getNumScreens(), outputArg2, outputArg3, numPages
				//);
				//newProcess->parseAndLoadInstructions(instructionString); // new method to set instruction list
				//ConsoleManager::getInstance()->createBaseScreen(newProcess, true);
				//GlobalScheduler::getInstance()->addProcess(newProcess);

				

				/*int frameSize = ConfigReader::getInstance()->getMemPerFrame();*/
				int numPages = (outputArg3 + ConfigReader::getInstance()->getMemPerFrame() - 1) / outputArg3;

				std::shared_ptr<Process> newProcess = std::make_shared<Process>(
					ConsoleManager::getInstance()->getNumScreens(), outputArg2, outputArg3, numPages
				);

				newProcess->parseAndLoadInstructions(instructionString);

				ConsoleManager::getInstance()->createBaseScreen(newProcess, true);
				GlobalScheduler::getInstance()->addProcess(newProcess);
				std::cout << "Process '" << outputArg2 << "' created with " << instructions.size() << " instructions." << std::endl;
			}
			else
			{
				std::cout << "Invalid memory size. Please enter a value between 64 and 65536." << std::endl;
			}
		}

		if (commandMessage == "scheduler-start")
		{
			commandMessage = "";
			GlobalScheduler::getInstance()->schedulerStart();
		}

		if (commandMessage == "scheduler-stop")
		{
			commandMessage = "";
			GlobalScheduler::getInstance()->schedulerStop();
		}

		if (commandMessage == "report-util")
		{
			std::string filename = "csopesy-log.txt";
			std::ofstream file(filename);
			file << this->screenLS;
			file.close();

			std::cout << "Report generated at " << fs::absolute(filename) << std::endl;

		}

		if (commandMessage == "show-ticks")
		{
			std::cout << CPUTick::getInstance()->getTicks() << std::endl;
		}

		if (commandMessage == "unknown-command")
		{
			commandMessage = "";
			std::cout << "Unknown command. Please try again." << std::endl;
		}
	} 

	else if (commandMessage == "exit")
	{
		commandMessage = "";
		ConsoleManager::getInstance()->exitApplication();
	}
	else 
	{
		std::cout << "Console has not been initialized yet. Please type 'initialize' to start." << std::endl;
	}


}

void MainConsole::process() // this function handles the input from the user 
{
	std::string commandInput;
	std::cout << "root:\\\\>";
	getline(std::cin, commandInput);

	std::stringstream ss(commandInput);
	std::string command, arg1, arg2;
	int arg3 = 0;
	ss >> command >> arg1 >> arg2 >> arg3;

	if (command == "exit") 
	{
		commandMessage = command;
	}
	else if (command == "clear") 
	{
		commandMessage = command;
	}
	else if (command == "initialize") 
	{
		commandMessage = command;
		isInitialized = true;
	}
	else if (command == "screen" && arg1 == "-r" && !arg2.empty()) 
	{
		commandMessage = "screenR";
		outputArg2 = arg2;
	}
	else if (command == "screen" && arg1 == "-s" && !arg2.empty() && arg3 != 0) 
	{
		commandMessage = "screenS";
		outputArg2 = arg2;
		outputArg3 = arg3;
	}
	else if (command == "screen" && arg1 == "-ls") 
	{
		commandMessage = "screenLS";
	}
	else if (command == "screen" && arg1 == "-c" && !arg2.empty() && arg3 != 0)
	{
		commandMessage = "screenC";
		outputArg2 = arg2;
		outputArg3 = arg3;

		// Get the rest as the instruction string
		std::string instructions;
		getline(ss, instructions);

		// Remove surrounding quotes if present
		if (!instructions.empty() && instructions.front() == '\"' && instructions.back() == '\"') {
			instructions = instructions.substr(1, instructions.length() - 2);
		}

		// Store everything
		commandMessage = "screenC";
		instructionString = instructions; // new member variable
	}
	else if (command == "scheduler-start") 
	{
		commandMessage = command;
	}
	else if (command == "scheduler-stop")
	{
		commandMessage = command;
	}
	else if (command == "report-util") 
	{
		commandMessage = command;
	}
	else if (command == "show-ticks")
	{
		commandMessage = command;
	}
	else {
		commandMessage = "unknown-command";
	}
}

void MainConsole::printHeader() const
{
	std::cout << "   ______     ______     ______     ______   ______     ______     __  __    " << std::endl;
	std::cout << "  /\\  ___\\   /\\  ___\\   /\\  __ \\   /\\  == \\ /\\  ___\\   /\\  ___\\   /\\ \\_\\ \\   " << std::endl;
	std::cout << "  \\ \\ \\____  \\ \\___  \\  \\ \\ \\/\\ \\  \\ \\  _-/ \\ \\  __\\   \\ \\___  \\  \\ \\____ \\  " << std::endl;
	std::cout << "   \\ \\_____\\  \\/\\_____\\  \\ \\_____\\  \\ \\_\\    \\ \\_____\\  \\/\\_____\\  \\/\\_____\\ " << std::endl;
	std::cout << "    \\/_____/   \\/_____/   \\/_____/   \\/_/     \\/_____/   \\/_____/   \\/_____/ " << std::endl;
	std::cout << "Hello, Welcome to CSOPESY command line!" << std::endl;
	if (isInitialized == false) 
	{
		std::cout << "Type 'initialize' to initialize the console, 'exit' to quit" << std::endl;
	}
	else 
	{
		std::cout << "Type 'clear' to clear the screen, 'exit' to quit" << std::endl;
	}
}
