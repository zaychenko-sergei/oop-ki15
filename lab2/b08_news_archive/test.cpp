// (C) 2016, Sergei Zaychenko, KNURE, Kharkiv, Ukraine

/*****************************************************************************/

#include "news_archive.hpp"
#include "messages.hpp"

#include "testslib.hpp"

#include <sstream>

/*****************************************************************************/

/*
	Test plan:
	Done	1) Empty archive
	Done	2) Add entry
	Done		2.1) Simple
	Done		2.2) Several entries
	Done		2.3) With empty title
	Done		2.4) With empty text
	Done		2.5) With duplicated title
	Done		2.6) With duplicated text
	Done		2.7) With duplicated date
	Done	3) Get entry
	Done		3.1) Text by empty title
	Done		3.2) Title by missing date
	Done		3.3) Text by missing date
	Done		3.4) Text by missing title
	Done		3.5) Date by empty title
	Done		3.6) Date by missing title
	Done	4) Has entry
	Done		4.1) By empty title
	Done		4.2) By missing date
	Done		4.3) By missing title
	Done	5) For each entry
	Done		5.1) Empty archive
	Done		5.2) Single entry
	Done		5.3) Entries ordered by date
	Done	6) Modify entry title
	Done		6.1) Once
	Done		6.2) Several times for different entries
	Done		6.3) Several times for the same entry
	Done		6.4) By missing date
	Done		6.5) With empty title
	Done	7) Modify entry text
	Done		7.1) Once by date
	Done		7.2) Several times by date for different entries
	Done		7.3) Several times by date for the same entry
	Done		7.4) Once by title
	Done		7.5) Several times by title for different entries
	Done		7.6) Several times by title for the same entry
	Done		7.7) By missing date
	Done		7.8) By date and with empty text
	Done		7.9) By missing title
	Done		7.10) By title and missing text
	Done	8) Remove entry
	Done		8.1) Simple by date
	Done		8.2) Simple by title
	Done		8.3) Several times by date for different entries
	Done		8.4) Several times by date for the same entry
	Done		8.5) Several times by title for different entries
	Done		8.6) Several times by title for the same entry
	Done		8.7) By missing date
	Done		8.8) By missing title
	Done	9) Get entry titles with text contains
	Done		9.1) Simple
	Done		9.2) By empty string
	Done		9.3) Nothing is matched
	Done		9.4) Only several entries are matched
	Done		9.5) All entries are matched
*/

/*****************************************************************************/


DECLARE_OOP_TEST( archive_1_empty_archive )
{
	const NewsArchive a;

	assert( a.getEntriesCount() == 0 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( archive_2_1_add_entry_simple )
{
	NewsArchive a;
	const NewsArchive & rA = a;

	DateTime dt;
	const std::string title = "newTitle";
	const std::string text = "newText";
	a.addEntry( dt, title, text );

	assert( rA.getEntriesCount() == 1 );
	assert( rA.hasEntry( title ) );
	assert( rA.hasEntry( dt ) );

	const std::string & resultTitle = rA.getEntryTitle( dt );
	const std::string & resultText1 = rA.getEntryText( dt );
	const std::string & resultText2 = rA.getEntryText( title );
	DateTime resultDate = rA.getEntryDate( title );

	assert( resultTitle == title );
	assert( resultText1 == text );
	assert( resultText2 == text );
	assert( resultDate == dt );
}


/*****************************************************************************/


DECLARE_OOP_TEST( archive_2_2_add_entry_several_entries )
{
	NewsArchive a;
	const NewsArchive & rA = a;

	DateTime date1( 2000, 5, 5, 10, 10, 10 );
	const std::string title1 = "newTitle";
	const std::string text1 = "newText";
	a.addEntry( date1, title1, text1 );

	DateTime date2( 3000, 10, 10, 15, 15, 15 );
	const std::string title2 = "otherTitle";
	const std::string text2 = "otherText";
	a.addEntry( date2, title2, text2 );

	assert( rA.getEntriesCount() == 2 );
	assert( rA.hasEntry( date1 ) );
	assert( rA.hasEntry( title1 ) );
	assert( rA.hasEntry( date2 ) );
	assert( rA.hasEntry( title2 ) );

	assert( rA.getEntryTitle( date1 ) == title1 );
	assert( rA.getEntryText( date1 ) == text1 );
	assert( rA.getEntryText( title1 ) == text1 );
	assert( rA.getEntryDate( title1 ) == date1 );

	assert( rA.getEntryTitle( date2 ) == title2 );
	assert( rA.getEntryText( date2 ) == text2 );
	assert( rA.getEntryText( title2 ) == text2 );
	assert( rA.getEntryDate( title2 ) == date2 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( archive_2_3_add_entry_with_empty_title )
{
	NewsArchive a;
	const NewsArchive & rA = a;

	DateTime date;
	const std::string title = "";
	const std::string text = "newText";

	ASSERT_THROWS(
			a.addEntry( date, title, text );
		,	Messages::EmptyEntryTitle
	);

	assert( rA.getEntriesCount() == 0 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( archive_2_4_add_entry_with_empty_text )
{
	NewsArchive a;
	const NewsArchive & rA = a;

	DateTime date;
	const std::string title = "newTitle";
	const std::string text = "";

	ASSERT_THROWS(
			a.addEntry( date, title, text );
		,	Messages::EmptyEntryText
	);

	assert( rA.getEntriesCount() == 0 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( archive_2_5_add_entry_with_duplicated_title )
{
	NewsArchive a;
	const NewsArchive & rA = a;

	DateTime date;
	const std::string title = "newTitle";
	const std::string text1 = "newText1";
	const std::string text2 = "newText2";

	a.addEntry( date, title, text1 );

	ASSERT_THROWS(
			a.addEntry( date, title, text2 );
		,	Messages::DuplicatedEntry
	);

	assert( rA.getEntriesCount() == 1 );
	assert( rA.hasEntry( title ) );
	assert( rA.getEntryText( title ) == text1 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( archive_2_6_add_entry_with_duplicated_text )
{
	NewsArchive a;
	const NewsArchive & rA = a;

	DateTime date1( 1111, 1, 1, 1, 1, 1 );
	DateTime date2( 2222, 2, 2, 2, 2, 2 );
	const std::string title1 = "newTitle1";
	const std::string title2 = "newTitle2";
	const std::string text = "newText";

	a.addEntry( date1, title1, text );
	a.addEntry( date2, title2, text );

	assert( rA.getEntriesCount() == 2 );
	assert( rA.hasEntry( date1 ) );
	assert( rA.hasEntry( title1 ) );
	assert( rA.hasEntry( date2 ) );
	assert( rA.hasEntry( title2 ) );
}


/*****************************************************************************/


DECLARE_OOP_TEST( archive_2_7_add_entry_with_duplicated_date )
{
	NewsArchive a;
	const NewsArchive & rA = a;

	DateTime date;
	const std::string title1 = "newTitle1";
	const std::string title2 = "newTitle2";
	const std::string text1 = "newText1";
	const std::string text2 = "newText2";

	a.addEntry( date, title1, text1 );

	ASSERT_THROWS(
			a.addEntry( date, title2, text2 );
		,	Messages::DuplicatedEntry
	);

	assert( rA.getEntriesCount() == 1 );
	assert( rA.hasEntry( title1 ) );
	assert( rA.getEntryText( title1 ) == text1 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( archive_3_1_get_entry_text_by_empty_title )
{
	NewsArchive a;
	const NewsArchive & rA = a;

	DateTime date;
	const std::string title = "newTitle";
	const std::string text = "newText";

	a.addEntry( date, title, text );

	ASSERT_THROWS(
			rA.getEntryText( "" );
		,	Messages::EntryDoesNotExist
	);
}


/*****************************************************************************/


DECLARE_OOP_TEST( archive_3_2_get_entry_title_by_missing_date )
{
	NewsArchive a;
	const NewsArchive & rA = a;

	DateTime date;
	const std::string title = "newTitle";
	const std::string text = "newText";

	a.addEntry( date, title, text );

	ASSERT_THROWS(
			rA.getEntryTitle( DateTime( 1111, 1, 1, 1, 1, 1 ) );
		,	Messages::EntryDoesNotExist
	);
}


/*****************************************************************************/


DECLARE_OOP_TEST( archive_3_3_get_entry_text_by_missing_date )
{
	NewsArchive a;
	const NewsArchive & rA = a;

	DateTime date;
	const std::string title = "newTitle";
	const std::string text = "newText";

	a.addEntry( date, title, text );

	ASSERT_THROWS(
			rA.getEntryText( DateTime( 1111, 1, 1, 1, 1, 1 ) );
		,	Messages::EntryDoesNotExist
	);
}


/*****************************************************************************/


DECLARE_OOP_TEST( archive_3_4_get_entry_text_by_missing_title )
{
	NewsArchive a;
	const NewsArchive & rA = a;

	DateTime date;
	const std::string title = "newTitle";
	const std::string text = "newText";

	a.addEntry( date, title, text );

	ASSERT_THROWS(
			rA.getEntryText( "missing" );
		,	Messages::EntryDoesNotExist
	);
}


/*****************************************************************************/


DECLARE_OOP_TEST( archive_3_5_get_entry_date_by_empty_title )
{
	NewsArchive a;
	const NewsArchive & rA = a;

	DateTime date;
	const std::string title = "newTitle";
	const std::string text = "newText";

	a.addEntry( date, title, text );

	ASSERT_THROWS(
			rA.getEntryDate( "" );
		,	Messages::EntryDoesNotExist
	);
}


/*****************************************************************************/


DECLARE_OOP_TEST( archive_3_6_get_entry_date_by_missing_title )
{
	NewsArchive a;
	const NewsArchive & rA = a;

	DateTime date;
	const std::string title = "newTitle";
	const std::string text = "newText";

	a.addEntry( date, title, text );

	ASSERT_THROWS(
			rA.getEntryDate( "missing title" );
		,	Messages::EntryDoesNotExist
	);
}


/*****************************************************************************/


DECLARE_OOP_TEST( archive_4_1_has_entry_by_empty_title )
{
	NewsArchive a;
	const NewsArchive & rA = a;

	DateTime date;
	const std::string title = "newTitle";
	const std::string text = "newText";

	a.addEntry( date, title, text );

	assert( !rA.hasEntry( "" ) );
}


/*****************************************************************************/


DECLARE_OOP_TEST( archive_4_2_has_entry_by_missing_date )
{
	NewsArchive a;
	const NewsArchive & rA = a;

	DateTime date;
	const std::string title = "newTitle";
	const std::string text = "newText";

	a.addEntry( date, title, text );

	assert( !rA.hasEntry( DateTime( 1, 2, 3, 4, 5, 6 ) ) );
}


/*****************************************************************************/


DECLARE_OOP_TEST( archive_4_3_has_entry_by_missing_title )
{
	NewsArchive a;
	const NewsArchive & rA = a;

	DateTime date;
	const std::string title = "newTitle";
	const std::string text = "newText";

	a.addEntry( date, title, text );

	assert( !rA.hasEntry( "some of other" ) );
}


/*****************************************************************************/


DECLARE_OOP_TEST( archive_5_1_for_each_entry_empty_archive )
{
	NewsArchive a;
	const NewsArchive & rA = a;

	rA.forEachEntry(
		[&]( DateTime _date, const std::string & _title, const std::string & _text )
		{
			assert( ! "We should never be here without registrations" );
		}
	);
}


/*****************************************************************************/


DECLARE_OOP_TEST( archive_5_2_for_each_entry_single_entry )
{
	NewsArchive a;
	const NewsArchive & rA = a;

	DateTime date( 1, 2, 3, 4, 5, 6 );
	const std::string title = "newTitle";
	const std::string text = "newText";

	a.addEntry( date, title, text );

	int iterationsCount = 0;
	rA.forEachEntry(
		[&]( DateTime _date, const std::string & _title, const std::string & _text )
		{
			if ( iterationsCount == 0 )
			{
				assert( _date == date );
				assert( _title == title );
				assert( _text == text );

				iterationsCount = 1;
			}
			else
				assert( ! "There was only 1 entry" );
		}
	);
}


/*****************************************************************************/


DECLARE_OOP_TEST( archive_5_3_for_each_entry_entries_ordered_by_date )
{
	NewsArchive a;
	const NewsArchive & rA = a;

	a.addEntry( DateTime( 1, 2, 3, 4, 5, 1 ), "newTitle1", "newText1" );
	a.addEntry( DateTime( 1, 2, 3, 4, 5, 4 ), "newTitle4", "newText4" );
	a.addEntry( DateTime( 1, 2, 3, 4, 5, 3 ), "newTitle3", "newText3" );
	a.addEntry( DateTime( 1, 2, 3, 4, 5, 5 ), "newTitle5", "newText5" );
	a.addEntry( DateTime( 1, 2, 3, 4, 5, 6 ), "newTitle6", "newText6" );
	a.addEntry( DateTime( 1, 2, 3, 4, 5, 2 ), "newTitle2", "newText2" );

	std::vector< DateTime > result;
	rA.forEachEntry(
		[&]( DateTime _date, const std::string & _title, const std::string & _text )
		{
			result.push_back( _date );
		}
	);

	assert( result.size() == 6 );
	assert( result[ 0 ] == DateTime( 1, 2, 3, 4, 5, 1 ) );
	assert( result[ 1 ] == DateTime( 1, 2, 3, 4, 5, 2 ) );
	assert( result[ 2 ] == DateTime( 1, 2, 3, 4, 5, 3 ) );
	assert( result[ 3 ] == DateTime( 1, 2, 3, 4, 5, 4 ) );
	assert( result[ 4 ] == DateTime( 1, 2, 3, 4, 5, 5 ) );
	assert( result[ 5 ] == DateTime( 1, 2, 3, 4, 5, 6 ) );
}


/*****************************************************************************/


DECLARE_OOP_TEST( archive_6_1_modify_entry_title_once )
{
	NewsArchive a;
	const NewsArchive & rA = a;

	DateTime dt;
	const std::string titleBefore = "newTitle";
	const std::string titleAfter = "fff";
	const std::string text = "newText";
	a.addEntry( dt, titleBefore, text );

	a.modifyEntryTitle( dt, titleAfter );

	assert( rA.getEntriesCount() == 1 );
	assert( !rA.hasEntry( titleBefore ) );
	assert( rA.hasEntry( titleAfter ) );
	assert( rA.hasEntry( dt ) );

	const std::string & resultTitle = rA.getEntryTitle( dt );
	const std::string & resultText1 = rA.getEntryText( dt );
	const std::string & resultText2 = rA.getEntryText( titleAfter );
	DateTime resultDate = rA.getEntryDate( titleAfter );

	assert( resultTitle == titleAfter );
	assert( resultText1 == text );
	assert( resultText2 == text );
	assert( resultDate == dt );
}


/*****************************************************************************/


DECLARE_OOP_TEST( archive_6_2_modify_entry_title_several_times_for_different_entries )
{
	NewsArchive a;
	const NewsArchive & rA = a;

	DateTime date1( 2000, 5, 5, 10, 10, 10 );
	const std::string title1before = "newTitleBefore";
	const std::string title1after = "newTitleAfter";
	const std::string text1 = "newText";
	a.addEntry( date1, title1before, text1 );

	DateTime date2( 3000, 10, 10, 15, 15, 15 );
	const std::string title2before = "otherTitleBefore";
	const std::string title2after = "otherTitleAfter";
	const std::string text2 = "otherText";
	a.addEntry( date2, title2before, text2 );

	a.modifyEntryTitle( date1, title1after );
	a.modifyEntryTitle( date2, title2after );
	
	assert( rA.getEntriesCount() == 2 );
	assert( rA.hasEntry( date1 ) );
	assert( !rA.hasEntry( title1before ) );
	assert( rA.hasEntry( title1after ) );
	assert( rA.hasEntry( date2 ) );
	assert( !rA.hasEntry( title2before ) );
	assert( rA.hasEntry( title2after ) );

	assert( rA.getEntryTitle( date1 ) == title1after );
	assert( rA.getEntryText( date1 ) == text1 );
	assert( rA.getEntryText( title1after ) == text1 );
	assert( rA.getEntryDate( title1after ) == date1 );

	assert( rA.getEntryTitle( date2 ) == title2after );
	assert( rA.getEntryText( date2 ) == text2 );
	assert( rA.getEntryText( title2after ) == text2 );
	assert( rA.getEntryDate( title2after ) == date2 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( archive_6_3_modify_entry_title_several_times_for_the_same_entry )
{
	NewsArchive a;
	const NewsArchive & rA = a;

	DateTime dt;
	const std::string titleBefore = "newTitle";
	const std::string titleAfter = "fff";
	const std::string text = "newText";
	a.addEntry( dt, titleBefore, text );

	a.modifyEntryTitle( dt, "111" );
	a.modifyEntryTitle( dt, "aaa" );
	a.modifyEntryTitle( dt, "sss" );
	a.modifyEntryTitle( dt, "ddd" );
	a.modifyEntryTitle( dt, titleAfter );

	assert( rA.getEntriesCount() == 1 );
	assert( !rA.hasEntry( titleBefore ) );
	assert( rA.hasEntry( titleAfter ) );
	assert( rA.hasEntry( dt ) );

	const std::string & resultTitle = rA.getEntryTitle( dt );
	const std::string & resultText1 = rA.getEntryText( dt );
	const std::string & resultText2 = rA.getEntryText( titleAfter );
	DateTime resultDate = rA.getEntryDate( titleAfter );

	assert( resultTitle == titleAfter );
	assert( resultText1 == text );
	assert( resultText2 == text );
	assert( resultDate == dt );
}


/*****************************************************************************/


DECLARE_OOP_TEST( archive_6_4_modify_entry_title_by_missing_date )
{
	NewsArchive a;
	const NewsArchive & rA = a;

	DateTime dt;
	const std::string titleBefore = "newTitle";
	const std::string titleAfter = "fff";
	const std::string text = "newText";
	a.addEntry( dt, titleBefore, text );

	ASSERT_THROWS(
			a.modifyEntryTitle( DateTime( 1993, 7, 19, 11, 15, 00 ), titleAfter );
		,	Messages::EntryDoesNotExist
	);

	assert( rA.getEntriesCount() == 1 );
	assert( rA.hasEntry( titleBefore ) );
	assert( !rA.hasEntry( titleAfter ) );
}


/*****************************************************************************/


DECLARE_OOP_TEST( archive_6_5_modify_entry_title_with_empty_title )
{
	NewsArchive a;
	const NewsArchive & rA = a;

	DateTime dt;
	const std::string titleBefore = "newTitle";
	const std::string titleAfter = "fff";
	const std::string text = "newText";
	a.addEntry( dt, titleBefore, text );

	ASSERT_THROWS(
			a.modifyEntryTitle( dt, "" );
		,	Messages::EmptyEntryTitle
	);

	assert( rA.getEntriesCount() == 1 );
	assert( rA.hasEntry( titleBefore ) );
	assert( !rA.hasEntry( titleAfter ) );
}


/*****************************************************************************/


DECLARE_OOP_TEST( archive_7_1_modify_entry_text_once_by_date )
{
	NewsArchive a;
	const NewsArchive & rA = a;

	DateTime dt;
	const std::string title = "newTitle";
	const std::string textBefore = "newText1";
	const std::string textAfter = "newText2";
	a.addEntry( dt, title, textBefore );

	a.modifyEntryText( dt, textAfter );

	assert( rA.getEntriesCount() == 1 );
	assert( rA.hasEntry( title ) );
	assert( rA.hasEntry( dt ) );

	const std::string & resultTitle = rA.getEntryTitle( dt );
	const std::string & resultText1 = rA.getEntryText( dt );
	const std::string & resultText2 = rA.getEntryText( title );
	DateTime resultDate = rA.getEntryDate( title );

	assert( resultTitle == title );
	assert( resultText1 == textAfter );
	assert( resultText2 == textAfter );
	assert( resultDate == dt );
}


/*****************************************************************************/


DECLARE_OOP_TEST( archive_7_2_modify_entry_text_several_times_by_date_for_different_entries )
{
	NewsArchive a;
	const NewsArchive & rA = a;

	DateTime date1( 2010, 1, 1, 10, 00, 00 );
	DateTime date2( 2011, 1, 1, 10, 00, 00 );
	const std::string title1 = "newTitle1";
	const std::string title2 = "newTitle2";
	const std::string textBefore = "textBefore";
	const std::string textAfter1 = "textAfter1";
	const std::string textAfter2 = "textAfter2";
	a.addEntry( date1, title1, textBefore );
	a.addEntry( date2, title2, textBefore );

	a.modifyEntryText( date1, textAfter1 );
	a.modifyEntryText( date2, textAfter2 );

	assert( rA.getEntriesCount() == 2 );
	assert( rA.hasEntry( title1 ) );
	assert( rA.hasEntry( date1 ) );
	assert( rA.hasEntry( title2 ) );
	assert( rA.hasEntry( date2 ) );

	assert( rA.getEntryTitle( date1 ) == title1 );
	assert( rA.getEntryText( date1 ) == textAfter1 );
	assert( rA.getEntryText( title1 ) == textAfter1 );
	assert( rA.getEntryDate( title1 ) == date1 );

	assert( rA.getEntryTitle( date2 ) == title2 );
	assert( rA.getEntryText( date2 ) == textAfter2 );
	assert( rA.getEntryText( title2 ) == textAfter2 );
	assert( rA.getEntryDate( title2 ) == date2 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( archive_7_3_modify_entry_text_several_times_by_date_for_the_same_entry )
{
	NewsArchive a;
	const NewsArchive & rA = a;

	DateTime dt;
	const std::string title = "newTitle";
	const std::string textBefore = "newText1";
	const std::string textAfter = "newText2";
	a.addEntry( dt, title, textBefore );
	
	a.modifyEntryText( dt, "asdf" );
	a.modifyEntryText( dt, "gshdfkgj" );
	a.modifyEntryText( dt, "12423dsd" );
	a.modifyEntryText( dt, textAfter );

	assert( rA.getEntriesCount() == 1 );
	assert( rA.hasEntry( title ) );
	assert( rA.hasEntry( dt ) );

	const std::string & resultTitle = rA.getEntryTitle( dt );
	const std::string & resultText1 = rA.getEntryText( dt );
	const std::string & resultText2 = rA.getEntryText( title );
	DateTime resultDate = rA.getEntryDate( title );

	assert( resultTitle == title );
	assert( resultText1 == textAfter );
	assert( resultText2 == textAfter );
	assert( resultDate == dt );
}


/*****************************************************************************/


DECLARE_OOP_TEST( archive_7_4_modify_entry_text_once_by_title )
{
	NewsArchive a;
	const NewsArchive & rA = a;

	DateTime dt;
	const std::string title = "newTitle";
	const std::string textBefore = "newText1";
	const std::string textAfter = "newText2";
	a.addEntry( dt, title, textBefore );

	a.modifyEntryText( title, textAfter );

	assert( rA.getEntriesCount() == 1 );
	assert( rA.hasEntry( title ) );
	assert( rA.hasEntry( dt ) );

	const std::string & resultTitle = rA.getEntryTitle( dt );
	const std::string & resultText1 = rA.getEntryText( dt );
	const std::string & resultText2 = rA.getEntryText( title );
	DateTime resultDate = rA.getEntryDate( title );

	assert( resultTitle == title );
	assert( resultText1 == textAfter );
	assert( resultText2 == textAfter );
	assert( resultDate == dt );
}


/*****************************************************************************/


DECLARE_OOP_TEST( archive_7_5_modify_entry_text_several_times_by_title_for_different_entries )
{
	NewsArchive a;
	const NewsArchive & rA = a;

	DateTime date1( 2010, 1, 1, 10, 00, 00 );
	DateTime date2( 2011, 1, 1, 10, 00, 00 );
	const std::string title1 = "newTitle1";
	const std::string title2 = "newTitle2";
	const std::string textBefore = "textBefore";
	const std::string textAfter1 = "textAfter1";
	const std::string textAfter2 = "textAfter2";
	a.addEntry( date1, title1, textBefore );
	a.addEntry( date2, title2, textBefore );

	a.modifyEntryText( title1, textAfter1 );
	a.modifyEntryText( title2, textAfter2 );

	assert( rA.getEntriesCount() == 2 );
	assert( rA.hasEntry( title1 ) );
	assert( rA.hasEntry( date1 ) );
	assert( rA.hasEntry( title2 ) );
	assert( rA.hasEntry( date2 ) );

	assert( rA.getEntryTitle( date1 ) == title1 );
	assert( rA.getEntryText( date1 ) == textAfter1 );
	assert( rA.getEntryText( title1 ) == textAfter1 );
	assert( rA.getEntryDate( title1 ) == date1 );

	assert( rA.getEntryTitle( date2 ) == title2 );
	assert( rA.getEntryText( date2 ) == textAfter2 );
	assert( rA.getEntryText( title2 ) == textAfter2 );
	assert( rA.getEntryDate( title2 ) == date2 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( archive_7_6_modify_entry_text_several_times_by_title_for_the_same_entry )
{
	NewsArchive a;
	const NewsArchive & rA = a;

	DateTime dt;
	const std::string title = "newTitle";
	const std::string textBefore = "newText1";
	const std::string textAfter = "newText2";
	a.addEntry( dt, title, textBefore );
	
	a.modifyEntryText( title, "asdf" );
	a.modifyEntryText( title, "gshdfkgj" );
	a.modifyEntryText( title, "12423dsd" );
	a.modifyEntryText( title, textAfter );

	assert( rA.getEntriesCount() == 1 );
	assert( rA.hasEntry( title ) );
	assert( rA.hasEntry( dt ) );

	const std::string & resultTitle = rA.getEntryTitle( dt );
	const std::string & resultText1 = rA.getEntryText( dt );
	const std::string & resultText2 = rA.getEntryText( title );
	DateTime resultDate = rA.getEntryDate( title );

	assert( resultTitle == title );
	assert( resultText1 == textAfter );
	assert( resultText2 == textAfter );
	assert( resultDate == dt );
}


/*****************************************************************************/


DECLARE_OOP_TEST( archive_7_7_modify_entry_text_by_missing_date )
{
	NewsArchive a;
	const NewsArchive & rA = a;

	DateTime dt;
	const std::string title = "newTitle";
	const std::string textBefore = "newText1";
	const std::string textAfter = "newText2";
	a.addEntry( dt, title, textBefore );

	ASSERT_THROWS(
			a.modifyEntryText( DateTime( 1993, 11, 13, 13, 13, 13 ), textAfter );
		,	Messages::EntryDoesNotExist
	);
}


/*****************************************************************************/


DECLARE_OOP_TEST( archive_7_8_modify_entry_text_by_date_and_with_empty_text )
{
	NewsArchive a;
	const NewsArchive & rA = a;

	DateTime dt;
	const std::string title = "newTitle";
	const std::string textBefore = "newText1";
	const std::string textAfter = "newText2";
	a.addEntry( dt, title, textBefore );

	ASSERT_THROWS(
			a.modifyEntryText( dt, "" );
		,	Messages::EmptyEntryText
	);
}


/*****************************************************************************/


DECLARE_OOP_TEST( archive_7_9_modify_entry_text_by_missing_title )
{
	NewsArchive a;
	const NewsArchive & rA = a;

	DateTime dt;
	const std::string title = "newTitle";
	const std::string textBefore = "newText1";
	const std::string textAfter = "newText2";
	a.addEntry( dt, title, textBefore );

	ASSERT_THROWS(
			a.modifyEntryText( "missing title", textAfter );
		,	Messages::EntryDoesNotExist
	);
}


/*****************************************************************************/


DECLARE_OOP_TEST( archive_7_10_modify_entry_text_by_title_and_missing_text )
{
	NewsArchive a;
	const NewsArchive & rA = a;

	DateTime dt;
	const std::string title = "newTitle";
	const std::string textBefore = "newText1";
	const std::string textAfter = "newText2";
	a.addEntry( dt, title, textBefore );

	ASSERT_THROWS(
			a.modifyEntryText( title, "" );
		,	Messages::EmptyEntryText
	);
}


/*****************************************************************************/


DECLARE_OOP_TEST( archive_8_1_remove_entry_simple_by_date )
{
	NewsArchive a;
	const NewsArchive & rA = a;

	DateTime dt;
	const std::string title = "newTitle";
	const std::string text = "newText";
	a.addEntry( dt, title, text );

	a.removeEntry( dt );

	assert( rA.getEntriesCount() == 0 );
	assert( !rA.hasEntry( title ) );
	assert( !rA.hasEntry( dt ) );
}


/*****************************************************************************/


DECLARE_OOP_TEST( archive_8_2_remove_entry_simple_by_title )
{
	NewsArchive a;
	const NewsArchive & rA = a;

	DateTime dt;
	const std::string title = "newTitle";
	const std::string text = "newText";
	a.addEntry( dt, title, text );

	a.removeEntry( title );

	assert( rA.getEntriesCount() == 0 );
	assert( !rA.hasEntry( title ) );
	assert( !rA.hasEntry( dt ) );
}


/*****************************************************************************/


DECLARE_OOP_TEST( archive_8_3_remove_entry_several_times_by_date_for_different_entries )
{
	NewsArchive a;
	const NewsArchive & rA = a;

	DateTime date1( 2010, 1, 1, 10, 00, 00 );
	DateTime date2( 2011, 1, 1, 10, 00, 00 );
	const std::string title1 = "newTitle1";
	const std::string title2 = "newTitle2";
	const std::string text1 = "text1";
	const std::string text2 = "text2";
	a.addEntry( date1, title1, text1 );
	a.addEntry( date2, title2, text2 );

	a.removeEntry( date1 );
	a.removeEntry( date2 );

	assert( rA.getEntriesCount() == 0 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( archive_8_4_remove_entry_several_times_by_date_for_the_same_entry )
{
	NewsArchive a;
	const NewsArchive & rA = a;

	DateTime dt;
	const std::string title = "newTitle";
	const std::string text = "newText";
	a.addEntry( dt, title, text );

	a.removeEntry( dt );

	ASSERT_THROWS(
			a.removeEntry( dt );
		,	Messages::EntryDoesNotExist
	);

	assert( rA.getEntriesCount() == 0 );
	assert( !rA.hasEntry( title ) );
	assert( !rA.hasEntry( dt ) );
}


/*****************************************************************************/


DECLARE_OOP_TEST( archive_8_5_remove_entry_several_times_by_title_for_different_entries )
{
	NewsArchive a;
	const NewsArchive & rA = a;

	DateTime date1( 2010, 1, 1, 10, 00, 00 );
	DateTime date2( 2011, 1, 1, 10, 00, 00 );
	const std::string title1 = "newTitle1";
	const std::string title2 = "newTitle2";
	const std::string text1 = "text1";
	const std::string text2 = "text2";
	a.addEntry( date1, title1, text1 );
	a.addEntry( date2, title2, text2 );

	a.removeEntry( title1 );
	a.removeEntry( title2 );

	assert( rA.getEntriesCount() == 0 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( archive_8_6_remove_entry_several_times_by_title_for_the_same_entry )
{
	NewsArchive a;
	const NewsArchive & rA = a;

	DateTime dt;
	const std::string title = "newTitle";
	const std::string text = "newText";
	a.addEntry( dt, title, text );

	a.removeEntry( title );

	ASSERT_THROWS(
			a.removeEntry( title );
		,	Messages::EntryDoesNotExist
	);

	assert( rA.getEntriesCount() == 0 );
	assert( !rA.hasEntry( title ) );
	assert( !rA.hasEntry( dt ) );
}


/*****************************************************************************/


DECLARE_OOP_TEST( archive_8_7_remove_entry_by_missing_date )
{
	NewsArchive a;
	const NewsArchive & rA = a;

	DateTime dt;
	const std::string title = "newTitle";
	const std::string text = "newText";
	a.addEntry( dt, title, text );


	ASSERT_THROWS(
			a.removeEntry( DateTime( 1993, 3, 3, 3, 3, 3 ) );
		,	Messages::EntryDoesNotExist
	);

	assert( rA.getEntriesCount() == 1 );
	assert( rA.hasEntry( title ) );
	assert( rA.hasEntry( dt ) );
}


/*****************************************************************************/


DECLARE_OOP_TEST( archive_8_8_remove_entry_by_missing_title )
{
	NewsArchive a;
	const NewsArchive & rA = a;

	DateTime dt;
	const std::string title = "newTitle";
	const std::string text = "newText";
	a.addEntry( dt, title, text );

	ASSERT_THROWS(
			a.removeEntry( "mt" );
		,	Messages::EntryDoesNotExist
	);

	assert( rA.getEntriesCount() == 1 );
	assert( rA.hasEntry( title ) );
	assert( rA.hasEntry( dt ) );
}


/*****************************************************************************/


DECLARE_OOP_TEST( archive_9_1_get_entry_titles_with_text_contains_simple )
{
	NewsArchive a;
	const NewsArchive & rA = a;

	DateTime dt;
	const std::string title = "newTitle";
	const std::string text = "newText";
	a.addEntry( dt, title, text );

	std::vector< std::string > result = rA.getEntryTitlesWithTextContains( "ewTe" );
	std::vector< std::string > expected = { "newTitle" };

	assert( result == expected );
}


/*****************************************************************************/


DECLARE_OOP_TEST( archive_9_2_get_entry_titles_with_text_contains_by_empty_string )
{
	NewsArchive a;
	const NewsArchive & rA = a;
	
	a.addEntry( DateTime( 2015, 1, 1, 2, 2, 4 ), "title3", "newText3" );
	a.addEntry( DateTime( 2015, 1, 1, 2, 2, 2 ), "title1", "newText1" );
	a.addEntry( DateTime( 2015, 1, 1, 2, 2, 3 ), "title2", "newText2" );

	std::vector< std::string > result = rA.getEntryTitlesWithTextContains( "" );
	std::vector< std::string > expected = { "title1", "title2", "title3" };

	assert( result == expected );
}


/*****************************************************************************/


DECLARE_OOP_TEST( archive_9_3_get_entry_titles_with_text_contains_nothing_is_matched )
{
	NewsArchive a;
	const NewsArchive & rA = a;
	
	a.addEntry( DateTime( 2015, 1, 1, 2, 2, 4 ), "title3", "newText3" );
	a.addEntry( DateTime( 2015, 1, 1, 2, 2, 2 ), "title1", "newText1" );
	a.addEntry( DateTime( 2015, 1, 1, 2, 2, 3 ), "title2", "newText2" );

	std::vector< std::string > result = rA.getEntryTitlesWithTextContains( "magic" );
	std::vector< std::string > expected = {};

	assert( result == expected );
}


/*****************************************************************************/


DECLARE_OOP_TEST( archive_9_4_get_entry_titles_with_text_contains_only_several_entries_are_matched )
{
	NewsArchive a;
	const NewsArchive & rA = a;
	
	a.addEntry( DateTime( 2015, 1, 1, 2, 2, 4 ), "title3", "my_2_text_3" );
	a.addEntry( DateTime( 2015, 1, 1, 2, 2, 2 ), "title1", "my_1_text_1" );
	a.addEntry( DateTime( 2015, 1, 1, 2, 2, 3 ), "title2", "my_2_text_2" );

	std::vector< std::string > result = rA.getEntryTitlesWithTextContains( "_2_t" );
	std::vector< std::string > expected = { "title2", "title3" };

	assert( result == expected );
}


/*****************************************************************************/


DECLARE_OOP_TEST( archive_9_5_get_entry_titles_with_text_contains_all_entries_are_matched )
{
	NewsArchive a;
	const NewsArchive & rA = a;
	
	a.addEntry( DateTime( 2015, 1, 1, 2, 2, 4 ), "title3", "my_2_text_3" );
	a.addEntry( DateTime( 2015, 1, 1, 2, 2, 2 ), "title1", "my_1_text_1" );
	a.addEntry( DateTime( 2015, 1, 1, 2, 2, 3 ), "title2", "my_2_text_2" );

	std::vector< std::string > result = rA.getEntryTitlesWithTextContains( "_text_" );
	std::vector< std::string > expected = { "title1", "title2", "title3" };

	assert( result == expected );
}


/*****************************************************************************/
