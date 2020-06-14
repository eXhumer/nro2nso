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