// (C) 2016, Sergei Zaychenko, KNURE, Kharkiv, Ukraine

#ifndef _PROFILES_MANAGER_HPP_
#define _PROFILES_MANAGER_HPP_

/*****************************************************************************/

#include <string>

/*****************************************************************************/


class ProfilesManager
{

/*-----------------------------------------------------------------*/

public:

/*-----------------------------------------------------------------*/

	struct ProfileData
	{
		std::string m_serviceName;
		std::string m_login;
		std::string m_password;

		ProfileData ( std::string const & _serviceName, 
			          std::string const & _login,
			          std::string const & _password );

		bool operator == ( const ProfileData & _profile ) const;

		bool operator != ( const ProfileData & _profile ) const;
	};

/*-----------------------------------------------------------------*/

	// TODO: public members

/*-----------------------------------------------------------------*/

private:

/*-----------------------------------------------------------------*/

	// TODO: private members

/*-----------------------------------------------------------------*/

};


/*****************************************************************************/


// TODO: inline functions

/*****************************************************************************/

#endif // _PROFILES_MANAGER_HPP_
