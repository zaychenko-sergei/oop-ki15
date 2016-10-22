// (C) 2016, Sergei Zaychenko, KNURE, Kharkiv, Ukraine

/*****************************************************************************/

#include "testslib.hpp"

#include "passport.hpp"
#include "messages.hpp"

/*****************************************************************************/

DECLARE_OOP_TEST ( test_PassportConstructor )
{
	Passport p( "AB123456", Date( 2010, 1, 1 ), "Ivan Ivanov" );

	assert( p.getPassportId() == "AB123456" );
	assert( p.getIssued() == Date( 2010, 1, 1 ) );
	assert( p.getOwnerFullName() == "Ivan Ivanov" );
}

/*****************************************************************************/

DECLARE_OOP_TEST ( test_PassportConstructor_BadPassportId )
{
	ASSERT_THROWS(
			Passport( "", Date( 2010, 1, 1 ), "Ivan Ivanov" )
		,	Messages::BadPassportId
	)

	ASSERT_THROWS(
			Passport( "   ", Date( 2010, 1, 1 ), "Ivan Ivanov" )
		,	Messages::BadPassportId
	)

	ASSERT_THROWS(
			Passport( "AB1111111", Date( 2010, 1, 1 ), "Ivan Ivanov" )
		,	Messages::BadPassportId
	)

	ASSERT_THROWS(
			Passport( "A1111111", Date( 2010, 1, 1 ), "Ivan Ivanov" )
		,	Messages::BadPassportId
	)

	ASSERT_THROWS(
			Passport( "ABC11111", Date( 2010, 1, 1 ), "Ivan Ivanov" )
		,	Messages::BadPassportId
	)
}

/*****************************************************************************/

DECLARE_OOP_TEST ( test_PassportConstructor_IssuedInFuture )
{
	ASSERT_THROWS(
			Passport( "AB123456", Date( 2100, 1, 1 ), "Ivan Ivanov" )
		,	Messages::PassportIssuedInFuture
	)

	Passport p ( "AB123456", Date( /* today */ ), "Ivan Ivanov" );
	assert( p.getIssued() == Date() );
}

/*****************************************************************************/

DECLARE_OOP_TEST ( test_PassportConstructor_BadOwnerName )
{
	ASSERT_THROWS(
			Passport( "AB123456", Date( 2010, 1, 1 ), "" )
		,	Messages::OwnerEmptyName
	)
}

/*****************************************************************************/

DECLARE_OOP_TEST ( test_PassportNoRegistrationsAtBeginning )
{
	Passport p( "AB123456", Date( 2010, 1, 1 ), "Ivan Ivanov" );

	assert( ! p.getTotalRegistrationRecordsCount() );
	assert( ! p.isCurrentlyRegistered() );

	ASSERT_THROWS( p.getCurrentRegistration(), Messages::NoCurrentRegistration );
}

/*****************************************************************************/

DECLARE_OOP_TEST ( test_PassportSingleRegistration_StillValid )
{
	Passport p( "AB123456", Date( 2010, 1, 1 ), "Ivan Ivanov" );
	p.trackRegistration( "Sumskaya 1", Date( 2010, 1, 1 ) );

	assert( p.isCurrentlyRegistered() );
	assert( p.getTotalRegistrationRecordsCount() == 1 );

	Passport::RegistrationRecord expectedRecord = Passport::RegistrationRecord( "Sumskaya 1", Date( 2010, 1, 1 ), true );
	assert( p.getCurrentRegistration() == expectedRecord );
	assert( p.getRegistrationRecord( 0 ) == expectedRecord );
}

/*****************************************************************************/

DECLARE_OOP_TEST ( test_PassportSingleRegistration_DateBeforePassport )
{
	Passport p( "AB123456", Date( 2010, 1, 1 ), "Ivan Ivanov" );

	ASSERT_THROWS(
			p.trackRegistration( "Sumskaya 1", Date( 2009, 12, 31 ) )
		,	Messages::RegistrationDateBeforePassportIssue
	)
}

/*****************************************************************************/

DECLARE_OOP_TEST ( test_PassportSingleRegistration_DateInFuture )
{
	Passport p( "AB123456", Date( 2010, 1, 1 ), "Ivan Ivanov" );

	ASSERT_THROWS(
			p.trackRegistration( "Sumskaya 1", Date( 2100, 1, 1 ) )
		,	Messages::RegistrationDateInFuture
	)
}

/*****************************************************************************/

DECLARE_OOP_TEST ( test_PassportSingleRegistration_DoubleRegistration )
{
	Passport p( "AB123456", Date( 2010, 1, 1 ), "Ivan Ivanov" );
	p.trackRegistration( "Sumskaya 1", Date( 2010, 1, 1 ) );

	ASSERT_THROWS(
			p.trackRegistration( "Pushkinskaya 1", Date( 2011, 11, 1 ) )
		,	Messages::CurrentRegistrationStillActive
	)
}

/*****************************************************************************/

DECLARE_OOP_TEST ( test_PassportSingleRegistrationUnregistration )
{
	Passport p( "AB123456", Date( 2010, 1, 1 ), "Ivan Ivanov" );
	p.trackRegistration( "Sumskaya 1", Date( 2010, 1, 1 ) );
	p.trackUnregistration( Date( 2012, 1, 1 ) );

	assert( ! p.isCurrentlyRegistered() );
	assert( p.getTotalRegistrationRecordsCount() == 2 );
	ASSERT_THROWS( p.getCurrentRegistration(), Messages::NoCurrentRegistration );

	Passport::RegistrationRecord expectedRecord0 = Passport::RegistrationRecord( "Sumskaya 1", Date( 2010, 1, 1 ), true );
	assert( p.getRegistrationRecord( 0 ) == expectedRecord0 );

	Passport::RegistrationRecord expectedRecord1 = Passport::RegistrationRecord( "Sumskaya 1", Date( 2012, 1, 1 ), false );
	assert( p.getRegistrationRecord( 1 ) == expectedRecord1 );
}

/*****************************************************************************/

DECLARE_OOP_TEST ( test_PassportUnregisterSameDayAsRegisteredIsFine )
{
	Passport p( "AB123456", Date( 2010, 1, 1 ), "Ivan Ivanov" );
	p.trackRegistration( "Sumskaya 1", Date( 2010, 1, 1 ) );
	p.trackUnregistration( Date( 2010, 1, 1 ) );

	assert( !p.isCurrentlyRegistered() );
	assert( p.getTotalRegistrationRecordsCount() == 2 );
	ASSERT_THROWS( p.getCurrentRegistration(), Messages::NoCurrentRegistration );

	Passport::RegistrationRecord expectedRecord0 = Passport::RegistrationRecord( "Sumskaya 1", Date( 2010, 1, 1 ), true );
	assert( p.getRegistrationRecord( 0 ) == expectedRecord0 );

	Passport::RegistrationRecord expectedRecord1 = Passport::RegistrationRecord( "Sumskaya 1", Date( 2010, 1, 1 ), false );
	assert( p.getRegistrationRecord( 1 ) == expectedRecord1 );
}

/*****************************************************************************/

DECLARE_OOP_TEST ( test_PassportUnregisterWithDateLaterThanRegister )
{
	Passport p( "AB123456", Date( 2010, 1, 1 ), "Ivan Ivanov" );
	p.trackRegistration( "Sumskaya 1", Date( 2010, 1, 1 ) );

	ASSERT_THROWS(
			p.trackUnregistration( Date( 2009, 12, 31 ) )
		,	Messages::UnregisteringEarlierThanRegistered
	)
}

/*****************************************************************************/

DECLARE_OOP_TEST ( test_PassportUnregisterInFuture )
{
	Passport p( "AB123456", Date( 2010, 1, 1 ), "Ivan Ivanov" );
	p.trackRegistration( "Sumskaya 1", Date( 2010, 1, 1 ) );

	ASSERT_THROWS(
			p.trackUnregistration( Date( 2100, 12, 31 ) )
		,	Messages::UnregistrationDateInFuture
	)
}

/*****************************************************************************/

DECLARE_OOP_TEST ( test_PassportUnregisterWithoutRegistering )
{
	Passport p( "AB123456", Date( 2010, 1, 1 ), "Ivan Ivanov" );

	ASSERT_THROWS(
			p.trackUnregistration( Date( 2012, 1, 1 ) );
		,	Messages::NoCurrentRegistration
	)
}

/*****************************************************************************/

DECLARE_OOP_TEST ( test_PassportRegister_ButUnregisterTwice )
{
	Passport p( "AB123456", Date( 2010, 1, 1 ), "Ivan Ivanov" );
	p.trackRegistration( "Sumskaya 1", Date( 2010, 1, 1 ) );
	p.trackUnregistration( Date( 2012, 1, 1 ) );

	ASSERT_THROWS(
			p.trackUnregistration( Date( 2012, 1, 1 ) );
		,	Messages::NoCurrentRegistration
	)
}

/*****************************************************************************/

DECLARE_OOP_TEST ( test_PassportRegisterUnregisterRegister )
{
	Passport p( "AB123456", Date( 2010, 1, 1 ), "Ivan Ivanov" );
	p.trackRegistration( "Sumskaya 1", Date( 2010, 1, 1 ) );
	p.trackUnregistration( Date( 2012, 1, 1 ) );
	p.trackRegistration( "Pushkinskaya 1", Date( 2012, 1, 5 ) );

	assert( p.isCurrentlyRegistered() );
	assert( p.getTotalRegistrationRecordsCount() == 3 );

	Passport::RegistrationRecord expectedRecord0 = Passport::RegistrationRecord( "Sumskaya 1", Date( 2010, 1, 1 ), true );
	assert( p.getRegistrationRecord( 0 ) == expectedRecord0 );

	Passport::RegistrationRecord expectedRecord1 = Passport::RegistrationRecord( "Sumskaya 1", Date( 2012, 1, 1 ), false );
	assert( p.getRegistrationRecord( 1 ) == expectedRecord1 );

	Passport::RegistrationRecord expectedRecord2 = Passport::RegistrationRecord( "Pushkinskaya 1", Date( 2012, 1, 5 ), true );
	assert( p.getRegistrationRecord( 2 ) == expectedRecord2 );

	assert( p.getCurrentRegistration() == expectedRecord2 );
}

/*****************************************************************************/

DECLARE_OOP_TEST ( test_PassportRegisterAfterUnregisterOnSameDay_IsFine )
{
	Passport p( "AB123456", Date( 2010, 1, 1 ), "Ivan Ivanov" );
	p.trackRegistration( "Sumskaya 1", Date( 2010, 1, 1 ) );
	p.trackUnregistration( Date( 2012, 1, 1 ) );
	p.trackRegistration( "Pushkinskaya 1", Date( 2012, 1, 1 ) );

	assert( p.isCurrentlyRegistered() );
	assert( p.getTotalRegistrationRecordsCount() == 3 );

	Passport::RegistrationRecord expectedRecord0 = Passport::RegistrationRecord( "Sumskaya 1", Date( 2010, 1, 1 ), true );
	assert( p.getRegistrationRecord( 0 ) == expectedRecord0 );

	Passport::RegistrationRecord expectedRecord1 = Passport::RegistrationRecord( "Sumskaya 1", Date( 2012, 1, 1 ), false );
	assert( p.getRegistrationRecord( 1 ) == expectedRecord1 );

	Passport::RegistrationRecord expectedRecord2 = Passport::RegistrationRecord( "Pushkinskaya 1", Date( 2012, 1, 1 ), true );
	assert( p.getRegistrationRecord( 2 ) == expectedRecord2 );

	assert( p.getCurrentRegistration() == expectedRecord2 );
}

/*****************************************************************************/

DECLARE_OOP_TEST ( test_PassportReregisterSameAddress_IsFine )
{
	Passport p( "AB123456", Date( 2010, 1, 1 ), "Ivan Ivanov" );
	p.trackRegistration( "Sumskaya 1", Date( 2010, 1, 1 ) );
	p.trackUnregistration( Date( 2012, 1, 1 ) );
	p.trackRegistration( "Sumskaya 1", Date( 2012, 1, 5 ) );

	assert( p.isCurrentlyRegistered() );
	assert( p.getTotalRegistrationRecordsCount() == 3 );

	Passport::RegistrationRecord expectedRecord0 = Passport::RegistrationRecord( "Sumskaya 1", Date( 2010, 1, 1 ), true );
	assert( p.getRegistrationRecord( 0 ) == expectedRecord0 );

	Passport::RegistrationRecord expectedRecord1 = Passport::RegistrationRecord( "Sumskaya 1", Date( 2012, 1, 1 ), false );
	assert( p.getRegistrationRecord( 1 ) == expectedRecord1 );

	Passport::RegistrationRecord expectedRecord2 = Passport::RegistrationRecord( "Sumskaya 1", Date( 2012, 1, 5 ), true );
	assert( p.getRegistrationRecord( 2 ) == expectedRecord2 );

	assert( p.getCurrentRegistration() == expectedRecord2 );
}

/*****************************************************************************/

DECLARE_OOP_TEST ( test_PassportReregisterEarlierThanLastUnregisteration_IsBad )
{
	Passport p( "AB123456", Date( 2010, 1, 1 ), "Ivan Ivanov" );
	p.trackRegistration( "Sumskaya 1", Date( 2010, 1, 1 ) );
	p.trackUnregistration( Date( 2012, 1, 1 ) );

	ASSERT_THROWS(
			p.trackRegistration( "Pushkinskaya 1", Date( 2011, 1, 1 ) );
		,	Messages::PreviousRegistrationLaterThanCurrent
	)
}

/*****************************************************************************/

DECLARE_OOP_TEST ( test_PassportIteratingEmptyRegistrationList )
{
	Passport p( "AB123456", Date( 2010, 1, 1 ), "Ivan Ivanov" );
	p.forEachRegistration(
		[ & ] ( Date _registration, Date _unregistration, std::string const & _address, bool _stillOpen )
		{
			assert( ! "We should never be here without registrations" );
		}
	);
}


/*****************************************************************************/

DECLARE_OOP_TEST ( test_PassportIteratingSingleRegistration )
{
	Passport p( "AB123456", Date( 2010, 1, 1 ), "Ivan Ivanov" );
	p.trackRegistration( "Sumskaya 1", Date( 2011, 1, 1 ) );

	int nMatched = 0;
	p.forEachRegistration(
		[ & ] ( Date _registration, Date _unregistration, std::string const & _address, bool _stillOpen )
		{
			if ( nMatched == 0 )
			{
				assert( _registration == Date( 2011, 1, 1 ) );
				assert( _unregistration == Date() );
				assert( _address == "Sumskaya 1" );
				assert( _stillOpen );

				nMatched = 1;
			}
			else
				assert( ! "There was only 1 record" );
		}
	);
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_PassportIteratingRegisterUnregister )
{
	Passport p( "AB123456", Date( 2010, 1, 1 ), "Ivan Ivanov" );
	p.trackRegistration( "Sumskaya 1", Date( 2011, 1, 1 ) );
	p.trackUnregistration( Date( 2015, 1, 1 ) );

	int nMatched = 0;
	p.forEachRegistration(
		[ & ] ( Date _registration, Date _unregistration, std::string const & _address, bool _stillOpen )
		{
			if ( nMatched == 0 )
			{
				assert( _registration == Date( 2011, 1, 1 ) );
				assert( _unregistration == Date( 2015, 1, 1 ) );
				assert( _address == "Sumskaya 1" );
				assert( ! _stillOpen );

				nMatched = 1;
			}
			else
				assert( ! "There was only 1 record" );
		}
	);
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_PassportIteratingRegisterUnregisterRegisterAgain )
{
	Passport p( "AB123456", Date( 2010, 1, 1 ), "Ivan Ivanov" );
	p.trackRegistration( "Sumskaya 1", Date( 2011, 1, 1 ) );
	p.trackUnregistration( Date( 2015, 1, 1 ) );
	p.trackRegistration( "Pushkinskaya 1", Date( 2015, 1, 5 ) );

	int nMatched = 0;
	p.forEachRegistration(
		[ & ] ( Date _registration, Date _unregistration, std::string const & _address, bool _stillOpen )
		{
			if ( nMatched == 0 )
			{
				assert( _registration == Date( 2011, 1, 1 ) );
				assert( _unregistration == Date( 2015, 1, 1 ) );
				assert( _address == "Sumskaya 1" );
				assert( ! _stillOpen );

				nMatched = 1;
			}

			else if ( nMatched == 1 )
			{
				assert( _registration == Date( 2015, 1, 5 ) );
				assert( _unregistration == Date() );
				assert( _address == "Pushkinskaya 1" );
				assert( _stillOpen );

				nMatched = 2;
			}

			else
				assert( ! "There were only 2 records" );
		}
	);
}


/*****************************************************************************/

DECLARE_OOP_TEST ( test_PassportGetAddressSingleRecord_ValidMoment )
{
	Passport p( "AB123456", Date( 2010, 1, 1 ), "Ivan Ivanov" );
	p.trackRegistration( "Sumskaya 1", Date( 2011, 1, 1 ) );

	assert( p.getAddressAt( Date( 2015, 1, 1 ) ) == "Sumskaya 1" );
	assert( p.getAddressAt( Date( 2011, 1, 1 ) ) == "Sumskaya 1" );
	assert( p.getAddressAt( Date() ) == "Sumskaya 1" );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_PassportGetAddressSingleRecord_InvalidMoment )
{
	Passport p( "AB123456", Date( 2010, 1, 1 ), "Ivan Ivanov" );
	p.trackRegistration( "Sumskaya 1", Date( 2011, 1, 1 ) );

	assert( p.getAddressAt( Date( 2010, 12, 31 ) ) == "" );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_PassportGetAddressTwoRecords_ValidMoment )
{
	Passport p( "AB123456", Date( 2010, 1, 1 ), "Ivan Ivanov" );
	p.trackRegistration( "Sumskaya 1", Date( 2011, 1, 1 ) );
	p.trackUnregistration( Date( 2015, 1, 1 ) );

	assert( p.getAddressAt( Date( 2013, 1,   1 ) ) == "Sumskaya 1" );
	assert( p.getAddressAt( Date( 2011, 1,   1 ) ) == "Sumskaya 1" );
	assert( p.getAddressAt( Date( 2014, 12, 31 ) ) == "Sumskaya 1" );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_PassportGetAddressTwoRecords_InvalidMoment )
{
	Passport p( "AB123456", Date( 2010, 1, 1 ), "Ivan Ivanov" );
	p.trackRegistration( "Sumskaya 1", Date( 2011, 1, 1 ) );
	p.trackUnregistration( Date( 2015, 1, 1 ) );

	assert( p.getAddressAt( Date( 2010, 12, 31 ) ) == "" );
	assert( p.getAddressAt( Date( 2015,  1,  1 ) ) == "" );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_PassportGetAddressThreeRecords_ValidMoment )
{
	Passport p( "AB123456", Date( 2010, 1, 1 ), "Ivan Ivanov" );
	p.trackRegistration( "Sumskaya 1", Date( 2011, 1, 1 ) );
	p.trackUnregistration( Date( 2015, 1, 1 ) );
	p.trackRegistration( "Pushkinskaya 1", Date( 2015, 1, 3 ) );

	assert( p.getAddressAt( Date( 2013, 1, 1 ) ) == "Sumskaya 1" );
	assert( p.getAddressAt( Date( 2011, 1, 1 ) ) == "Sumskaya 1" );
	assert( p.getAddressAt( Date( 2014, 12, 31 ) ) == "Sumskaya 1" );

	assert( p.getAddressAt( Date( 2015, 1, 3 ) ) == "Pushkinskaya 1" );
	assert( p.getAddressAt( Date() ) == "Pushkinskaya 1" );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_PassportGetAddressThreeRecords_InvalidMoment )
{
	Passport p( "AB123456", Date( 2010, 1, 1 ), "Ivan Ivanov" );
	p.trackRegistration( "Sumskaya 1", Date( 2011, 1, 1 ) );
	p.trackUnregistration( Date( 2015, 1, 1 ) );
	p.trackRegistration( "Pushkinskaya 1", Date( 2015, 1, 3 ) );

	assert( p.getAddressAt( Date( 2010, 12, 31 ) ) == "" );
	assert( p.getAddressAt( Date( 2015, 1, 1 ) )   == "" );
	assert( p.getAddressAt( Date( 2015, 1, 2 ) )   == "" );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_PassportGetAddressThreeRecords_NoGap )
{
	Passport p( "AB123456", Date( 2010, 1, 1 ), "Ivan Ivanov" );
	p.trackRegistration( "Sumskaya 1", Date( 2011, 1, 1 ) );
	p.trackUnregistration( Date( 2015, 1, 1 ) );
	p.trackRegistration( "Pushkinskaya 1", Date( 2015, 1, 1 ) );

	assert( p.getAddressAt( Date( 2014, 12, 31 ) ) == "Sumskaya 1" );
	assert( p.getAddressAt( Date( 2015, 1, 1 ) ) == "Pushkinskaya 1" );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_PassportIsCohabitantWithSelfWhenRegistered )
{
	Passport p( "AB123456", Date( 2010, 1, 1 ), "Ivan Ivanov" );
	p.trackRegistration( "Sumskaya 1", Date( 2012, 12, 1 ) );
	assert( p.isCohabitantWith( p ) );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_PassportIsCohabitantWithPersonOnSameAddress)
{
	Passport p1( "AB123456", Date( 2010, 1, 1 ), "Ivan Ivanov" );
	p1.trackRegistration( "Sumskaya 1", Date( 2012, 12, 1 ) );

	Passport p2( "CD123456", Date( 2011, 1, 1 ), "Elena Ivanova" );
	p2.trackRegistration( "Sumskaya 1", Date( 2012, 12, 1 ) );

	Passport p3( "EF123456", Date( 2011, 1, 1 ), "Piotr Petrov" );
	p3.trackRegistration( "Pushkinskaya 1", Date( 2015, 1, 1 ) );

	assert( p1.isCohabitantWith( p2 ) );
	assert( p2.isCohabitantWith( p1 ) );

	assert( ! p1.isCohabitantWith( p3 ) );
	assert( ! p2.isCohabitantWith( p3 ) );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_PassportIsCohabitant_OneOfPassportsHasNoRegistration_Fails )
{
	Passport p1( "AB123456", Date( 2010, 1, 1 ), "Ivan Ivanov" );
	p1.trackRegistration( "Sumskaya 1", Date( 2012, 12, 1 ) );

	Passport p2( "CD123456", Date( 2011, 1, 1 ), "Elena Ivanova" );

	assert( ! p1.isCohabitantWith( p2 ) );
	assert( ! p2.isCohabitantWith( p1 ) );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_PassportIsCohabitant_SameAddressInPast_Fails )
{
	Passport p1( "AB123456", Date( 2010, 1, 1 ), "Ivan Ivanov" );
	p1.trackRegistration( "Sumskaya 1", Date( 2012, 12, 1 ) );
	p1.trackUnregistration( Date( 2013, 12, 1 ) );
	p1.trackRegistration( "Pushkinskaya 1", Date( 2013, 12, 2 ) );

	Passport p2( "CD123456", Date( 2011, 1, 1 ), "Piotr Petrov" );
	p2.trackRegistration( "Sumskaya 1", Date( 2013, 12, 2 ) );

	assert( ! p1.isCohabitantWith( p2 ) );
	assert( ! p2.isCohabitantWith( p1 ) );
}


/*****************************************************************************/


