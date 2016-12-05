// (C) 2013-2016, Sergei Zaychenko, KNURE, Kharkiv, Ukraine

#ifndef _CONTROLLER_HPP_
#define _CONTROLLER_HPP_

/*****************************************************************************/

#include <string>

/*****************************************************************************/


class Controller
{

/*-----------------------------------------------------------------*/

public:

/*-----------------------------------------------------------------*/

	Controller () = default;

	Controller ( const Controller & ) = delete;
	Controller & operator = ( const Controller & ) = delete;

	~ Controller () = default;

	void createHourlyEmployee ( std::string const & _name, double _hourlyRate );

	void createSalaryEmployee ( std::string const & _name, double _monthPayment );

	void createInternEmployee ( std::string const & _name, double _workHoursLimit );

	void trackWork ( std::string const & _employeeName, double _hours );

	void giveBonus ( std::string const & _employeeName, double _bonusPayment );

	double getPayment ( std::string const & _employeeName ) const;

	double getTotalPayment () const;

/*-----------------------------------------------------------------*/

private:

/*-----------------------------------------------------------------*/

	// TODO
	
/*-----------------------------------------------------------------*/

};


/*****************************************************************************/

#endif //  _CONTROLLER_HPP_
