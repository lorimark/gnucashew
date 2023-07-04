#line 2 "src/Dbo/Session.cpp"

#include <iostream>

#include <Wt/Dbo/backend/Sqlite3.h>

#include "Session.h"
#include "Account.h"

bool GCW::Dbo::Session::open( const std::string & _path )
{
  /*
  ** Clear this so that is we don't get opened, then we don't
  **  accidently indicate that we are open.
  **
  */
  m_isOpen = false;

  /*
  ** Remember the path
  **
  */
  m_path = _path;

  /*
  ** This never opens true
  **
  */
  return false;

} // endbool GCW::Dbo::Session::open( const std::string & _path )

bool GCW::Dbo::GnuCash::Session::open( const std::string & _path )
{
  /*
  ** Call the base class.
  **
  */
  GCW::Dbo::Session::open( _path );

  /*
  ** Try to get the sqlite3 file open
  **
  */
  try
  {
    auto connection = std::make_unique< Wt::Dbo::backend::Sqlite3 >( _path );

    setConnection( std::move( connection ) );

    init();

    return isOpen();
  }

  /*
  ** Handle the error
  **
  */
  catch( std::exception & e )
  {
    std::cout << __FILE__ << ":" << __LINE__ << " " << e.what() << std::endl;
  }

  return isOpen();

} // endbool GCW::Dbo::Session::open_gnucash( const std::string & _path )

void GCW::Dbo::GnuCash::Session::init()
{
  mapClass< Account::Item >( "accounts" );

  m_isOpen = true;

} // endvoid GCW::Dbo::Session::GnuCash::init()

bool GCW::Dbo::GnuCashew::Session::open( const std::string & _path )
{
  return false;

} // endbool GCW::Dbo::GnuCashew::Session::open( const std::string & _path )


