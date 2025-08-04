#include "Screen.h"

void Screen::printTimestamp() {
    std::tm local_tm;
    localtime_s(&local_tm, &timeCreated); 

    std::cout << "Timestamp of when the screen is created: ";
    std::cout << std::put_time(&local_tm, "%m/%d/%Y, %I:%M %p") << std::endl; // Correct usage of std::put_time
}

void Screen::displayScreen() {
    std::cout << "Screen Name: " << this->screenName << std::endl;
    std::cout << "Current Instruction: " << this->currentInstruction << std::endl;
    std::cout << "Total Instructions: " << this->totalInstruction << std::endl;
    printTimestamp();
}
