#line 2 "src/Dbo/Session.cpp"

#include <iostream>

#include <Wt/Dbo/backend/Sqlite3.h>

#include "Session.h"
#include "Account.h"

class TestClass
: public Wt::Dbo::ptr< TestClass >
{
  public:

    TestClass() {};

    template< class Action > void persist( Action & action )
    {
      Wt::Dbo::field( action, m_data, "dataField" );
    }

    std::string m_data;
};


GCW::Dbo::Session::Session()
: Wt::Dbo::Session()
{
}

bool GCW::Dbo::Session::open_sqlite3( const std::string & _path )
{
  auto connection = std::make_unique< Wt::Dbo::backend::Sqlite3 >( _path + ".gnucashew" );

  setConnection( std::move( connection ) );

  init();

  return true;
}

bool GCW::Dbo::Session::open_gnucash( const std::string & _path )
{
  try
  {
    auto connection = std::make_unique< Wt::Dbo::backend::Sqlite3 >( _path );

    setConnection( std::move( connection ) );

    init();

    return true;
  }
  catch( std::exception & e )
  {
    std::cout << __FILE__ << ":" << __LINE__ << " " << e.what() << std::endl;
  }

  return false;

} // endbool GCW::Dbo::Session::open_gnucash( const std::string & _path )

void GCW::Dbo::Session::init()
{
  mapClass< Account >( "accounts" );

//  createTables();
}

