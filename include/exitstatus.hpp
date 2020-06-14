#ifndef EXIT_STATUS_HPP
#define EXIT_STATUS_HPP
enum ExitStatus
{
	ExitSuccess = 0x0,
	ExitFailInputNonExistant,
	ExitFailInputUnableToOpen,
	ExitFailInputInvalidNROMagic,
	ExitFailInputReadError,
	ExitFailOutputAlreadyExists,
	ExitFailOutputUnableToOpen,
	ExitFailOutputWriteError,
	ExitUsage
};
#endif