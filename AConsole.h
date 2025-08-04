#pragma once  
#include <string>  
typedef std::string String;

class AConsole  
{  
public:   
	AConsole();  
	AConsole(String name);
	~AConsole() = default;  

	String getName() const;
	virtual void onEnabled() = 0;  
	virtual void display() = 0;  
	virtual void process() = 0;  

	String name;
	friend class ConsoleManager;  
};
