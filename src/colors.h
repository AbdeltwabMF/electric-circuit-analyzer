#ifndef COLORS_H
#define COLORS_H

#include <string>

/**
	Used to change the color of the lines writted to the user's console.
	Check: https://en.wikipedia.org/wiki/ANSI_escape_code
*/
const std::string Black 	= "\e[1;30m";
const std::string Red 		= "\e[1;31m";
const std::string Green 	= "\e[1;32m";
const std::string Yellow	= "\e[1;33m";
const std::string Blue  	= "\e[1;34m";
const std::string Purple	= "\e[1;35m";
const std::string Cyan  	= "\e[1;36m";
const std::string White 	= "\e[1;37m";
const std::string Reset		= "\e[0m";

#endif // COLORS_H

