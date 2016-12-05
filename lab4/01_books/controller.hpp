// (C) 2013-2016, Sergei Zaychenko, KNURE, Kharkiv, Ukraine

#ifndef _CONTROLLER_HPP_
#define _CONTROLLER_HPP_

/*****************************************************************************/

#include <string>
#include <vector>

/*****************************************************************************/


class Controller
{

/*-----------------------------------------------------------------*/

public:

/*-----------------------------------------------------------------*/

	Controller ();

	Controller ( const Controller & ) = delete;

	Controller & operator = ( const Controller & ) = delete;

	~ Controller ();

/*-----------------------------------------------------------------*/

	struct BookKey
	{
		std::string const m_title;
		const int m_edition;

		BookKey ( std::string const & _title, int _edition = 1 )
			:	m_title( _title ), m_edition( _edition ) 
		{}

		bool operator < ( const BookKey & _key ) const
		{
			if ( m_title < _key.m_title )
				return true;

			else if ( m_title > _key.m_title )
				return false;

			else
				return m_edition < _key.m_edition;
		}

		bool operator == ( const BookKey & _key ) const
		{
			return	m_title == _key.m_title
				&&	m_edition == _key.m_edition
			;
		}
	};

	void addOriginalBook (
			BookKey const & _key
		,	std::initializer_list< std::string > _authors
		,	const char * _language
		,	int _publicationYear
	);

	void addTranslation (
			std::string const & _translatedTitle
		,	BookKey const & _originalKey
		,	const char * _translatedLanguage
		,	int _translationYear
	);

	void addRevised (
			BookKey const & _revisedKey
		,	BookKey const & _originalKey
		,	int _revisionYear
	);

	const char * getBookLanguage ( BookKey const & _key ) const;

	int getBookPublicationYear ( BookKey const & _key ) const;

	std::vector< std::string > getBookAuthors ( BookKey const & _key ) const;

	std::vector< BookKey > findBookEditions ( std::string const & _title ) const;

	int getBookLatestEdition ( std::string const & _title );

	std::vector< BookKey > findBookTranslations ( BookKey const & _key ) const;

	BookKey getOriginalBook ( BookKey const & _derivedBook ) const;

/*-----------------------------------------------------------------*/

private:

/*-----------------------------------------------------------------*/

	// TODO
	
/*-----------------------------------------------------------------*/

};


/*****************************************************************************/

#endif // _CONTROLLER_HPP_
