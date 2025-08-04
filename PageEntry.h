#pragma once

#include <cstdint>

class PageEntry
{
public:
	PageEntry();

	bool isPageValid() const; // check if page is valid
	int getFrameNumber() const; // get the frame number where the page is loaded
	void setFrameNumber(int frameNumber); // set the frame number where the page is loaded
	void invalidatePage(); // invalidate the page

private:
	bool isValid; // if page is loaded in memory
	int frameNumber; // frame number in memory where the page is loaded
};

