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
 * along with this program located in the file LICENSE.md found at
 * the root of the project directory. If not, see
 * <https://www.gnu.org/licenses/>.
 */

#ifndef NSO_HPP
#define NSO_HPP
#include <stdint.h>
#include <array>

typedef struct
{
	uint32_t magic;
	uint32_t version;
	uint32_t reserved1;
	uint32_t flags;
	NsoSegmentHeader textSegmentHeader;
	NsoSegmentHeader roSegmentHeader;
	NsoSegmentHeader dataSegmentHeader;
	std::array<uint8_t, 0x20> buildId;
	uint32_t textCmpSize;
	uint32_t roCmpSize;
	uint32_t dataCmpSize;
	std::array<uint8_t, 0x1C> reserved2;
	NsoSegmentHeaderRelative apiInfo;
	NsoSegmentHeaderRelative dynstr;
	NsoSegmentHeaderRelative dynsym;
	std::array<uint8_t, 0x20> textHash;
	std::array<uint8_t, 0x20> roHash;
	std::array<uint8_t, 0x20> dataHash;
} NsoHeader;

typedef struct
{
	uint32_t fileOff;
	uint32_t memOff;
	uint32_t sizeDec;
	uint32_t cSize;
} NsoSegmentHeader;

typedef struct
{
	uint32_t offset;
	uint32_t size;
} NsoSegmentHeaderRelative;
#endif