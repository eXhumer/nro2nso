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
	uint32_t fileOffset;
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