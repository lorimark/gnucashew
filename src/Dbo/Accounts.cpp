
#include "../App.h"

#include "Accounts.h"
#include "Transactions.h"

GCW::Dbo::Accounts::Item::Ptr
GCW::Dbo::Accounts::root()
{
  GCW::Dbo::Accounts::Item::Ptr retVal;

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
      GCW::app()-> gnucash_session().find< GCW::Dbo::Accounts::Item >()
      .where( "(parent_guid = '' OR parent_guid IS NULL) AND name = 'Root Account'" )
      .resultList()
      ;

    if( results.size() == 1 )
    {
      retVal = *results.begin();
    }

  } // endif( GCW::app()-> gnucash_session().isOpen() )

  return retVal;

} // endGCW::Dbo::Accounts::Item::Ptr GCW::Dbo::Accounts::root()

GCW::Dbo::Accounts::Item::Ptr
GCW::Dbo::Accounts::byGuid( const std::string & _guid )
{
  GCW::Dbo::Accounts::Item::Ptr retVal;

  /*
  ** If the session isn't open then there's nothing to load.
  **
  */
  if( GCW::app()-> gnucash_session().isOpen() )
  {

    Wt::Dbo::Transaction t( GCW::app()-> gnucash_session() );

    retVal =
      GCW::app()-> gnucash_session().load< GCW::Dbo::Accounts::Item >( _guid )
      ;

  } // endif( GCW::app()-> gnucash_session().isOpen() )

  return retVal;

} // endGCW::Dbo::Accounts::Item::Ptr GCW::Dbo::Accounts::byGuid( const std::string & _guid )


GCW::Dbo::Accounts::Item::Vector
GCW::Dbo::Accounts::Children::vector( const std::string & _parentGuid )
{
  GCW::Dbo::Accounts::Item::Vector retVal;


  return retVal;

} // endGCW::Dbo::Accounts::Item::Vector GCW::Dbo::Accounts::Children::vector( const std::string & _parentGuid )


std::string
GCW::Dbo::Accounts::fullName( const std::string & _accountGuid )
{
  /*
  ** If the guid is blank, then we for sure have
  **  nothing.
  **
  */
  if( _accountGuid == "" )
    return "";

  /*
  ** Fetch the account by Guid
  **
  */
  auto accountItem = byGuid( _accountGuid );

  /*
  ** If this is the root account, then we're done like
  **  we were done when it was a blank guid.
  **
  */
  if( accountItem == root() )
    return "";

  /*
  ** Recursive call to the parent.
  **
  */
  std::string retVal = fullName( accountItem-> parent_guid() );

  /*
  ** If we got anything then we need a separator
  **
  */
  if( retVal != "" )
      retVal += ":";

  /*
  ** And, finally the name of our account.
  **
  */
  retVal += accountItem-> name();

  /*
  ** Recursively, this should generate a name such as;
  **   "Assets:2023:Cash:FGB:OLB:2300-LSI"
  **
  */
  return retVal;

} // endstd::string fullName( const std::string * _guid );



