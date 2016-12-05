// (C) 2013-2016, Sergei Zaychenko, KNURE, Kharkiv, Ukraine

/*****************************************************************************/

#include "controller.hpp"
#include "messages.hpp"

#include "testslib.hpp"

/*****************************************************************************/

const double eps = 0.00001;

#define CHECK_EQUAL( _a, _b ) assert( abs( ( _a ) - ( _b ) ) < eps )

/*****************************************************************************/

/*
	Test Plan:
	Done	1) Hourly employee
	Done		1.1) Single
	Done		1.2) Several employees
	Done		1.3) With empty name
	Done		1.4) With negative hour rate
	Done		1.5) With duplicated name
	Done	2) Salary employee
	Done		2.1) Single
	Done		2.2) Several employees
	Done		2.3) With empty name
	Done		2.4) With negative hour rate
	Done		2.5) With duplicated name
	Done	3) Intern employee
	Done		3.1) Single
	Done		3.2) Several employees
	Done		3.3) With empty name
	Done		3.4) With negative hour rate
	Done		3.5) With duplicated name
	Done	4) Track work
	Done		4.1) Several hourly employees
	Done		4.2) Several salary employees
	Done		4.3) Several intern employees
	Done		4.4) Mix of employees
	Done		4.5) Empty name
	Done		4.6) Negative hours
	Done		4.7) Not existing name
	Done		4.8) Exactly feeting intern time
	Done		4.9) Exceeding intern time
	Done	5) Give bonus
	Done		5.1) Bonus for hourly worker
	Done		5.2) Bonus for salary worker
	Done		5.3) Bonus for intern
	Done		5.4) Mix of employees
	Done		5.5) Empty name
	Done		5.6) Non-positive bonus
	Done		5.7) Not existing name
*/

/*****************************************************************************/

DECLARE_OOP_TEST( employees_1_1_hourly_employee_single )
{
	Controller c;

	c.createHourlyEmployee( "Winston Smith", 5.0 );

	CHECK_EQUAL( c.getPayment( "Winston Smith" ), 0.0 );
	CHECK_EQUAL( c.getTotalPayment(), 0.0 );
}

/*****************************************************************************/

DECLARE_OOP_TEST( employees_1_2_hourly_employee_several )
{
	Controller c;

	c.createHourlyEmployee( "Winston Smith", 5.0 );

	CHECK_EQUAL( c.getPayment( "Winston Smith" ), 0.0 );
	CHECK_EQUAL( c.getTotalPayment(), 0.0 );

	c.createHourlyEmployee( "Emmanuel Goldstein", 10.25 );

	CHECK_EQUAL( c.getPayment( "Emmanuel Goldstein" ), 0.0 );
	CHECK_EQUAL( c.getTotalPayment(), 0.0 );
}

/*****************************************************************************/

DECLARE_OOP_TEST( employees_1_3_hourly_employee_empty_name )
{
	Controller c;

	ASSERT_THROWS(
			c.createHourlyEmployee( "", 5.0 )
		,	Messages::EmptyName
	);
}

/*****************************************************************************/

DECLARE_OOP_TEST( employees_1_4_hourly_employee_negative_hour_rate )
{
	Controller c;

	ASSERT_THROWS(
			c.createHourlyEmployee( "Winston Smith", -0.5 )
		,	Messages::NonPositivePayment
	);
}

/*****************************************************************************/

DECLARE_OOP_TEST( employees_1_5_hourly_employee_duplicated_name )
{
	Controller c;

	c.createHourlyEmployee( "Winston Smith", 5.0 );

	CHECK_EQUAL( c.getPayment( "Winston Smith" ), 0.0 );
	CHECK_EQUAL( c.getTotalPayment(), 0.0 );

	ASSERT_THROWS(
			c.createHourlyEmployee( "Winston Smith", 10.0 )
		,	Messages::NonUniqueName
	);
}

/*****************************************************************************/

DECLARE_OOP_TEST( employees_2_1_salary_employee_single )
{
	Controller c;

	c.createSalaryEmployee( "Winston Smith", 1000.0 );

	CHECK_EQUAL( c.getPayment( "Winston Smith" ), 1000.0 );
	CHECK_EQUAL( c.getTotalPayment(), 1000.0 );
}

/*****************************************************************************/

DECLARE_OOP_TEST( employees_2_2_salary_employee_several )
{
	Controller c;

	c.createSalaryEmployee( "Winston Smith", 700.0 );

	CHECK_EQUAL( c.getPayment( "Winston Smith" ), 700.0 );
	CHECK_EQUAL( c.getTotalPayment(), 700.0 );

	c.createSalaryEmployee( "Emmanuel Goldstein", 1500.0 );

	CHECK_EQUAL( c.getPayment( "Emmanuel Goldstein" ), 1500.0 );
	CHECK_EQUAL( c.getTotalPayment(), 2200.0 );
}

/*****************************************************************************/

DECLARE_OOP_TEST( employees_2_3_salary_employee_empty_name )
{
	Controller c;

	ASSERT_THROWS(
			c.createSalaryEmployee( "", 500.0 )
		,	Messages::EmptyName
	);
}

/*****************************************************************************/

DECLARE_OOP_TEST( employees_2_4_salary_employee_negative_salary )
{
	Controller c;

	ASSERT_THROWS(
			c.createSalaryEmployee( "Winston Smith", -0.3 )
		,	Messages::NonPositivePayment
	);
}

/*****************************************************************************/

DECLARE_OOP_TEST( employees_2_5_salary_employee_duplicated_name )
{
	Controller c;

	c.createSalaryEmployee( "Winston Smith", 500.0 );

	CHECK_EQUAL( c.getPayment( "Winston Smith" ), 500.0 );
	CHECK_EQUAL( c.getTotalPayment(), 500.0 );

	ASSERT_THROWS(
			c.createSalaryEmployee( "Winston Smith", 1000.0 )
		,	Messages::NonUniqueName
	);
}

/*****************************************************************************/

DECLARE_OOP_TEST( employees_3_1_intern_employee_single )
{
	Controller c;

	c.createInternEmployee( "Winston Smith", 80.0 );

	CHECK_EQUAL( c.getPayment( "Winston Smith" ), 0.0 );
	CHECK_EQUAL( c.getTotalPayment(), 0.0 );
}

/*****************************************************************************/

DECLARE_OOP_TEST( employees_3_2_intern_employee_several )
{
	Controller c;

	c.createInternEmployee( "Winston Smith", 80.0 );

	CHECK_EQUAL( c.getPayment( "Winston Smith" ), 0.0 );
	CHECK_EQUAL( c.getTotalPayment(), 0.0 );

	c.createInternEmployee( "Emmanuel Goldstein", 100.0 );

	CHECK_EQUAL( c.getPayment( "Emmanuel Goldstein" ), 0.0 );
	CHECK_EQUAL( c.getTotalPayment(), 0.0 );
}

/*****************************************************************************/

DECLARE_OOP_TEST( employees_3_3_intern_employee_empty_name )
{
	Controller c;

	ASSERT_THROWS(
			c.createInternEmployee( "", 80.0 )
		,	Messages::EmptyName
	);
}

/*****************************************************************************/

DECLARE_OOP_TEST( employees_3_4_intern_employee_negative_hours_limit )
{
	Controller c;

	ASSERT_THROWS(
			c.createInternEmployee( "Winston Smith", -0.5 )
		,	Messages::NonPositiveHoursLimit
	);
}

/*****************************************************************************/

DECLARE_OOP_TEST( employees_3_5_intern_employee_duplicated_name )
{
	Controller c;

	c.createInternEmployee( "Winston Smith", 80.0 );

	CHECK_EQUAL( c.getPayment( "Winston Smith" ), 0.0 );
	CHECK_EQUAL( c.getTotalPayment(), 0.0 );

	ASSERT_THROWS(
			c.createInternEmployee( "Winston Smith", 160.0 )
		,	Messages::NonUniqueName
	);
}

/*****************************************************************************/

DECLARE_OOP_TEST( employees_4_1_track_work_several_hourly_employees )
{
	Controller c;

	std::string emp1( "Winston Smith" );
	std::string emp2( "Emmanuel Goldstein" );
	std::string emp3( "Julia" );

	c.createHourlyEmployee( emp1, 5.0 );

	CHECK_EQUAL( c.getPayment( emp1 ), 0.0 );
	CHECK_EQUAL( c.getTotalPayment(), 0.0 );

	c.trackWork( emp1, 25.0 );

	CHECK_EQUAL( c.getPayment( emp1 ), 125.0 /* 5 * 25 */ );
	CHECK_EQUAL( c.getTotalPayment(), 125.0 );

	c.createHourlyEmployee( emp2, 50.0 );

	CHECK_EQUAL( c.getPayment( emp2 ), 0.0 );
	CHECK_EQUAL( c.getTotalPayment(), 125.0 );

	c.trackWork( emp2, 60.0 );

	CHECK_EQUAL( c.getPayment( emp2 ), 3000.0 /* 50 * 60 */ );
	CHECK_EQUAL( c.getTotalPayment(), 3125.0 );

	c.createHourlyEmployee( emp3, 10.0 );

	CHECK_EQUAL( c.getPayment( emp3 ), 0.0 );
	CHECK_EQUAL( c.getTotalPayment(), 3125.0 );

	c.trackWork( emp3, 160.0 );

	CHECK_EQUAL( c.getPayment( emp3 ), 1600.0 );
	CHECK_EQUAL( c.getTotalPayment(), 4725.0 );
}

/*****************************************************************************/

DECLARE_OOP_TEST( employees_4_2_track_work_several_salary_employees )
{
	Controller c;

	std::string emp1( "Winston Smith" );
	std::string emp2( "Emmanuel Goldstein" );
	std::string emp3( "Julia" );

	c.createSalaryEmployee( emp1, 500.0 );

	CHECK_EQUAL( c.getPayment( emp1 ), 500.0 );
	CHECK_EQUAL( c.getTotalPayment(), 500.0 );

	c.trackWork( emp1, 25.0 );

	CHECK_EQUAL( c.getPayment( emp1 ), 500.0 );
	CHECK_EQUAL( c.getTotalPayment(), 500.0 );

	c.createSalaryEmployee( emp2, 1000.0 );

	CHECK_EQUAL( c.getPayment( emp2 ), 1000.0 );
	CHECK_EQUAL( c.getTotalPayment(), 1500.0 );

	c.trackWork( emp2, 60.0 );

	CHECK_EQUAL( c.getPayment( emp2 ), 1000.0 );
	CHECK_EQUAL( c.getTotalPayment(), 1500.0 );

	c.createSalaryEmployee( emp3, 1500.0 );

	CHECK_EQUAL( c.getPayment( emp3 ), 1500.0 );
	CHECK_EQUAL( c.getTotalPayment(), 3000.0 );

	c.trackWork( emp3, 160.0 );

	CHECK_EQUAL( c.getPayment( emp3 ), 1500.0 );
	CHECK_EQUAL( c.getTotalPayment(), 3000.0 );
}

/*****************************************************************************/

DECLARE_OOP_TEST( employees_4_3_track_work_several_intern_employees )
{
	Controller c;

	std::string emp1( "Winston Smith" );
	std::string emp2( "Emmanuel Goldstein" );

	c.createInternEmployee( emp1, 80.0 );

	CHECK_EQUAL( c.getPayment( emp1 ), 0.0 );
	CHECK_EQUAL( c.getTotalPayment(), 0.0 );

	c.trackWork( emp1, 40.0 );

	CHECK_EQUAL( c.getPayment( emp1 ), 0.0 );
	CHECK_EQUAL( c.getTotalPayment(), 0.0 );

	c.createInternEmployee( emp2, 160.0 );

	CHECK_EQUAL( c.getPayment( emp2 ), 0.0 );
	CHECK_EQUAL( c.getTotalPayment(), 0.0 );

	c.trackWork( emp2, 160.0 );

	CHECK_EQUAL( c.getPayment( emp2 ), 0.0 );
	CHECK_EQUAL( c.getTotalPayment(), 0.0 );
}

/*****************************************************************************/

DECLARE_OOP_TEST( employees_4_4_track_work_mix_of_employees )
{
	Controller c;

	std::string emp1( "Winston Smith" );
	std::string emp2( "Emmanuel Goldstein" );
	std::string emp3( "Julia" );

	c.createSalaryEmployee( emp1, 500.0 );

	CHECK_EQUAL( c.getPayment( emp1 ), 500.0 );
	CHECK_EQUAL( c.getTotalPayment(), 500.0 );

	c.trackWork( emp1, 25.0 );

	CHECK_EQUAL( c.getPayment( emp1 ), 500.0 );
	CHECK_EQUAL( c.getTotalPayment(), 500.0 );

	c.createHourlyEmployee( emp2, 25.0 );

	CHECK_EQUAL( c.getPayment( emp2 ), 0.0 );
	CHECK_EQUAL( c.getTotalPayment(), 500.0 );

	c.trackWork( emp2, 75.0 );

	CHECK_EQUAL( c.getPayment( emp2 ), 1875.0 );
	CHECK_EQUAL( c.getTotalPayment(), 2375.0 );

	c.createInternEmployee( emp3, 80.0 );

	CHECK_EQUAL( c.getPayment( emp3 ), 0.0 );
	CHECK_EQUAL( c.getTotalPayment(), 2375.0 );

	c.trackWork( emp3, 80.0 );

	CHECK_EQUAL( c.getPayment( emp3 ), 0.0 );
	CHECK_EQUAL( c.getTotalPayment(), 2375.0 );
}

/*****************************************************************************/

DECLARE_OOP_TEST( employees_4_5_track_work_empty_name )
{
	Controller c;

	std::string emp1( "Winston Smith" );

	c.createHourlyEmployee( emp1, 50.0 );

	CHECK_EQUAL( c.getPayment( emp1 ), 0.0 );
	CHECK_EQUAL( c.getTotalPayment(), 0.0 );

	ASSERT_THROWS(
			c.trackWork( "", 25.0 )
		,	Messages::EmptyName
	);

	CHECK_EQUAL( c.getPayment( emp1 ), 0.0 );
	CHECK_EQUAL( c.getTotalPayment(), 0.0 );
}

/*****************************************************************************/

DECLARE_OOP_TEST( employees_4_6_track_work_non_existing_name )
{
	Controller c;

	std::string emp1( "Winston Smith" );

	c.createHourlyEmployee( emp1, 50.0 );

	CHECK_EQUAL( c.getPayment( emp1 ), 0.0 );
	CHECK_EQUAL( c.getTotalPayment(), 0.0 );

	ASSERT_THROWS(
			c.trackWork( "Julia", 25.0 )
		,	Messages::UnknownEmployeeName
	);

	CHECK_EQUAL( c.getPayment( emp1 ), 0.0 );
	CHECK_EQUAL( c.getTotalPayment(), 0.0 );
}

/*****************************************************************************/

DECLARE_OOP_TEST( employees_4_7_track_work_negative_hours_amount )
{
	Controller c;

	std::string emp1( "Winston Smith" );

	c.createHourlyEmployee( emp1, 50.0 );

	CHECK_EQUAL( c.getPayment( emp1 ), 0.0 );
	CHECK_EQUAL( c.getTotalPayment(), 0.0 );

	ASSERT_THROWS(
			c.trackWork( emp1, -0.7 )
		,	Messages::NegativeWorkHours
	);

	CHECK_EQUAL( c.getPayment( emp1 ), 0.0 );
	CHECK_EQUAL( c.getTotalPayment(), 0.0 );
}

/*****************************************************************************/

DECLARE_OOP_TEST( employees_4_8_track_work_exact_intern_hours )
{
	Controller c;

	std::string emp1( "Winston Smith" );

	c.createInternEmployee( emp1, 160.0 );

	CHECK_EQUAL( c.getPayment( emp1 ), 0.0 );
	CHECK_EQUAL( c.getTotalPayment(), 0.0 );

	c.trackWork( emp1, 80.0 );

	CHECK_EQUAL( c.getPayment( emp1 ), 0.0 );
	CHECK_EQUAL( c.getTotalPayment(), 0.0 );

	c.trackWork( emp1, 80.0 );

	CHECK_EQUAL( c.getPayment( emp1 ), 0.0 );
	CHECK_EQUAL( c.getTotalPayment(), 0.0 );
}

/*****************************************************************************/

DECLARE_OOP_TEST( employees_4_9_track_work_exceeding_intern_hours )
{
	Controller c;

	std::string emp1( "Winston Smith" );

	c.createInternEmployee( emp1, 160.0 );

	CHECK_EQUAL( c.getPayment( emp1 ), 0.0 );
	CHECK_EQUAL( c.getTotalPayment(), 0.0 );

	c.trackWork( emp1, 80.0 );

	CHECK_EQUAL( c.getPayment( emp1 ), 0.0 );
	CHECK_EQUAL( c.getTotalPayment(), 0.0 );

	ASSERT_THROWS(
			c.trackWork( emp1, 81.0 )
		,	Messages::OverloadedIntern
	);

	c.trackWork( emp1, 80.0 );
}

/*****************************************************************************/

DECLARE_OOP_TEST( employees_5_1_add_bonus_for_hour_employee )
{
	Controller c;

	std::string emp1( "Winston Smith" );

	c.createHourlyEmployee( emp1, 5.0 );

	CHECK_EQUAL( c.getPayment( emp1 ), 0.0 );
	CHECK_EQUAL( c.getTotalPayment(), 0.0 );

	c.trackWork( emp1, 80.0 );

	CHECK_EQUAL( c.getPayment( emp1 ), 400.0 );
	CHECK_EQUAL( c.getTotalPayment(), 400.0 );

	c.giveBonus( emp1, 125.5 );

	CHECK_EQUAL( c.getPayment( emp1 ), 525.5 );
	CHECK_EQUAL( c.getTotalPayment(), 525.5 );

}

/*****************************************************************************/

DECLARE_OOP_TEST( employees_5_2_add_bonus_for_salary_employee )
{
	Controller c;

	std::string emp1( "Winston Smith" );

	c.createSalaryEmployee( emp1, 500.0 );

	CHECK_EQUAL( c.getPayment( emp1 ), 500.0 );
	CHECK_EQUAL( c.getTotalPayment(), 500.0 );

	c.trackWork( emp1, 80.0 );

	CHECK_EQUAL( c.getPayment( emp1 ), 500.0 );
	CHECK_EQUAL( c.getTotalPayment(), 500.0 );

	c.giveBonus( emp1, 220.0 );

	CHECK_EQUAL( c.getPayment( emp1 ), 720.0 );
	CHECK_EQUAL( c.getTotalPayment(), 720.0 );

}

/*****************************************************************************/

DECLARE_OOP_TEST( employees_5_3_add_bonus_for_intern )
{
	Controller c;

	std::string emp1( "Winston Smith" );

	c.createInternEmployee( emp1, 160.0 );

	CHECK_EQUAL( c.getPayment( emp1 ), 0.0 );
	CHECK_EQUAL( c.getTotalPayment(), 0.0 );

	c.trackWork( emp1, 80.0 );

	CHECK_EQUAL( c.getPayment( emp1 ), 0.0 );
	CHECK_EQUAL( c.getTotalPayment(), 0.0 );

	c.giveBonus( emp1, 220.0 );

	CHECK_EQUAL( c.getPayment( emp1 ), 220.0 );
	CHECK_EQUAL( c.getTotalPayment(), 220.0 );
}

/*****************************************************************************/

DECLARE_OOP_TEST( employees_5_4_add_bonus_for_mix_of_employees )
{
	Controller c;

	std::string emp1( "Winston Smith" );
	std::string emp2( "O'Brien" );
	std::string emp3( "Julia" );

	c.createInternEmployee( emp1, 160.0 );
	c.createSalaryEmployee( emp2, 500.0 );
	c.createHourlyEmployee( emp3, 15.0 );

	c.trackWork( emp1, 160.0 );
	c.trackWork( emp2, 146.0 );
	c.trackWork( emp3, 112.0 );

	c.giveBonus( emp1, 220.0 );
	c.giveBonus( emp2, 1.0 );
	c.giveBonus( emp3, 10.0 );

	CHECK_EQUAL( c.getPayment( emp1 ), 220.0 /* 0 + 220 */);
	CHECK_EQUAL( c.getPayment( emp2 ), 501.0 /* 500 + 1 */);
	CHECK_EQUAL( c.getPayment( emp3 ), 1690.0 /* 1680 + 10 */ );
	CHECK_EQUAL( c.getTotalPayment(), 2411.0 );
}

/*****************************************************************************/

DECLARE_OOP_TEST( employees_5_4_add_bonus_empty_name )
{
	Controller c;

	std::string emp1( "Winston Smith" );

	c.createInternEmployee( emp1, 160.0 );
	c.trackWork( emp1, 160.0 );

	ASSERT_THROWS(
			c.giveBonus( "", 220.0 )
		,	Messages::EmptyName
	);
}

/*****************************************************************************/

DECLARE_OOP_TEST( employees_5_5_add_bonus_non_positive )
{
	Controller c;

	std::string emp1( "Winston Smith" );

	c.createSalaryEmployee( emp1, 200.0 );

	ASSERT_THROWS(
			c.giveBonus( emp1, -220.0 )
		,	Messages::NonPositivePayment
	);

	ASSERT_THROWS(
			c.giveBonus( emp1, 0.0 )
		,	Messages::NonPositivePayment
	);

	CHECK_EQUAL( c.getPayment( emp1 ), 200.0 );
	CHECK_EQUAL( c.getTotalPayment(), 200.0 );
}

/*****************************************************************************/

DECLARE_OOP_TEST( employees_5_6_add_bonus_non_existing_item )
{
	Controller c;

	std::string emp1( "Winston Smith" );

	c.createSalaryEmployee( emp1, 200.0 );

	ASSERT_THROWS(
			c.giveBonus( "Julia", 220.0 )
		,	Messages::UnknownEmployeeName
	);

	CHECK_EQUAL( c.getPayment( emp1 ), 200.0 );
	CHECK_EQUAL( c.getTotalPayment(), 200.0 );
}

/*****************************************************************************/
