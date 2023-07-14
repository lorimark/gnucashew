
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
GCW::Dbo::Accounts::allAccounts()
{
  GCW::Dbo::Accounts::Item::Vector retVal;

  /*
  ** If the session isn't open then there's nothing to load.
  **
  */
  if( GCW::app()-> gnucash_session().isOpen() )
  {

    Wt::Dbo::Transaction t( GCW::app()-> gnucash_session() );

    auto results =
      GCW::app()-> gnucash_session().find< GCW::Dbo::Accounts::Item >()
      .resultList()
      ;

    for( auto result : results )
      retVal.push_back( result );

  } // endif( GCW::app()-> gnucash_session().isOpen() )


  return retVal;

} // endGCW::Dbo::Accounts::Item::Vector GCW::Dbo::Accounts::Children::vector( const std::string & _parentGuid )


GCW::Dbo::Accounts::Item::Vector
GCW::Dbo::Accounts::activeAccounts()
{
  GCW::Dbo::Accounts::Item::Vector retVal;

  /*
  ** If the session isn't open then there's nothing to load.
  **
  */
  if( GCW::app()-> gnucash_session().isOpen() )
  {

    Wt::Dbo::Transaction t( GCW::app()-> gnucash_session() );

    auto results =
      GCW::app()-> gnucash_session().find< GCW::Dbo::Accounts::Item >()
      .resultList()
      ;

    for( auto result : results )
      if( !result-> hidden()
       && !result-> placeHolder()
        )
      retVal.push_back( result );

  } // endif( GCW::app()-> gnucash_session().isOpen() )

  return retVal;

} // endGCW::Dbo::Accounts::Item::Vector GCW::Dbo::Accounts::Children::vector( const std::string & _parentGuid )


GCW::Dbo::Accounts::Item::Vector
GCW::Dbo::Accounts::Children::vector( const std::string & _parentGuid )
{
  GCW::Dbo::Accounts::Item::Vector retVal;


  return retVal;

} // endGCW::Dbo::Accounts::Item::Vector GCW::Dbo::Accounts::Children::vector( const std::string & _parentGuid )

/*!
** \brief Compute Account Full-Name from Heirarchy
**
** This function will calculate the "full account name" from
**  the accountGuid up to the root parent.
**
*/
std::string
GCW::Dbo::Accounts::fullName( const std::string & _accountGuid )
{
  /*!
  ** If the provided account guid is blank, then just return
  **  an empty string.
  **
  */
  if( _accountGuid == "" )
    return "";

  /*
  ** Fetch the account by Guid
  **
  */
  auto accountItem = byGuid( _accountGuid );

  /*!
  ** During the building process, even though the "root account"
  **  is a valid account, it is ignored and not included in the
  **  results.
  **
  */
  if( accountItem == root() )
    return "";

  /*
  ** This is a recursive function that extracts the portions of
  **  the account names and assembles them in to a contiguous
  **  string with ':' color separator.
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

  /*!
  ** Recursively, this should generate a name such as;
  **   "Assets:2023:Cash:FGB:OLB:2300-LSI"
  **
  */
  return retVal;

} // endstd::string fullName( const std::string * _guid );



