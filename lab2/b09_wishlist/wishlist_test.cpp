// (C) 2016, Sergei Zaychenko, KNURE, Kharkiv, Ukraine

/*****************************************************************************/

#include "testslib.hpp"

#include "wishlist.hpp"
#include "messages.hpp"

/*****************************************************************************/

#define CREATE_COMPLEX_WISHLIST()												\
	std::vector< Wish > baseValues (											\
		{																		\
				Wish( "Think about six impossible things at breakfast", 12 )	\
			,	Wish( "Try that bottle", 5 )									\
			,	Wish( "Guess Madhatter's riddle", 7 )							\
			,	Wish( "Do not go among mad people", 0 )							\
			,	Wish( "Do not talk when you don't think", 7 )					\
			,	Wish( "Find out how long is forever", 5 )						\
			,	Wish( "Find out when it's teatime", 5 )							\
			,	Wish( "Find out which way you ought to go", 12 )				\
		} 																		\
	);																			\
																				\
	Wishlist wl ( "Alice" );													\
																				\
	for( Wish const & wish : baseValues )										\
		wl.addWish( wish );

/*****************************************************************************/

template< typename _T >
bool compareContainers( _T const & _c1, _T const & _c2 )
{
	return
		std::equal(
				_c1.begin()
			,	_c1.end()
			,	_c2.begin()
			,	_c2.end()
		);
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_Wishlist_Constructors )
{
	Wishlist w( "Alice" );

	assert( w.getOwner() == "Alice" );
	assert( w.getWishesCount() == 0 );

	Wishlist fromMove = Wishlist( "Bob", { Wish( "Eat", 1 ), Wish( "Sleep", 2 ) } );

	assert( fromMove.getOwner() == "Bob" );
	assert( fromMove.getWishesCount() == 2 );
	assert( fromMove.getWishPriority( "Eat" ) == 1 );
	assert( fromMove.getWishPriority( "Sleep" ) == 2 );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_Wishlist_Constructor_EmptyOwnerName )
{
	ASSERT_THROWS(
			Wishlist( "" )
		,	Messages::OwnerNameEmpty
	)

	ASSERT_THROWS(
			Wishlist( "", { Wish( "Eat", 1 ) } )
		,	Messages::OwnerNameEmpty
	)
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_Wishlist_ChangeOwner )
{
	Wishlist wl ( "Bob", { Wish( "Eat", 1 ), Wish( "Sleep", 2 ) } );

	ASSERT_THROWS(
			wl.correctOwner( "" );
		,	Messages::OwnerNameEmpty
	);

	assert( wl.getOwner() == "Bob" );

	wl.correctOwner( "Alice" );

	assert( wl.getOwner() == "Alice" );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_Wishlist_Constructor_EmptyWishName )
{
	ASSERT_THROWS(
			Wishlist( "Bob", { Wish( "Eat", 1 ), Wish( "", 2 ) } )
		,	Messages::WishNameEmpty
	)
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_Wishlist_Constructor_Zero_And_NegativePriority )
{
	Wishlist wl( "Bob", { Wish( "Eat", 0 ) } );

	ASSERT_THROWS(
			Wishlist( "Bob", { Wish( "Eat", -1 ) } )
		,	Messages::NegativePriority
	)
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_Wishlist_AddWish )
{
	Wishlist wl( "Alice" );

	assert( wl.getWishesCount() == 0 );

	std::string impossibleThings( "Think about six impossible things at breakfast" );

	wl.addWish( impossibleThings, 23 );
	
	assert( wl.getWishesCount() == 1 );
	assert( wl.hasWish( impossibleThings ) );
	assert( wl.getWishPriority( impossibleThings ) == 23 );

	std::string tryTheBottle( "Try that bottle" );

	Wish wish( tryTheBottle, 11 );

	wl.addWish( wish );

	assert( wl.getWishesCount() == 2 );
	assert( wl.hasWish( tryTheBottle ) );
	assert( wl.getWishPriority( tryTheBottle ) == 11 );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_Wishlist_AddWish_BadOrExistingWish )
{
	Wishlist wl( "Alice" );

	assert( wl.getWishesCount() == 0 );

	ASSERT_THROWS(
			wl.addWish( "", 33 )
		,	Messages::WishNameEmpty
	);

	assert( wl.getWishesCount() == 0 );

	ASSERT_THROWS(
			wl.addWish( "Test", -1 )
		,	Messages::NegativePriority
	);

	assert( wl.getWishesCount() == 0 );

	wl.addWish( "Test", 1 );

	ASSERT_THROWS(
			wl.addWish( "Test", 12 )
		,	Messages::WishAlreadyExists
	);

	assert( wl.getWishesCount() == 1 );

}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_Wishlist_UpdateWishPriority_CorrectCases )
{
	std::string impossibleThings( "Think about six impossible things at breakfast" );

	Wishlist wl( "Alice", { Wish( impossibleThings, 5 ) } );

	assert( wl.getWishesCount() == 1 );
	assert( wl.getWishPriority( impossibleThings ) == 5 );

	wl.updateWishPriority( impossibleThings, 23 );
	
	assert( wl.getWishesCount() == 1 );
	assert( wl.getWishPriority( impossibleThings ) == 23 );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_Wishlist_UpdateWishPriority_ExceptionCases )
{
	std::string impossibleThings( "Think about six impossible things at breakfast" );

	Wishlist wl( "Alice", { Wish( impossibleThings, 5 ) } );

	assert( wl.getWishesCount() == 1 );
	assert( wl.getWishPriority( impossibleThings ) == 5 );

	ASSERT_THROWS(
			wl.updateWishPriority( impossibleThings, -5 )
		,	Messages::NegativePriority
	);
	
	assert( wl.getWishesCount() == 1 );
	assert( wl.getWishPriority( impossibleThings ) == 5 );

	ASSERT_THROWS(
			wl.updateWishPriority( "Test", 22 )
		,	Messages::WishDoesNotExist
	);
	
	ASSERT_THROWS(
			wl.updateWishPriority( "", 22 )
		,	Messages::WishNameEmpty
	);
	
	assert( wl.getWishesCount() == 1 );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_Wishlist_DropWish_CorrectCases )
{
	std::string impossibleThings( "Think about six impossible things at breakfast" );
	std::string tryTheBottle( "Try that bottle" );
	std::string guessTheRiddle( "Guess Madhatter's riddle" );

	Wishlist wl(
			"Alice"
		,	{
					Wish( impossibleThings, 5 )
				,	Wish( tryTheBottle, 12 )
				,	Wish( guessTheRiddle, 7 )
			}
	);

	wl.dropWish( tryTheBottle );

	assert( wl.getWishesCount() == 2 );
	assert( wl.getWishPriority( impossibleThings ) == 5 );
	assert( wl.getWishPriority( guessTheRiddle ) == 7 );

	wl.dropWish( impossibleThings );

	assert( wl.getWishesCount() == 1 );
	assert( wl.getWishPriority( guessTheRiddle ) == 7 );

	wl.dropWish( guessTheRiddle );

	assert( wl.getWishesCount() == 0 );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_Wishlist_DropWish_ExceptionCases )
{
	std::string impossibleThings( "Think about six impossible things at breakfast" );
	std::string tryTheBottle( "Try that bottle" );
	std::string guessTheRiddle( "Guess Madhatter's riddle" );

	Wishlist wl(
			"Alice"
		,	{
					Wish( impossibleThings, 5 )
				,	Wish( tryTheBottle, 12 )
				,	Wish( guessTheRiddle, 7 )
			}
	);

	ASSERT_THROWS(
			wl.dropWish( "" )
		,	Messages::WishNameEmpty
	);

	ASSERT_THROWS(
			wl.dropWish( "Test" )
		,	Messages::WishDoesNotExist
	);

	wl.dropWish( tryTheBottle );

	ASSERT_THROWS(
			wl.dropWish( tryTheBottle )
		,	Messages::WishDoesNotExist
	);

	assert( wl.getWishesCount() == 2 );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_Wishlist_GetWishesWithPriority )
{
	CREATE_COMPLEX_WISHLIST()

	std::vector< std::string > expectedResult1(
		{ baseValues[0].first, baseValues[7].first }
	);

	auto actualResult1 = wl.getWishesWithPriority( 12 );

	assert( compareContainers( expectedResult1, actualResult1 ) );

	std::vector< std::string > expectedResult2(
		{ baseValues[2].first, baseValues[4].first }
	);

	auto actualResult2 = wl.getWishesWithPriority( 7 );

	assert( compareContainers( expectedResult2, actualResult2 ) );

	std::vector< std::string > expectedResult3(
		{ baseValues[1].first, baseValues[5].first, baseValues[6].first }
	);

	auto actualResult3 = wl.getWishesWithPriority( 5 );

	assert( compareContainers( expectedResult3, actualResult3 ) );

	std::vector< std::string > expectedResult4(
		{ baseValues[3].first }
	);

	auto actualResult4 = wl.getWishesWithPriority( 0 );

	assert( compareContainers( expectedResult4, actualResult4 ) );

	std::vector< std::string > expectedResult5;

	auto actualResult5 = wl.getWishesWithPriority( 9 );

	assert( compareContainers( expectedResult5, actualResult5 ) );

	ASSERT_THROWS(
			wl.getWishesWithPriority( -5 )
		,	Messages::NegativePriority
	)
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_Wishlist_GetMostPrioritizedWishes )
{
	CREATE_COMPLEX_WISHLIST()

	std::vector< std::string > expectedResult1(
		{
				baseValues[0].first
			,	baseValues[7].first
			,	baseValues[2].first
		}
	);

	auto actualResult1 = wl.getMostPrioritizedWishes( 3 );

	assert( compareContainers( expectedResult1, actualResult1 ) );

	Wishlist wishlist2( "Test" );

	assert( wishlist2.getMostPrioritizedWishes( 5 ).size() == 0 );

	wishlist2.addWish( "w", 11 );

	std::vector< std::string > expectedResult3 = { "w" };

	auto actualResult3 = wishlist2.getMostPrioritizedWishes( 45 );

	assert( compareContainers( expectedResult3, actualResult3 ) );

	ASSERT_THROWS(
			wl.getMostPrioritizedWishes( 0 )
		,	Messages::NonPositiveAmount
	)
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_Wishlist_GetLeastPrioritizedWishes )
{
	CREATE_COMPLEX_WISHLIST()

	std::vector< std::string > expectedResult1(
		{
				baseValues[3].first
			,	baseValues[6].first
			,	baseValues[5].first
		}
	);

	//NOTE: when we're taking in count same priority wishes,
	//      main criteria to extract same wishes after each launch

	auto actualResult1 = wl.getLeastPrioritizedWishes( 3 );

	assert( expectedResult1[0] == actualResult1[0] );
	assert( wl.getWishPriority( actualResult1[1] ) == wl.getWishPriority( actualResult1[2] ) );

	assert(
		compareContainers(
				wl.getLeastPrioritizedWishes( 3 )
			,	wl.getLeastPrioritizedWishes( 3 )
		)
	);

	Wishlist wishlist2( "Test" );

	assert( wishlist2.getLeastPrioritizedWishes( 5 ).size() == 0 );

	wishlist2.addWish( "w", 11 );

	std::vector< std::string > expectedResult3 = { "w" };

	auto actualResult3 = wishlist2.getLeastPrioritizedWishes( 45 );

	assert( compareContainers( expectedResult3, actualResult3 ) );

	ASSERT_THROWS(
			wl.getLeastPrioritizedWishes( 0 )
		,	Messages::NonPositiveAmount
	)
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_Wishlist_GetSimilarWishes )
{
	CREATE_COMPLEX_WISHLIST()

	Wishlist wl2 ( "Bob", { baseValues[2], baseValues[5], Wish( "test", 15 ) } );

	std::set< std::string > expectedResult( { baseValues[5].first, baseValues[2].first } );

	std::set< std::string > actualResult = wl.getSimilarWishes( wl2 );

	assert( compareContainers( expectedResult, actualResult ) );

	Wishlist wl3( "Eddy", { Wish ( "t1", 7 ), Wish( "t2", 5 ) } );

	assert( wl3.getSimilarWishes( wl ).size() == 0 );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_Wishlist_DropWishesWithPriorityLessThan )
{
	CREATE_COMPLEX_WISHLIST()

	wl.dropWishesWithPriorityLessThan( 5 );
	assert( wl.getWishesCount() == 7 );

	wl.dropWishesWithPriorityLessThan( 6 );
	assert( wl.getWishesCount() == 4 );

	wl.dropWishesWithPriorityLessThan( 7 );
	assert( wl.getWishesCount() == 4 );

	wl.dropWishesWithPriorityLessThan( 8 );
	assert( wl.getWishesCount() == 2 );

	wl.dropWishesWithPriorityLessThan( 12 );
	assert( wl.getWishesCount() == 2 );

	wl.dropWishesWithPriorityLessThan( 13 );
	assert( wl.getWishesCount() == 0 );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_Wishlist_Clean )
{
	CREATE_COMPLEX_WISHLIST()

	assert( wl.getWishesCount() == 8 );

	wl.clean();

	assert( wl.getWishesCount() == 0 );

	wl.addWish( "test", 3 );

	assert( wl.getWishesCount() == 1 );

	wl.clean();

	assert( wl.getWishesCount() == 0 );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_Wishlist_overloaded_operators )
{
	CREATE_COMPLEX_WISHLIST()

	for( int i = 0; i < wl.getWishesCount(); ++i )
		assert( baseValues[i] == wl[i] );

	ASSERT_THROWS(
			wl += baseValues[2]
		,	Messages::WishAlreadyExists
	);

	ASSERT_THROWS(
			wl += Wish( "", 45 )
		,	Messages::WishNameEmpty
	);

	ASSERT_THROWS(
			wl += Wish( "Test", -11 )
		,	Messages::NegativePriority
	);

	wl += Wish( "Test", 11 );
	assert( wl.getWishPriority( "Test" ) == 11 );
	assert( wl.getWishesCount() == baseValues.size() + 1 );
}


/*****************************************************************************/