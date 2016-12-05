// (C) 2013-2016, Sergei Zaychenko, KNURE, Kharkiv, Ukraine

#ifndef _MESSAGES_HPP_
#define _MESSAGES_HPP_

/*****************************************************************************/

namespace Messages
{

/*-----------------------------------------------------------------*/

	const char * const EmptyName   = "Name of the employee must be present";
	const char * const NonUniqueName   = "Name of the employee must be unique";
	const char * const NonPositivePayment = "Payment amount must be positive";
	const char * const NonPositiveHoursLimit = "Limit of working hours must be positive";
	const char * const UnknownEmployeeName = "Employee with such name was not previously registered";
	const char * const NegativeWorkHours = "Number of work hours used must not be negative";
	const char * const OverloadedIntern = "Intern working capacity was exceeded";
	
/*-----------------------------------------------------------------*/

};


/*****************************************************************************/


#endif // _MESSAGES_HPP_
