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

#include <vector>
#include "utils.hpp"
#include "nso.hpp"
#include "exitstatus.hpp"
#include "lz4.h"
#include <botan/hash.h>

ExitStatus utils::validateNroFile(std::ifstream& inStream, NroHeader& nroHeader)
{
	if(inStream.good())
	{
		inStream.seekg(0);
		inStream.read((char*)&nroHeader, sizeof(NroHeader));
		if(inStream.good())
			return (nroHeader.magic == NRO_MAGIC) ? ExitSuccess : ExitFailInputInvalidNROMagic;
	}
	return ExitFailInputReadError;
}

uint32_t getNsoFlags(bool decompressSegments, bool verifySegments)
{
	uint32_t flag = 0b000000;
	if(!decompressSegments)
		flag += 0b000111;
	if(verifySegments)
		flag += 0b111000;
	return flag;
}

ExitStatus utils::extractAssets(std::filesystem::path outDirPath)
{

}

int compressLz4(std::vector<uint8_t>& inData, std::vector<uint8_t>& outData)
{
	if(outData.size() < inData.size())
		outData.resize(inData.size());
	return LZ4_compress_default((char*)inData.data(), (char*)outData.data(), inData.size(), outData.size());
}

ExitStatus utils::createNsoFromNro(std::ifstream& nroStream, std::ofstream& nsoStream, NroHeader& nroHeader, bool decompressSegments, bool verifySegments)
{
	// if input stream has bad bit set
	if(!nroStream.good())
		return ExitFailInputReadError;

	// if output stream has bad bit set
	if(!nsoStream.good())
		return ExitFailOutputWriteError;

	// create new header for new nso
	NsoHeader nsoHeader;

	// set header magic
	nsoHeader.magic = NSO_MAGIC;

	// set header flags
	nsoHeader.flags = getNsoFlags(decompressSegments, verifySegments);

	// set section memOff addr in nso header
	nsoHeader.textSegmentHeader.memOff = nroHeader.textSegmentHeader.memOff;
	nsoHeader.roSegmentHeader.memOff = nroHeader.roSegmentHeader.memOff;
	nsoHeader.dataSegmentHeader.memOff = nroHeader.dataSegmentHeader.memOff;

	// fun fact, faggots over at switchbrew has warned me for pointing this
	// bug in their own code lol. bug still present as of the moment i am
	// writing this code (pointed out the bug more than 24 hours ago)
	// https://github.com/switchbrew/switch-tools/blob/17276a6bc47c66f04f3aab8aed5e86da72601545/src/elf2nso.c#L134
	// moral of the story: never point out any bug to switchbrew, they seem to enjoy making and using buggy software
	nsoHeader.textSegmentHeader.cSize = sizeof(NsoHeader);
	nsoHeader.roSegmentHeader.cSize = 1;

	// temp store nro segment sizes from headers
	uint32_t textPaddedSize = nroHeader.textSegmentHeader.size;
	uint32_t roPaddedSize = nroHeader.roSegmentHeader.size;
	uint32_t dataPaddedSize = nroHeader.dataSegmentHeader.size;

	// temo store bssSize
	uint32_t nroBssSize = nroHeader.bssSize;

	std::vector<uint8_t> textSegment;
	std::vector<uint8_t> roSegment;
	std::vector<uint8_t> dataSegment;

	if(nroStream.good())
	{
		textSegment.resize(textPaddedSize);
		roSegment.resize(roPaddedSize);
		dataSegment.resize(dataPaddedSize);
		nroStream.seekg(sizeof(NroHeader));
		nroStream.read((char*)textSegment.data(), textPaddedSize);
		nroStream.read((char*)roSegment.data(), roPaddedSize);
		nroStream.read((char*)dataSegment.data(), dataPaddedSize);

		for(int i = 0; i < textPaddedSize; i++)
		{
			if(textSegment.data()[i-1] != 0)
			{
				textSegment.resize(textSegment.size() - i);
				break;
			}
			continue;
		}

		for(int i = 0; i < roPaddedSize; i++)
		{
			if(roSegment.data()[i-1] != 0)
			{
				roSegment.resize(roSegment.size() - i);
				break;
			}
			continue;
		}

		for(int i = 0; i < dataPaddedSize; i++)
		{
			if(dataSegment.data()[i-1] != 0)
			{
				dataSegment.resize(dataSegment.size() - i);
				break;
			}
			continue;
		}
	}
	else
		return ExitFailInputReadError;

	std::copy(nroHeader.moduleId.begin(), nroHeader.moduleId.end(), nsoHeader.moduleId.begin());

	if(verifySegments)
	{
		std::unique_ptr<Botan::HashFunction> textHashCtx(Botan::HashFunction::create("SHA-256"));
		textHashCtx->update(textSegment.data(), textSegment.size());
		auto textHash = textHashCtx->final();
		std::copy(textHash.begin(), textHash.end(), nsoHeader.textHash.begin());
		std::unique_ptr<Botan::HashFunction> roHashCtx(Botan::HashFunction::create("SHA-256"));
		roHashCtx->update(roSegment.data(), roSegment.size());
		auto roHash = textHashCtx->final();
		std::copy(roHash.begin(), roHash.end(), nsoHeader.roHash.begin());
		std::unique_ptr<Botan::HashFunction> dataHashCtx(Botan::HashFunction::create("SHA-256"));
		dataHashCtx->update(dataSegment.data(), dataSegment.size());
		auto dataHash = textHashCtx->final();
		std::copy(dataHash.begin(), dataHash.end(), nsoHeader.dataHash.begin());
	}

	std::vector<uint8_t> roSegmentCompressed;
	std::vector<uint8_t> dataSegmentCompressed;
	std::vector<uint8_t> textSegmentCompressed;

	if(!decompressSegments)
	{
		if(compressLz4(textSegment, textSegmentCompressed) == 0)
			return ExitCompressError;
		textSegment.resize(0);

		if(compressLz4(roSegment, roSegmentCompressed) == 0)
			return ExitCompressError;
		roSegment.resize(0);

		if(compressLz4(dataSegment, dataSegmentCompressed) == 0)
			return ExitCompressError;
		dataSegment.resize(0);
	}

	// TODO - Get .api_info/.dynstr/.dynsym SegmentHeaderRelative for NSO

	if(nsoStream.good())
	{
		// TODO - Write header to NSO stream, then text, ro & data.
	}
	else
		return ExitFailInputReadError;
}