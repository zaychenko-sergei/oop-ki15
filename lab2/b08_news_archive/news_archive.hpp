// (C) 2016, Sergei Zaychenko, KNURE, Kharkiv, Ukraine

#ifndef _NEWS_ARCHIVE_HPP_
#define _NEWS_ARCHIVE_HPP_

/*****************************************************************************/

#include "datetime.hpp"

#include <string>
#include <functional>
#include <vector>

/*****************************************************************************/

class NewsArchive
{

/*-----------------------------------------------------------------*/

public:

/*-----------------------------------------------------------------*/

	typedef
		std::function< void ( DateTime, const std::string &, const std::string & ) >
		EntryHandler;

/*-----------------------------------------------------------------*/

	// TODO: public members

/*-----------------------------------------------------------------*/

private:

/*-----------------------------------------------------------------*/

	// TODO: private members

/*-----------------------------------------------------------------*/

};

/*****************************************************************************/

#endif // _NEWS_ARCHIVE_HPP_
