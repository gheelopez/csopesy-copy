#include "FrameEntry.h"

FrameEntry::FrameEntry()
	: processID(-1), pageNumber(-1), isValid(false) {
}

FrameEntry::FrameEntry(int processID, int pageNumber, bool isValid)
{
	this->processID = processID;
	this->pageNumber = pageNumber;
	this->isValid = isValid;
}

void FrameEntry::setProcessID(int processID)
{
	this->processID = processID;
}

void FrameEntry::setPageNumber(int pageNumber)
{
	this->pageNumber = pageNumber;
}

void FrameEntry::setState(bool isValid)
{
	this->isValid = isValid;
}

int FrameEntry::getProcessID() const
{
	return this->processID;
}

int FrameEntry::getPageNumber() const
{
	return this->pageNumber;
}

bool FrameEntry::isFrameValid() const
{
	return this->isValid;
}