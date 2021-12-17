#ifndef COLORS_H
#define COLORS_H

#include <string>

/**
	Used to change the color of the lines writted to the user's console.
	Check: https://en.wikipedia.org/wiki/ANSI_escape_code
*/
const std::string Black 	= "\033[1;30m";
const std::string Red 		= "\033[1;31m";
const std::string Green 	= "\033[1;32m";
const std::string Yellow	= "\033[1;33m";
const std::string Blue  	= "\033[1;34m";
const std::string Purple	= "\033[1;35m";
const std::string Cyan  	= "\033[1;36m";
const std::string White 	= "\033[1;37m";
const std::string Reset		= "\033[0m";

#endif // COLORS_H

