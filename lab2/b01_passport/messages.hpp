// (C) 2016, Sergei Zaychenko, KNURE, Kharkiv, Ukraine

#ifndef _MESSAGES_HPP_
#define _MESSAGES_HPP_

/*****************************************************************************/

namespace Messages
{
	const char * const BadPassportId = "Bad passport ID";
	const char * const PassportIssuedInFuture = "Issued date in future";
	const char * const OwnerEmptyName = "Empty owner name";
	const char * const NoCurrentRegistration = "Passport has no current registration";
	const char * const CurrentRegistrationStillActive = "Current registration is still active";
	const char * const RegistrationDateBeforePassportIssue = "Registration date before passport was issued";
	const char * const RegistrationDateInFuture = "Registration date in future";
	const char * const UnregistrationDateInFuture = "Unregistering in future";
	const char * const PreviousRegistrationLaterThanCurrent = "Previous unregistration record at later date than current registration";
	const char * const UnregisteringEarlierThanRegistered = "Unregistering earlier than registered";
}

/*****************************************************************************/

#endif // _MESSAGES_HPP_
