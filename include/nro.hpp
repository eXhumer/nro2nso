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

#ifndef NRO_HPP
#define NRO_HPP
#include <stdint.h>
#include <array>

#define NRO_MAGIC 0x304F524E

typedef struct
{
	uint32_t entryPointInsn;
	uint32_t modOffset;
	uint64_t padding;
} NroStart;

typedef struct
{
	uint32_t memOff;
	uint32_t size;
} NroSegmentHeader;

typedef struct
{
	NroStart nroStart;
	uint32_t magic;
	uint32_t version;
	uint32_t size;
	uint32_t flags;
	NroSegmentHeader textSegmentHeader;
	NroSegmentHeader roSegmentHeader;
	NroSegmentHeader dataSegmentHeader;
	uint32_t bssSize;
	uint32_t reserved1;
	std::array<uint8_t, 0x20> moduleId;
	uint32_t dsoHandleOff;
	uint32_t reserved2;
	NroSegmentHeader apiInfoSegmentHeader;
	NroSegmentHeader dynstrSegmentHeader;
	NroSegmentHeader dynstmSegmentHeader;
} NroHeader;

typedef struct
{
	uint64_t offset;
	uint64_t size;
} NroAssetSection;

typedef struct
{
	uint32_t magic;
	uint32_t version;
	NroAssetSection iconSection;
	NroAssetSection nacpSection;
	NroAssetSection romfsSection;
} NroAssetsHeader;
#endif