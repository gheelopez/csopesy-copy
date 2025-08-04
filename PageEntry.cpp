#include "PageEntry.h"

PageEntry::PageEntry()
{
	this->frameNumber = -1; // Default value indicating no frame is assigned
	this->isValid = false; // Default value indicating the page is not loaded in memory
}

bool PageEntry::isPageValid() const
{
	return this->isValid;
}

int PageEntry::getFrameNumber() const
{
	return this->frameNumber;
}

void PageEntry::setFrameNumber(int frameNumber)
{
	this->frameNumber = frameNumber;
	this->isValid = true; // Set the page as valid when a frame number is assigned
}

void PageEntry::invalidatePage()
{
	this->isValid = false; // Mark the page as invalid
	this->frameNumber = -1; // Reset frame number to indicate no frame is assigned
}