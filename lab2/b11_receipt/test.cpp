// (C) 2016, Sergei Zaychenko, KNURE, Kharkiv, Ukraine

/*****************************************************************************/

#include "receipt.hpp"
#include "messages.hpp"

#include "testslib.hpp"

#include <sstream>

/*****************************************************************************/

/*
	Test plan:
	Done	1) Create receipt
	Done		1.1) Simple
	Done		1.2) With empty name
	Done		1.3) With negative account number
	Done	3) Add service
	Done		3.1) Simple
	Done		3.2) With empty name
	Done		3.3) Several services
	Done		3.4) With duplicated name
	Done	4) Remove service
	Done		4.1) Simple
	Done		4.2) With empty name
	Done		4.3) For missing name
	Done		4.4) Add after Remove for the same name
	Done	5) Rename service
	Done		5.1) Simple
	Done		5.2) With empty old name
	Done		5.3) With empty new name
	Done		5.4) For missing old name
	Done		5.5) For duplicated new name
	Done		5.6) Several renames for different services
	Done		5.7) Several renames for the same service
	Done	6) Track debt
	Done		6.1) Simple
	Done		6.2) With empty name
	Done		6.3) For missing service
	Done		6.4) With negative amount
	Done		6.5) Several times
	Done		6.6) After remove service
	Done		6.7) After remove and add service
	Done		6.8) After rename service
	Done	7) Track pay
	Done		7.1) Simple
	Done		7.2) For missing service
	Done		7.3) For empty name
	Done		7.4) With negative amount
	Done		7.5) Several times
	Done		7.6) After remove service
	Done		7.7) After remove and add service
	Done		7.8) After rename service
	Done	8) Debt and Pay
	Done		8.1) For the same service
	Done		8.2) For the same service several times
	Done	9) Has service
	Done		9.1) With empty name
	Done		9.2) For missing name
	Done	10) Get dept amount
	Done		10.1) With empty name
	Done		10.2) For missing name
	Done	11) Get paid amount
	Done		11.1) With empty name
	Done		11.2) For missing name
	Done	12) For each service
	Done		12.1) Empty
	Done		12.2) Single service
	Done		12.3) Several services
	Done	13) Fetch overpaid
	Done		13.1) Empty
	Done		13.2) Matches nothing
	Done		13.3) Matches only part of services
	Done		13.4) Matches all services
	Done	14) Fetch underpaid
	Done		14.1) Empty
	Done		14.2) Matches nothing
	Done		14.3) Matches only part of services
	Done		14.4) Matches all services
	Done	15) Get total debt
	Done		15.1) Empty
	Done		15.2) Several services
	Done	16) Get total paid
	Done		16.1) Empty
	Done		16.2) Several services
*/

/*****************************************************************************/


DECLARE_OOP_TEST( receipt_1_1_create_receipt_simple )
{
	const Receipt r( "me", 12345678 );

	const std::string & payerName = r.getPayerName();
	assert( payerName == "me" );

	assert( r.getAccountNumber() == 12345678 );
	assert( !r.hasServices() );
	assert( r.getServicesCount() == 0 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( receipt_1_2_create_receipt_with_empty_name )
{
	ASSERT_THROWS(
			const Receipt r( "", 12345678 );
		,	Messages::EmptyPayerName
	);
}


/*****************************************************************************/


DECLARE_OOP_TEST( receipt_1_3_create_receipt_with_negative_account_number )
{
	ASSERT_THROWS(
			const Receipt r( "me", 0 );
		,	Messages::InvalidAccountNumber
	);
	ASSERT_THROWS(
			const Receipt r( "me", -1 );
		,	Messages::InvalidAccountNumber
	);
	ASSERT_THROWS(
			const Receipt r( "me", -1000 );
		,	Messages::InvalidAccountNumber
	);
}

/*****************************************************************************/


DECLARE_OOP_TEST( receipt_3_1_add_service_simple )
{
	Receipt r( "me", 12345678 );
	const Receipt & rR = r;

	r.addService( "water" );

	assert( rR.hasServices() );
	assert( rR.getServicesCount() == 1 );

	assert( rR.hasService( "water" ) );
	assert( rR.getDebtAmount( "water" ) == 0.0 );
	assert( rR.getPaidAmount( "water" ) == 0.0 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( receipt_3_2_add_service_with_empty_name )
{
	Receipt r( "me", 12345678 );

	ASSERT_THROWS(
			r.addService( "" );
		,	Messages::EmptyServiceName
	);

	assert( r.getServicesCount() == 0 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( receipt_3_3_add_service_several_services )
{
	Receipt r( "me", 12345678 );
	const Receipt & rR = r;

	r.addService( "water" );
	r.addService( "heat" );
	r.addService( "light" );

	assert( rR.hasServices() );
	assert( rR.getServicesCount() == 3 );

	assert( rR.hasService( "water" ) );
	assert( rR.getDebtAmount( "water" ) == 0.0 );
	assert( rR.getPaidAmount( "water" ) == 0.0 );

	assert( rR.hasService( "heat" ) );
	assert( rR.getDebtAmount( "heat" ) == 0.0 );
	assert( rR.getPaidAmount( "heat" ) == 0.0 );

	assert( rR.hasService( "light" ) );
	assert( rR.getDebtAmount( "light" ) == 0.0 );
	assert( rR.getPaidAmount( "light" ) == 0.0 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( receipt_3_4_add_service_with_duplicated_name )
{
	Receipt r( "me", 12345678 );

	r.addService( "water" );

	ASSERT_THROWS(
			r.addService( "water" );
		,	Messages::DuplicatedService
	);

	assert( r.getServicesCount() == 1 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( receipt_4_1_remove_service_simple )
{
	Receipt r( "me", 12345678 );
	const Receipt & rR = r;

	r.addService( "water" );
	r.addService( "heat" );
	r.addService( "light" );

	r.removeService( "heat" );

	assert( rR.hasServices() );
	assert( rR.getServicesCount() == 2 );

	assert( rR.hasService( "water" ) );
	assert( rR.getDebtAmount( "water" ) == 0.0 );
	assert( rR.getPaidAmount( "water" ) == 0.0 );

	assert( !rR.hasService( "heat" ) );

	assert( rR.hasService( "light" ) );
	assert( rR.getDebtAmount( "light" ) == 0.0 );
	assert( rR.getPaidAmount( "light" ) == 0.0 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( receipt_4_2_remove_service_with_empty_name )
{
	Receipt r( "me", 12345678 );

	r.addService( "water" );
	r.addService( "heat" );
	r.addService( "light" );

	ASSERT_THROWS(
			r.removeService( "" );
		,	Messages::EmptyServiceName
	);

	assert( r.getServicesCount() == 3 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( receipt_4_3_remove_service_with_missing_name )
{
	Receipt r( "me", 12345678 );

	r.addService( "water" );
	r.addService( "heat" );
	r.addService( "light" );

	ASSERT_THROWS(
			r.removeService( "missing" );
		,	Messages::ServiceDoesNotExist
	);

	assert( r.getServicesCount() == 3 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( receipt_4_4_remove_service_add_after_remove_for_the_same_name )
{
	Receipt r( "me", 12345678 );
	const Receipt & rR = r;

	r.addService( "water" );
	r.removeService( "water" );
	r.addService( "water" );

	assert( rR.hasServices() );
	assert( rR.getServicesCount() == 1 );

	assert( rR.hasService( "water" ) );
	assert( rR.getDebtAmount( "water" ) == 0.0 );
	assert( rR.getPaidAmount( "water" ) == 0.0 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( receipt_5_1_rename_service_simple )
{
	Receipt r( "me", 12345678 );
	const Receipt & rR = r;

	r.addService( "water" );
	r.addService( "heat" );

	r.renameService( "water", "black tea" );

	assert( rR.hasServices() );
	assert( rR.getServicesCount() == 2 );

	assert( rR.hasService( "black tea" ) );
	assert( rR.getDebtAmount( "black tea" ) == 0.0 );
	assert( rR.getPaidAmount( "black tea" ) == 0.0 );

	assert( rR.hasService( "heat" ) );
	assert( rR.getDebtAmount( "heat" ) == 0.0 );
	assert( rR.getPaidAmount( "heat" ) == 0.0 );

	assert( !rR.hasService( "water" ) );
}


/*****************************************************************************/


DECLARE_OOP_TEST( receipt_5_2_rename_service_with_empty_old_name )
{
	Receipt r( "me", 12345678 );

	r.addService( "water" );

	ASSERT_THROWS(
			r.renameService( "", "new name" );
		,	Messages::EmptyServiceName
	);

	assert( r.getServicesCount() == 1 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( receipt_5_3_rename_service_with_empty_new_name )
{
	Receipt r( "me", 12345678 );

	r.addService( "water" );

	ASSERT_THROWS(
			r.renameService( "water", "" );
		,	Messages::EmptyServiceName
	);

	assert( r.getServicesCount() == 1 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( receipt_5_4_rename_service_for_missing_old_name )
{
	Receipt r( "me", 12345678 );

	r.addService( "water" );

	ASSERT_THROWS(
			r.renameService( "heat", "new name" );
		,	Messages::ServiceDoesNotExist
	);

	assert( r.getServicesCount() == 1 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( receipt_5_5_rename_service_for_duplicated_new_name )
{
	Receipt r( "me", 12345678 );

	r.addService( "water" );
	r.addService( "heat" );

	ASSERT_THROWS(
			r.renameService( "water", "heat" );
		,	Messages::DuplicatedService
	);

	assert( r.getServicesCount() == 2 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( receipt_5_6_rename_service_several_renames_for_different_services )
{
	Receipt r( "me", 12345678 );
	const Receipt & rR = r;

	r.addService( "water" );
	r.addService( "heat" );

	r.renameService( "water", "black tea" );
	r.renameService( "heat", "socks" );

	assert( rR.hasServices() );
	assert( rR.getServicesCount() == 2 );

	assert( rR.hasService( "black tea" ) );
	assert( rR.getDebtAmount( "black tea" ) == 0.0 );
	assert( rR.getPaidAmount( "black tea" ) == 0.0 );

	assert( rR.hasService( "socks" ) );
	assert( rR.getDebtAmount( "socks" ) == 0.0 );
	assert( rR.getPaidAmount( "socks" ) == 0.0 );

	assert( !rR.hasService( "water" ) );
	assert( !rR.hasService( "heat" ) );
}


/*****************************************************************************/


DECLARE_OOP_TEST( receipt_5_7_rename_service_several_renames_for_same_service )
{
	Receipt r( "me", 12345678 );
	const Receipt & rR = r;

	r.addService( "water" );
	r.addService( "heat" );

	r.renameService( "heat", "white socks" );
	r.renameService( "white socks", "grey socks" );
	r.renameService( "grey socks", "black socks" );

	assert( rR.hasServices() );
	assert( rR.getServicesCount() == 2 );

	assert( rR.hasService( "water" ) );
	assert( rR.getDebtAmount( "water" ) == 0.0 );
	assert( rR.getPaidAmount( "water" ) == 0.0 );

	assert( rR.hasService( "black socks" ) );
	assert( rR.getDebtAmount( "black socks" ) == 0.0 );
	assert( rR.getPaidAmount( "black socks" ) == 0.0 );

	assert( !rR.hasService( "heat" ) );
	assert( !rR.hasService( "white socks" ) );
	assert( !rR.hasService( "grey socks" ) );
}


/*****************************************************************************/


DECLARE_OOP_TEST( receipt_6_1_track_debt_simple )
{
	Receipt r( "me", 12345678 );
	const Receipt & rR = r;

	r.addService( "water" );
	r.trackDebt( "water", 100.0 );

	assert( rR.getDebtAmount( "water" ) == 100.0 );
	assert( rR.getPaidAmount( "water" ) == 0.0 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( receipt_6_2_track_debt_with_empty_name )
{
	Receipt r( "me", 12345678 );
	const Receipt & rR = r;

	r.addService( "water" );

	ASSERT_THROWS(
			r.trackDebt( "", 100.0 );
		,	Messages::EmptyServiceName
	);

	assert( rR.getDebtAmount( "water" ) == 0.0 );
	assert( rR.getPaidAmount( "water" ) == 0.0 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( receipt_6_3_track_debt_for_missing_service )
{
	Receipt r( "me", 12345678 );
	const Receipt & rR = r;

	r.addService( "water" );

	ASSERT_THROWS(
			r.trackDebt( "missing", 100.0 );
		,	Messages::ServiceDoesNotExist
	);

	assert( rR.getDebtAmount( "water" ) == 0.0 );
	assert( rR.getPaidAmount( "water" ) == 0.0 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( receipt_6_4_track_debt_with_negative_amount )
{
	Receipt r( "me", 12345678 );
	const Receipt & rR = r;

	r.addService( "water" );
	r.trackDebt( "water", 0.0 );
	
	assert( r.getDebtAmount( "water" ) == 0.0 );

	r.trackDebt( "water", -1.0 );
	assert( r.getDebtAmount( "water" ) == -1.0 );

	assert( rR.getDebtAmount( "water" ) == -1.0 );
	assert( rR.getPaidAmount( "water" ) == 0.0 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( receipt_6_5_track_debt_several_times )
{
	Receipt r( "me", 12345678 );
	const Receipt & rR = r;

	r.addService( "water" );
	r.trackDebt( "water", 100.0 );
	r.trackDebt( "water", 25.5 );
	r.trackDebt( "water", 27.1 );

	assert( rR.getDebtAmount( "water" ) == 152.6 );
	assert( rR.getPaidAmount( "water" ) == 0.0 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( receipt_6_6_track_debt_after_remove_service )
{
	Receipt r( "me", 12345678 );
	const Receipt & rR = r;

	r.addService( "water" );
	r.removeService( "water" );

	ASSERT_THROWS(
			r.trackDebt( "water", 0.0 );
		,	Messages::ServiceDoesNotExist
	);
}


/*****************************************************************************/


DECLARE_OOP_TEST( receipt_6_7_track_debt_after_remove_and_add_service )
{
	Receipt r( "me", 12345678 );
	const Receipt & rR = r;

	r.addService( "water" );
	r.trackDebt( "water", 300.0 );

	r.removeService( "water" );
	r.addService( "water" );
	r.trackDebt( "water", 200.0 );
	r.trackDebt( "water", 25.5 );
	r.trackDebt( "water", 27.1 );

	assert( rR.getDebtAmount( "water" ) == 252.6 );
	assert( rR.getPaidAmount( "water" ) == 0.0 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( receipt_6_8_track_debt_after_rename_service )
{
	Receipt r( "me", 12345678 );
	const Receipt & rR = r;

	r.addService( "water" );
	r.trackDebt( "water", 300.0 );

	r.renameService( "water", "bu" );
	r.trackDebt( "bu", 200.0 );
	r.trackDebt( "bu", 25.5 );
	r.trackDebt( "bu", 27.1 );

	assert( rR.getDebtAmount( "bu" ) == 552.6 );
	assert( rR.getPaidAmount( "bu" ) == 0.0 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( receipt_7_1_track_pay_simple )
{
	Receipt r( "me", 12345678 );
	const Receipt & rR = r;

	r.addService( "water" );
	r.trackPay( "water", 100.0 );

	assert( rR.getDebtAmount( "water" ) == 0.0 );
	assert( rR.getPaidAmount( "water" ) == 100.0 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( receipt_7_2_track_pay_with_empty_name )
{
	Receipt r( "me", 12345678 );
	const Receipt & rR = r;

	r.addService( "water" );

	ASSERT_THROWS(
			r.trackPay( "", 100.0 );
		,	Messages::EmptyServiceName
	);

	assert( rR.getDebtAmount( "water" ) == 0.0 );
	assert( rR.getPaidAmount( "water" ) == 0.0 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( receipt_7_3_track_pay_for_missing_service )
{
	Receipt r( "me", 12345678 );
	const Receipt & rR = r;

	r.addService( "water" );

	ASSERT_THROWS(
			r.trackPay( "missing", 100.0 );
		,	Messages::ServiceDoesNotExist
	);

	assert( rR.getDebtAmount( "water" ) == 0.0 );
	assert( rR.getPaidAmount( "water" ) == 0.0 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( receipt_7_4_track_pay_with_negative_amount )
{
	Receipt r( "me", 12345678 );
	const Receipt & rR = r;

	r.addService( "water" );

	r.trackPay( "water", 0.0 );
	assert( r.getPaidAmount( "water" ) == 0.0 );

	ASSERT_THROWS(
			r.trackPay( "water", -1.0 );
		,	Messages::InvalidAmountValue
	);
	ASSERT_THROWS(
			r.trackPay( "water", -100.0 );
		,	Messages::InvalidAmountValue
	);

	assert( rR.getDebtAmount( "water" ) == 0.0 );
	assert( rR.getPaidAmount( "water" ) == 0.0 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( receipt_7_5_track_pay_several_times )
{
	Receipt r( "me", 12345678 );
	const Receipt & rR = r;

	r.addService( "water" );
	r.trackPay( "water", 100.0 );
	r.trackPay( "water", 25.5 );
	r.trackPay( "water", 27.1 );

	assert( rR.getDebtAmount( "water" ) == 0.0 );
	assert( rR.getPaidAmount( "water" ) == 152.6 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( receipt_7_6_track_pay_after_remove_service )
{
	Receipt r( "me", 12345678 );
	const Receipt & rR = r;

	r.addService( "water" );
	r.removeService( "water" );

	ASSERT_THROWS(
			r.trackPay( "water", 0.0 );
		,	Messages::ServiceDoesNotExist
	);
}


/*****************************************************************************/


DECLARE_OOP_TEST( receipt_7_7_track_pay_after_remove_and_add_service )
{
	Receipt r( "me", 12345678 );
	const Receipt & rR = r;

	r.addService( "water" );
	r.trackPay( "water", 300.0 );

	r.removeService( "water" );
	r.addService( "water" );
	r.trackPay( "water", 200.0 );
	r.trackPay( "water", 25.5 );
	r.trackPay( "water", 27.1 );

	assert( rR.getDebtAmount( "water" ) == 0.0 );
	assert( rR.getPaidAmount( "water" ) == 252.6 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( receipt_7_8_track_pay_after_rename_service )
{
	Receipt r( "me", 12345678 );
	const Receipt & rR = r;

	r.addService( "water" );
	r.trackPay( "water", 300.0 );

	r.renameService( "water", "bu" );
	r.trackPay( "bu", 200.0 );
	r.trackPay( "bu", 25.5 );
	r.trackPay( "bu", 27.1 );

	assert( rR.getDebtAmount( "bu" ) == 0.0 );
	assert( rR.getPaidAmount( "bu" ) == 552.6 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( receipt_8_1_debt_and_pay_for_the_same_service )
{
	Receipt r( "me", 12345678 );
	const Receipt & rR = r;

	r.addService( "water" );
	r.trackDebt( "water", 21.0 );
	r.trackPay( "water", 75.0 );

	assert( rR.getDebtAmount( "water" ) == 21.0 );
	assert( rR.getPaidAmount( "water" ) == 75.0 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( receipt_8_2_debt_and_pay_for_the_same_service_several_times )
{
	Receipt r( "me", 12345678 );
	const Receipt & rR = r;

	r.addService( "water" );
	r.trackPay( "water", 75.0 );
	r.trackDebt( "water", 21.0 );
	r.trackPay( "water", 11.0 );
	r.trackDebt( "water", 44.0 );

	assert( rR.getDebtAmount( "water" ) == 65.0 );
	assert( rR.getPaidAmount( "water" ) == 86.0 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( receipt_9_1_has_service_with_empty_name )
{
	Receipt r( "me", 12345678 );
	const Receipt & rR = r;

	r.addService( "water" );

	assert( !rR.hasService( "" ) );
}


/*****************************************************************************/


DECLARE_OOP_TEST( receipt_9_2_has_service_for_missing_name )
{
	Receipt r( "me", 12345678 );
	const Receipt & rR = r;

	r.addService( "water" );

	assert( !rR.hasService( "missing" ) );
}


/*****************************************************************************/


DECLARE_OOP_TEST( receipt_10_1_get_dept_amount_with_empty_name )
{
	Receipt r( "me", 12345678 );
	const Receipt & rR = r;

	r.addService( "water" );
	r.trackPay( "water", 75.0 );
	r.trackDebt( "water", 21.0 );

	ASSERT_THROWS(
			rR.getDebtAmount( "" );
		,	Messages::EmptyServiceName
	);
}


/*****************************************************************************/


DECLARE_OOP_TEST( receipt_10_2_get_dept_amount_for_missing_name )
{
	Receipt r( "me", 12345678 );
	const Receipt & rR = r;

	r.addService( "water" );
	r.trackPay( "water", 75.0 );
	r.trackDebt( "water", 21.0 );

	ASSERT_THROWS(
			rR.getDebtAmount( "missing" );
		,	Messages::ServiceDoesNotExist
	);
}


/*****************************************************************************/


DECLARE_OOP_TEST( receipt_11_1_get_paid_amount_with_empty_name )
{
	Receipt r( "me", 12345678 );
	const Receipt & rR = r;

	r.addService( "water" );
	r.trackPay( "water", 75.0 );
	r.trackDebt( "water", 21.0 );

	ASSERT_THROWS(
			rR.getPaidAmount( "" );
		,	Messages::EmptyServiceName
	);
}


/*****************************************************************************/


DECLARE_OOP_TEST( receipt_11_2_get_paid_amount_for_missing_name )
{
	Receipt r( "me", 12345678 );
	const Receipt & rR = r;

	r.addService( "water" );
	r.trackPay( "water", 75.0 );
	r.trackDebt( "water", 21.0 );

	ASSERT_THROWS(
			rR.getPaidAmount( "missing" );
		,	Messages::ServiceDoesNotExist
	);
}


/*****************************************************************************/


DECLARE_OOP_TEST( receipt_12_1_for_each_service_empty )
{
	Receipt r( "me", 12345678 );
	const Receipt & rR = r;

	rR.forEachService(
		[&]( const std::string & _serviceName, double _debt, double _paid )
		{
			assert( ! "We should never be here without services" );
		}
	);
}


/*****************************************************************************/


DECLARE_OOP_TEST( receipt_12_2_for_each_service_single_service )
{
	Receipt r( "me", 12345678 );
	const Receipt & rR = r;

	r.addService( "water" );
	r.trackDebt( "water", 12.1 );
	r.trackPay( "water", 13.2 );

	int matchesCount = 0;
	rR.forEachService(
		[&]( const std::string & _serviceName, double _debt, double _paid )
		{
			if ( matchesCount == 0 )
			{
				assert( _serviceName == "water" );
				assert( _debt == 12.1 );
				assert( _paid == 13.2 );

				++matchesCount;
			}
			else
				assert( ! "We should never be here without services" );
		}
	);

	assert( matchesCount == 1 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( receipt_12_3_for_each_service_several_services )
{
	Receipt r( "me", 12345678 );
	const Receipt & rR = r;

	r.addService( "water" );
	r.trackDebt( "water", 12.1 );
	r.trackPay( "water", 13.2 );

	r.addService( "whisky" );
	r.trackDebt( "whisky", 0.7 );
	r.trackPay( "whisky", 0.5 );

	int matchesCount = 0;
	bool waterMatched = false;
	bool whiskyMatched = false;
	rR.forEachService(
		[&]( const std::string & _serviceName, double _debt, double _paid )
		{
			if ( matchesCount == 0 || matchesCount == 1 )
			{
				if ( _serviceName == "water" )
				{
					assert( _serviceName == "water" );
					assert( _debt == 12.1 );
					assert( _paid == 13.2 );

					waterMatched = true;
				}
				if ( _serviceName == "whisky" )
				{
					assert( _serviceName == "whisky" );
					assert( _debt == 0.7 );
					assert( _paid == 0.5 );

					whiskyMatched = true;
				}

				++matchesCount;
			}
			else
				assert( ! "We should never be here without services" );
		}
	);

	assert( matchesCount == 2 );
	assert( waterMatched );
	assert( whiskyMatched );
}


/*****************************************************************************/


DECLARE_OOP_TEST( receipt_13_1_fetch_overpaid_empty )
{
	Receipt r( "me", 12345678 );
	const Receipt & rR = r;

	std::vector< std::string > result = rR.fetchOverpaid();
	std::vector< std::string > expected = {};

	assert( result == expected );
}


/*****************************************************************************/


DECLARE_OOP_TEST( receipt_13_2_fetch_overpaid_matches_nothing )
{
	Receipt r( "me", 12345678 );
	const Receipt & rR = r;

	r.addService( "service 1" );
		r.trackDebt( "service 1", 100.0 );
		r.trackPay( "service 1", 99.0 );
	r.addService( "service 2" );
		r.trackDebt( "service 2", 200.0 );
		r.trackPay( "service 2", 1.0 );
	r.addService( "service 3" );
		r.trackDebt( "service 3", 300.0 );
		r.trackPay( "service 3", 222.0 );

	std::vector< std::string > result = rR.fetchOverpaid();
	std::vector< std::string > expected = {};

	assert( result == expected );
}


/*****************************************************************************/


DECLARE_OOP_TEST( receipt_13_3_fetch_overpaid_matches_only_part_of_services )
{
	Receipt r( "me", 12345678 );
	const Receipt & rR = r;

	r.addService( "service 1" );
		r.trackDebt( "service 1", 100.0 );
		r.trackPay( "service 1", 99.0 );
	r.addService( "service 2" );
		r.trackDebt( "service 2", -200.0 );
		r.trackPay( "service 2", 0.0 );
	r.addService( "service 3" );
		r.trackDebt( "service 3", -300.0 );
		r.trackPay( "service 3", 0.0 );

	std::vector< std::string > result = rR.fetchOverpaid();
	std::vector< std::string > expected = { "service 2", "service 3" };

	assert( result == expected );
}


/*****************************************************************************/


DECLARE_OOP_TEST( receipt_13_4_fetch_overpaid_matches_all_services )
{
	Receipt r( "me", 12345678 );
	const Receipt & rR = r;

	r.addService( "service 1" );
		r.trackDebt( "service 1", -100.0 );
		r.trackPay( "service 1", 0.0 );
	r.addService( "service 2" );
		r.trackDebt( "service 2", -200.0 );
		r.trackPay( "service 2", 200.0 );
	r.addService( "service 3" );
		r.trackDebt( "service 3", -300.0 );
		r.trackPay( "service 3", 350.0 );

	std::vector< std::string > result = rR.fetchOverpaid();
	std::vector< std::string > expected = { "service 1", "service 2", "service 3" };

	assert( result == expected );
}


/*****************************************************************************/


DECLARE_OOP_TEST( receipt_14_1_fetch_underpaid_empty )
{
	Receipt r( "me", 12345678 );
	const Receipt & rR = r;

	std::vector< std::string > result = rR.fetchUnderpaid();
	std::vector< std::string > expected = {};

	assert( result == expected );
}


/*****************************************************************************/


DECLARE_OOP_TEST( receipt_14_2_fetch_underpaid_matches_nothing )
{
	Receipt r( "me", 12345678 );
	const Receipt & rR = r;

	r.addService( "service 1" );
		r.trackDebt( "service 1", 0.0 );
		r.trackPay( "service 1", 1.0 );
	r.addService( "service 2" );
		r.trackDebt( "service 2", -1.0 );
		r.trackPay( "service 2", 200.0 );
	r.addService( "service 3" );
		r.trackDebt( "service 3", -222.0 );
		r.trackPay( "service 3", 300.0 );

	std::vector< std::string > result = rR.fetchUnderpaid();
	std::vector< std::string > expected = {};

	assert( result == expected );
}


/*****************************************************************************/


DECLARE_OOP_TEST( receipt_14_3_fetch_underpaid_matches_only_part_of_services )
{
	Receipt r( "me", 12345678 );
	const Receipt & rR = r;

	r.addService( "service 1" );
		r.trackDebt( "service 1", -22.0 );
		r.trackPay( "service 1", 23.0 );
	r.addService( "service 2" );
		r.trackDebt( "service 2", 200.0 );
		r.trackPay( "service 2", 199.0 );
	r.addService( "service 3" );
		r.trackDebt( "service 3", 300.0 );

	std::vector< std::string > result = rR.fetchUnderpaid();
	std::vector< std::string > expected = { "service 2", "service 3" };

	assert( result == expected );
}


/*****************************************************************************/


DECLARE_OOP_TEST( receipt_14_4_fetch_underpaid_matches_all_services )
{
	Receipt r( "me", 12345678 );
	const Receipt & rR = r;

	r.addService( "service 1" );
		r.trackDebt( "service 1", 100.0 );
		r.trackPay( "service 1", 99.0 );
	r.addService( "service 2" );
		r.trackDebt( "service 2", 200.0 );
		r.trackPay( "service 2", 100.0 );
	r.addService( "service 3" );
		r.trackDebt( "service 3", 300.0 );
		r.trackPay( "service 3", 50.0 );

	std::vector< std::string > result = rR.fetchUnderpaid();
	std::vector< std::string > expected = { "service 1", "service 2", "service 3" };

	assert( result == expected );
}


/*****************************************************************************/


DECLARE_OOP_TEST( receipt_15_1_get_total_debt_empty )
{
	Receipt r( "me", 12345678 );
	const Receipt & rR = r;

	assert( rR.getTotalDebt() == 0.0 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( receipt_15_2_get_total_debt_several_services )
{
	Receipt r( "me", 12345678 );
	const Receipt & rR = r;

	r.addService( "service 1" );
		r.trackDebt( "service 1", 100.0 );
		r.trackPay( "service 1", 99.0 );
	r.addService( "service 2" );
		r.trackDebt( "service 2", 200.0 );
		r.trackPay( "service 2", 100.0 );
	r.addService( "service 3" );
		r.trackDebt( "service 3", 300.0 );
		r.trackPay( "service 3", 50.0 );

	assert( rR.getTotalDebt() == 600.0 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( receipt_16_1_get_total_paid_empty )
{
	Receipt r( "me", 12345678 );
	const Receipt & rR = r;

	assert( rR.getTotalPaid() == 0.0 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( receipt_16_2_get_total_paid_several_services )
{
	Receipt r( "me", 12345678 );
	const Receipt & rR = r;

	r.addService( "service 1" );
		r.trackDebt( "service 1", 100.0 );
		r.trackPay( "service 1", 99.0 );
	r.addService( "service 2" );
		r.trackDebt( "service 2", 200.0 );
		r.trackPay( "service 2", 100.0 );
	r.addService( "service 3" );
		r.trackDebt( "service 3", 300.0 );
		r.trackPay( "service 3", 50.0 );

	assert( rR.getTotalPaid() == 249.0 );
}


/*****************************************************************************/
