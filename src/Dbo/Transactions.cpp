
#include "../App.h"
#include "Transactions.h"

GCW::Dbo::Transactions::Item::Ptr GCW::Dbo::Transactions::byGuid( const std::string & _txGuid )
{
  GCW::Dbo::Transactions::Item::Ptr retVal;

  /*
  ** If the session isn't open then there's nothing to load.
  **
  */
  if( GCW::app()-> gnucash_session().isOpen() )
  {

    Wt::Dbo::Transaction t( GCW::app()-> gnucash_session() );

    retVal =
      GCW::app()-> gnucash_session().load< GCW::Dbo::Transactions::Item >( _txGuid )
      ;

  } // endif( GCW::app()-> gnucash_session().isOpen() )


  return retVal;

} // endGCW::Dbo::Transactions::Item::Ptr GCW::Dbo::Transactions::byGuid( const std::string & _txGuid )


