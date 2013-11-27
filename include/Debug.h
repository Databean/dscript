#ifndef DEBUG_H
#define DEBUG_H

#include <string>
#include <iostream>

#define DEBUG(message) \
	std::cout << "(DEBUG)" << __FILE__ << ":" << __LINE__ << ": " << message << std::endl

#endif