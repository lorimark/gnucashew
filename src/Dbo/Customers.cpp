
#include "../App.h"
#include "Customers.h"

GCW::Dbo::Customers::Item::Ptr
GCW::Dbo::Customers::byGuid( const std::string & _guid )
{
  GCW::Dbo::Customers::Item::Ptr retVal;

  /*
  ** If the session isn't open then there's nothing to load.
  **
  */
  if( GCW::app()-> gnucash_session().isOpen() )
  {

    Wt::Dbo::Transaction t( GCW::app()-> gnucash_session() );

    retVal =
      GCW::app()-> gnucash_session().load< GCW::Dbo::Customers::Item >( _guid )
      ;

  } // endif( GCW::app()-> gnucash_session().isOpen() )

  return retVal;

} // endGCW::Dbo::Customers::Item::Ptr GCW::Dbo::Customers::byGuid( const std::string & _guid )



