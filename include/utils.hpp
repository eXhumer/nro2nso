/*
 * nro2nso - Convert Nintendo Switch Homebrew Application to NSO
 * Copyright (C) 2020 eXhumer
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program located in the file LICENSE. If not, see
 * <https://www.gnu.org/licenses/>.
 */

#ifndef UTILS_HPP
#define UTILS_HPP
#include "exitstatus.hpp"
#include "nro.hpp"
#include <filesystem>
#include <fstream>

namespace utils
{
	ExitStatus validateNroFile(std::ifstream& inStream, NroHeader& nroHeader);
	ExitStatus extractAssets(std::filesystem::path outDirPath);
	ExitStatus createNsoFromNro(std::ifstream& nroStream, std::ofstream& nsoStream, NroHeader& nroHeader, bool decompressSegments, bool verifySegments);
};
#endif