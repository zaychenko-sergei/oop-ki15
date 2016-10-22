// (C) 2016, Sergei Zaychenko, KNURE, Kharkiv, Ukraine

#ifndef _PASSPORT_HPP_
#define _PASSPORT_HPP_

/*****************************************************************************/

#include "date.hpp"

#include <string>

/*****************************************************************************/


class Passport
{

/*-----------------------------------------------------------------*/

public:

/*-----------------------------------------------------------------*/

	struct RegistrationRecord
	{
		const std::string m_address;
		const Date m_date;
		const bool m_isRegistration;

		RegistrationRecord ( std::string const & _address, Date _date, bool _isRegistration );
		
		bool operator == ( RegistrationRecord  const & _r ) const;

		bool operator != ( RegistrationRecord const & _r ) const;
	};

/*-----------------------------------------------------------------*/

	// TODO: public members

/*-----------------------------------------------------------------*/

private:

/*-----------------------------------------------------------------*/

	// TODO: private members

/*-----------------------------------------------------------------*/

};


/*****************************************************************************/

// TODO: inline methods implementation

/*****************************************************************************/

#endif // _PASSPORT_HPP_
