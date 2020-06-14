#include "utils.hpp"

ExitStatus utils::validateNroFile(std::ifstream& inStream, NroHeader& nroHeader)
{
	inStream.seekg(0);
	inStream.read((char*)&nroHeader, sizeof(NroHeader));
	if(nroHeader.magic != NRO_MAGIC)
		return ExitFailInputInvalidNROMagic;
	return ExitSuccess;
}