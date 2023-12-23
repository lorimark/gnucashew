#line 2 "src/Dbo/Customers.cpp"

#include "../App.h"
#include "Customers.h"

const char * GCW::Dbo::Customers::s_tableName = "customers";

GCW::Dbo::Customers::Item::Ptr
GCW::Dbo::Customers::
byGuid( const std::string & _guid )
{
  GCW::Dbo::Customers::Item::Ptr retVal;

  retVal =
    GCW::app()-> gnucashew_session().load< GCW::Dbo::Customers::Item >( _guid )
    ;

  return retVal;

} // endbyGuid( const std::string & _guid )



