// (C) 2016, Sergei Zaychenko, KNURE, Kharkiv, Ukraine

/*****************************************************************************/

#include "testslib.hpp"

#include "profiles_manager.hpp"
#include "messages.hpp"

/*****************************************************************************/


DECLARE_OOP_TEST ( test_ProfilesManager_Constructor )
{
	ProfilesManager pm( "Ivan" );
	
	assert( pm.getUserName() == "Ivan" );
	assert( pm.getProfilesCount() == 0 );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_ProfilesManager_BadUserName )
{
	ASSERT_THROWS(
			ProfilesManager( "" )
		,	Messages::EmptyUserName
	);
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_ProfilesManager_QueryProfile_Empty )
{
	ProfilesManager pm( "Ivan" );

	assert( ! pm.getProfilesCount() );
	assert( ! pm.hasProfileFor( "Gmail" ) );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_ProfilesManager_QueryProfile_BadName )
{
	ProfilesManager pm( "Ivan" );

	ASSERT_THROWS(
			pm.hasProfileFor( "" )
		,	Messages::EmptyServiceName
	)
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_ProfilesManager_AddProfile )
{
	ProfilesManager pm( "Ivan" );
	pm.addProfileData( "Gmail", "ivan@gmail.com", "12345" );

	assert( pm.getProfilesCount() == 1 );

	assert( pm.hasProfileFor( "Gmail" ) ); 
	ProfilesManager::ProfileData expectedData( "Gmail", "ivan@gmail.com", "12345" );
	assert( pm.getProfile( "Gmail" ) == expectedData );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_ProfilesManager_AddTwoProfiles )
{
	ProfilesManager pm( "Ivan" );
	pm.addProfileData( "Gmail", "ivan@gmail.com", "12345" );
	pm.addProfileData( "Facebook", "ivan@ukr.net", "23456" );

	assert( pm.getProfilesCount() == 2 );
	
	assert( pm.hasProfileFor( "Gmail" ) );
	ProfilesManager::ProfileData expectedDataGmail( "Gmail", "ivan@gmail.com", "12345" );
	assert( pm.getProfile( "Gmail" ) == expectedDataGmail );

	assert( pm.hasProfileFor( "Facebook" ) );
	ProfilesManager::ProfileData expectedDataFacebook( "Facebook", "ivan@ukr.net", "23456" );
	assert( pm.getProfile( "Facebook" ) == expectedDataFacebook );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_ProfilesManager_AddOneProfile_RequestOther )
{
	ProfilesManager pm( "Ivan" );
	pm.addProfileData( "Gmail", "ivan@gmail.com", "12345" );

	assert( ! pm.hasProfileFor( "Facebook" ) );
	
	ASSERT_THROWS(
			pm.getProfile( "Facebook" )
		,	Messages::NoServiceProfile
	)
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_ProfilesManager_AddDuplicateProfile )
{
	ProfilesManager pm( "Ivan" );
	pm.addProfileData( "Gmail", "ivan@gmail.com", "12345" );

	ASSERT_THROWS(
			pm.addProfileData( "Gmail", "ivan@gmail.com", "12345" );
		,	Messages::ServiceProfileExists
	)
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_ProfilesManager_AddBadServiceName )
{
	ProfilesManager pm( "Ivan" );

	ASSERT_THROWS(
			pm.addProfileData( "", "ivan@gmail.com", "12345" );
		,	Messages::EmptyServiceName
	)
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_ProfilesManager_AddBadLogin )
{
	ProfilesManager pm( "Ivan" );

	ASSERT_THROWS(
			pm.addProfileData( "Gmail", "", "12345" );
		,	Messages::EmptyLogin
	)
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_ProfilesManager_AddBadPassword )
{
	ProfilesManager pm( "Ivan" );

	ASSERT_THROWS(
			pm.addProfileData( "Gmail", "ivan@gmail.com", "" );
		,	Messages::EmptyPassword
	)
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_ProfilesManager_AddUpdateProfile )
{
	ProfilesManager pm( "Ivan" );
	pm.addProfileData( "Gmail", "ivan@gmail.com", "12345" );
	pm.updateProfileData( "Gmail", "ivan2@gmail.com", "23456" );

	assert( pm.getProfilesCount() == 1 );

	assert( pm.hasProfileFor( "Gmail" ) );
	ProfilesManager::ProfileData expectedData( "Gmail", "ivan2@gmail.com", "23456" );
	assert( pm.getProfile( "Gmail" ) == expectedData );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_ProfilesManager_AddTwo_UpdateOne )
{
	ProfilesManager pm( "Ivan" );
	pm.addProfileData( "Gmail", "ivan@gmail.com", "12345" );
	pm.addProfileData( "Facebook", "ivan@gmail.com", "23456" );
	pm.updateProfileData( "Gmail", "ivan2@gmail.com", "12345" );

	assert( pm.getProfilesCount() == 2 );
	
	assert( pm.hasProfileFor( "Gmail" ) );
	ProfilesManager::ProfileData expectedDataGmail( "Gmail", "ivan2@gmail.com", "12345" );
	assert( pm.getProfile( "Gmail" ) == expectedDataGmail );

	assert( pm.hasProfileFor( "Facebook" ) );
	ProfilesManager::ProfileData expectedDataFacebook( "Facebook", "ivan@gmail.com", "23456" );
	assert( pm.getProfile( "Facebook" ) == expectedDataFacebook );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_ProfilesManager_AddOne_UpdateOther )
{
	ProfilesManager pm( "Ivan" );
	pm.addProfileData( "Facebook", "ivan@gmail.com", "23456" );

	ASSERT_THROWS(
			pm.updateProfileData( "Gmail", "ivan2@gmail.com", "12345" )
		,	Messages::NoServiceProfile
	)

	assert( pm.getProfilesCount() == 1 );

	assert( pm.hasProfileFor( "Facebook" ) );
	ProfilesManager::ProfileData expectedDataFacebook( "Facebook", "ivan@gmail.com", "23456" );
	assert( pm.getProfile( "Facebook" ) == expectedDataFacebook );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_ProfilesManager_UpdateBadServiceName )
{
	ProfilesManager pm( "Ivan" );

	ASSERT_THROWS(
			pm.updateProfileData( "", "ivan@gmail.com", "12345" );
		,	Messages::EmptyServiceName
	)
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_ProfilesManager_UpdateBadLogin )
{
	ProfilesManager pm( "Ivan" );
	pm.addProfileData( "Gmail", "ivan@gmail.com", "12345" );

	ASSERT_THROWS(
			pm.updateProfileData( "Gmail", "", "12345" );
		,	Messages::EmptyLogin
	)
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_ProfilesManager_UpdateBadPassword )
{
	ProfilesManager pm( "Ivan" );
	pm.addProfileData( "Gmail", "ivan@gmail.com", "12345" );

	ASSERT_THROWS(
			pm.updateProfileData( "Gmail", "ivan@gmail.com", "" );
		,	Messages::EmptyPassword
	)
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_ProfilesManager_AddRemoveProfile )
{
	ProfilesManager pm( "Ivan" );
	pm.addProfileData( "Gmail", "ivan@gmail.com", "12345" );
	pm.removeProfile( "Gmail" );

	assert( pm.getProfilesCount() == 0 );
	assert( !pm.hasProfileFor( "Gmail" ) );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_ProfilesManager_AddTwo_RemoveOne )
{
	ProfilesManager pm( "Ivan" );
	pm.addProfileData( "Gmail", "ivan@gmail.com", "12345" );
	pm.addProfileData( "Facebook", "ivan@gmail.com", "23456" );
	pm.removeProfile( "Gmail" );

	assert( pm.getProfilesCount() == 1 );
	assert( ! pm.hasProfileFor( "Gmail" ) );
	assert( pm.hasProfileFor( "Facebook" ) );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_ProfilesManager_RemoveUnexisting )
{
	ProfilesManager pm( "Ivan" );

	ASSERT_THROWS(
			pm.removeProfile( "Gmail" )
		,	Messages::NoServiceProfile
	)
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_ProfilesManager_RemoveAll_WhenEmpty )
{
	ProfilesManager pm( "Ivan" );
	pm.removeAllProfiles();
	
	assert( ! pm.getProfilesCount() );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_ProfilesManager_RemoveAll_WhenNonEmpty )
{
	ProfilesManager pm( "Ivan" );
	pm.addProfileData( "Gmail", "ivan@gmail.com", "12345" );
	pm.addProfileData( "Facebook", "ivan@gmail.com", "23456" );
	pm.removeAllProfiles();

	assert( ! pm.getProfilesCount() );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_ProfilesManager_GetServicesWithLogin_Empty )
{
	ProfilesManager pm( "Ivan" );
	auto result = pm.getServicesWithLogin( "ivan@gmail.com" );
	assert( result.empty() );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_ProfilesManager_GetServicesWithLogin_OneMatching )
{
	ProfilesManager pm( "Ivan" );
	pm.addProfileData( "Gmail", "ivan@gmail.com", "12345" );
	auto result = pm.getServicesWithLogin( "ivan@gmail.com" );
	
	assert( result.size() == 1 );
	assert( result[ 0 ] == "Gmail" );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_ProfilesManager_GetServicesWithLogin_TwoMatching )
{
	ProfilesManager pm( "Ivan" );
	pm.addProfileData( "Gmail", "ivan@gmail.com", "12345" );
	pm.addProfileData( "Facebook", "ivan@gmail.com", "23456" );
	auto result = pm.getServicesWithLogin( "ivan@gmail.com" );

	assert( result.size() == 2 );
	assert( result[ 0 ] == "Facebook" );
	assert( result[ 1 ] == "Gmail" );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_ProfilesManager_GetServicesWithLogin_TwoMatchingOneNot )
{
	ProfilesManager pm( "Ivan" );
	pm.addProfileData( "Gmail", "ivan@gmail.com", "12345" );
	pm.addProfileData( "Facebook", "ivan@gmail.com", "23456" );
	pm.addProfileData( "Twitter", "ivan2@gmail.com", "34567" );

	auto result = pm.getServicesWithLogin( "ivan@gmail.com" );

	assert( result.size() == 2 );
	assert( result[ 0 ] == "Facebook" );
	assert( result[ 1 ] == "Gmail" );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_ProfilesManager_GetServicesWithLogin_BadLogin )
{
	ProfilesManager pm( "Ivan" );

	ASSERT_THROWS(
			pm.getServicesWithLogin( "" )
		,	Messages::EmptyLogin
	)
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_ProfilesManager_FindSamePassword_Empty )
{
	ProfilesManager pm( "Ivan" );
	assert( pm.findServicesWithIdenticalPassword().empty() );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_ProfilesManager_FindSamePassword_TwoDifferentPasswords )
{
	ProfilesManager pm( "Ivan" );
	pm.addProfileData( "Gmail", "ivan@gmail.com", "12345" );
	pm.addProfileData( "Facebook", "ivan@gmail.com", "23456" );

	assert( pm.findServicesWithIdenticalPassword().empty() );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_ProfilesManager_FindSamePassword_TwoIdenticalPasswords )
{
	ProfilesManager pm( "Ivan" );
	pm.addProfileData( "Gmail", "ivan@gmail.com", "12345" );
	pm.addProfileData( "Facebook", "ivan@gmail.com", "12345" );

	std::vector< std::string > matchingServices{ "Facebook", "Gmail" };
	std::vector< std::vector< std::string > > expectedServices;
	expectedServices.push_back( matchingServices );

	assert( pm.findServicesWithIdenticalPassword() == expectedServices );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_ProfilesManager_FindSamePassword_ManyProfilesSomeIdentical )
{
	ProfilesManager pm( "Ivan" );
	pm.addProfileData( "Gmail", "ivan@gmail.com", "12345" );
	pm.addProfileData( "Facebook", "ivan@gmail.com", "12345" );
	pm.addProfileData( "Twitter", "ivan@gmail.com", "23456" );
	pm.addProfileData( "Instagram", "ivan@gmail.com", "23456" );
	pm.addProfileData( "TripAdvisor", "ivan@gmail.com", "34567" );

	std::vector< std::string > matchingServices1{ "Facebook", "Gmail" };
	std::vector< std::string > matchingServices2{ "Instagram", "Twitter" };
	
	std::vector< std::vector< std::string > > expectedServices;
	expectedServices.push_back( matchingServices1 );
	expectedServices.push_back( matchingServices2 );

	assert( pm.findServicesWithIdenticalPassword() == expectedServices );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_ProfilesManager_FindSamePassword_ManyProfilesAllIdentical )
{
	ProfilesManager pm( "Ivan" );
	pm.addProfileData( "Gmail", "ivan@gmail.com", "12345" );
	pm.addProfileData( "Facebook", "ivan@gmail.com", "12345" );
	pm.addProfileData( "Twitter", "ivan@gmail.com", "12345" );
	pm.addProfileData( "Instagram", "ivan@gmail.com", "12345" );
	pm.addProfileData( "TripAdvisor", "ivan@gmail.com", "12345" );

	std::vector< std::string > matchingServices{ "Facebook", "Gmail", "Instagram", "TripAdvisor", "Twitter" };
	std::vector< std::vector< std::string > > expectedServices;
	expectedServices.push_back( matchingServices );

	assert( pm.findServicesWithIdenticalPassword() == expectedServices );
}


/*****************************************************************************/
