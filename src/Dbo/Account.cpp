
#include "../App.h"

#include "Account.h"

GCW::Dbo::Account::Item::Ptr GCW::Dbo::Account::root()
{
  GCW::Dbo::Account::Item::Ptr retVal;

  /*
  ** If the session isn't open then there's nothing to load.
  **
  */
  if( GCW::app()-> gnucash_session().isOpen() )
  {

    Wt::Dbo::Transaction t( GCW::app()-> gnucash_session() );

    /*
    ** Get a handle on the root account.  The root account is the only
    **  account that has no parent, and has a name == "Root Account".
    **  There should only be one of these.
    **
    */
    auto results =
      GCW::app()-> gnucash_session().find< GCW::Dbo::Account::Item >()
      .where( "(parent_guid = '' OR parent_guid IS NULL) AND name = 'Root Account'" )
      .resultList()
      ;

    if( results.size() == 1 )
    {
      retVal = *results.begin();
    }

  } // endif( GCW::app()-> gnucash_session().isOpen() )

  return retVal;

} // endGCW::Dbo::Account::Item::Ptr GCW::Dbo::Account::load()

GCW::Dbo::Account::Item::Ptr GCW::Dbo::Account::byGuid( const std::string & _guid )
{
  GCW::Dbo::Account::Item::Ptr retVal;

  /*
  ** If the session isn't open then there's nothing to load.
  **
  */
  if( GCW::app()-> gnucash_session().isOpen() )
  {

    Wt::Dbo::Transaction t( GCW::app()-> gnucash_session() );

    retVal =
      GCW::app()-> gnucash_session().load< GCW::Dbo::Account::Item >( _guid )
      ;

  } // endif( GCW::app()-> gnucash_session().isOpen() )

  return retVal;

} // endGCW::Dbo::Account::Item::Ptr GCW::Dbo::Account::byGuid( const std::string & _guid )



