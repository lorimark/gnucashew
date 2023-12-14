#line 2 "src/Dbo/Session.cpp"

#include <iostream>

#include <Wt/Dbo/backend/Sqlite3.h>

#include <gcwglobal.h>

#include "Accounts.h"
#include "Customers.h"
#include "Slots.h"
#include "Splits.h"
#include "Transactions.h"
#include "Vars.h"
#include "Session.h"

bool
GCW::Dbo::Session::
open( const std::string & _path )
{
  /*
  ** Clear this so that if we don't get opened, then we don't
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

bool
GCW::Dbo::GnuCash::Session::
open( const std::string & _path )
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

//    connection-> setProperty( "show-queries", "true" );

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

void
GCW::Dbo::GnuCash::Session::
init()
{
  mapClass< GCW::Dbo:: Accounts     ::Item >( GCW::Dbo:: Accounts     ::s_tableName );
  mapClass< GCW::Dbo:: Customers    ::Item >( GCW::Dbo:: Customers    ::s_tableName );
  mapClass< GCW::Dbo:: Transactions ::Item >( GCW::Dbo:: Transactions ::s_tableName );
  mapClass< GCW::Dbo:: Slots        ::Item >( GCW::Dbo:: Slots        ::s_tableName );
  mapClass< GCW::Dbo:: Splits       ::Item >( GCW::Dbo:: Splits       ::s_tableName );
  mapClass< GCW::Dbo:: Vars         ::Item >( GCW::Dbo:: Vars         ::s_tableName );

  m_isOpen = true;

} // endvoid GCW::Dbo::Session::GnuCash::init()

bool
GCW::Dbo::GnuCash::Session::
hasGnuCashewExtensions()
{
  /*
  ** assume no extensions
  */
  bool retVal = false;

  using ResultRow = std::tuple< std::string, std::string, std::string, int, std::string >;

  /*
  **  query the list of tables
  */
  Wt::Dbo::Transaction t( *this );
  auto results =
    query< ResultRow >
    (
     "select type,name,tbl_name,rootpage,sql from sqlite_master"
    )
    .resultList()
    ;

  /*
  ** loop the list, looking for the extension tables
  */
  for( auto result : results )
  {
    std::string _type;
    std::string _name;
    std::string tbl_name;
    int         rootpage;
    std::string _text;

    std::tie( _type, _name, tbl_name, rootpage, _text ) = result;

    if( _type != "table" )
      continue;

    if( _name == GCW::Dbo::Vars::s_tableName )
    {
      retVal = true;
      break;
    }

#ifdef NEVER
    std::cout << __FILE__ << ":" << __LINE__
      << " type:" << _type
      << " name:" << _name
//      << " tbln:" << tbl_name
//      << " root:" << rootpage
//      << " text:" << _text
      << std::endl
      ;
#endif

  } // endfor( auto result : results )

  return retVal;

} // endhasGnuCashewExtensions() const

bool
GCW::Dbo::GnuCash::Session::
addGnuCashewExtensions()
{
  if( hasGnuCashewExtensions() )
    return true;

  Wt::Dbo::Transaction t( *this );

  /*
  ** Add a table to contain the vars
  **
  */
  auto sql =
    Wt::WString( TR("gcw_sql.create_vars") )
    .arg( GCW::Dbo::Vars::s_tableName )
    .toUTF8()
    ;

  execute( sql );

  /*
  ** poke some initial values in to the vars table
  **
  */
  auto item = GCW::Dbo::Vars::get( "gnucashew","sys" );

  item.modify()-> setVar( "createdOn",  Wt::WDateTime::currentDateTime().toString( ISO_DATE_FORMAT ) );
  item.modify()-> setVar( "gcwVersion", 1 );

  return true;

} // endaddGnuCashewExtensions()

bool
GCW::Dbo::GnuCashew::Session::
open( const std::string & _path )
{

  return false;

} // endbool GCW::Dbo::GnuCashew::Session::open( const std::string & _path )



