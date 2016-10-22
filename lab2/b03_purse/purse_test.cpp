// (C) 2016, Sergei Zaychenko, KNURE, Kharkiv, Ukraine

/*****************************************************************************/

#include "testslib.hpp"

#include "purse.hpp"
#include "messages.hpp"

/*****************************************************************************/


DECLARE_OOP_TEST ( test_Purse_Constructor )
{
	Purse p( "Gucci", 5 );
	
	assert( p.getBrand() == "Gucci" );
	assert( p.getMaxItemsCount() == 5 );
	assert( p.getTotalItemsCount() == 0 );
	assert( p.getUniqueItemNames().empty() );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_Purse_Constructor_EmptyName )
{
	ASSERT_THROWS(
			Purse ( "", 5 )
		,	Messages::EmptyBrandName
	);
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_Purse_Constructor_BadItemsCount )
{
	ASSERT_THROWS(
			Purse ( "Gucci", 0 )
		,	Messages::NonPositiveItemsCount
	);

	ASSERT_THROWS(
			Purse ( "Gucci", -1 )
		,	Messages::NonPositiveItemsCount
	);
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_Purse_QueryItems_WhenEmpty )
{
	Purse p( "Gucci", 5 );

	assert( ! p.hasItem( "Lipstick" ) );
	assert( ! p.getItemInstancesCount( "Lipstick" ) );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_Purse_QueryItems_InvalidName )
{
	Purse p( "Gucci", 5 );

	ASSERT_THROWS(
			p.hasItem( "" )
		,	Messages::EmptyItemName
	)

	ASSERT_THROWS(
			p.getItemInstancesCount( "" )
		,	Messages::EmptyItemName
	)
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_Purse_PutSingle )
{
	Purse p( "Gucci", 5 );
	p.putItem( "Lipstick" );

	assert( p.getTotalItemsCount() == 1 );
	assert( p.hasItem( "Lipstick" ) );
	assert( p.getItemInstancesCount( "Lipstick" ) == 1 );
	assert( p.getUniqueItemNames() == std::set< std::string >{ "Lipstick" } );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_Purse_PutSingle_N_Instances )
{
	Purse p( "Gucci", 5 );
	p.putItem( "Lipstick", 3 );

	assert( p.getTotalItemsCount() == 3 );
	assert( p.hasItem( "Lipstick" ) );
	assert( p.getItemInstancesCount( "Lipstick" ) == 3 );
	assert( p.getUniqueItemNames() == std::set< std::string >{ "Lipstick" } );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_Purse_PutSingle_N_Instances_ToLimit )
{
	Purse p( "Gucci", 5 );
	p.putItem( "Lipstick", 5 );

	assert( p.getTotalItemsCount() == 5 );
	assert( p.hasItem( "Lipstick" ) );
	assert( p.getItemInstancesCount( "Lipstick" ) == 5 );
	assert( p.getUniqueItemNames() == std::set< std::string >{ "Lipstick" } );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_Purse_PutSingle_N_Instances_BeyondLimit )
{
	Purse p( "Gucci", 5 );

	ASSERT_THROWS(
			p.putItem( "Lipstick", 6 )
		,	Messages::NoSpaceInPurse
	);

	assert( p.getTotalItemsCount() == 0 );
	assert( ! p.hasItem( "Lipstick" ) );
	assert( ! p.getItemInstancesCount( "Lipstick" ) );
	assert( p.getUniqueItemNames().empty() );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_Purse_PutSameItemTwice )
{
	Purse p( "Gucci", 5 );
	p.putItem( "Lipstick" );
	p.putItem( "Lipstick" );

	assert( p.getTotalItemsCount() == 2 );
	assert( p.hasItem( "Lipstick" ) );
	assert( p.getItemInstancesCount( "Lipstick" ) == 2 );
	assert( p.getUniqueItemNames() == std::set< std::string >{ "Lipstick" } );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_Purse_PutSameItem_NM_WithinLimit )
{
	Purse p( "Gucci", 5 );
	p.putItem( "Lipstick", 2 );
	p.putItem( "Lipstick", 3 );

	assert( p.getTotalItemsCount() == 5 );
	assert( p.hasItem( "Lipstick" ) );
	assert( p.getItemInstancesCount( "Lipstick" ) == 5 );
	assert( p.getUniqueItemNames() == std::set< std::string >{ "Lipstick" } );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_Purse_PutSameItem_NM_BeyondLimit )
{
	Purse p( "Gucci", 5 );
	p.putItem( "Lipstick", 2 );

	ASSERT_THROWS(
			p.putItem( "Lipstick", 4 )
		,	Messages::NoSpaceInPurse
	);

	assert( p.getTotalItemsCount() == 2 );
	assert( p.hasItem( "Lipstick" ) );
	assert( p.getItemInstancesCount( "Lipstick" ) == 2 );
	assert( p.getUniqueItemNames() == std::set< std::string >{ "Lipstick" } );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_Purse_PutItem_BadName )
{
	Purse p( "Gucci", 5 );

	ASSERT_THROWS(
			p.putItem( "", 1 )
		,	Messages::EmptyItemName
	)

	assert( !p.getTotalItemsCount() );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_Purse_PutItem_BadCount )
{
	Purse p( "Gucci", 5 );

	ASSERT_THROWS(
			p.putItem( "Lipstick", 0 )
		,	Messages::NonPositiveItemsCount
	)

	ASSERT_THROWS(
			p.putItem( "Lipstick", -1 )
		,	Messages::NonPositiveItemsCount
	)

	assert( ! p.getTotalItemsCount() );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_Purse_PutDifferentItems )
{
	Purse p( "Gucci", 5 );
	p.putItem( "Lipstick" );
	p.putItem( "Brushes" );

	assert( p.getTotalItemsCount() == 2 );

	assert( p.hasItem( "Lipstick" ) );
	assert( p.getItemInstancesCount( "Lipstick" ) == 1 );

	assert( p.hasItem( "Brushes" ) );
	assert( p.getItemInstancesCount( "Brushes" ) == 1 );

	std::set< std::string > expectedItemNames{ "Lipstick", "Brushes" };
	assert( p.getUniqueItemNames() == expectedItemNames );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_Purse_PutDifferentItems_NM_WithinLimit )
{
	Purse p( "Gucci", 6 );
	p.putItem( "Lipstick", 2 );
	p.putItem( "Brushes", 3 );

	assert( p.getTotalItemsCount() == 5 );

	assert( p.hasItem( "Lipstick" ) );
	assert( p.getItemInstancesCount( "Lipstick" ) == 2 );

	assert( p.hasItem( "Brushes" ) );
	assert( p.getItemInstancesCount( "Brushes" ) == 3 );

	std::set< std::string > expectedItemNames{ "Lipstick", "Brushes" };
	assert( p.getUniqueItemNames() == expectedItemNames );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_Purse_PutDifferentItems_NM_OneItemTwice )
{
	Purse p( "Gucci", 6 );
	p.putItem( "Lipstick", 2 );
	p.putItem( "Brushes", 1 );
	p.putItem( "Lipstick", 1 );

	assert( p.getTotalItemsCount() == 4 );

	assert( p.hasItem( "Lipstick" ) );
	assert( p.getItemInstancesCount( "Lipstick" ) == 3 );

	assert( p.hasItem( "Brushes" ) );
	assert( p.getItemInstancesCount( "Brushes" ) == 1 );

	std::set< std::string > expectedItemNames{ "Lipstick", "Brushes" };
	assert( p.getUniqueItemNames() == expectedItemNames );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_Purse_PutDifferentItems_NM_BeyondLimitImmediately )
{
	Purse p( "Gucci", 5 );
	p.putItem( "Lipstick", 2 );

	ASSERT_THROWS(
			p.putItem( "Brushes", 4 )
		,	Messages::NoSpaceInPurse
	)

	assert( p.getTotalItemsCount() == 2 );

	assert( p.hasItem( "Lipstick" ) );
	assert( p.getItemInstancesCount( "Lipstick" ) == 2 );

	assert( ! p.hasItem( "Brushes" ) );
	assert( ! p.getItemInstancesCount( "Brushes" ) );

	std::set< std::string > expectedItemNames{ "Lipstick" };
	assert( p.getUniqueItemNames() == expectedItemNames );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_Purse_PutDifferentItems_NM_BeyondLimitOn2ndCall )
{
	Purse p( "Gucci", 5 );
	p.putItem( "Lipstick", 2 );
	p.putItem( "Brushes", 3 );

	ASSERT_THROWS(
			p.putItem( "Brushes", 1 )
		,	Messages::NoSpaceInPurse
	)

	assert( p.getTotalItemsCount() == 5 );

	assert( p.hasItem( "Lipstick" ) );
	assert( p.getItemInstancesCount( "Lipstick" ) == 2 );

	assert( p.hasItem( "Brushes" ) );
	assert( p.getItemInstancesCount( "Brushes" ) == 3 );

	std::set< std::string > expectedItemNames{ "Lipstick", "Brushes" };
	assert( p.getUniqueItemNames() == expectedItemNames );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_Purse_PutRemoveItem )
{
	Purse p( "Gucci", 5 );
	p.putItem( "Lipstick" );
	p.removeItem( "Lipstick" );

	assert( ! p.getTotalItemsCount()  );
	assert( ! p.hasItem( "Lipstick" ) );
	assert( ! p.getItemInstancesCount( "Lipstick" ) );
	assert( p.getUniqueItemNames().empty() );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_Purse_PutN_RemoveOne )
{
	Purse p( "Gucci", 5 );
	p.putItem( "Lipstick", 3 );
	p.removeItem( "Lipstick" );

	assert( p.getTotalItemsCount() == 2 );
	assert( p.hasItem( "Lipstick" ) );
	assert( p.getItemInstancesCount( "Lipstick" ) == 2 );
	assert( p.getUniqueItemNames() == std::set< std::string >{ "Lipstick" } );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_Purse_PutN_RemoveN )
{
	Purse p( "Gucci", 5 );
	p.putItem( "Lipstick", 3 );
	p.removeItem( "Lipstick", 3 );

	assert( ! p.getTotalItemsCount()  );
	assert( ! p.hasItem( "Lipstick" ) );
	assert( ! p.getItemInstancesCount( "Lipstick" ) );
	assert( p.getUniqueItemNames().empty() );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_Purse_PutN_RemoveM )
{
	Purse p( "Gucci", 5 );
	p.putItem( "Lipstick", 3 );
	p.removeItem( "Lipstick", 2 );

	assert( p.getTotalItemsCount() == 1 );
	assert( p.hasItem( "Lipstick" ) );
	assert( p.getItemInstancesCount( "Lipstick" ) == 1 );
	assert( p.getUniqueItemNames() == std::set< std::string >{ "Lipstick" } );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_Purse_RemoveWhatWasntPut )
{
	Purse p( "Gucci", 5 );

	ASSERT_THROWS(
			p.removeItem( "Lipstick" )
		,	Messages::NoSuchItemInPurse
	)

	ASSERT_THROWS(
			p.removeItem( "Lipstick", 2 )
		,	Messages::NoSuchItemInPurse
	)
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_Purse_RemoveMoreThanPut )
{
	Purse p( "Gucci", 5 );
	p.putItem( "Lipstick", 2 );

	ASSERT_THROWS(
			p.removeItem( "Lipstick", 3 )
		,	Messages::NotEnoughItemsInPurse
	)

	assert( p.getTotalItemsCount() == 2 );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_Purse_RemoveBadName )
{
	Purse p( "Gucci", 5 );
	p.putItem( "Lipstick", 2 );

	ASSERT_THROWS(
			p.removeItem( "", 1 )
		,	Messages::EmptyItemName
	)

	assert( p.getTotalItemsCount() == 2 );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_Purse_RemoveBadCount )
{
	Purse p( "Gucci", 5 );
	p.putItem( "Lipstick", 2 );

	ASSERT_THROWS(
			p.removeItem( "Lipstick", 0 )
		,	Messages::NonPositiveItemsCount
	)

	ASSERT_THROWS(
			p.removeItem( "Lipstick", -1 )
		,	Messages::NonPositiveItemsCount
	)

	assert( p.getTotalItemsCount() == 2 );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_Purse_PutDifferent_RemoveOne )
{
	Purse p( "Gucci", 5 );
	p.putItem( "Lipstick", 3 );
	p.putItem( "Brushes", 2 );

	p.removeItem( "Lipstick" );

	assert( p.getTotalItemsCount() == 4 );
	
	assert( p.hasItem( "Lipstick" ) );
	assert( p.getItemInstancesCount( "Lipstick" ) == 2 );
	
	assert( p.hasItem( "Brushes" ) );
	assert( p.getItemInstancesCount( "Brushes" ) == 2 );

	std::set< std::string > expectedItemNames = { "Lipstick", "Brushes" };
	assert( p.getUniqueItemNames() == expectedItemNames );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_Purse_PutDifferent_RemoveN_BothLeft )
{
	Purse p( "Gucci", 5 );
	p.putItem( "Lipstick", 3 );
	p.putItem( "Brushes", 2 );

	p.removeItem( "Lipstick", 2 );

	assert( p.getTotalItemsCount() == 3 );

	assert( p.hasItem( "Lipstick" ) );
	assert( p.getItemInstancesCount( "Lipstick" ) == 1 );

	assert( p.hasItem( "Brushes" ) );
	assert( p.getItemInstancesCount( "Brushes" ) == 2 );

	std::set< std::string > expectedItemNames = { "Lipstick", "Brushes" };
	assert( p.getUniqueItemNames() == expectedItemNames );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_Purse_PutDifferent_RemoveN_OneLeft )
{
	Purse p( "Gucci", 5 );
	p.putItem( "Lipstick", 3 );
	p.putItem( "Brushes", 2 );

	p.removeItem( "Lipstick", 3 );

	assert( p.getTotalItemsCount() == 2 );

	assert( ! p.hasItem( "Lipstick" ) );
	assert( ! p.getItemInstancesCount( "Lipstick" ) );

	assert( p.hasItem( "Brushes" ) );
	assert( p.getItemInstancesCount( "Brushes" ) == 2 );

	std::set< std::string > expectedItemNames = { "Brushes" };
	assert( p.getUniqueItemNames() == expectedItemNames );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_Purse_PutDifferent_RemoveAll )
{
	Purse p( "Gucci", 5 );
	p.putItem( "Lipstick", 3 );
	p.putItem( "Brushes", 2 );

	p.removeItem( "Lipstick", 3 );
	p.removeItem( "Brushes", 2 );

	assert( ! p.getTotalItemsCount() );

	assert( ! p.hasItem( "Lipstick" ) );
	assert( ! p.getItemInstancesCount( "Lipstick" ) );

	assert( ! p.hasItem( "Brushes" ) );
	assert( ! p.getItemInstancesCount( "Brushes" ) );

	assert( p.getUniqueItemNames().empty() );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_Purse_PutDifferent_RemoveOneBeyond )
{
	Purse p( "Gucci", 5 );
	p.putItem( "Lipstick", 3 );
	p.putItem( "Brushes", 2 );

	ASSERT_THROWS(
			p.removeItem( "Lipstick", 4 )
		,	Messages::NotEnoughItemsInPurse
	)

	assert( p.getTotalItemsCount() == 5 );

	assert( p.hasItem( "Lipstick" ) );
	assert( p.getItemInstancesCount( "Lipstick" ) == 3 );

	assert( p.hasItem( "Brushes" ) );
	assert( p.getItemInstancesCount( "Brushes" ) == 2 );

	std::set< std::string > expectedItemNames = { "Lipstick", "Brushes" };
	assert( p.getUniqueItemNames() == expectedItemNames );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_Purse_PutDifferent_RemoveOneBeyondOver2Calls )
{
	Purse p( "Gucci", 5 );
	p.putItem( "Lipstick", 3 );
	p.putItem( "Brushes", 2 );

	p.removeItem( "Lipstick", 2 );

	ASSERT_THROWS(
			p.removeItem( "Lipstick", 2 )
		,	Messages::NotEnoughItemsInPurse
	)

	assert( p.getTotalItemsCount() == 3 );

	assert( p.hasItem( "Lipstick" ) );
	assert( p.getItemInstancesCount( "Lipstick" ) == 1 );

	assert( p.hasItem( "Brushes" ) );
	assert( p.getItemInstancesCount( "Brushes" ) == 2 );

	std::set< std::string > expectedItemNames = { "Lipstick", "Brushes" };
	assert( p.getUniqueItemNames() == expectedItemNames );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_Purse_PutOne_RemoveAllOf )
{
	Purse p( "Gucci", 5 );
	p.putItem( "Lipstick", 3 );
	p.removeAllOf( "Lipstick" );

	assert( ! p.getTotalItemsCount() );
	assert( ! p.hasItem( "Lipstick" ) );
	assert( ! p.getItemInstancesCount( "Lipstick" ) );
	assert( p.getUniqueItemNames().empty() );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_Purse_PutTwo_RemoveAllOfOne )
{
	Purse p( "Gucci", 5 );
	p.putItem( "Lipstick", 3 );
	p.putItem( "Brushes", 2 );
	p.removeAllOf( "Lipstick" );

	assert( p.getTotalItemsCount() == 2 );

	assert( ! p.hasItem( "Lipstick" ) );
	assert( ! p.getItemInstancesCount( "Lipstick" ) );

	assert( p.hasItem( "Brushes" ) );
	assert( p.getItemInstancesCount( "Brushes" ) == 2 );

	std::set< std::string > expectedItemNames = { "Brushes" };
	assert( p.getUniqueItemNames() == expectedItemNames );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_Purse_PutTwo_RemoveAllOfTwo )
{
	Purse p( "Gucci", 5 );
	p.putItem( "Lipstick", 3 );
	p.putItem( "Brushes", 2 );
	p.removeAllOf( "Lipstick" );
	p.removeAllOf( "Brushes" );

	assert( ! p.getTotalItemsCount() );
	assert( ! p.hasItem( "Lipstick" ) );
	assert( ! p.getItemInstancesCount( "Lipstick" ) );
	assert( ! p.hasItem( "Brushes" ) );
	assert( ! p.getItemInstancesCount( "Brushes" ) );
	assert( p.getUniqueItemNames().empty() );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_Purse_RemoveAllOfBadName )
{
	Purse p( "Gucci", 5 );

	ASSERT_THROWS(
			p.removeAllOf( "" )
		,	Messages::EmptyItemName
	)
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_Purse_RemoveAllOfWhatWasntPut )
{
	Purse p( "Gucci", 5 );
	p.putItem( "Lipstick" );

	ASSERT_THROWS(
			p.removeAllOf( "Brushes" )
		,	Messages::NoSuchItemInPurse
	)

	assert( p.getTotalItemsCount() == 1 );
	assert( p.hasItem( "Lipstick" ) );
	assert( p.getItemInstancesCount( "Lipstick" ) == 1 );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_Purse_RemoveAllEmptyPurse )
{
	Purse p( "Gucci", 5 );
	p.removeAll();

	assert( ! p.getTotalItemsCount() );
	assert( p.getUniqueItemNames().empty() );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_Purse_RemoveAllNonEmptyPurse )
{
	Purse p( "Gucci", 5 );
	p.putItem( "Lipstick", 2 );
	p.putItem( "Brushes", 3 );
	p.removeAll();

	assert( !p.getTotalItemsCount() );
	assert( p.getUniqueItemNames().empty() );
	assert( ! p.hasItem( "Lipstick" ) );
	assert( ! p.hasItem( "Brushes" ) );
}


/*****************************************************************************/
