#ifndef UTILS_HPP
#define UTILS_HPP
#include "exitstatus.hpp"
#include "nro.hpp"
#include <fstream>

namespace utils
{
	ExitStatus validateNroFile(std::ifstream& inStream, NroHeader& nroHeader);
};
#endif