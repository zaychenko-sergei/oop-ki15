// (C) 2016, Sergei Zaychenko, KNURE, Kharkiv, Ukraine

/*****************************************************************************/

#include "testslib.hpp"

#include "blacklist.hpp"
#include "messages.hpp"

/*****************************************************************************/

#define INIT_COMPEX_LIST()													\
																			\
std::initializer_list< std::string > list1init (							\
		{																	\
				"+17034820623"												\
			,	"example@dot.com"											\
			,	"ai-core@neuromancer.com"									\
			,	"56458110"													\
																			\
			,	"count.zero@cyberspace.net"									\
			,	"Willis.Corto@wintermute.com"								\
			,	"machine13@test_nodes_requests.corp"						\
		}																	\
);																			\
																			\
	Blacklist list1(														\
		list1init															\
	);																		\
																			\
	std::set< std::string > list1actuals( list1init );						\
																			\
	Blacklist list2(														\
		{																	\
				"+17034820623"												\
			,	"example@dot.com"											\
			,	"56458110"													\
			,	"ai-core@neuromancer.com"									\
																			\
			,	"+6434774000"												\
			,	"henry.case@chiba.net"										\
		}																	\
	);																		\
																			\
	Blacklist intersectResult(												\
		{																	\
				"+17034820623"												\
			,	"example@dot.com"											\
			,	"56458110"													\
			,	"ai-core@neuromancer.com"									\
		}																	\
	);																		\
																			\
	Blacklist unionResult(													\
		{																	\
				"+17034820623"												\
			,	"example@dot.com"											\
			,	"56458110"													\
			,	"ai-core@neuromancer.com"									\
			,	"count.zero@cyberspace.net"									\
			,	"Willis.Corto@wintermute.com"								\
			,	"machine13@test_nodes_requests.corp"						\
			,	"+6434774000"												\
			,	"henry.case@chiba.net"										\
		}																	\
	);																		\
																			\
	Blacklist differenceResult(												\
		{																	\
				"count.zero@cyberspace.net"									\
			,	"Willis.Corto@wintermute.com"								\
			,	"machine13@test_nodes_requests.corp"						\
			,	"+6434774000"												\
			,	"henry.case@chiba.net"										\
		}																	\
	);



/*****************************************************************************/


DECLARE_OOP_TEST ( test_Blacklist_Constructors )
{
	Blacklist blacklist;

	assert( blacklist.getEntriesCount() == 0 );

	Blacklist b2( Blacklist( { "+124", "5846423", "mail@dot.com" } ) );
	
	assert( b2.getEntriesCount() == 3 );
	assert( b2.hasEntry( "+124" ) );
	assert( b2.hasEntry( "mail@dot.com" ) );
	assert( b2.hasEntry( "5846423" ) );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_Blacklist_AddEntry_CorrectCases )
{
	Blacklist bl;

	bl.addEntry( "some_addr@test.com" );

	assert( bl.getEntriesCount() == 1 );
	assert( bl.hasEntry( "some_addr@test.com" ) );

	bl.addEntry( "+3805412387465" );

	assert( bl.getEntriesCount() == 2 );
	assert( bl.hasEntry( "+3805412387465" ) );

	bl.addEntry( "87654321547" );

	assert( bl.getEntriesCount() == 3 );
	assert( bl.hasEntry( "87654321547" ) );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_Blacklist_AddEntry_IncorrectCases )
{
	Blacklist bl;

	ASSERT_THROWS(
			bl.addEntry( "" )
		,	Messages::EntryNameIsIncorrect
	);

	bl.addEntry( "some_addr@test.com" );

	ASSERT_THROWS(
			bl.addEntry( "some_addr@test.com" )
		,	Messages::EntryAlreadyExists
	);

	assert( bl.getEntriesCount() == 1 );
	assert( bl.hasEntry( "some_addr@test.com" ) );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_Blacklist_DropEntry_CorrectCases )
{
	Blacklist bl;

	bl.addEntry( "some_addr@test.com" );
	bl.addEntry( "+3805412387465" );
	bl.addEntry( "87654321547" );

	bl.dropEntry( "+3805412387465" );

	assert( bl.getEntriesCount() == 2 );
	assert( !bl.hasEntry( "+3805412387465" ) );

	bl.dropEntry( "some_addr@test.com" );

	assert( bl.getEntriesCount() == 1 );
	assert( !bl.hasEntry( "some_addr@test.com" ) );

	bl.dropEntry( "87654321547" );

	assert( bl.getEntriesCount() == 0 );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_Blacklist_DropEntry_IncorrectCases )
{
	Blacklist bl;

	bl.addEntry( "some_addr@test.com" );
	bl.addEntry( "+3805412387465" );
	bl.addEntry( "87654321547" );

	ASSERT_THROWS(
			bl.dropEntry( "" )
		,	Messages::EntryNameIsIncorrect
	);

	ASSERT_THROWS(
			bl.dropEntry( "wrong_format" )
		,	Messages::EntryNameIsIncorrect
	);

	ASSERT_THROWS(
			bl.dropEntry( "test@mail.com" )
		,	Messages::EntryDoesNotExist
	);

	bl.dropEntry( "+3805412387465" );

	ASSERT_THROWS(
			bl.dropEntry( "+3805412387465" )
		,	Messages::EntryDoesNotExist
	);
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_Blacklist_AddAndDropAndAddEntry )
{
	Blacklist bl;

	bl.addEntry( "some_addr@test.com" );
	bl.addEntry( "+3805412387465" );
	bl.addEntry( "87654321547" );

	assert( bl.hasEntry( "+3805412387465" ) );

	bl.dropEntry( "+3805412387465" );

	assert( !bl.hasEntry( "+3805412387465" ) );

	bl.addEntry( "+3805412387465" );

	assert( bl.hasEntry( "+3805412387465" ) );

	assert( bl.getEntriesCount() == 3 );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_Blacklist_Clear )
{
	Blacklist bl;

	bl.addEntry( "some_addr@test.com" );
	bl.addEntry( "+3805412387465" );
	bl.addEntry( "87654321547" );

	assert( bl.getEntriesCount() == 3 );

	bl.clear();

	assert( bl.getEntriesCount() == 0 );

	bl.clear();
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_Blacklist_AddEntries )
{
	Blacklist bl;

	bl.addEntries( { "wrong_mail", "+345F566", "correct@mail", "111" } );
	assert( bl.getEntriesCount() == 2 );
	assert( bl.hasEntry( "correct@mail" ) );
	assert( bl.hasEntry( "111" ) );

	bl.addEntries( { "correct@mail", "111" } );
	assert( bl.getEntriesCount() == 2 );
	assert( bl.hasEntry( "correct@mail" ) );
	assert( bl.hasEntry( "111" ) );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_Blacklist_DropEntries )
{
	Blacklist bl;

	bl.addEntries( { "mail1@test.com", "+345784245", "correct@mail", "111" } );

	assert( bl.getEntriesCount() == 4 );

	bl.dropEntries( { "unexisting_correct_mail@domain.net", "wrong_mail", "111" } );

	assert( bl.getEntriesCount() == 3 );
	assert( !bl.hasEntry( "111" ) );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_Blacklist_GetIntersection )
{
	INIT_COMPEX_LIST();

	assert( intersectResult == list1.getIntersection( list2 ) );
	assert( intersectResult == list2.getIntersection( list1 ) );

	Blacklist emptyList1, emptyList2;

	assert( list1.getIntersection( emptyList1 ).getEntriesCount() == 0 );
	assert( emptyList1.getIntersection( emptyList2 ).getEntriesCount() == 0 );
	assert( list2.getIntersection( list2 ) == list2 );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_Blacklist_GetUnion )
{
	INIT_COMPEX_LIST();

	assert( unionResult == list1.getUnion( list2 ) );
	assert( unionResult == list2.getUnion( list1 ) );

	Blacklist emptyList1, emptyList2;

	assert( list1.getUnion( emptyList1 ) == list1 );
	assert( emptyList1.getUnion( list1 ) == list1 );

	assert( emptyList1.getUnion( emptyList2 ).getEntriesCount() == 0 );

	assert( list1 == list1.getUnion( list1 ) );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_Blacklist_GetDifference )
{
	INIT_COMPEX_LIST();

	assert( differenceResult == list1.getDifference( list2 ) );
	assert( differenceResult == list2.getDifference( list1 ) );

	Blacklist emptyList1, emptyList2;

	assert( list1.getDifference( emptyList1 ) == list1 );
	assert( emptyList1.getDifference( list1 ) == list1 );

	assert( emptyList1.getDifference( emptyList2 ).getEntriesCount() == 0 );

	assert( list1.getDifference( list1 ).getEntriesCount() == 0 );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_Blacklist_For_Ranged_Loop_Iterators )
{
	INIT_COMPEX_LIST();

	for( std::string const & entry : list1  )
		assert( list1actuals.count( entry ) );

	for( auto it = list1.begin(); it != list1.end(); ++it )
		assert( list1actuals.count( *it ) );
}


/*****************************************************************************/