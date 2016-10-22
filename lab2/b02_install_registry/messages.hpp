// (C) 2016, Sergei Zaychenko, KNURE, Kharkiv, Ukraine

#ifndef _MESSAGES_HPP_
#define _MESSAGES_HPP_

/*****************************************************************************/

namespace Messages
{
	const char * const MachineNameEmpty = "Empty machine name";
	const char * const ProgramNameEmpty = "Empty program name";
	const char * const ProgramNotInstalled = "Program was not installed";
	const char * const ProgramAlreadyInstalled = "Program was already installed";
	const char * const InstallationDateInFuture = "Installation date is in future";
	const char * const UpdateDateInFuture = "Update date is in future";
	const char * const UpdateDateEarlierThanPreviousVersion = "Update date cannot be earlier than the previous installation date";
}

/*****************************************************************************/

#endif // _MESSAGES_HPP_
