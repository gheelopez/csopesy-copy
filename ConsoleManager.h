#pragma once
#include <memory>
#include <vector>
#include "AConsole.h"
#include "MainConsole.h"
#include "BaseScreen.h"
#include <unordered_map>
#include <Windows.h>

typedef std::string String;

const String MAIN_CONSOLE = "MAIN_CONSOLE";
const String MARQUEE_CONSOLE = "MARQUEE_CONSOLE";
const String SCHEDULING_CONSOLE = "SCHEDULING_CONSOLE";
const String MEMORY_CONSOLE = "MEMORY_CONSOLE";

class ConsoleManager {
public:
	typedef std::unordered_map<String, std::shared_ptr<AConsole>> ConsoleTable;

	static ConsoleManager* getInstance();
	static void initialize();
	static void destroy();

	void drawConsole() const;
	void process() const;
	void switchConsole(String consoleName);
	void returnToPreviousConsole();

	void registerScreen(std::shared_ptr<BaseScreen> screenRef);
	void createBaseScreen(std::shared_ptr<Process> process, bool screenS);

	void switchToScreen(String screenName);
	void unregisterScreen(String screenName);

	void exitApplication();
	bool isRunning() const;

	int getNumScreens();

	//HANDLE getConsoleHandle() const;

	//void setCursorPosition(int posX, int posY);

private:
	ConsoleManager();
	~ConsoleManager() = default;
	//ConsoleManager(ConsoleManager const&) {}; // copy constructor is private
	ConsoleManager& operator=(ConsoleManager const&) {}; // assignment operator is private
	static ConsoleManager* sharedInstance;

	ConsoleTable consoleTable;
	std::shared_ptr<AConsole> currentConsole;
	std::shared_ptr<AConsole> previousConsole;

	HANDLE consoleHandle;
	bool running = true;
	int numScreens = 0; // to add counter for screens
};
