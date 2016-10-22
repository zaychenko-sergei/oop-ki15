// (C) 2016, Sergei Zaychenko, KNURE, Kharkiv, Ukraine

/*****************************************************************************/

#include "diary.hpp"
#include "messages.hpp"

#include "testslib.hpp"

#include <sstream>

/*****************************************************************************/

/*
	Test plan:
	Done	1) Create empty diary
	Done	2) Create diary with empty name
	Done	3) Add entry
	Done		3.1) Simple
	Done		3.2) With empty text
	Done	4) Add several entries
	Done		4.1) For same day
	Done		4.2) For different days
	Done		4.3) For same time in same day
	Done		4.4) For same time in different days
	Done		4.5) Second entry with time less previous entry
	Done		4.6) With the same text
	Done	5) Get entry
	Done		5.1) Simple
	Done		5.2) Several entries
	Done		5.3) For missing Date
	Done		5.4) For correct Date, but missing Time
	Done		5.5) For correct Date and correct Time, but for other day
	Done	6) Modify entry
	Done		6.1) Simple modification
	Done		6.2) With empty text
	Done		6.3) For missing Date
	Done		6.4) For correct Date but missing Time
	Done		6.5) For correct Date and correct Time, but for other day
	Done		6.6) Several modifications for different entries
	Done		6.7) Several modifications for the same entry
	Done	7) Remove entries by Date
	Done		7.1) Simple
	Done		7.2) For missing Date
	Done	8) Remove entries by Time
	Done		8.1) Only 1 day has entry for this time
	Done		8.2) Several days have entries for this time
	Done		8.3) Any day has not entries for this time
	Done	9) Remove entry by Date and Time
	Done		9.1) Simple
	Done		9.2) For missing Date
	Done		9.3) For correct Date, but missing Time
	Done		9.4) For correct Date and correct Time, but for other day
	Done	10) Print diary
	Done		10.1) Empty
	Done		10.2) With 1 day
	Done		10.3) With several days
	Done		10.4) With empty day
	Done	11) Copy of Diary
	Done		11.1) Stack
	Done		11.2) Heap
*/

/*****************************************************************************/


DECLARE_OOP_TEST( diary_1_create_empty_diary )
{
	const Diary d( "me" );

	const std::string & ownerName = d.getOwnerName();
	assert( ownerName == "me" );

	assert( d.getDaysCount() == 0 );
	assert( d.begin() == d.end() );
}


/*****************************************************************************/


DECLARE_OOP_TEST( diary_2_create_diary_with_empty_name )
{
	ASSERT_THROWS(
			const Diary d( "" );
		,	Messages::EmptyOwnerName
	);
}


/*****************************************************************************/


DECLARE_OOP_TEST( diary_3_1_add_entry_simple )
{
	Diary d( "someone" );
	const Diary & rD = d;

	Date date( 2016, 07, 19 );
	Time time( 11, 00, 00 );
	d.addEntry( date, time, "I want to eat" );

	assert( rD.getOwnerName() == "someone" );

	assert( rD.getDaysCount() == 1 );
	assert( rD.getEntriesCount( date ) == 1 );
	assert( rD.begin() != rD.end() );

	assert( rD.hasEntries( date ) );
	assert( rD.hasEntries( time ) );
	assert( rD.hasEntry( date, time ) );
}


/*****************************************************************************/


DECLARE_OOP_TEST( diary_3_2_add_entry_with_empty_text )
{
	Diary d( "test1" );

	Date date( 2016, 07, 19 );
	Time time( 11, 00, 00 );

	ASSERT_THROWS(
			d.addEntry( date, time, "" );
		,	Messages::EmptyEntryText
	);
}


/*****************************************************************************/


DECLARE_OOP_TEST( diary_4_1_add_several_entries_for_same_day )
{
	Diary d( "kukushka" );
	const Diary & rD = d;

	Date date( 2016, 07, 19 );
	Time time1( 11, 00, 00 );
	Time time2( 11, 01, 00 );
	d.addEntry( date, time1, "hello" );
	d.addEntry( date, time2, "world" );

	assert( rD.getDaysCount() == 1 );
	assert( rD.getEntriesCount( date ) == 2 );
	assert( rD.begin() != rD.end() );

	assert( rD.hasEntries( date ) );
	assert( rD.hasEntries( time1 ) );
	assert( rD.hasEntries( time2 ) );
	assert( rD.hasEntry( date, time1 ) );
	assert( rD.hasEntry( date, time2 ) );
}


/*****************************************************************************/


DECLARE_OOP_TEST( diary_4_2_add_several_entries_for_different_days )
{
	Diary d( "kukushka" );
	const Diary & rD = d;

	Date date1( 2016, 07, 19 );
	Date date2( 2016, 07, 27 );
	Time time1( 11, 00, 00 );
	Time time2( 11, 01, 00 );
	d.addEntry( date1, time1, "hello" );
	d.addEntry( date2, time2, "world" );

	assert( rD.getDaysCount() == 2 );
	assert( rD.getEntriesCount( date1 ) == 1 );
	assert( rD.getEntriesCount( date2 ) == 1 );
	assert( rD.begin() != rD.end() );

	assert( rD.hasEntries( date1 ) );
	assert( rD.hasEntries( date2 ) );
	assert( rD.hasEntries( time1 ) );
	assert( rD.hasEntries( time2 ) );
	assert( rD.hasEntry( date1, time1 ) );
	assert( rD.hasEntry( date2, time2 ) );

	assert( !rD.hasEntry( date1, time2 ) );
	assert( !rD.hasEntry( date2, time1 ) );
}


/*****************************************************************************/


DECLARE_OOP_TEST( diary_4_3_add_several_entries_for_same_time_in_same_day )
{
	Diary d( "test2" );
	const Diary & rD = d;

	Date date( 2016, 07, 19 );
	Time time( 11, 00, 00 );

	d.addEntry( date, time, "hello" );

	ASSERT_THROWS(
			d.addEntry( date, time, "world" );
		,	Messages::InvalidEntryTime
	);

	assert( rD.getDaysCount() == 1 );
	assert( rD.getEntriesCount( date ) == 1 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( diary_4_4_add_several_entries_for_same_time_in_different_days )
{
	Diary d( "kukushka" );
	const Diary & rD = d;

	Date date1( 2016, 07, 19 );
	Date date2( 2016, 07, 27 );
	Time time( 11, 00, 00 );
	d.addEntry( date1, time, "hello" );
	d.addEntry( date2, time, "world" );

	assert( rD.getDaysCount() == 2 );
	assert( rD.getEntriesCount( date1 ) == 1 );
	assert( rD.getEntriesCount( date2 ) == 1 );
	assert( rD.begin() != rD.end() );

	assert( rD.hasEntries( date1 ) );
	assert( rD.hasEntries( date2 ) );
	assert( rD.hasEntries( time ) );
	assert( rD.hasEntry( date1, time ) );
	assert( rD.hasEntry( date2, time ) );
}


/*****************************************************************************/


DECLARE_OOP_TEST( diary_4_5_add_several_entries_second_entry_with_time_less_previous_entry )
{
	Diary d( "test2" );
	const Diary & rD = d;

	Date date( 2016, 07, 19 );
	Time time1( 11, 11, 11 );
	Time time2( 10, 11, 11 );
	Time time3( 11, 10, 11 );
	Time time4( 11, 11, 10 );

	d.addEntry( date, time1, "hello" );

	ASSERT_THROWS(
			d.addEntry( date, time2, "text1" );
		,	Messages::InvalidEntryTime
	);
	ASSERT_THROWS(
			d.addEntry( date, time3, "text2" );
		,	Messages::InvalidEntryTime
	);
	ASSERT_THROWS(
			d.addEntry( date, time4, "text3" );
		,	Messages::InvalidEntryTime
	);

	assert( rD.getDaysCount() == 1 );
	assert( rD.getEntriesCount( date ) == 1 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( diary_4_6_add_several_entries_with_the_same_text )
{
	Diary d( "kukushka" );
	const Diary & rD = d;

	Date date1( 2016, 07, 19 );
	Date date2( 2016, 07, 27 );
	Time time1( 11, 00, 00 );
	Time time2( 11, 01, 00 );
	d.addEntry( date1, time1, "long time ago we were young" );
	d.addEntry( date2, time2, "long time ago we were young" );

	assert( rD.getDaysCount() == 2 );
	assert( rD.getEntriesCount( date1 ) == 1 );
	assert( rD.getEntriesCount( date2 ) == 1 );
	assert( rD.begin() != rD.end() );

	assert( rD.hasEntries( date1 ) );
	assert( rD.hasEntries( date2 ) );
	assert( rD.hasEntries( time1 ) );
	assert( rD.hasEntries( time2 ) );
	assert( rD.hasEntry( date1, time1 ) );
	assert( rD.hasEntry( date2, time2 ) );

	assert( !rD.hasEntry( date1, time2 ) );
	assert( !rD.hasEntry( date2, time1 ) );
}


/*****************************************************************************/


DECLARE_OOP_TEST( diary_5_1_get_entry_simple )
{
	Diary d( "kukushka" );
	const Diary & rD = d;

	Date date( 1985, 01, 01 );
	Time time( 11, 59, 59 );
	d.addEntry( date, time, "friendly" );

	const std::string & text = rD.getEntry( date, time );
	assert( text == "friendly" );
}


/*****************************************************************************/


DECLARE_OOP_TEST( diary_5_2_get_entry_several_entries )
{
	Diary d( "kukushka" );
	const Diary & rD = d;

	Date date( 1985, 01, 01 );
	Time time1( 11, 58, 58 );
	Time time2( 11, 59, 59 );
	d.addEntry( date, time1, "friendly" );
	d.addEntry( date, time2, "user" );

	const std::string & text1 = rD.getEntry( date, time1 );
	const std::string & text2 = rD.getEntry( date, time2 );
	assert( text1 == "friendly" );
	assert( text2 == "user" );
}


/*****************************************************************************/


DECLARE_OOP_TEST( diary_5_3_get_entry_for_missing_date )
{
	Diary d( "kukushka" );
	const Diary & rD = d;

	Date date( 1985, 01, 01 );
	Time time( 11, 59, 59 );
	d.addEntry( date, time, "friendly" );
	
	Date missingDate( 1111, 11, 11 );

	ASSERT_THROWS(
			rD.getEntry( missingDate, time );
		,	Messages::EntryDoesNotExist
	);
}


/*****************************************************************************/


DECLARE_OOP_TEST( diary_5_4_get_entry_for_correct_date_but_missing_time )
{
	Diary d( "kukushka" );
	const Diary & rD = d;

	Date date( 1985, 01, 01 );
	Time time( 11, 59, 59 );
	d.addEntry( date, time, "friendly" );
	
	Time missingTime( 11, 11, 11 );

	ASSERT_THROWS(
			rD.getEntry( date, missingTime );
		,	Messages::EntryDoesNotExist
	);
}


/*****************************************************************************/


DECLARE_OOP_TEST( diary_5_5_get_entry_for_correct_date_correct_time_but_for_other_day )
{
	Diary d( "kukushka" );
	const Diary & rD = d;

	Date date1( 1985, 01, 01 );
	Date date2( 1985, 11, 11 );
	Time time1( 11, 59, 59 );
	Time time2( 10, 22, 33 );
	d.addEntry( date1, time1, "s1" );
	d.addEntry( date2, time2, "s2" );
	
	ASSERT_THROWS(
			rD.getEntry( date1, time2 );
		,	Messages::EntryDoesNotExist
	);
	ASSERT_THROWS(
			rD.getEntry( date2, time1 );
		,	Messages::EntryDoesNotExist
	);
}


/*****************************************************************************/


DECLARE_OOP_TEST( diary_6_1_modify_entry_simple_modifications )
{
	Diary d( "kukushka" );
	const Diary & rD = d;

	Date date( 2016, 07, 19 );
	Time time( 11, 00, 00 );
	d.addEntry( date, time, "hello" );
	d.modifyEntry( date, time, "world" );

	assert( rD.getDaysCount() == 1 );
	assert( rD.getEntriesCount( date ) == 1 );

	const std::string & text = rD.getEntry( date, time );
	assert( text == "world" );
}


/*****************************************************************************/


DECLARE_OOP_TEST( diary_6_2_modify_entry_with_empty_text )
{
	Diary d( "kukushka" );

	Date date( 2016, 07, 19 );
	Time time( 11, 00, 00 );
	d.addEntry( date, time, "hello" );

	ASSERT_THROWS(
			d.modifyEntry( date, time, "" );
		,	Messages::EmptyEntryText
	);
}


/*****************************************************************************/


DECLARE_OOP_TEST( diary_6_3_modify_entry_for_missing_date )
{
	Diary d( "kukushka" );

	Date date1( 2016, 07, 19 );
	Date date2( 2015, 07, 19 );
	Time time( 11, 00, 00 );
	d.addEntry( date1, time, "hello" );

	ASSERT_THROWS(
			d.modifyEntry( date2, time, "it's me" );
		,	Messages::EntryDoesNotExist
	);
}


/*****************************************************************************/


DECLARE_OOP_TEST( diary_6_4_modify_entry_for_correct_date_but_missing_time )
{
	Diary d( "kukushka" );

	Date date( 2016, 07, 19 );
	Time time1( 11, 00, 00 );
	Time time2( 11, 01, 00 );
	d.addEntry( date, time1, "hello" );

	ASSERT_THROWS(
			d.modifyEntry( date, time2, "it's me" );
		,	Messages::EntryDoesNotExist
	);
}


/*****************************************************************************/


DECLARE_OOP_TEST( diary_6_5_modify_entry_for_correct_date_and_correct_time_but_for_other_day )
{
	Diary d( "kukushka" );

	Date date1( 2016, 7, 19 );
	Date date2( 2016, 8, 19 );
	Time time1( 11, 00, 00 );
	Time time2( 11, 01, 00 );
	d.addEntry( date1, time1, "hello" );
	d.addEntry( date2, time2, "it's me" );

	ASSERT_THROWS(
			d.modifyEntry( date1, time2, "1111" );
		,	Messages::EntryDoesNotExist
	);
	ASSERT_THROWS(
			d.modifyEntry( date2, time1, "2222" );
		,	Messages::EntryDoesNotExist
	);
}


/*****************************************************************************/


DECLARE_OOP_TEST( diary_6_6_modify_entry_several_modifications_for_different_entries )
{
	Diary d( "kukushka" );
	const Diary & rD = d;

	Date date( 2016, 07, 19 );
	Time time1( 11, 00, 00 );
	Time time2( 11, 00, 01 );
	d.addEntry( date, time1, "hello1" );
	d.addEntry( date, time2, "hello2" );
	d.modifyEntry( date, time1, "4world" );
	d.modifyEntry( date, time2, "5world" );

	assert( rD.getDaysCount() == 1 );
	assert( rD.getEntriesCount( date ) == 2 );

	const std::string & text1 = rD.getEntry( date, time1 );
	const std::string & text2 = rD.getEntry( date, time2 );
	assert( text1 == "4world" );
	assert( text2 == "5world" );
}


/*****************************************************************************/


DECLARE_OOP_TEST( diary_6_7_modify_entry_several_modifications_for_the_same_entry )
{
	Diary d( "kukushka" );
	const Diary & rD = d;

	Date date( 2016, 07, 19 );
	Time time( 11, 00, 00 );
	d.addEntry( date, time, "my word" );
	d.modifyEntry( date, time, "other" );
	d.modifyEntry( date, time, "next" );
	d.modifyEntry( date, time, "finally" );

	assert( rD.getDaysCount() == 1 );
	assert( rD.getEntriesCount( date ) == 1 );

	const std::string & text = rD.getEntry( date, time );
	assert( text == "finally" );
}


/*****************************************************************************/


DECLARE_OOP_TEST( diary_7_1_remove_entries_by_date_simple )
{
	Diary d( "kukushka" );
	const Diary & rD = d;

	Date date( 2016, 07, 19 );
	Time time1( 11, 00, 00 );
	Time time2( 11, 00, 01 );
	Time time3( 11, 00, 02 );
	Time time4( 11, 00, 03 );
	d.addEntry( date, time1, "my word" );
	d.addEntry( date, time2, "other" );
	d.addEntry( date, time3, "next" );
	d.addEntry( date, time4, "finally" );

	assert( rD.getDaysCount() == 1 );
	assert( rD.getEntriesCount( date ) == 4 );

	d.removeEntries( date );

	assert( rD.getDaysCount() == 0 );
	assert( rD.getEntriesCount( date ) == 0 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( diary_7_2_remove_entries_by_date_for_missing_date )
{
	Diary d( "kukushka" );
	const Diary & rD = d;

	Date date( 2016, 07, 19 );
	Time time1( 11, 00, 00 );
	Time time2( 11, 00, 01 );
	Time time3( 11, 00, 02 );
	Time time4( 11, 00, 03 );
	d.addEntry( date, time1, "my word" );
	d.addEntry( date, time2, "other" );
	d.addEntry( date, time3, "next" );
	d.addEntry( date, time4, "finally" );

	assert( rD.getDaysCount() == 1 );
	assert( rD.getEntriesCount( date ) == 4 );

	Date missingDate( 1016, 07, 19 );
	ASSERT_THROWS(
			d.removeEntries( missingDate );
		,	Messages::EntryDoesNotExist
	);

	assert( rD.getDaysCount() == 1 );
	assert( rD.getEntriesCount( date ) == 4 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( diary_8_1_remove_entries_by_time_only_1_day_has_entry_for_this_time )
{
	Diary d( "kukushka" );
	const Diary & rD = d;

	Date date1( 2016, 07, 19 );
	Date date2( 2016, 07, 20 );
	Time time1( 11, 00, 00 );
	Time time2( 11, 00, 01 );
	d.addEntry( date1, time1, "my word" );
	d.addEntry( date1, time2, "other" );
	d.addEntry( date2, time1, "next" );

	assert( rD.getDaysCount() == 2 );
	assert( rD.getEntriesCount( date1 ) == 2 );
	assert( rD.getEntriesCount( date2 ) == 1 );

	d.removeEntries( time2 );

	assert( rD.getDaysCount() == 2  );
	assert( rD.getEntriesCount( date1 ) == 1 );
	assert( rD.getEntriesCount( date2 ) == 1 );

	assert( rD.getEntry( date1, time1 ) == "my word" );
	assert( rD.getEntry( date2, time1 ) == "next" );
}


/*****************************************************************************/


DECLARE_OOP_TEST( diary_8_2_remove_entries_by_time_several_days_have_entries_for_this_time )
{
	Diary d( "kukushka" );
	const Diary & rD = d;

	Date date1( 2016, 07, 19 );
	Date date2( 2016, 07, 20 );
	Time time1( 11, 00, 00 );
	Time time2( 11, 00, 01 );
	d.addEntry( date1, time1, "my word" );
	d.addEntry( date1, time2, "other" );
	d.addEntry( date2, time1, "next" );

	assert( rD.getDaysCount() == 2 );
	assert( rD.getEntriesCount( date1 ) == 2 );
	assert( rD.getEntriesCount( date2 ) == 1 );

	d.removeEntries( time1 );

	assert( rD.getDaysCount() == 2  );
	assert( rD.getEntriesCount( date1 ) == 1 );
	assert( rD.getEntriesCount( date2 ) == 0 );

	assert( rD.getEntry( date1, time2 ) == "other" );
}


/*****************************************************************************/


DECLARE_OOP_TEST( diary_8_3_remove_entries_by_time_any_day_has_not_entries_for_this_time )
{
	Diary d( "kukushka" );
	const Diary & rD = d;

	Date date1( 2016, 07, 19 );
	Date date2( 2016, 07, 20 );
	Time time1( 11, 00, 00 );
	Time time2( 11, 00, 01 );
	d.addEntry( date1, time1, "my word" );
	d.addEntry( date2, time1, "next" );

	assert( rD.getDaysCount() == 2 );
	assert( rD.getEntriesCount( date1 ) == 1 );
	assert( rD.getEntriesCount( date2 ) == 1 );

	d.removeEntries( time2 );

	assert( rD.getDaysCount() == 2  );
	assert( rD.getEntriesCount( date1 ) == 1 );
	assert( rD.getEntriesCount( date2 ) == 1 );

	assert( rD.getEntry( date1, time1 ) == "my word" );
	assert( rD.getEntry( date2, time1 ) == "next" );
}


/*****************************************************************************/


DECLARE_OOP_TEST( diary_9_1_remove_entry_by_date_and_time_simple )
{
	Diary d( "kukushka" );
	const Diary & rD = d;

	Date date1( 2016, 07, 19 );
	Date date2( 2016, 07, 20 );
	Time time1( 11, 00, 00 );
	Time time2( 11, 00, 01 );
	d.addEntry( date1, time1, "my word" );
	d.addEntry( date1, time2, "other" );
	d.addEntry( date2, time1, "next" );
	d.addEntry( date2, time2, "finally" );

	assert( rD.getDaysCount() == 2 );
	assert( rD.getEntriesCount( date1 ) == 2 );
	assert( rD.getEntriesCount( date2 ) == 2 );

	d.removeEntry( date1, time2 );
	d.removeEntry( date2, time1 );

	assert( rD.getDaysCount() == 2  );
	assert( rD.getEntriesCount( date1 ) == 1 );
	assert( rD.getEntriesCount( date2 ) == 1 );

	assert( rD.getEntry( date1, time1 ) == "my word" );
	assert( rD.getEntry( date2, time2 ) == "finally" );
}


/*****************************************************************************/


DECLARE_OOP_TEST( diary_9_2_remove_entry_by_date_and_time_for_missing_date )
{
	Diary d( "kukushka" );
	const Diary & rD = d;

	Date date1( 2016, 07, 19 );
	Date date2( 2016, 07, 20 );
	Time time( 11, 00, 00 );
	d.addEntry( date1, time, "next" );

	assert( rD.getDaysCount() == 1 );
	assert( rD.getEntriesCount( date1 ) == 1 );

	ASSERT_THROWS(
			d.removeEntry( date2, time );
		,	Messages::EntryDoesNotExist
	);

	assert( rD.getDaysCount() == 1 );
	assert( rD.getEntriesCount( date1 ) == 1 );

	assert( rD.getEntry( date1, time ) == "next" );
}


/*****************************************************************************/


DECLARE_OOP_TEST( diary_9_3_remove_entry_by_date_and_time_for_correct_date_but_missing_time )
{
	Diary d( "kukushka" );
	const Diary & rD = d;

	Date date( 2016, 07, 19 );
	Time time1( 11, 00, 00 );
	Time time2( 11, 00, 01 );
	d.addEntry( date, time1, "next" );

	assert( rD.getDaysCount() == 1 );
	assert( rD.getEntriesCount( date ) == 1 );

	ASSERT_THROWS(
			d.removeEntry( date, time2 );
		,	Messages::EntryDoesNotExist
	);

	assert( rD.getDaysCount() == 1 );
	assert( rD.getEntriesCount( date ) == 1 );

	assert( rD.getEntry( date, time1 ) == "next" );
}


/*****************************************************************************/


DECLARE_OOP_TEST( diary_9_4_remove_entry_by_date_and_time_for_correct_date_and_correct_time_but_for_other_day )
{
	Diary d( "kukushka" );
	const Diary & rD = d;

	Date date1( 2016, 07, 11 );
	Date date2( 2016, 07, 23 );
	Time time1( 11, 00, 00 );
	Time time2( 11, 00, 01 );
	d.addEntry( date1, time1, "next1" );
	d.addEntry( date2, time2, "next2" );

	assert( rD.getDaysCount() == 2 );
	assert( rD.getEntriesCount( date1 ) == 1 );
	assert( rD.getEntriesCount( date2 ) == 1 );

	ASSERT_THROWS(
			d.removeEntry( date1, time2 );
		,	Messages::EntryDoesNotExist
	);
	ASSERT_THROWS(
			d.removeEntry( date2, time1 );
		,	Messages::EntryDoesNotExist
	);

	assert( rD.getDaysCount() == 2 );
	assert( rD.getEntriesCount( date1 ) == 1 );
	assert( rD.getEntriesCount( date2 ) == 1 );

	assert( rD.getEntry( date1, time1 ) == "next1" );
	assert( rD.getEntry( date2, time2 ) == "next2" );
}


/*****************************************************************************/


DECLARE_OOP_TEST( diary_10_1_print_diary_empty )
{
	const Diary d( "kukushka" );

	std::stringstream stream;
	stream << d;

	std::string actual = stream.str();
	std::string expected =
		"Owner: kukushka\n"
		"No entries\n"
	;

	assert( actual == expected );
}


/*****************************************************************************/


DECLARE_OOP_TEST( diary_10_2_print_diary_with_1_day )
{
	Diary d( "kukushka" );
	const Diary & rD = d;

	Date date( 2000, 01, 01 );
	d.addEntry( date, Time( 10, 10, 10 ), "some text #1" );
	d.addEntry( date, Time( 10, 10, 11 ), "some text #2" );
	d.addEntry( date, Time( 10, 11, 10 ), "some text #3" );
	d.addEntry( date, Time( 11, 10, 10 ), "some text #4" );

	std::stringstream stream;
	stream << rD;

	std::string actual = stream.str();
	std::string expected =
		"Owner: kukushka\n"
		"Date: 2000/1/1\n"
		"    10:10:10: some text #1\n"
		"    10:10:11: some text #2\n"
		"    10:11:10: some text #3\n"
		"    11:10:10: some text #4\n"
	;

	assert( actual == expected );
}


/*****************************************************************************/


DECLARE_OOP_TEST( diary_10_3_print_diary_with_several_days )
{
	Diary d( "kukushka" );
	const Diary & rD = d;

	Date date1( 2000, 01, 01 );
	Date date2( 2001, 01, 05 );
	d.addEntry( date1, Time( 10, 10, 10 ), "some text #1" );
	d.addEntry( date1, Time( 10, 10, 11 ), "some text #2" );
	d.addEntry( date2, Time( 10, 11, 10 ), "some text #3" );
	d.addEntry( date2, Time( 11, 10, 10 ), "some text #4" );

	std::stringstream stream;
	stream << rD;

	std::string actual = stream.str();
	std::string expected =
		"Owner: kukushka\n"
		"Date: 2000/1/1\n"
		"    10:10:10: some text #1\n"
		"    10:10:11: some text #2\n"
		"Date: 2001/1/5\n"
		"    10:11:10: some text #3\n"
		"    11:10:10: some text #4\n"
	;

	assert( actual == expected );
}


/*****************************************************************************/


DECLARE_OOP_TEST( diary_10_4_print_diary_with_empty_day )
{
	Diary d( "kukushka" );
	const Diary & rD = d;

	Date date1( 2000, 01, 01 );
	Date date2( 2001, 01, 05 );
	Date date3( 2001, 01, 10 );
	d.addEntry( date1, Time( 10, 10, 10 ), "some text #1" );
	d.addEntry( date1, Time( 10, 10, 11 ), "some text #2" );
	d.addEntry( date2, Time( 10, 11, 10 ), "some text #3" );
	d.addEntry( date3, Time( 11, 10, 10 ), "some text #4" );

	d.removeEntry( date2, Time( 10, 11, 10 ) );

	std::stringstream stream;
	stream << rD;

	std::string actual = stream.str();
	std::string expected =
		"Owner: kukushka\n"
		"Date: 2000/1/1\n"
		"    10:10:10: some text #1\n"
		"    10:10:11: some text #2\n"
		"Date: 2001/1/5\n"
		"Date: 2001/1/10\n"
		"    11:10:10: some text #4\n"
	;

	assert( actual == expected );
}


/*****************************************************************************/


DECLARE_OOP_TEST( diary_11_1_copy_of_diary_stack )
{
	Diary d( "kukushka" );
	const Diary & rD = d;

	Date date1( 2016, 07, 19 );
	Date date2( 2016, 06, 19 );
	Time time1( 11, 00, 00 );
	Time time2( 11, 01, 00 );
	d.addEntry( date1, time1, "hello" );
	d.addEntry( date2, time2, "world" );

	Diary newD( rD );
	d.removeEntries( date1 );
	d.removeEntries( date2 );

	assert( d.getDaysCount() == 0 );
	assert( newD.getEntry( date1, time1 ) == "hello" );
	assert( newD.getEntry( date2, time2 ) == "world" );
}


/*****************************************************************************/


DECLARE_OOP_TEST( diary_11_2_copy_of_diary_heap )
{
	Diary * d = new Diary( "kukushka" );
	const Diary & rD = * d;

	Date date1( 2016, 07, 19 );
	Date date2( 2016, 06, 19 );
	Time time1( 11, 00, 00 );
	Time time2( 11, 01, 00 );
	d->addEntry( date1, time1, "hello" );
	d->addEntry( date2, time2, "world" );

	Diary * newD = new Diary( rD );

	d->removeEntries( date1 );
	d->removeEntries( date2 );

	assert( d->getDaysCount() == 0 );

	delete d;

	assert( newD->getEntry( date1, time1 ) == "hello" );
	assert( newD->getEntry( date2, time2 ) == "world" );

	delete newD;
}


/*****************************************************************************/
