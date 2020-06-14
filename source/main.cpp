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

#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include "exitstatus.hpp"
#include "cxxopts.hpp"
#include "utils.hpp"
#include "nro.hpp"
#include "nso.hpp"

int main(int argc, char* argv[])
{
	cxxopts::Options options(APP_NAME, "Create NSO from NRO. Can be used to convert NRO to NSP.");

	std::string inPathStr = "";
	std::string outPathStr = "";
	std::string assetOutPathStr = "";
	bool decompressSegments = false;
	bool verifySegments = false;
	bool showUsage = false;

	options.add_options()
		("i,input", "Input NRO file path", cxxopts::value(inPathStr))
		("o,output", "Output NSO path", cxxopts::value(outPathStr))
		("e,extract-asset-dir", "Output path to extract NRO assets to", cxxopts::value(assetOutPathStr))
		("decompressed", "Flag to keep NSO sections decompressed in output", cxxopts::value(decompressSegments)->default_value("false"))
		("no-verify-hash", "Flag to not verify section hash before loading", cxxopts::value(verifySegments)->default_value("false"))
		("h,help", "Shows this help message", cxxopts::value(showUsage)->default_value("false"))
		;

	std::string helpMessage = options.help();

	auto argResults = options.parse(argc, argv);

	std::filesystem::path inPath(inPathStr);
	std::filesystem::path outPath(outPathStr);
	std::filesystem::path assetOutPath(assetOutPathStr);

	showUsage = (((argResults.count("input") == 0) || (argResults.count("output") == 0)) && showUsage == false);

	if(!showUsage)
	{
		// check if output file already exists, warning of overwrite
		if(std::filesystem::exists(assetOutPath))
		{
			if(!std::filesystem::is_directory(assetOutPath))
			{
				if(std::filesystem::exists(assetOutPath/"control.nacp"))
					std::cerr << "WARNING: control.nacp already exists! File will be overwritten!\n";

				if(std::filesystem::exists(assetOutPath/"romfs.bin"))
					std::cerr << "WARNING: romfs.bin already exists! File will be overwritten!\n";

				if(std::filesystem::exists(assetOutPath/"icon.jpg"))
					std::cerr << "WARNING: icon.jpg already exists! File will be overwritten!\n";
			}
			else
			{
				std::cerr << "Output assets path already exists, but is not a directory! Unable to continue.\n";
				return ExitFailOutputAssetPathNotDirectory;
			}
		}
		else
		{
			std::filesystem::create_directory(assetOutPath);
		}

		// create stream for input file, exit if cant
		std::ifstream nroStream;
		nroStream.open(inPath, std::ios::binary | std::ios::in);

		if(!nroStream.is_open())
		{
			std::cerr << "ERROR: Unable to open input file!\n";
			return ExitFailInputUnableToOpen;
		}

		NroHeader nroHeader;

		// perform validation test on nro and store results
		ExitStatus validationResult = utils::validateNroFile(nroStream, nroHeader);

		// exit immediately closing input stream if it fails NRO0 magic check
		if(validationResult != ExitSuccess)
		{
			nroStream.close();
			std::cerr << "ERROR: Failed NRO Validation!\nReason: ";
			if(validationResult == ExitFailInputInvalidNROMagic)
				std::cout << "Invalid NRO Magic!\n";
			else if(validationResult == ExitFailInputReadError)
				std::cout << "Unable to read from input file!\n";
			return validationResult;
		}

		// create stream for output file, exit if cant
		std::ofstream nsoStream;
		nsoStream.open(outPath, std::ios::binary | std::ios::out);

		if(!nsoStream.is_open())
		{
			nroStream.close();
			std::cerr << "ERROR: Unable to open output file!\n";
			return ExitFailOutputUnableToOpen;
		}

		ExitStatus createResult = utils::createNsoFromNro(nroStream, nsoStream, nroHeader, assetOutPath, decompressSegments, verifySegments);
		nroStream.close();
		nsoStream.close();
		return createResult;
	}
	else
	{
		std::cout << helpMessage << "\n";
		return ExitUsage;
	}
}