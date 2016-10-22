// (C) 2016, Sergei Zaychenko, KNURE, Kharkiv, Ukraine

/*****************************************************************************/

#include "testslib.hpp"

#include "install_registry.hpp"
#include "messages.hpp"

/*****************************************************************************/


DECLARE_OOP_TEST ( test_InstallRegistry_Constructor )
{
	InstallRegistry r( "node" );

	assert( r.getMachineName() == "node" );

	assert( r.isClean() );
	assert( r.fetchAllInstalledProgramNames().empty() );
	assert( r.getNumInstalledPrograms() == 0 );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_InstallRegistry_Constructor_EmptyMachineName )
{
	ASSERT_THROWS(
			InstallRegistry( "" )
		,	Messages::MachineNameEmpty
	)
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_InstallRegistry_UpdateMachineName )
{
	InstallRegistry r( "node" );
	
	r.updateMachineName( "node2" );
	assert( r.getMachineName() == "node2" );

	r.updateMachineName( "node2" );
	assert( r.getMachineName() == "node2" );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_InstallRegistry_UpdateMachineName_EmptyName )
{
	InstallRegistry r( "node" );

	ASSERT_THROWS(
			r.updateMachineName( "" )
		,	Messages::MachineNameEmpty
	)
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_InstallRegistry_IsInstalled_WhenClean )
{
	InstallRegistry r( "node" );
	assert( ! r.isProgramInstalled( "Skype" ) );
}



/*****************************************************************************/


DECLARE_OOP_TEST ( test_InstallRegistry_GetInstallationDate_WhenClean )
{
	InstallRegistry r( "node" );

	ASSERT_THROWS(
			r.getProgramInstallationDate( "Skype" )
		,	Messages::ProgramNotInstalled
	)
}



/*****************************************************************************/


DECLARE_OOP_TEST ( test_InstallRegistry_GetInstallationDate_EmptyName )
{
	InstallRegistry r( "node" );

	ASSERT_THROWS(
			r.getProgramInstallationDate( "" )
		,	Messages::ProgramNameEmpty
	)
}



/*****************************************************************************/


DECLARE_OOP_TEST ( test_InstallRegistry_InstallSingle )
{
	InstallRegistry r( "node" );
	r.installProgram( "Skype", Date() );

	assert( ! r.isClean() );
	assert( r.getNumInstalledPrograms() == 1 );
	assert( r.isProgramInstalled( "Skype" ) );
	assert( r.getProgramInstallationDate( "Skype" ) == Date() );
	assert( r.fetchAllInstalledProgramNames() == std::set< std::string >{ "Skype" } );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_InstallRegistry_Install_Two )
{
	InstallRegistry r( "node" );
	r.installProgram( "Skype", Date( 2016, 1, 1 ) );
	r.installProgram( "Chrome", Date( 2016, 2, 2 ) );

	assert( !r.isClean() );
	assert( r.getNumInstalledPrograms() == 2 );

	assert( r.isProgramInstalled( "Skype" ) );
	assert( r.getProgramInstallationDate( "Skype" ) == Date( 2016, 1, 1 ) );

	assert( r.isProgramInstalled( "Chrome" ) );
	assert( r.getProgramInstallationDate( "Chrome" ) == Date( 2016, 2, 2 ) );

	std::set< std::string > expectedProgramNames{ "Chrome", "Skype" };
	assert( r.fetchAllInstalledProgramNames() == expectedProgramNames );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_InstallRegistry_Install_Two_CaseDifferenceOnly )
{
	InstallRegistry r( "node" );
	r.installProgram( "Skype", Date( 2016, 1, 1 ) );
	r.installProgram( "skype", Date( 2016, 2, 2 ) );

	assert( !r.isClean() );
	assert( r.getNumInstalledPrograms() == 2 );

	assert( r.isProgramInstalled( "Skype" ) );
	assert( r.getProgramInstallationDate( "Skype" ) == Date( 2016, 1, 1 ) );

	assert( r.isProgramInstalled( "skype" ) );
	assert( r.getProgramInstallationDate( "skype" ) == Date( 2016, 2, 2 ) );

	std::set< std::string > expectedProgramNames{ "Skype", "skype" };
	assert( r.fetchAllInstalledProgramNames() == expectedProgramNames );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_InstallRegistry_Install_EmptyProgramName )
{
	InstallRegistry r( "node" );

	ASSERT_THROWS(
			r.installProgram( "", Date() )
		,	Messages::ProgramNameEmpty
	)
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_InstallRegistry_Install_DateInFuture )
{
	InstallRegistry r( "node" );

	ASSERT_THROWS(
			r.installProgram( "Skype", Date( 2100, 1, 1 ) )
		,	Messages::InstallationDateInFuture
	)
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_InstallRegistry_Install_DuplicateProgram )
{
	InstallRegistry r( "node" );
	r.installProgram( "Skype", Date() );

	ASSERT_THROWS(
			r.installProgram( "Skype", Date() )
		,	Messages::ProgramAlreadyInstalled
	)
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_InstallRegistry_InstallAndUpdate )
{
	InstallRegistry r( "node" );
	r.installProgram( "Skype", Date( 2015, 1, 1 ) );
	r.updateProgram( "Skype", Date( 2016, 1, 1 ) );

	assert( !r.isClean() );
	assert( r.getNumInstalledPrograms() == 1 );
	assert( r.isProgramInstalled( "Skype" ) );
	assert( r.getProgramInstallationDate( "Skype" ) == Date( 2016, 1, 1 ) );
	assert( r.fetchAllInstalledProgramNames() == std::set< std::string >{ "Skype" } );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_InstallRegistry_Install_Two_UpdateOne )
{
	InstallRegistry r( "node" );
	r.installProgram( "Skype", Date( 2016, 1, 1 ) );
	r.installProgram( "Chrome", Date( 2016, 2, 2 ) );
	r.updateProgram( "Skype", Date( 2016, 3, 3 ) );

	assert( !r.isClean() );
	assert( r.getNumInstalledPrograms() == 2 );

	assert( r.isProgramInstalled( "Skype" ) );
	assert( r.getProgramInstallationDate( "Skype" ) == Date( 2016, 3, 3 ) );

	assert( r.isProgramInstalled( "Chrome" ) );
	assert( r.getProgramInstallationDate( "Chrome" ) == Date( 2016, 2, 2 ) );

	std::set< std::string > expectedProgramNames{ "Chrome", "Skype" };
	assert( r.fetchAllInstalledProgramNames() == expectedProgramNames );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_InstallRegistry_Update_EmptyProgramName )
{
	InstallRegistry r( "node" );
	
	ASSERT_THROWS(
			r.updateProgram( "", Date() )
		,	Messages::ProgramNameEmpty
	)
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_InstallRegistry_Update_NoProgramsInstalled )
{
	InstallRegistry r( "node" );

	ASSERT_THROWS(
			r.updateProgram( "Skype", Date() )
		,	Messages::ProgramNotInstalled
	)
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_InstallRegistry_Update_ProgramWasntInstalled )
{
	InstallRegistry r( "node" );
	r.installProgram( "Skype", Date() );

	ASSERT_THROWS(
			r.updateProgram( "Chrome", Date() )
		,	Messages::ProgramNotInstalled
	)
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_InstallRegistry_Update_InFuture )
{
	InstallRegistry r( "node" );
	r.installProgram( "Skype", Date() );

	ASSERT_THROWS(
			r.updateProgram( "Skype", Date( 2100, 1, 1 ) )
		,	Messages::UpdateDateInFuture
	)
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_InstallRegistry_Update_EarlierThanPreviousVersion )
{
	InstallRegistry r( "node" );
	r.installProgram( "Skype", Date( 2015, 1, 1 ) );

	ASSERT_THROWS(
			r.updateProgram( "Skype", Date( 2014, 12, 31 ) )
		,	Messages::UpdateDateEarlierThanPreviousVersion
	)

	assert( r.isProgramInstalled( "Skype" ) );
	assert( r.getProgramInstallationDate( "Skype" ) == Date( 2015, 1, 1 ) );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_InstallRegistry_Update_SameDay_Fine )
{
	InstallRegistry r( "node" );
	r.installProgram( "Skype", Date( 2015, 1, 1 ) );
	r.updateProgram( "Skype", Date( 2015, 1, 1 ) );

	assert( r.isProgramInstalled( "Skype" ) );
	assert( r.getProgramInstallationDate( "Skype" ) == Date( 2015, 1, 1 ) );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_InstallRegistry_Instrall_Uninstall )
{
	InstallRegistry r( "node" );
	r.installProgram( "Skype", Date( 2015, 1, 1 ) );
	r.uninstallProgram( "Skype" );

	assert( r.isClean() );
	assert( r.getNumInstalledPrograms() == 0 );
	assert( r.fetchAllInstalledProgramNames().empty() );
	assert( !r.isProgramInstalled( "Skype" ) );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_InstallRegistry_InstrallTwo_UninstallOne )
{
	InstallRegistry r( "node" );
	r.installProgram( "Skype", Date( 2015, 1, 1 ) );
	r.installProgram( "Chrome", Date( 2015, 2, 2 ) );
	r.uninstallProgram( "Skype" );

	assert( ! r.isClean() );
	assert( r.getNumInstalledPrograms() == 1 );
	assert( r.fetchAllInstalledProgramNames() == std::set< std::string > { "Chrome" } );
	assert( r.isProgramInstalled( "Chrome" ) );
	assert( r.getProgramInstallationDate( "Chrome" ) == Date( 2015, 2, 2 ) );
	assert( ! r.isProgramInstalled( "Skype" ) );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_InstallRegistry_InstrallUninstallReinstall )
{
	InstallRegistry r( "node" );
	r.installProgram( "Skype", Date( 2015, 1, 1 ) );
	r.uninstallProgram( "Skype" );
	r.installProgram( "Skype", Date( 2015, 1, 2 ) );

	assert( ! r.isClean() );
	assert( r.getNumInstalledPrograms() == 1 );
	assert( r.fetchAllInstalledProgramNames() == std::set< std::string > { "Skype" } );
	assert( r.isProgramInstalled( "Skype" ) );
	assert( r.getProgramInstallationDate( "Skype" ) == Date( 2015, 1, 2 ) );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_InstallRegistry_Uninstall_EmptyProgramName )
{
	InstallRegistry r( "node" );

	ASSERT_THROWS(
			r.uninstallProgram( "" )
		,	 Messages::ProgramNameEmpty
	)
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_InstallRegistry_Uninstall_NoProgramsYet )
{
	InstallRegistry r( "node" );

	ASSERT_THROWS(
			r.uninstallProgram( "Skype" )
		,	Messages::ProgramNotInstalled
	)
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_InstallRegistry_Uninstall_NoSuchProgram )
{
	InstallRegistry r( "node" );
	r.installProgram( "Chrome", Date( 2015, 1, 1 ) );

	ASSERT_THROWS(
		r.uninstallProgram( "Skype" )
		, Messages::ProgramNotInstalled
	)

	assert( r.isProgramInstalled( "Chrome" ) );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_InstallRegistry_UninstallAll_WasCleanAlready )
{
	InstallRegistry r( "node" );
	r.uninstallAll();

	assert( r.isClean() );
	assert( r.getNumInstalledPrograms() == 0 );
	assert( r.fetchAllInstalledProgramNames().empty() );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_InstallRegistry_UninstallAll_WhenCoupleProgramsInstalled )
{
	InstallRegistry r( "node" );
	r.installProgram( "Skype", Date() );
	r.installProgram( "Chrome", Date() );
	r.uninstallAll();

	assert( r.isClean() );
	assert( r.getNumInstalledPrograms() == 0 );
	assert( r.fetchAllInstalledProgramNames().empty() );

	assert( ! r.isProgramInstalled( "Skype" ) );
	assert( ! r.isProgramInstalled( "Chrome" ) );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_InstallRegistry_ComparePrograms_BothEmpty )
{
	InstallRegistry r1( "node1" );
	InstallRegistry r2( "node2" );
	
	assert( r1.hasIdenticalPrograms( r2 ) );
	assert( r2.hasIdenticalPrograms( r1 ) );

	assert( r1.getUniqueProgramsComparedTo( r2 ).empty() );
	assert( r2.getUniqueProgramsComparedTo( r1 ).empty() );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_InstallRegistry_ComparePrograms_BothNonEmptyAndEqual )
{
	InstallRegistry r1( "node1" );
	InstallRegistry r2( "node2" );

	r1.installProgram( "Skype", Date() );
	r1.installProgram( "Chrome", Date() );

	r2.installProgram( "Skype", Date( 2015, 1, 1 ) );
	r2.installProgram( "Chrome", Date( 2016, 1, 1 ) );

	assert( r1.hasIdenticalPrograms( r2 ) );
	assert( r2.hasIdenticalPrograms( r1 ) );

	assert( r1.getUniqueProgramsComparedTo( r2 ).empty() );
	assert( r2.getUniqueProgramsComparedTo( r1 ).empty() );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_InstallRegistry_ComparePrograms_FirstHasUniqueProgram )
{
	InstallRegistry r1( "node1" );
	InstallRegistry r2( "node2" );

	r1.installProgram( "Skype", Date() );
	r1.installProgram( "Chrome", Date() );

	r2.installProgram( "Skype", Date( 2015, 1, 1 ) );

	assert( ! r1.hasIdenticalPrograms( r2 ) );
	assert( ! r2.hasIdenticalPrograms( r1 ) );

	assert( r1.getUniqueProgramsComparedTo( r2 ) == std::set< std::string >{ "Chrome" } );
	assert( r2.getUniqueProgramsComparedTo( r1 ).empty() );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_InstallRegistry_ComparePrograms_FirstHasCoupleUniquePrograms )
{
	InstallRegistry r1( "node1" );
	InstallRegistry r2( "node2" );

	r1.installProgram( "Skype", Date() );
	r1.installProgram( "Chrome", Date() );

	assert( !r1.hasIdenticalPrograms( r2 ) );
	assert( !r2.hasIdenticalPrograms( r1 ) );

	std::set< std::string > expectedResults = { "Chrome", "Skype" };
	assert( r1.getUniqueProgramsComparedTo( r2 ) == expectedResults );
	assert( r2.getUniqueProgramsComparedTo( r1 ).empty() );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_InstallRegistry_ComparePrograms_AllUnique )
{
	InstallRegistry r1( "node1" );
	InstallRegistry r2( "node2" );

	r1.installProgram( "Skype", Date() );

	r2.installProgram( "Chrome", Date() );

	assert( !r1.hasIdenticalPrograms( r2 ) );
	assert( !r2.hasIdenticalPrograms( r1 ) );

	assert( r1.getUniqueProgramsComparedTo( r2 ) == std::set< std::string >{ "Skype" } );
	assert( r2.getUniqueProgramsComparedTo( r1 ) == std::set< std::string >{ "Chrome"  } );
}


/*****************************************************************************/


DECLARE_OOP_TEST ( test_InstallRegistry_ComparePrograms_OneCommon_OtherUnique )
{
	InstallRegistry r1( "node1" );
	InstallRegistry r2( "node2" );

	r1.installProgram( "Skype", Date() );
	r1.installProgram( "Chrome", Date() );

	r2.installProgram( "Chrome", Date() );
	r2.installProgram( "Firefox", Date() );

	assert( !r1.hasIdenticalPrograms( r2 ) );
	assert( !r2.hasIdenticalPrograms( r1 ) );

	assert( r1.getUniqueProgramsComparedTo( r2 ) == std::set< std::string >{ "Skype" } );
	assert( r2.getUniqueProgramsComparedTo( r1 ) == std::set< std::string >{ "Firefox"  } );
}


/*****************************************************************************/





