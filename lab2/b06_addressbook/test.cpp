// (C) 2016, Sergei Zaychenko, KNURE, Kharkiv, Ukraine

/*****************************************************************************/

#include "addressbook.hpp"
#include "messages.hpp"

#include "testslib.hpp"

#include <sstream>

/*****************************************************************************/

/*
	Test plan:
	Done	1) Empty book
	Done	2) Add entry
	Done		2.1) One entry
	Done		2.2) Several entries
	Done		2.3) With empty name
	Done		2.4) With empty phone number
	Done		2.5) Duplicated entry
	Done	3) Get name
	Done		3.1) By phone number
	Done		3.2) By empty phone number
	Done		3.3) By missing phone number
	Done	4) Get phone number
	Done		4.1) By name
	Done		4.2) By empty name
	Done		4.3) By missing name
	Done	5) Remove entry
	Done		5.1) One entry
	Done		5.2) Several entries
	Done		5.3) By empty name
	Done		5.4) By empty phone number
	Done		5.5) By name and missing phone number
	Done		5.6) By phone number and missing name
	Done		5.7) By missing name and missing phone number
	Done	6) Remove entry by name
	Done		6.1) One entry
	Done		6.2) Several entries
	Done		6.3) By empty name
	Done		6.4) By missing name
	Done	7) Remove entry by phone number
	Done		7.1) One entry
	Done		7.2) Several entries
	Done		7.3) By empty phone number
	Done		7.4) By missing phone number
	Done	8) Has entry by name
	Done		8.1) By simple name
	Done		8.1) By empty name
	Done		8.2) By missing name
	Done	9) Has entry by phone number
	Done		9.1) By simple phone number
	Done		9.1) By empty phone number
	Done		9.2) By missing phone number
	Done	10) Get entries count by name
	Done		10.1) Empty book
	Done		10.2) Filter is empty
	Done		10.3) Filter matches several names
	Done		10.4) Filter matches all names
	Done		10.5) Filter matches one name
	Done		10.6) Filter does not match any name
	Done	11) Get entries count by phone number
	Done		11.1) Empty book
	Done		11.2) Filter is empty
	Done		11.3) Filter matches several phone numbers
	Done		11.4) Filter matches all phone numbers
	Done		11.5) Filter matches one phone number
	Done		11.6) Filter does not match any phone number
	Done	12) Create ordered book
	Done		12.1) Empty book
	Done		12.2) Only 1 entry
	Done		12.3) Several entries
	Done	13) Print to output stream
	Done		13.1) Empty book
	Done		13.2) Several entries
	Done	14) Complex cases
	Done		14.1) Add entry with the same name but different phone numbers
	Done		14.2) Add entry with the same phone number but different names
	Done		14.3) Add entry with name and phone number from other entries
*/

/*****************************************************************************/


DECLARE_OOP_TEST( addressbook_1_empty_book )
{
	const AddressBook b;

	assert( b.getEntriesCount() == 0 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( addressbook_2_1_add_entry_one_entry )
{
	AddressBook b;
	b.addEntry( "name", "phone" );

	assert( b.getEntriesCount() == 1 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( addressbook_2_2_add_entry_several_entries )
{
	AddressBook b;
	b.addEntry( "name1", "phone1" );
	b.addEntry( "name2", "phone2" );

	assert( b.getEntriesCount() == 2 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( addressbook_2_3_add_entry_with_empty_name )
{
	AddressBook b;

	ASSERT_THROWS(
			b.addEntry( "", "phone" );
		,	Messages::EmptyName
	);
}


/*****************************************************************************/


DECLARE_OOP_TEST( addressbook_2_4_add_entry_with_empty_phone_number )
{
	AddressBook b;

	ASSERT_THROWS(
			b.addEntry( "name", "" );
		,	Messages::EmptyPhoneNumber
	);
}


/*****************************************************************************/


DECLARE_OOP_TEST( addressbook_2_5_add_entry_duplicated_entry )
{
	AddressBook b;
	b.addEntry( "name", "phone" );
	b.addEntry( "name", "phone" );
	b.addEntry( "name", "phone" );

	assert( b.getEntriesCount() == 1 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( addressbook_3_1_get_name_by_phone_number )
{
	AddressBook b;
	b.addEntry( "name", "phone" );

	assert( b.getEntriesCount() == 1 );
	assert( b.getName( "phone" ) == "name" );
}


/*****************************************************************************/


DECLARE_OOP_TEST( addressbook_3_2_get_name_by_empty_phone_number )
{
	AddressBook b;
	b.addEntry( "name", "phone" );

	ASSERT_THROWS(
			b.getName( "" );
		,	Messages::EmptyPhoneNumber
	);
}


/*****************************************************************************/


DECLARE_OOP_TEST( addressbook_3_3_get_name_by_missing_phone_number )
{
	AddressBook b;
	b.addEntry( "name", "phone" );

	ASSERT_THROWS(
			b.getName( "missing" );
		,	Messages::EntryDoesNotExist
	);
}


/*****************************************************************************/


DECLARE_OOP_TEST( addressbook_4_1_get_phone_number_by_name )
{
	AddressBook b;
	b.addEntry( "name", "phone" );

	assert( b.getEntriesCount() == 1 );
	assert( b.getPhoneNumber( "name" ) == "phone" );
}


/*****************************************************************************/


DECLARE_OOP_TEST( addressbook_4_2_get_phone_number_by_empty_name )
{
	AddressBook b;
	b.addEntry( "name", "phone" );

	ASSERT_THROWS(
			b.getPhoneNumber( "" );
		,	Messages::EmptyName
	);
}


/*****************************************************************************/


DECLARE_OOP_TEST( addressbook_4_3_get_phone_number_by_missing_name )
{
	AddressBook b;
	b.addEntry( "name", "phone" );

	ASSERT_THROWS(
			b.getPhoneNumber( "missing" );
		,	Messages::EntryDoesNotExist
	);
}


/*****************************************************************************/


DECLARE_OOP_TEST( addressbook_5_1_remove_entry_one_entry )
{
	AddressBook b;
	b.addEntry( "name1", "phone1" );
	b.addEntry( "name2", "phone2" );
	b.removeEntry( "name1", "phone1" );

	assert( b.getEntriesCount() == 1 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( addressbook_5_2_remove_entry_several_entries )
{
	AddressBook b;
	b.addEntry( "name1", "phone1" );
	b.addEntry( "name2", "phone2" );
	b.removeEntry( "name1", "phone1" );
	b.removeEntry( "name2", "phone2" );

	assert( b.getEntriesCount() == 0 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( addressbook_5_3_remove_entry_by_empty_name )
{
	AddressBook b;
	b.addEntry( "name1", "phone1" );
	b.addEntry( "name2", "phone2" );

	ASSERT_THROWS(
			b.removeEntry( "", "phone1" )
		,	Messages::EmptyName
	);
}


/*****************************************************************************/


DECLARE_OOP_TEST( addressbook_5_4_remove_entry_by_empty_phone_number )
{
	AddressBook b;
	b.addEntry( "name1", "phone1" );
	b.addEntry( "name2", "phone2" );

	ASSERT_THROWS(
			b.removeEntry( "name1", "" )
		,	Messages::EmptyPhoneNumber
	);
}


/*****************************************************************************/


DECLARE_OOP_TEST( addressbook_5_5_remove_entry_by_name_and_missing_phone_number )
{
	AddressBook b;
	b.addEntry( "name1", "phone1" );
	b.addEntry( "name2", "phone2" );

	ASSERT_THROWS(
			b.removeEntry( "name1", "phone3" )
		,	Messages::EntryDoesNotExist
	);
}


/*****************************************************************************/


DECLARE_OOP_TEST( addressbook_5_6_remove_entry_by_phone_number_and_missing_number )
{
	AddressBook b;
	b.addEntry( "name1", "phone1" );
	b.addEntry( "name2", "phone2" );

	ASSERT_THROWS(
			b.removeEntry( "name3", "phone1" )
		,	Messages::EntryDoesNotExist
	);
}


/*****************************************************************************/


DECLARE_OOP_TEST( addressbook_5_7_remove_entry_by_missing_name_and_missing_phone_number )
{
	AddressBook b;
	b.addEntry( "name1", "phone1" );
	b.addEntry( "name2", "phone2" );

	ASSERT_THROWS(
			b.removeEntry( "name3", "phone3" )
		,	Messages::EntryDoesNotExist
	);
}


/*****************************************************************************/


DECLARE_OOP_TEST( addressbook_6_1_remove_entry_by_name_one_entry )
{
	AddressBook b;
	b.addEntry( "name1", "phone1" );
	b.addEntry( "name2", "phone2" );
	b.removeEntryByName( "name1" );

	assert( b.getEntriesCount() == 1 );
	assert( b.getName( "phone2" ) == "name2" );
}


/*****************************************************************************/


DECLARE_OOP_TEST( addressbook_6_2_remove_entry_by_name_several_entries )
{
	AddressBook b;
	b.addEntry( "name1", "phone1" );
	b.addEntry( "name2", "phone2" );
	b.removeEntryByName( "name1" );
	b.removeEntryByName( "name2" );

	assert( b.getEntriesCount() == 0 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( addressbook_6_3_remove_entry_by_empty_name )
{
	AddressBook b;
	b.addEntry( "name1", "phone1" );
	b.addEntry( "name2", "phone2" );

	ASSERT_THROWS(
			b.removeEntryByName( "" );
		,	Messages::EmptyName
	);
}


/*****************************************************************************/


DECLARE_OOP_TEST( addressbook_6_4_remove_entry_by_missing_name )
{
	AddressBook b;
	b.addEntry( "name1", "phone1" );
	b.addEntry( "name2", "phone2" );

	ASSERT_THROWS(
			b.removeEntryByName( "missing" );
		,	Messages::EntryDoesNotExist
	);
}


/*****************************************************************************/


DECLARE_OOP_TEST( addressbook_7_1_remove_entry_by_phone_number_one_entry )
{
	AddressBook b;
	b.addEntry( "name1", "phone1" );
	b.addEntry( "name2", "phone2" );
	b.removeEntryByPhoneNumber( "phone1" );

	assert( b.getEntriesCount() == 1 );
	assert( b.getName( "phone2" ) == "name2" );
}


/*****************************************************************************/


DECLARE_OOP_TEST( addressbook_7_2_remove_entry_by_phone_number_several_entries )
{
	AddressBook b;
	b.addEntry( "name1", "phone1" );
	b.addEntry( "name2", "phone2" );
	b.removeEntryByPhoneNumber( "phone1" );
	b.removeEntryByPhoneNumber( "phone2" );

	assert( b.getEntriesCount() == 0 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( addressbook_7_3_remove_entry_by_empty_phone_number )
{
	AddressBook b;
	b.addEntry( "name1", "phone1" );
	b.addEntry( "name2", "phone2" );

	ASSERT_THROWS(
			b.removeEntryByPhoneNumber( "" );
		,	Messages::EmptyPhoneNumber
	);
}


/*****************************************************************************/


DECLARE_OOP_TEST( addressbook_7_4_remove_entry_by_missing_phone_number )
{
	AddressBook b;
	b.addEntry( "name1", "phone1" );
	b.addEntry( "name2", "phone2" );

	ASSERT_THROWS(
			b.removeEntryByPhoneNumber( "missing" );
		,	Messages::EntryDoesNotExist
	);
}


/*****************************************************************************/


DECLARE_OOP_TEST( addressbook_8_1_has_entry_by_name_by_simple_name )
{
	AddressBook b;
	b.addEntry( "name", "phone" );

	assert( b.hasEntryByName( "name" ) );
}


/*****************************************************************************/


DECLARE_OOP_TEST( addressbook_8_2_has_entry_by_name_by_empty_name )
{
	AddressBook b;
	b.addEntry( "name", "phone" );

	assert( !b.hasEntryByName( "" ) );
}


/*****************************************************************************/


DECLARE_OOP_TEST( addressbook_8_3_has_entry_by_name_by_missing_name )
{
	AddressBook b;
	b.addEntry( "name", "phone" );

	assert( !b.hasEntryByName( "missing" ) );
}


/*****************************************************************************/


DECLARE_OOP_TEST( addressbook_9_1_has_entry_by_phone_number_by_simple_name )
{
	AddressBook b;
	b.addEntry( "name", "phone" );

	assert( b.hasEntryByPhoneNumber( "phone" ) );
}


/*****************************************************************************/


DECLARE_OOP_TEST( addressbook_9_2_has_entry_by_phone_number_by_empty_name )
{
	AddressBook b;
	b.addEntry( "name", "phone" );

	assert( !b.hasEntryByPhoneNumber( "" ) );
}


/*****************************************************************************/


DECLARE_OOP_TEST( addressbook_9_3_has_entry_by_phone_number_by_missing_name )
{
	AddressBook b;
	b.addEntry( "name", "phone" );

	assert( !b.hasEntryByPhoneNumber( "missing" ) );
}


/*****************************************************************************/


DECLARE_OOP_TEST( addressbook_10_1_get_entries_count_by_name_empty_book )
{
	AddressBook b;

	assert( b.getEntriesCountByName( "" ) == 0 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( addressbook_10_2_get_entries_count_by_name_filter_is_empty )
{
	AddressBook b;
	b.addEntry( "aname", "phone" );
	b.addEntry( "amira", "009900" );
	b.addEntry( "amike", "unknown" );
	b.addEntry( "akitty", "askbatman" );

	assert( b.getEntriesCountByName( "" ) == 4 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( addressbook_10_3_get_entries_count_by_name_filter_matches_several_names )
{
	AddressBook b;
	b.addEntry( "aname", "phone" );
	b.addEntry( "amira", "009900" );
	b.addEntry( "amike", "unknown" );
	b.addEntry( "akitty", "askbatman" );

	assert( b.getEntriesCountByName( "ami" ) == 2 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( addressbook_10_4_get_entries_count_by_name_filter_matches_all_names )
{
	AddressBook b;
	b.addEntry( "aname", "phone" );
	b.addEntry( "amira", "009900" );
	b.addEntry( "amike", "unknown" );
	b.addEntry( "akitty", "askbatman" );

	assert( b.getEntriesCountByName( "a" ) == 4 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( addressbook_10_5_get_entries_count_by_name_filter_matches_one_name )
{
	AddressBook b;
	b.addEntry( "aname", "phone" );
	b.addEntry( "amira", "009900" );
	b.addEntry( "amike", "unknown" );
	b.addEntry( "akitty", "askbatman" );

	assert( b.getEntriesCountByName( "amik" ) == 1 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( addressbook_10_6_get_entries_count_by_name_filterdoes_not_match_any_name )
{
	AddressBook b;
	b.addEntry( "aname", "phone" );
	b.addEntry( "amira", "009900" );
	b.addEntry( "amike", "unknown" );
	b.addEntry( "akitty", "askbatman" );

	assert( b.getEntriesCountByName( "as" ) == 0 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( addressbook_11_1_get_entries_count_by_phone_number_empty_book )
{
	AddressBook b;

	assert( b.getEntriesCountByPhoneNumber( "" ) == 0 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( addressbook_11_2_get_entries_count_by_phone_number_filter_is_empty )
{
	AddressBook b;
	b.addEntry( "aname", "0105569" );
	b.addEntry( "amira", "0204567456" );
	b.addEntry( "amike", "018876" );
	b.addEntry( "akitty", "02052" );

	assert( b.getEntriesCountByPhoneNumber( "" ) == 4 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( addressbook_11_3_get_entries_count_by_phone_number_filter_matches_several_phone_numbers )
{
	AddressBook b;
	b.addEntry( "aname", "0105569" );
	b.addEntry( "amira", "0204567456" );
	b.addEntry( "amike", "018876" );
	b.addEntry( "akitty", "02052" );

	assert( b.getEntriesCountByPhoneNumber( "02" ) == 2 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( addressbook_11_4_get_entries_count_by_phone_number_filter_matches_all_phone_numbers )
{
	AddressBook b;
	b.addEntry( "aname", "0105569" );
	b.addEntry( "amira", "0204567456" );
	b.addEntry( "amike", "018876" );
	b.addEntry( "akitty", "02052" );

	assert( b.getEntriesCountByPhoneNumber( "0" ) == 4 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( addressbook_11_5_get_entries_count_by_phone_number_filter_matches_one_phone_number )
{
	AddressBook b;
	b.addEntry( "aname", "0105569" );
	b.addEntry( "amira", "0204567456" );
	b.addEntry( "amike", "018876" );
	b.addEntry( "akitty", "02052" );

	assert( b.getEntriesCountByPhoneNumber( "0205" ) == 1 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( addressbook_11_6_get_entries_count_by_phone_number_filterdoes_not_match_any_phone_number )
{
	AddressBook b;
	b.addEntry( "aname", "0105569" );
	b.addEntry( "amira", "0204567456" );
	b.addEntry( "amike", "018876" );
	b.addEntry( "akitty", "02052" );

	assert( b.getEntriesCountByPhoneNumber( "03" ) == 0 );
}


/*****************************************************************************/


DECLARE_OOP_TEST( addressbook_12_1_create_ordered_book_empty_book )
{
	AddressBook b;

	std::vector< std::string > expectation = {};
	assert( b.createOrdered() == expectation );
}


/*****************************************************************************/


DECLARE_OOP_TEST( addressbook_12_2_create_ordered_book_only_1_entry )
{
	AddressBook b;
	b.addEntry( "name", "phone" );

	std::vector< std::string > expectation = { "name" };
	assert( b.createOrdered() == expectation );
}


/*****************************************************************************/


DECLARE_OOP_TEST( addressbook_12_3_create_ordered_book_only_several_entries )
{
	AddressBook b;
	b.addEntry( "a", "000" );
	b.addEntry( "d", "003" );
	b.addEntry( "f", "005" );
	b.addEntry( "b", "001" );
	b.addEntry( "c", "002" );
	b.addEntry( "e", "004" );

	std::vector< std::string > expectation = { "a", "b", "c", "d", "e", "f" };
	assert( b.createOrdered() == expectation );
}


/*****************************************************************************/


DECLARE_OOP_TEST( addressbook_13_1_print_to_output_stream_empty_book )
{
	AddressBook b;

	std::stringstream stream;
	stream << b;

	assert( stream.str() == "" );
}


/*****************************************************************************/


DECLARE_OOP_TEST( addressbook_13_2_print_to_output_stream_several_entries )
{
	AddressBook b;
	b.addEntry( "a", "000" );
	b.addEntry( "d", "003" );
	b.addEntry( "f", "005" );
	b.addEntry( "b", "001" );
	b.addEntry( "c", "002" );
	b.addEntry( "e", "004" );

	std::stringstream stream;
	stream << b;

	assert(
			stream.str()
		==	"a - 000\n"
			"b - 001\n"
			"c - 002\n"
			"d - 003\n"
			"e - 004\n"
			"f - 005\n"
	);
}


/*****************************************************************************/


DECLARE_OOP_TEST( addressbook_14_1_complex_cases_add_entry_with_the_same_name_but_different_phone_numbers )
{
	AddressBook b;
	b.addEntry( "name", "phone1" );
	b.addEntry( "name", "phone2" );
	b.addEntry( "name", "phone3" );

	assert( b.getEntriesCount() == 1 );
	assert( b.getPhoneNumber( "name" ) == "phone3" );
}


/*****************************************************************************/


DECLARE_OOP_TEST( addressbook_14_2_complex_cases_add_entry_with_the_same_phone_number_but_different_names )
{
	AddressBook b;
	b.addEntry( "name1", "phone" );
	b.addEntry( "name2", "phone" );
	b.addEntry( "name3", "phone" );

	assert( b.getEntriesCount() == 1 );
	assert( b.getName( "phone" ) == "name3" );
}


/*****************************************************************************/


DECLARE_OOP_TEST( addressbook_14_3_complex_cases_add_entry_with_name_and_phone_number_from_other_entries )
{
	AddressBook b;
	b.addEntry( "name1", "phone1" );
	b.addEntry( "name2", "phone2" );
	b.addEntry( "name3", "phone3" );

	b.addEntry( "name1", "phone3" );

	assert( b.getEntriesCount() == 2 );
	assert( b.getName( "phone2" ) == "name2" );
	assert( b.getName( "phone3" ) == "name1" );
}


/*****************************************************************************/
