// (C) 2013-2016, Sergei Zaychenko, KNURE, Kharkiv, Ukraine

#ifndef _MESSAGES_HPP_
#define _MESSAGES_HPP_

/*****************************************************************************/

namespace Messages
{

/*-----------------------------------------------------------------*/

	const char * const EmptyTitle               = "Book title is empty";
	const char * const NonUniqueTitle           = "Book title must be unique";
	const char * const LanguageCodeNot2Chars    = "Language code must consist exactly of 2 characters";
	const char * const EmptyAuthorsList         = "List of authors should not be empty";
	const char * const DuplicateAuthor			= "Same author was already specified for this book";
	const char * const NonPositiveEdition       = "Edition number must be positive";
	const char * const BookNotFound             = "Book with the specified title & edition was not previusly registered";
	const char * const DerivedBeforeOriginal    = "Derived book cannot be published earlier than original";
	const char * const TranslatedToSameLanguage = "Language of translation should be different from original language";
	const char * const RevisedEditionLessEqu    = "Revised edition number should be greated that original edition number";
	const char * const NotADerivedBook          = "The book is not derived (translated, revised) from any other book";

/*-----------------------------------------------------------------*/

};


/*****************************************************************************/


#endif // _MESSAGES_HPP_
