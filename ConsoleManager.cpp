#include "ConsoleManager.h"
#include <iostream>

ConsoleManager* ConsoleManager::sharedInstance = nullptr;

ConsoleManager* ConsoleManager::getInstance()
{
	return sharedInstance;
}

void ConsoleManager::initialize()
{
	if (sharedInstance == nullptr)
	{
		sharedInstance = new ConsoleManager();
	}
}

void ConsoleManager::destroy()
{
	if (sharedInstance != nullptr)
	{
		delete sharedInstance;
		sharedInstance = nullptr;
	}
}

void ConsoleManager::drawConsole() const
{
	if (this->currentConsole != nullptr) 
	{
		this->currentConsole->display();
	}
	else
	{
		std::cerr << "No console is currently set." << std::endl;
	}

}

void ConsoleManager::process() const
{
	if (this->currentConsole != nullptr)
	{
		this->currentConsole->process();
	}
	else
	{
		std::cerr << "No console is currently set." << std::endl;
	}
}

void ConsoleManager::registerScreen(std::shared_ptr<BaseScreen> screenRef)
{
	if (this->consoleTable.contains(screenRef->getName()))
	{
		std::cerr << "Screen name " << screenRef->getName() << " already exists." << std::endl;
		return;
	}
	this->consoleTable[screenRef->getName()] = screenRef;
	this->switchConsole(screenRef->getName());
}

void ConsoleManager::createBaseScreen(std::shared_ptr<Process> process, bool screenS)
{
	if (this->consoleTable.contains(process->getName()))
	{
		std::cerr << "Screen name " << process->getName() << " already exists." << std::endl;
		return;
	}
	this->numScreens++; // increment the screen count for unique process IDs when making processes manually
	std::shared_ptr<BaseScreen> baseScreen = std::make_shared<BaseScreen>(process, process->getName());
	this->consoleTable[baseScreen->getName()] = baseScreen;
	if (screenS) {
		this->switchConsole(baseScreen->getName());
	}
}


void ConsoleManager::switchConsole(String consoleName)
{
	if (this->consoleTable.contains(consoleName))
	{
		system("cls");
		this->previousConsole = this->currentConsole;
		this->currentConsole = this->consoleTable[consoleName];
		this->currentConsole->onEnabled();
	}
	else
	{
		std::cerr << "Console name '" << consoleName << "' not initialized." << std::endl;
	}
}

void ConsoleManager::unregisterScreen(String screenName)
{
	if (this->consoleTable.contains(screenName))
	{
		this->consoleTable.erase(screenName);
	}
	else
	{
		std::cerr << "Unable to unregister screen '" << screenName << "'. Was not found." << std::endl;
	}
}

void ConsoleManager::returnToPreviousConsole()
{
	if (this->previousConsole != nullptr)
	{
		this->currentConsole = this->previousConsole;
		this->currentConsole->onEnabled();
	}
	else
	{
		std::cerr << "No previous console to return to." << std::endl;
		return;
	}
}

void ConsoleManager::switchToScreen(String screenName)
{
	if(this->consoleTable.contains(screenName))
	{
		system("cls");
		this->previousConsole = this->currentConsole;
		this->currentConsole = this->consoleTable[screenName];
		this->currentConsole->onEnabled();
	}
	else
	{
		std::cerr << "Screen name '" << screenName << "' not found." << std::endl;
	}
}

void ConsoleManager::exitApplication()
{
	this->running = false;
}

bool ConsoleManager::isRunning() const
{
	return this->running;
}

int ConsoleManager::getNumScreens()
{
	return this->numScreens;
}

ConsoleManager::ConsoleManager()
{
	this->running = true;

	//initialize consoles
	this->consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	const std::shared_ptr<MainConsole> mainConsole = std::make_shared<MainConsole>();
	/*const std::shared_ptr<MarqueeConsole> marqueeConsole = std::make_shared<MarqueeConsole>();
	const std::shared_ptr<SchedulingConsole> schedulingConsole = std::make_shared<SchedulingConsole>();
	const std::shared_ptr<MemorySimulationConsole> memoryConsole = std::make_shared<MemorySimulationConsole>*/

	this->consoleTable["MAIN_CONSOLE"] = mainConsole;
	/*this->consoleTable["MARQUEE_CONSOLE"] = marqueeConsole;
	this->consoleTable["SCHEDULING_CONSOLE"] = schedulingConsole;
	this->consoleTable["MEMORY_CONSOLE"] = memoryConsole;*/

	this->switchConsole(MAIN_CONSOLE);


}


