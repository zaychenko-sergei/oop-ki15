// (C) 2013-2016, Sergei Zaychenko, KNURE, Kharkiv, Ukraine

/*****************************************************************************/

#include "controller.hpp"
#include "messages.hpp"

#include "testslib.hpp"

/*****************************************************************************/

/*
	Test Plan:
	Done	1) Original book
	Done		1.1) Single
	Done		1.2) Several books
	Done		1.3) With empty title
	Done		1.4) With bad edition
	Done		1.5) With duplicated title and equal editions
	Done		1.6) With duplicated title and different editions
	Done		1.7) With different titles and equal editions
	Done		1.8) With empty authors list
	Done		1.9) With duplicated authors
	Done		1.10) With invalid language
	Done	2) Translated book
	Done		2.1) Single
	Done		2.2) Several books
	Done		2.3) With empty title
	Done		2.4) With duplicated title
	Done		2.5) With invalid language
	Done		2.6) Translation year is earlier than origin book
	Done		2.7) Translation to same language
	Done		2.8) For translated book
	Done		2.9) For revised book
	Done		2.10) Translation for each book edition
	Done		2.11) With empty original title
	Done		2.12) With missing original title
	Done		2.13) With bad original edition
	Done		2.14) With duplicated original title
	Done		2.15) New name matches other book title and edition
	Done		2.16) New name matches only other book title
	Done	3) Revised book
	Done		3.1) Single
	Done		3.2) Several books
	Done		3.3) With empty title
	Done		3.4) With bad revised edition
	Done		3.5) With revised edition earlier than original edition
	Done		3.6) For translated book
	Done		3.7) For revised book
	Done		3.8) With empty original title
	Done		3.9) With missing original title
	Done		3.10) With bad original edition
	Done		3.11) New name matches other book title and edition
	Done		3.12) New name matches only other book title
	Done	4) Get book language
	Done		4.1) By missing title
	Done		4.2) By empty title
	Done		4.3) By missing edition
	Done		4.4) By bad edition
	Done	5) Get book publication year
	Done		5.1) By missing title
	Done		5.2) By empty title
	Done		5.3) By missing edition
	Done		5.4) By bad edition
	Done	6) Get book authors
	Done		6.1) By missing title
	Done		6.2) By empty title
	Done		6.3) By missing edition
	Done		6.4) By bad edition
	Done	7) Find book editions
	Done		7.1) Single edition
	Done		7.2) Several editions
	Done		7.3) By missing title
	Done		7.4) By empty title
	Done		7.5) Find translated book
	Done		7.6) Find revised book
	Done		7.7) Find translated book and not related original book
	Done		7.8) Find revised book and not related original book
	Done	8) Get book latest edition
	Done		8.1) Single edition
	Done		8.2) Several editions
	Done		8.3) By missing title
	Done		8.4) By empty title
	Done		8.5) Find translated book
	Done		8.6) Find revised book
	Done		8.7) Find translated book and not related original book
	Done		8.8) Find revised book and not related original book
	Done		8.9) Several editions added in different order
	Done	9) Find book translations
	Done		9.1) Single translation
	Done		9.2) Several translations
	Done		9.3) Without translations
	Done		9.4) By missing title
	Done		9.5) By empty title
	Done		9.6) By missing edition
	Done		9.7) Tranlations only for selected edition
	Done	10) Get original book
	Done		10.1) For translated book
	Done		10.2) For revised book
	Done		10.3) For original book
	Done		10.4) By missing title
	Done		10.5) By empty title
	Done		10.6) By missing edition
*/

/*****************************************************************************/

DECLARE_OOP_TEST( books_1_1_original_book_single )
{
	Controller c;
	Controller::BookKey key( "Windows 8 for Dummies" );
	c.addOriginalBook( key, { "A. Rathbone" }, "en", 2012 );

	assert( ! strcmp( c.getBookLanguage( key ), "en" ) );
	assert( c.getBookPublicationYear( key ) == 2012 );
	assert( c.getBookAuthors( key ) == std::vector< std::string >{ "A. Rathbone" } );
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_1_2_original_book_several_books )
{
	Controller c;
	Controller::BookKey key1( "Windows 8 for Dummies" );
	c.addOriginalBook( key1, { "A. Rathbone" }, "en", 2012 );

	Controller::BookKey key2( "How to train your dragon" );
	c.addOriginalBook( key2, { "Valerka", "Vitalka" }, "ua", 2000 );

	assert( ! strcmp( c.getBookLanguage( key1 ), "en" ) );
	assert( c.getBookPublicationYear( key1 ) == 2012 );
	assert( c.getBookAuthors( key1 ) == std::vector< std::string >{ "A. Rathbone" } );

	assert( ! strcmp( c.getBookLanguage( key2 ), "ua" ) );
	assert( c.getBookPublicationYear( key2 ) == 2000 );
	std::vector< std::string > authors2{ "Valerka", "Vitalka" };
	assert( c.getBookAuthors( key2 ) == authors2 );
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_1_3_original_book_with_empty_title )
{
	Controller c;
	Controller::BookKey key( "" );

	ASSERT_THROWS(
			c.addOriginalBook( key, { "A. Rathbone" }, "en", 2012 );
		,	Messages::EmptyTitle
	);
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_1_4_original_book_with_bad_edition )
{
	Controller c;

	Controller::BookKey key1( "Windows 8 for Dummies", 0 );
	ASSERT_THROWS(
			c.addOriginalBook( key1, { "A. Rathbone" }, "en", 2012 );
		,	Messages::NonPositiveEdition
	);
	Controller::BookKey key2( "Windows 8 for Dummies", -123 );
	ASSERT_THROWS(
			c.addOriginalBook( key2, { "A. Rathbone" }, "en", 2012 );
		,	Messages::NonPositiveEdition
	);
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_1_5_original_book_with_duplcated_title_and_equal_editions )
{
	Controller c;
	Controller::BookKey key( "Windows 8 for Dummies", 10 );
	c.addOriginalBook( key, { "A. Rathbone" }, "en", 2012 );

	ASSERT_THROWS(
			c.addOriginalBook( key, { "B. Rathbone" }, "uk", 2013 );
		,	Messages::NonUniqueTitle
	);
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_1_6_original_book_with_duplicated_title_and_different_editions )
{
	Controller c;
	Controller::BookKey key1( "Windows 8 for Dummies", 20 );
	c.addOriginalBook( key1, { "A. Rathbone" }, "en", 2012 );
	Controller::BookKey key2( "Windows 8 for Dummies", 40 );
	c.addOriginalBook( key2, { "C. Rathbone" }, "de", 2014 );

	assert( ! strcmp( c.getBookLanguage( key1 ), "en" ) );
	assert( c.getBookPublicationYear( key1 ) == 2012 );
	assert( c.getBookAuthors( key1 ) == std::vector< std::string >{ "A. Rathbone" } );

	assert( ! strcmp( c.getBookLanguage( key2 ), "de" ) );
	assert( c.getBookPublicationYear( key2 ) == 2014 );
	assert( c.getBookAuthors( key2 ) == std::vector< std::string >{ "C. Rathbone" } );
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_1_7_original_book_with_different_titles_and_equal_editions )
{
	Controller c;
	Controller::BookKey key1( "Windows 8 for Dummies", 20 );
	c.addOriginalBook( key1, { "A. Rathbone" }, "en", 2012 );
	Controller::BookKey key2( "Windows 9 for Dummies", 20 );
	c.addOriginalBook( key2, { "C. Rathbone" }, "de", 2014 );

	assert( ! strcmp( c.getBookLanguage( key1 ), "en" ) );
	assert( c.getBookPublicationYear( key1 ) == 2012 );
	assert( c.getBookAuthors( key1 ) == std::vector< std::string >{ "A. Rathbone" } );

	assert( ! strcmp( c.getBookLanguage( key2 ), "de" ) );
	assert( c.getBookPublicationYear( key2 ) == 2014 );
	assert( c.getBookAuthors( key2 ) == std::vector< std::string >{ "C. Rathbone" } );
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_1_8_original_book_with_empty_authors_list )
{
	Controller c;
	Controller::BookKey key( "Windows 8 for Dummies", 10 );

	ASSERT_THROWS(
			c.addOriginalBook( key, {}, "en", 2012 );
		,	Messages::EmptyAuthorsList
	);
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_1_9_original_book_with_duplicated_authors )
{
	Controller c;
	Controller::BookKey key( "Windows 8 for Dummies", 10 );

	ASSERT_THROWS(
			c.addOriginalBook( key, { "a", "a" }, "en", 2012 );
		,	Messages::DuplicateAuthor
	);
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_1_10_original_book_with_invalid_language )
{
	Controller c;
	Controller::BookKey key( "Windows 8 for Dummies", 10 );

	ASSERT_THROWS(
			c.addOriginalBook( key, { "Hennesy" }, "", 2012 );
		,	Messages::LanguageCodeNot2Chars
	);
	ASSERT_THROWS(
			c.addOriginalBook( key, { "Hennesy" }, "e", 2012 );
		,	Messages::LanguageCodeNot2Chars
	);
	ASSERT_THROWS(
			c.addOriginalBook( key, { "Hennesy" }, "eng", 2012 );
		,	Messages::LanguageCodeNot2Chars
	);
	ASSERT_THROWS(
			c.addOriginalBook( key, { "Hennesy" }, "very long language", 2012 );
		,	Messages::LanguageCodeNot2Chars
	);
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_2_1_translated_book_single )
{
	Controller c;
	Controller::BookKey key( "Windows 8 for Dummies" );
	c.addOriginalBook( key, { "A. Rathbone" }, "en", 2012 );
	c.addTranslation( "L for Linux", key, "jp", 2012 );
	
	assert( ! strcmp( c.getBookLanguage( key ), "en" ) );
	assert( c.getBookPublicationYear( key ) == 2012 );
	assert( c.getBookAuthors( key ) == std::vector< std::string >{ "A. Rathbone" } );

	Controller::BookKey translatedKey( "L for Linux" );
	assert( ! strcmp( c.getBookLanguage( translatedKey ), "jp" ) );
	assert( c.getBookPublicationYear( translatedKey ) == 2012 );
	assert( c.getBookAuthors( translatedKey ) == std::vector< std::string >{ "A. Rathbone" } );
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_2_2_translated_book_several_books )
{
	Controller c;
	Controller::BookKey key( "Windows 8 for Dummies" );
	c.addOriginalBook( key, { "A. Rathbone" }, "en", 2012 );
	c.addTranslation( "L for Linux #1", key, "jp", 2012 );
	c.addTranslation( "L for Linux #2", key, "ko", 2013 );

	assert( ! strcmp( c.getBookLanguage( key ), "en" ) );
	assert( c.getBookPublicationYear( key ) == 2012 );
	assert( c.getBookAuthors( key ) == std::vector< std::string >{ "A. Rathbone" } );

	Controller::BookKey translatedKey1( "L for Linux #1" );
	assert( ! strcmp( c.getBookLanguage( translatedKey1 ), "jp" ) );
	assert( c.getBookPublicationYear( translatedKey1 ) == 2012 );
	assert( c.getBookAuthors( translatedKey1 ) == std::vector< std::string >{ "A. Rathbone" } );

	Controller::BookKey translatedKey2( "L for Linux #2" );
	assert( ! strcmp( c.getBookLanguage( translatedKey2 ), "ko" ) );
	assert( c.getBookPublicationYear( translatedKey2 ) == 2013 );
	assert( c.getBookAuthors( translatedKey2 ) == std::vector< std::string >{ "A. Rathbone" } );
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_2_3_translated_book_with_empty_title )
{
	Controller c;
	Controller::BookKey key( "Windows 8 for Dummies" );
	c.addOriginalBook( key, { "A. Rathbone" }, "en", 2012 );

	ASSERT_THROWS(
			c.addTranslation( "", key, "jp", 2012 );
		,	Messages::EmptyTitle
	);
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_2_4_translated_book_with_duplicated_title )
{
	Controller c;
	Controller::BookKey key( "Windows 8 for Dummies" );
	c.addOriginalBook( key, { "A. Rathbone" }, "en", 2012 );
	c.addTranslation( "L for Linux", key, "jp", 2012 );

	ASSERT_THROWS(
			c.addTranslation( "L for Linux", key, "jp", 2012 );
		,	Messages::NonUniqueTitle
	);
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_2_5_translated_book_with_invalid_language )
{
	Controller c;
	Controller::BookKey key( "Windows 8 for Dummies" );
	c.addOriginalBook( key, { "A. Rathbone" }, "en", 2012 );

	ASSERT_THROWS(
			c.addTranslation( "L for Linux", key, "", 2012 );
		,	Messages::LanguageCodeNot2Chars
	);
	ASSERT_THROWS(
			c.addTranslation( "L for Linux", key, "e", 2012 );
		,	Messages::LanguageCodeNot2Chars
	);
	ASSERT_THROWS(
			c.addTranslation( "L for Linux", key, "eng", 2012 );
		,	Messages::LanguageCodeNot2Chars
	);
	ASSERT_THROWS(
			c.addTranslation( "L for Linux", key, "very long language", 2012 );
		,	Messages::LanguageCodeNot2Chars
	);
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_2_6_translated_book_translation_year_is_earlier_than_origin_book )
{
	Controller c;
	Controller::BookKey key( "Windows 8 for Dummies" );
	c.addOriginalBook( key, { "A. Rathbone" }, "en", 2012 );

	ASSERT_THROWS(
			c.addTranslation( "L for Linux", key, "jp", 2000 );
		,	Messages::DerivedBeforeOriginal
	);
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_2_7_translated_book_translation_to_same_language )
{
	Controller c;
	Controller::BookKey key( "Windows 8 for Dummies" );
	c.addOriginalBook( key, { "A. Rathbone" }, "en", 2012 );

	ASSERT_THROWS(
			c.addTranslation( "L for Linux", key, "en", 2013 );
		,	Messages::TranslatedToSameLanguage
	);
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_2_8_translated_book_for_translated_book )
{
	Controller c;
	Controller::BookKey key( "Windows 8 for Dummies" );
	c.addOriginalBook( key, { "A. Rathbone" }, "en", 2012 );

	Controller::BookKey translatedKey1( "L for Linux" );
	c.addTranslation( "L for Linux", key, "jp", 2012 );
	
	Controller::BookKey translatedKey2( "Meraving" );
	c.addTranslation( "Meraving", translatedKey1, "de", 2013 );
	
	assert( ! strcmp( c.getBookLanguage( key ), "en" ) );
	assert( c.getBookPublicationYear( key ) == 2012 );
	assert( c.getBookAuthors( key ) == std::vector< std::string >{ "A. Rathbone" } );

	assert( ! strcmp( c.getBookLanguage( translatedKey1 ), "jp" ) );
	assert( c.getBookPublicationYear( translatedKey1 ) == 2012 );
	assert( c.getBookAuthors( translatedKey1 ) == std::vector< std::string >{ "A. Rathbone" } );

	assert( ! strcmp( c.getBookLanguage( translatedKey2 ), "de" ) );
	assert( c.getBookPublicationYear( translatedKey2 ) == 2013 );
	assert( c.getBookAuthors( translatedKey2 ) == std::vector< std::string >{ "A. Rathbone" } );
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_2_9_translated_book_for_revised_book )
{
	Controller c;
	Controller::BookKey key( "Windows 8 for Dummies", 1 );
	c.addOriginalBook( key, { "A. Rathbone" }, "en", 2012 );

	Controller::BookKey revisedKey( "L for Linux", 2 );
	c.addRevised( revisedKey, key, 2012 );

	Controller::BookKey translatedKey( "Meraving", 2 );
	c.addTranslation( "Meraving", revisedKey, "de", 2014 );
	
	assert( ! strcmp( c.getBookLanguage( key ), "en" ) );
	assert( c.getBookPublicationYear( key ) == 2012 );
	assert( c.getBookAuthors( key ) == std::vector< std::string >{ "A. Rathbone" } );

	assert( ! strcmp( c.getBookLanguage( revisedKey ), "en" ) );
	assert( c.getBookPublicationYear( revisedKey ) == 2012 );
	assert( c.getBookAuthors( revisedKey ) == std::vector< std::string >{ "A. Rathbone" } );

	assert( ! strcmp( c.getBookLanguage( translatedKey ), "de" ) );
	assert( c.getBookPublicationYear( translatedKey ) == 2014 );
	assert( c.getBookAuthors( translatedKey ) == std::vector< std::string >{ "A. Rathbone" } );
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_2_10_translated_book_translation_for_each_book_edition )
{
	Controller c;
	Controller::BookKey key1( "Windows 8 for Dummies", 12 );
	Controller::BookKey translatedKey1( "L for Linux #1", 12 );
	c.addOriginalBook( key1, { "A. Rathbone" }, "en", 2012 );
	c.addTranslation( "L for Linux #1", key1, "jp", 2013 );

	Controller::BookKey key2( "Windows 8 for Dummies", 13 );
	Controller::BookKey translatedKey2( "L for Linux #2", 13 );
	c.addOriginalBook( key2, { "A. Rathbone" }, "en", 2012 );
	c.addTranslation( "L for Linux #2", key2, "jk", 2014 );

	assert( ! strcmp( c.getBookLanguage( key1 ), "en" ) );
	assert( c.getBookPublicationYear( key1 ) == 2012 );
	assert( c.getBookAuthors( key1 ) == std::vector< std::string >{ "A. Rathbone" } );

	assert( ! strcmp( c.getBookLanguage( key2 ), "en" ) );
	assert( c.getBookPublicationYear( key2 ) == 2012 );
	assert( c.getBookAuthors( key2 ) == std::vector< std::string >{ "A. Rathbone" } );

	assert( ! strcmp( c.getBookLanguage( translatedKey1 ), "jp" ) );
	assert( c.getBookPublicationYear( translatedKey1 ) == 2013 );
	assert( c.getBookAuthors( translatedKey1 ) == std::vector< std::string >{ "A. Rathbone" } );

	assert( ! strcmp( c.getBookLanguage( translatedKey2 ), "jk" ) );
	assert( c.getBookPublicationYear( translatedKey2 ) == 2014 );
	assert( c.getBookAuthors( translatedKey2 ) == std::vector< std::string >{ "A. Rathbone" } );
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_2_11_translated_book_with_empty_original_title )
{
	Controller c;
	Controller::BookKey key( "Windows 8 for Dummies", 10 );
	c.addOriginalBook( key, { "A. Rathbone" }, "en", 2012 );

	ASSERT_THROWS(
			c.addTranslation( "L for Linux", { "", 10 }, "jp", 2012 );
		,	Messages::BookNotFound
	);
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_2_12_translated_book_with_missing_original_title )
{
	Controller c;
	Controller::BookKey key( "Windows 8 for Dummies", 10 );
	c.addOriginalBook( key, { "A. Rathbone" }, "en", 2012 );

	ASSERT_THROWS(
			c.addTranslation( "L for Linux", { "hello", 10 }, "jp", 2012 );
		,	Messages::BookNotFound
	);
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_2_13_translated_book_with_bad_original_edition )
{
	Controller c;
	Controller::BookKey key( "Windows 8 for Dummies", 10 );
	c.addOriginalBook( key, { "A. Rathbone" }, "en", 2012 );

	ASSERT_THROWS(
			c.addTranslation( "L for Linux", { "Windows 8 for Dummies", 11 }, "jp", 2012 );
		,	Messages::BookNotFound
	);
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_2_14_translated_book_with_duplicated_original_title )
{
	Controller c;
	Controller::BookKey key( "Windows 8 for Dummies" );
	c.addOriginalBook( key, { "A. Rathbone" }, "en", 2012 );

	ASSERT_THROWS(
			c.addTranslation( "Windows 8 for Dummies", key, "jp", 2012 );
		,	Messages::NonUniqueTitle
	);
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_2_15_translated_book_new_name_matches_other_book_title_and_edition )
{
	Controller c;
	Controller::BookKey key1( "book #1", 10 );
	Controller::BookKey key2( "book #2", 10 );
	c.addOriginalBook( key1, { "author #1" }, "en", 2012 );
	c.addOriginalBook( key2, { "author #2" }, "en", 2012 );

	ASSERT_THROWS(
			c.addTranslation( "book #2", key1, "en", 2012 );
		,	Messages::NonUniqueTitle
	);
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_2_16_translated_book_new_name_matches_only_other_book_title )
{
	Controller c;
	Controller::BookKey key1( "book #1", 10 );
	Controller::BookKey key2( "book #2", 20 );
	c.addOriginalBook( key1, { "author #1" }, "en", 2012 );
	c.addOriginalBook( key2, { "author #2" }, "en", 2012 );
	
	Controller::BookKey key3( "book #2", 10 );
	c.addTranslation( "book #2", key1, "sa", 2013 );

	assert( ! strcmp( c.getBookLanguage( key1 ), "en" ) );
	assert( c.getBookPublicationYear( key1 ) == 2012 );
	assert( c.getBookAuthors( key1 ) == std::vector< std::string >{ "author #1" } );

	assert( ! strcmp( c.getBookLanguage( key2 ), "en" ) );
	assert( c.getBookPublicationYear( key2 ) == 2012 );
	assert( c.getBookAuthors( key2 ) == std::vector< std::string >{ "author #2" } );

	assert( ! strcmp( c.getBookLanguage( key3 ), "sa" ) );
	assert( c.getBookPublicationYear( key3 ) == 2013 );
	assert( c.getBookAuthors( key3 ) == std::vector< std::string >{ "author #1" } );
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_3_1_revised_book_single )
{
	Controller c;
	Controller::BookKey key( "Windows 8 for Dummies", 2 );
	Controller::BookKey revisedKey( "Windows 8 for Dummies", 3 );
	c.addOriginalBook( key, { "A. Rathbone" }, "en", 2012 );
	c.addRevised( revisedKey, key, 2012 );
	
	assert( ! strcmp( c.getBookLanguage( key ), "en" ) );
	assert( c.getBookPublicationYear( key ) == 2012 );
	assert( c.getBookAuthors( key ) == std::vector< std::string >{ "A. Rathbone" } );

	assert( ! strcmp( c.getBookLanguage( revisedKey ), "en" ) );
	assert( c.getBookPublicationYear( revisedKey ) == 2012 );
	assert( c.getBookAuthors( revisedKey ) == std::vector< std::string >{ "A. Rathbone" } );
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_3_2_revised_book_several_books )
{
	Controller c;
	Controller::BookKey key( "Windows 8 for Dummies", 2 );
	Controller::BookKey revisedKey1( "Windows 8 for Dummies", 3 );
	Controller::BookKey revisedKey2( "Windows 8 for Dummies", 4 );
	c.addOriginalBook( key, { "A. Rathbone" }, "en", 2012 );
	c.addRevised( revisedKey1, key, 2012 );
	c.addRevised( revisedKey2, key, 2012 );
	
	assert( ! strcmp( c.getBookLanguage( key ), "en" ) );
	assert( c.getBookPublicationYear( key ) == 2012 );
	assert( c.getBookAuthors( key ) == std::vector< std::string >{ "A. Rathbone" } );

	assert( ! strcmp( c.getBookLanguage( revisedKey1 ), "en" ) );
	assert( c.getBookPublicationYear( revisedKey1 ) == 2012 );
	assert( c.getBookAuthors( revisedKey1 ) == std::vector< std::string >{ "A. Rathbone" } );

	assert( ! strcmp( c.getBookLanguage( revisedKey2 ), "en" ) );
	assert( c.getBookPublicationYear( revisedKey2 ) == 2012 );
	assert( c.getBookAuthors( revisedKey2 ) == std::vector< std::string >{ "A. Rathbone" } );
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_3_3_revised_book_with_empty_title )
{
	Controller c;
	Controller::BookKey key( "Windows 8 for Dummies", 2 );
	Controller::BookKey revisedKey( "", 3 );
	c.addOriginalBook( key, { "A. Rathbone" }, "en", 2012 );

	ASSERT_THROWS(
			c.addRevised( revisedKey, key, 2012 );
		,	Messages::EmptyTitle
	);
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_3_4_revised_book_with_bad_revised_edition )
{
	Controller c;
	Controller::BookKey key( "Windows 8 for Dummies", 1 );
	c.addOriginalBook( key, { "A. Rathbone" }, "en", 2012 );

	Controller::BookKey revisedKey1( "Windows 8 for Dummies", 0 );
	ASSERT_THROWS(
			c.addRevised( revisedKey1, key, 2012 );
		,	Messages::RevisedEditionLessEqu
	);
	Controller::BookKey revisedKey2( "Windows 8 for Dummies", -100 );
	ASSERT_THROWS(
			c.addRevised( revisedKey2, key, 2012 );
		,	Messages::RevisedEditionLessEqu
	);
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_3_5_revised_book_with_revised_edition_earlier_than_original_edition )
{
	Controller c;
	Controller::BookKey key( "Windows 8 for Dummies", 3 );
	c.addOriginalBook( key, { "A. Rathbone" }, "en", 2012 );

	Controller::BookKey revisedKey( "Windows 8 for Dummies", 2 );
	ASSERT_THROWS(
			c.addRevised( revisedKey, key, 2012 );
		,	Messages::RevisedEditionLessEqu
	);
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_3_6_revised_book_for_translated_book )
{
	Controller c;
	Controller::BookKey key( "Windows 8 for Dummies", 5 );
	c.addOriginalBook( key, { "A. Rathbone" }, "en", 2012 );
	c.addTranslation( "New title", key, "ua", 2017 );

	Controller::BookKey translatedKey( "New title", 5 );
	Controller::BookKey revisedKey( "New title", 6 );
	c.addRevised( revisedKey, translatedKey, 2018 );

	assert( ! strcmp( c.getBookLanguage( key ), "en" ) );
	assert( c.getBookPublicationYear( key ) == 2012 );
	assert( c.getBookAuthors( key ) == std::vector< std::string >{ "A. Rathbone" } );

	assert( ! strcmp( c.getBookLanguage( translatedKey ), "ua" ) );
	assert( c.getBookPublicationYear( translatedKey ) == 2017 );
	assert( c.getBookAuthors( translatedKey ) == std::vector< std::string >{ "A. Rathbone" } );

	assert( ! strcmp( c.getBookLanguage( revisedKey ), "ua" ) );
	assert( c.getBookPublicationYear( revisedKey ) == 2018 );
	assert( c.getBookAuthors( revisedKey ) == std::vector< std::string >{ "A. Rathbone" } );
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_3_7_revised_book_for_revised_book )
{
	Controller c;
	Controller::BookKey key( "Windows 8 for Dummies", 5 );
	c.addOriginalBook( key, { "A. Rathbone" }, "en", 2012 );

	Controller::BookKey revisedKey1( "New title", 6 );
	c.addRevised( revisedKey1, key, 2017 );

	Controller::BookKey revisedKey2( "New title", 7 );
	c.addRevised( revisedKey2, revisedKey1, 2018 );

	assert( ! strcmp( c.getBookLanguage( key ), "en" ) );
	assert( c.getBookPublicationYear( key ) == 2012 );
	assert( c.getBookAuthors( key ) == std::vector< std::string >{ "A. Rathbone" } );

	assert( ! strcmp( c.getBookLanguage( revisedKey1 ), "en" ) );
	assert( c.getBookPublicationYear( revisedKey1 ) == 2017 );
	assert( c.getBookAuthors( revisedKey1 ) == std::vector< std::string >{ "A. Rathbone" } );

	assert( ! strcmp( c.getBookLanguage( revisedKey2 ), "en" ) );
	assert( c.getBookPublicationYear( revisedKey2 ) == 2018 );
	assert( c.getBookAuthors( revisedKey2 ) == std::vector< std::string >{ "A. Rathbone" } );
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_3_8_revised_book_with_empty_original_title )
{
	Controller c;
	Controller::BookKey key( "Windows 8 for Dummies", 3 );
	c.addOriginalBook( key, { "A. Rathbone" }, "en", 2012 );

	ASSERT_THROWS(
			c.addRevised( { "Windows 8 for Dummies", 5 }, { "", 3 }, 2013 );
		,	Messages::BookNotFound
	);
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_3_9_revised_book_with_missing_original_title )
{
	Controller c;
	Controller::BookKey key( "Windows 8 for Dummies", 3 );
	c.addOriginalBook( key, { "A. Rathbone" }, "en", 2012 );

	ASSERT_THROWS(
			c.addRevised( { "Windows 8 for Dummies", 5 }, { "missing", 3 }, 2013 );
		,	Messages::BookNotFound
	);
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_3_10_revised_book_with_bad_original_edition )
{
	Controller c;
	Controller::BookKey key( "Windows 8 for Dummies", 3 );
	c.addOriginalBook( key, { "A. Rathbone" }, "en", 2012 );

	ASSERT_THROWS(
			c.addRevised( { "Windows 8 for Dummies", 5 }, { "Windows 8 for Dummies", 0 }, 2013 );
		,	Messages::BookNotFound
	);
	ASSERT_THROWS(
			c.addRevised( { "Windows 8 for Dummies", 5 }, { "Windows 8 for Dummies", -100 }, 2013 );
		,	Messages::BookNotFound
	);
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_3_11_revised_book_new_name_matches_other_book_title_and_edition )
{
	Controller c;
	Controller::BookKey key1( "book #1", 10 );
	Controller::BookKey key2( "book #2", 10 );
	c.addOriginalBook( key1, { "author #1" }, "en", 2012 );
	c.addOriginalBook( key2, { "author #2" }, "en", 2012 );

	Controller::BookKey key3( "book #2", 10 );
	ASSERT_THROWS(
			c.addRevised( key3, key1, 2013 );
		,	Messages::NonUniqueTitle
	);
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_3_12_revised_book_new_name_matches_only_other_book_title )
{
	Controller c;
	Controller::BookKey key1( "book #1", 10 );
	Controller::BookKey key2( "book #2", 20 );
	c.addOriginalBook( key1, { "author #1" }, "en", 2012 );
	c.addOriginalBook( key2, { "author #2" }, "en", 2012 );
	
	Controller::BookKey key3( "book #2", 11 );
	c.addRevised( key3, key1, 2013 );

	assert( ! strcmp( c.getBookLanguage( key1 ), "en" ) );
	assert( c.getBookPublicationYear( key1 ) == 2012 );
	assert( c.getBookAuthors( key1 ) == std::vector< std::string >{ "author #1" } );

	assert( ! strcmp( c.getBookLanguage( key2 ), "en" ) );
	assert( c.getBookPublicationYear( key2 ) == 2012 );
	assert( c.getBookAuthors( key2 ) == std::vector< std::string >{ "author #2" } );

	assert( ! strcmp( c.getBookLanguage( key3 ), "en" ) );
	assert( c.getBookPublicationYear( key3 ) == 2013 );
	assert( c.getBookAuthors( key3 ) == std::vector< std::string >{ "author #1" } );
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_4_1_get_book_language_by_missing_title )
{
	Controller c;
	c.addOriginalBook( { "Windows 8 for Dummies", 1 }, { "A. Rathbone" }, "en", 2012 );
	c.addRevised( { "Windows 10", 2 }, { "Windows 8 for Dummies", 1 }, 2013 );
	c.addTranslation( "Linux", { "Windows 8 for Dummies", 1 }, "ok", 2014 );

	ASSERT_THROWS(
			c.getBookLanguage( { "missing" } );
		,	Messages::BookNotFound
	);
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_4_2_get_book_language_by_empty_title )
{
	Controller c;
	c.addOriginalBook( { "Windows 8 for Dummies", 1 }, { "A. Rathbone" }, "en", 2012 );
	c.addRevised( { "Windows 10", 2 }, { "Windows 8 for Dummies", 1 }, 2013 );
	c.addTranslation( "Linux", { "Windows 8 for Dummies", 1 }, "ok", 2014 );

	ASSERT_THROWS(
			c.getBookLanguage( { "" } );
		,	Messages::BookNotFound
	);
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_4_3_get_book_language_by_missing_edition )
{
	Controller c;
	c.addOriginalBook( { "Windows 8 for Dummies", 1 }, { "A. Rathbone" }, "en", 2012 );
	c.addRevised( { "Windows 10", 2 }, { "Windows 8 for Dummies", 1 }, 2013 );
	c.addTranslation( "Linux", { "Windows 8 for Dummies", 1 }, "ok", 2014 );

	ASSERT_THROWS(
			c.getBookLanguage( { "Windows 10", 3 } );
		,	Messages::BookNotFound
	);
	ASSERT_THROWS(
			c.getBookLanguage( { "Windows 8 for Dummies", 2 } );
		,	Messages::BookNotFound
	);
	ASSERT_THROWS(
			c.getBookLanguage( { "Linux", 2 } );
		,	Messages::BookNotFound
	);
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_4_4_get_book_language_by_bad_edition )
{
	Controller c;
	c.addOriginalBook( { "Windows 8 for Dummies", 1 }, { "A. Rathbone" }, "en", 2012 );
	c.addRevised( { "Windows 10", 2 }, { "Windows 8 for Dummies", 1 }, 2013 );
	c.addTranslation( "Linux", { "Windows 8 for Dummies", 1 }, "ok", 2014 );

	ASSERT_THROWS(
			c.getBookLanguage( { "Windows 10", 0 } );
		,	Messages::BookNotFound
	);
	ASSERT_THROWS(
			c.getBookLanguage( { "Windows 8 for Dummies", 0 } );
		,	Messages::BookNotFound
	);
	ASSERT_THROWS(
			c.getBookLanguage( { "Linux", 0 } );
		,	Messages::BookNotFound
	);

	ASSERT_THROWS(
			c.getBookLanguage( { "Windows 10", -55 } );
		,	Messages::BookNotFound
	);
	ASSERT_THROWS(
			c.getBookLanguage( { "Windows 8 for Dummies", -55 } );
		,	Messages::BookNotFound
	);
	ASSERT_THROWS(
			c.getBookLanguage( { "Linux", -55 } );
		,	Messages::BookNotFound
	);
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_5_1_get_book_publication_year_by_missing_title )
{
	Controller c;
	c.addOriginalBook( { "Windows 8 for Dummies", 1 }, { "A. Rathbone" }, "en", 2012 );
	c.addRevised( { "Windows 10", 2 }, { "Windows 8 for Dummies", 1 }, 2013 );
	c.addTranslation( "Linux", { "Windows 8 for Dummies", 1 }, "ok", 2014 );

	ASSERT_THROWS(
			c.getBookPublicationYear( { "missing" } );
		,	Messages::BookNotFound
	);
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_5_2_get_book_publication_year_by_empty_title )
{
	Controller c;
	c.addOriginalBook( { "Windows 8 for Dummies", 1 }, { "A. Rathbone" }, "en", 2012 );
	c.addRevised( { "Windows 10", 2 }, { "Windows 8 for Dummies", 1 }, 2013 );
	c.addTranslation( "Linux", { "Windows 8 for Dummies", 1 }, "ok", 2014 );

	ASSERT_THROWS(
			c.getBookPublicationYear( { "" } );
		,	Messages::BookNotFound
	);
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_5_3_get_book_publication_year_by_missing_edition )
{
	Controller c;
	c.addOriginalBook( { "Windows 8 for Dummies", 1 }, { "A. Rathbone" }, "en", 2012 );
	c.addRevised( { "Windows 10", 2 }, { "Windows 8 for Dummies", 1 }, 2013 );
	c.addTranslation( "Linux", { "Windows 8 for Dummies", 1 }, "ok", 2014 );

	ASSERT_THROWS(
			c.getBookPublicationYear( { "Windows 10", 3 } );
		,	Messages::BookNotFound
	);
	ASSERT_THROWS(
			c.getBookPublicationYear( { "Windows 8 for Dummies", 2 } );
		,	Messages::BookNotFound
	);
	ASSERT_THROWS(
			c.getBookPublicationYear( { "Linux", 2 } );
		,	Messages::BookNotFound
	);
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_5_4_get_book_publication_year_by_bad_edition )
{
	Controller c;
	c.addOriginalBook( { "Windows 8 for Dummies", 1 }, { "A. Rathbone" }, "en", 2012 );
	c.addRevised( { "Windows 10", 2 }, { "Windows 8 for Dummies", 1 }, 2013 );
	c.addTranslation( "Linux", { "Windows 8 for Dummies", 1 }, "ok", 2014 );

	ASSERT_THROWS(
			c.getBookPublicationYear( { "Windows 10", 0 } );
		,	Messages::BookNotFound
	);
	ASSERT_THROWS(
			c.getBookPublicationYear( { "Windows 8 for Dummies", 0 } );
		,	Messages::BookNotFound
	);
	ASSERT_THROWS(
			c.getBookPublicationYear( { "Linux", 0 } );
		,	Messages::BookNotFound
	);

	ASSERT_THROWS(
			c.getBookPublicationYear( { "Windows 10", -55 } );
		,	Messages::BookNotFound
	);
	ASSERT_THROWS(
			c.getBookPublicationYear( { "Windows 8 for Dummies", -55 } );
		,	Messages::BookNotFound
	);
	ASSERT_THROWS(
			c.getBookPublicationYear( { "Linux", -55 } );
		,	Messages::BookNotFound
	);
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_6_1_get_book_authors_by_missing_title )
{
	Controller c;
	c.addOriginalBook( { "Windows 8 for Dummies", 1 }, { "A. Rathbone" }, "en", 2012 );
	c.addRevised( { "Windows 10", 2 }, { "Windows 8 for Dummies", 1 }, 2013 );
	c.addTranslation( "Linux", { "Windows 8 for Dummies", 1 }, "ok", 2014 );

	ASSERT_THROWS(
			c.getBookAuthors( { "missing" } );
		,	Messages::BookNotFound
	);
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_6_2_get_book_authors_by_empty_title )
{
	Controller c;
	c.addOriginalBook( { "Windows 8 for Dummies", 1 }, { "A. Rathbone" }, "en", 2012 );
	c.addRevised( { "Windows 10", 2 }, { "Windows 8 for Dummies", 1 }, 2013 );
	c.addTranslation( "Linux", { "Windows 8 for Dummies", 1 }, "ok", 2014 );

	ASSERT_THROWS(
			c.getBookAuthors( { "" } );
		,	Messages::BookNotFound
	);
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_6_3_get_book_authors_by_missing_edition )
{
	Controller c;
	c.addOriginalBook( { "Windows 8 for Dummies", 1 }, { "A. Rathbone" }, "en", 2012 );
	c.addRevised( { "Windows 10", 2 }, { "Windows 8 for Dummies", 1 }, 2013 );
	c.addTranslation( "Linux", { "Windows 8 for Dummies", 1 }, "ok", 2014 );

	ASSERT_THROWS(
			c.getBookAuthors( { "Windows 10", 3 } );
		,	Messages::BookNotFound
	);
	ASSERT_THROWS(
			c.getBookAuthors( { "Windows 8 for Dummies", 2 } );
		,	Messages::BookNotFound
	);
	ASSERT_THROWS(
			c.getBookAuthors( { "Linux", 2 } );
		,	Messages::BookNotFound
	);
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_6_4_get_book_authors_by_bad_edition )
{
	Controller c;
	c.addOriginalBook( { "Windows 8 for Dummies", 1 }, { "A. Rathbone" }, "en", 2012 );
	c.addRevised( { "Windows 10", 2 }, { "Windows 8 for Dummies", 1 }, 2013 );
	c.addTranslation( "Linux", { "Windows 8 for Dummies", 1 }, "ok", 2014 );

	ASSERT_THROWS(
			c.getBookAuthors( { "Windows 10", 0 } );
		,	Messages::BookNotFound
	);
	ASSERT_THROWS(
			c.getBookAuthors( { "Windows 8 for Dummies", 0 } );
		,	Messages::BookNotFound
	);
	ASSERT_THROWS(
			c.getBookAuthors( { "Linux", 0 } );
		,	Messages::BookNotFound
	);

	ASSERT_THROWS(
			c.getBookAuthors( { "Windows 10", -55 } );
		,	Messages::BookNotFound
	);
	ASSERT_THROWS(
			c.getBookAuthors( { "Windows 8 for Dummies", -55 } );
		,	Messages::BookNotFound
	);
	ASSERT_THROWS(
			c.getBookAuthors( { "Linux", -55 } );
		,	Messages::BookNotFound
	);
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_7_1_find_book_editions_single_edition )
{
	Controller c;
	Controller::BookKey key( "Windows 8 for Dummies" );
	c.addOriginalBook( key, { "A. Rathbone" }, "en", 2012 );

	std::vector< Controller::BookKey > expected = { key };
	assert( c.findBookEditions( key.m_title ) == expected );
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_7_2_find_book_editions_several_editions )
{
	Controller c;
	Controller::BookKey key1( "Windows 8 for Dummies", 10 );
	Controller::BookKey key2( "Windows 8 for Dummies", 15 );
	Controller::BookKey key3( "Windows 8 for Dummies", 20 );
	Controller::BookKey key4( "Windows 8 for Dummies", 100 );
	Controller::BookKey key5( "Windows 8 for Dummies", 500 );
	c.addOriginalBook( key1, { "D. Rathbone" }, "le", 2012 );
	c.addOriginalBook( key2, { "E. Rathbone" }, "ld", 2012 );
	c.addOriginalBook( key3, { "C. Rathbone" }, "lc", 2012 );
	c.addOriginalBook( key4, { "A. Rathbone" }, "lb", 2012 );
	c.addOriginalBook( key5, { "B. Rathbone" }, "la", 2012 );

	std::vector< Controller::BookKey > expected = { key1, key2, key3, key4, key5 };
	assert( c.findBookEditions( key1.m_title ) == expected );
	assert( c.findBookEditions( key2.m_title ) == expected );
	assert( c.findBookEditions( key3.m_title ) == expected );
	assert( c.findBookEditions( key4.m_title ) == expected );
	assert( c.findBookEditions( key5.m_title ) == expected );
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_7_3_find_book_editions_by_missing_title )
{
	Controller c;
	Controller::BookKey key( "Windows 8 for Dummies" );
	c.addOriginalBook( key, { "A. Rathbone" }, "en", 2012 );

	ASSERT_THROWS(
			c.findBookEditions( "missing" )
		,	Messages::BookNotFound
	);
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_7_4_find_book_editions_by_empty_title )
{
	Controller c;
	Controller::BookKey key( "Windows 8 for Dummies" );
	c.addOriginalBook( key, { "A. Rathbone" }, "en", 2012 );

	ASSERT_THROWS(
			c.findBookEditions( "" )
		,	Messages::BookNotFound
	);
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_7_5_find_book_editions_find_translated_book )
{
	Controller c;
	Controller::BookKey key1( "Windows 8 for Dummies" );
	Controller::BookKey key2( "New title" );
	c.addOriginalBook( key1, { "A. Rathbone" }, "en", 2012 );
	c.addTranslation( "New title", key1, "de", 2012 );

	std::vector< Controller::BookKey > expected = { key2 };
	assert( c.findBookEditions( "New title" ) == expected );
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_7_6_find_book_editions_find_revised_book )
{
	Controller c;
	Controller::BookKey key1( "Windows 8 for Dummies", 10 );
	Controller::BookKey key2( "New title", 20 );
	c.addOriginalBook( key1, { "A. Rathbone" }, "en", 2012 );
	c.addRevised( key2, key1, 2012 );

	std::vector< Controller::BookKey > expected = { key2 };
	assert( c.findBookEditions( "New title" ) == expected );
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_7_7_find_book_editions_find_translated_book_and_not_related_original_book )
{
	Controller c;
	Controller::BookKey key1( "title #1", 10 );
	Controller::BookKey key2( "title #2", 20 );
	Controller::BookKey key3( "title #2", 10 );
	c.addOriginalBook( key1, { "author #1" }, "en", 2012 );
	c.addOriginalBook( key2, { "author #2" }, "en", 2012 );
	c.addTranslation( "title #2", key1, "de", 2012 );

	std::vector< Controller::BookKey > expected = { key2, key3 };
	assert( c.findBookEditions( "title #2" ) == expected );
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_7_8_find_book_editions_find_revised_book_and_not_related_original_book )
{
	Controller c;
	Controller::BookKey key1( "title #1", 10 );
	Controller::BookKey key2( "title #2", 20 );
	Controller::BookKey key3( "title #2", 11 );
	c.addOriginalBook( key1, { "author #1" }, "en", 2012 );
	c.addOriginalBook( key2, { "author #2" }, "en", 2012 );
	c.addRevised( key3, key1, 2012 );

	std::vector< Controller::BookKey > expected = { key2, key3 };
	assert( c.findBookEditions( "title #2" ) == expected );
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_8_1_find_book_latest_edition_single_edition )
{
	Controller c;
	Controller::BookKey key( "Windows 8 for Dummies" );
	c.addOriginalBook( key, { "A. Rathbone" }, "en", 2012 );

	assert( c.getBookLatestEdition( key.m_title ) == key.m_edition );
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_8_2_find_book_latest_edition_several_editions )
{
	Controller c;
	Controller::BookKey key1( "Windows 8 for Dummies", 10 );
	Controller::BookKey key2( "Windows 8 for Dummies", 15 );
	Controller::BookKey key3( "Windows 8 for Dummies", 20 );
	Controller::BookKey key4( "Windows 8 for Dummies", 100 );
	Controller::BookKey key5( "Windows 8 for Dummies", 500 );
	c.addOriginalBook( key1, { "D. Rathbone" }, "le", 2012 );
	c.addOriginalBook( key2, { "E. Rathbone" }, "ld", 2012 );
	c.addOriginalBook( key3, { "C. Rathbone" }, "lc", 2012 );
	c.addOriginalBook( key4, { "A. Rathbone" }, "lb", 2012 );
	c.addOriginalBook( key5, { "B. Rathbone" }, "la", 2012 );

	assert( c.getBookLatestEdition( key1.m_title ) == key5.m_edition );
	assert( c.getBookLatestEdition( key2.m_title ) == key5.m_edition );
	assert( c.getBookLatestEdition( key3.m_title ) == key5.m_edition );
	assert( c.getBookLatestEdition( key4.m_title ) == key5.m_edition );
	assert( c.getBookLatestEdition( key5.m_title ) == key5.m_edition );
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_8_3_find_book_latest_edition_by_missing_title )
{
	Controller c;
	Controller::BookKey key( "Windows 8 for Dummies" );
	c.addOriginalBook( key, { "A. Rathbone" }, "en", 2012 );

	ASSERT_THROWS(
			c.getBookLatestEdition( "missing" )
		,	Messages::BookNotFound
	);
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_8_4_find_book_latest_edition_by_empty_title )
{
	Controller c;
	Controller::BookKey key( "Windows 8 for Dummies" );
	c.addOriginalBook( key, { "A. Rathbone" }, "en", 2012 );

	ASSERT_THROWS(
			c.getBookLatestEdition( "" )
		,	Messages::BookNotFound
	);
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_8_5_find_book_latest_edition_find_translated_book )
{
	Controller c;
	Controller::BookKey key1( "Windows 8 for Dummies" );
	Controller::BookKey key2( "New title" );
	c.addOriginalBook( key1, { "A. Rathbone" }, "en", 2012 );
	c.addTranslation( "New title", key1, "de", 2012 );

	assert( c.getBookLatestEdition( "New title" ) == key1.m_edition );
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_8_6_find_book_latest_edition_find_revised_book )
{
	Controller c;
	Controller::BookKey key1( "Windows 8 for Dummies", 10 );
	Controller::BookKey key2( "New title", 20 );
	c.addOriginalBook( key1, { "A. Rathbone" }, "en", 2012 );
	c.addRevised( key2, key1, 2012 );

	assert( c.getBookLatestEdition( "New title" ) == key2.m_edition );
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_8_7_find_book_latest_edition_find_translated_book_and_not_related_original_book )
{
	Controller c;
	Controller::BookKey key1( "title #1", 10 );
	Controller::BookKey key2( "title #2", 20 );
	Controller::BookKey key3( "title #2", 10 );
	c.addOriginalBook( key1, { "author #1" }, "en", 2012 );
	c.addOriginalBook( key2, { "author #2" }, "en", 2012 );
	c.addTranslation( "title #2", key1, "de", 2012 );

	assert( c.getBookLatestEdition( "title #2" ) == key2.m_edition );
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_8_8_find_book_latest_edition_find_revised_book_and_not_related_original_book )
{
	Controller c;
	Controller::BookKey key1( "title #1", 10 );
	Controller::BookKey key2( "title #2", 20 );
	Controller::BookKey key3( "title #2", 11 );
	c.addOriginalBook( key1, { "author #1" }, "en", 2012 );
	c.addOriginalBook( key2, { "author #2" }, "en", 2012 );
	c.addRevised( key3, key1, 2012 );

	assert( c.getBookLatestEdition( "title #2" ) == key2.m_edition );
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_8_9_find_book_latest_edition_several_editions_added_in_different_order )
{
	Controller c;
	Controller::BookKey key1( "Windows 8 for Dummies", 500 );
	Controller::BookKey key2( "Windows 8 for Dummies", 100 );
	Controller::BookKey key3( "Windows 8 for Dummies", 700 );
	Controller::BookKey key4( "Windows 8 for Dummies", 200 );
	Controller::BookKey key5( "Windows 8 for Dummies", 450 );
	c.addOriginalBook( key1, { "D. Rathbone" }, "le", 2012 );
	c.addOriginalBook( key2, { "E. Rathbone" }, "ld", 2012 );
	c.addOriginalBook( key3, { "C. Rathbone" }, "lc", 2012 );
	c.addOriginalBook( key4, { "A. Rathbone" }, "lb", 2012 );
	c.addOriginalBook( key5, { "B. Rathbone" }, "la", 2012 );

	assert( c.getBookLatestEdition( key1.m_title ) == key3.m_edition );
	assert( c.getBookLatestEdition( key2.m_title ) == key3.m_edition );
	assert( c.getBookLatestEdition( key3.m_title ) == key3.m_edition );
	assert( c.getBookLatestEdition( key4.m_title ) == key3.m_edition );
	assert( c.getBookLatestEdition( key5.m_title ) == key3.m_edition );
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_9_1_find_book_translations_single_translation )
{
	Controller c;
	Controller::BookKey key1( "Windows 8 for Dummies" );
	Controller::BookKey key2( "Linux" );
	c.addOriginalBook( key1, { "A. Rathbone" }, "en", 2012 );
	c.addTranslation( "Linux", key1, "hu", 2014 );

	std::vector< Controller::BookKey > expected = { key2 };
	assert( c.findBookTranslations( key1 ) == expected );
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_9_2_find_book_translations_several_translations )
{
	Controller c;
	Controller::BookKey key1( "Windows 8 for Dummies" );
	Controller::BookKey key2( "Linux #1" );
	Controller::BookKey key3( "Linux #2" );
	Controller::BookKey key4( "Linux #3" );
	Controller::BookKey key5( "Linux #4" );
	c.addOriginalBook( key1, { "A. Rathbone" }, "en", 2012 );
	c.addTranslation( key2.m_title, key1, "hu", 2014 );
	c.addTranslation( key3.m_title, key1, "hu", 2014 );
	c.addTranslation( key4.m_title, key1, "hu", 2014 );
	c.addTranslation( key5.m_title, key1, "hu", 2014 );

	std::vector< Controller::BookKey > expected = { key2, key3, key4, key5 };
	assert( c.findBookTranslations( key1 ) == expected );
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_9_3_find_book_translations_without_translation )
{
	Controller c;
	Controller::BookKey key1( "Windows 8 for Dummies" );
	c.addOriginalBook( key1, { "A. Rathbone" }, "en", 2012 );

	std::vector< Controller::BookKey > expected = {};
	assert( c.findBookTranslations( key1 ) == expected );
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_9_4_find_book_translations_by_missing_title )
{
	Controller c;
	Controller::BookKey key1( "Windows 8 for Dummies" );
	c.addOriginalBook( key1, { "A. Rathbone" }, "en", 2012 );

	ASSERT_THROWS(
			c.findBookTranslations( { "missing" } )
		,	Messages::BookNotFound
	);
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_9_5_find_book_translations_by_empty_title )
{
	Controller c;
	Controller::BookKey key1( "Windows 8 for Dummies" );
	c.addOriginalBook( key1, { "A. Rathbone" }, "en", 2012 );

	ASSERT_THROWS(
			c.findBookTranslations( { "" } )
		,	Messages::BookNotFound
	);
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_9_6_find_book_translations_by_missing_edition )
{
	Controller c;
	Controller::BookKey key1( "Windows 8 for Dummies", 50 );
	c.addOriginalBook( key1, { "A. Rathbone" }, "en", 2012 );

	ASSERT_THROWS(
			c.findBookTranslations( { "Windows 8 for Dummies", 100 } )
		,	Messages::BookNotFound
	);
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_9_7_find_book_translations_translation_only_for_selected_edition )
{
	Controller c;
	Controller::BookKey key1( "Windows 8 for Dummies" );
	Controller::BookKey key2( "Windows 9 for Dummies" );
	Controller::BookKey key3( "Linux #1" );
	Controller::BookKey key4( "Linux #2" );
	Controller::BookKey key5( "Linux #3" );
	Controller::BookKey key6( "Linux #4" );
	c.addOriginalBook( key1, { "A. Rathbone" }, "en", 2012 );
	c.addOriginalBook( key2, { "A. Rathbone" }, "en", 2012 );
	c.addTranslation( key3.m_title, key1, "hu", 2014 );
	c.addTranslation( key4.m_title, key2, "hu", 2014 );
	c.addTranslation( key5.m_title, key1, "hu", 2014 );
	c.addTranslation( key6.m_title, key2, "hu", 2014 );

	std::vector< Controller::BookKey > expected1 = { key3, key5 };
	std::vector< Controller::BookKey > expected2 = { key4, key6 };
	assert( c.findBookTranslations( key1 ) == expected1 );
	assert( c.findBookTranslations( key2 ) == expected2 );
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_10_1_get_original_book_for_translated_book )
{
	Controller c;
	Controller::BookKey key1( "Windows 8 for Dummies" );
	Controller::BookKey key2( "Linux" );
	c.addOriginalBook( key1, { "A. Rathbone" }, "en", 2012 );
	c.addTranslation( "Linux", key1, "hu", 2014 );

	assert( c.getOriginalBook( key2 ) == key1 );
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_10_2_get_original_book_for_revised_book )
{
	Controller c;
	Controller::BookKey key1( "Windows 8 for Dummies", 10 );
	Controller::BookKey key2( "Linux", 20 );
	c.addOriginalBook( key1, { "A. Rathbone" }, "en", 2012 );
	c.addRevised( key2, key1, 2014 );

	assert( c.getOriginalBook( key2 ) == key1 );
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_10_3_get_original_book_for_original_book )
{
	Controller c;
	Controller::BookKey key1( "Windows 8 for Dummies", 10 );
	Controller::BookKey key2( "Linux", 20 );
	c.addOriginalBook( key1, { "A. Rathbone" }, "en", 2012 );
	c.addRevised( key2, key1, 2014 );

	ASSERT_THROWS(
			c.getOriginalBook( key1 )
		,	Messages::NotADerivedBook
	);
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_10_4_get_original_book_by_missing_title )
{
	Controller c;
	Controller::BookKey key1( "Windows 8 for Dummies", 10 );
	Controller::BookKey key2( "Linux", 20 );
	c.addOriginalBook( key1, { "A. Rathbone" }, "en", 2012 );
	c.addRevised( key2, key1, 2014 );

	ASSERT_THROWS(
			c.getOriginalBook( { "missing" } )
		,	Messages::BookNotFound
	);
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_10_5_get_original_book_by_empty_title )
{
	Controller c;
	Controller::BookKey key1( "Windows 8 for Dummies", 10 );
	Controller::BookKey key2( "Linux", 20 );
	c.addOriginalBook( key1, { "A. Rathbone" }, "en", 2012 );
	c.addRevised( key2, key1, 2014 );

	ASSERT_THROWS(
			c.getOriginalBook( { "" } )
		,	Messages::BookNotFound
	);
}

/*****************************************************************************/

DECLARE_OOP_TEST( books_10_6_get_original_book_by_missing_edition )
{
	Controller c;
	Controller::BookKey key1( "Windows 8 for Dummies", 10 );
	Controller::BookKey key2( "Linux", 20 );
	c.addOriginalBook( key1, { "A. Rathbone" }, "en", 2012 );
	c.addRevised( key2, key1, 2014 );

	ASSERT_THROWS(
			c.getOriginalBook( { "Windows 8 for Dummies", 100 } )
		,	Messages::BookNotFound
	);
}

/*****************************************************************************/
