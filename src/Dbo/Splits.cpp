
#include "../App.h"

#include "Splits.h"
#include "Transactions.h"

namespace {

void sort( GCW::Dbo::Splits::Item::Vector & _splitItems )
{
  /*!
  ** Sort the vector of splits by transaction date so that they can be loaded
  **  in to the model in proper sequential order.
  **
  */
  std::sort
  (
   _splitItems.begin(),
   _splitItems.end(),
   []( const GCW::Dbo::Splits::Item::Ptr item1,
       const GCW::Dbo::Splits::Item::Ptr item2
     )
     {
       auto trans1 = GCW::Dbo::Transactions::byGuid( item1-> tx_guid() );
       auto trans2 = GCW::Dbo::Transactions::byGuid( item2-> tx_guid() );

       /*
       ** return if the .trans1. date is .less than. the .trans2. date
       **
       */
       return trans1-> post_date_as_date()
            < trans2-> post_date_as_date();
     }
  );

} // endvoid sort( GCW::Dbo::Splits::Item::Vector & _splitItems )

} // endnamespace {

GCW::Dbo::Splits::Item::Ptr
GCW::Dbo::Splits::load( const std::string & _splitGuid )
{
  GCW::Dbo::Splits::Item::Ptr retVal;

  /*
  ** If the session isn't open then there's nothing to load.
  **
  */
  if( GCW::app()-> gnucash_session().isOpen() )
  {

    Wt::Dbo::Transaction t( GCW::app()-> gnucash_session() );

    retVal =
      GCW::app()-> gnucash_session().load< GCW::Dbo::Splits::Item >( _splitGuid )
      ;

  } // endif( GCW::app()-> gnucash_session().isOpen() )

  return retVal;

} // endGCW::Dbo::Splits::Item::Ptr GCW::Dbo::Splits::load( const std::string & _splitGuid )

GCW::Dbo::Splits::Item::Vector
GCW::Dbo::Splits::byAccount( const std::string & _accountGuid )
{
  GCW::Dbo::Splits::Item::Vector retVal;

  /*
  ** If the session isn't open then there's nothing to load.
  **
  */
  if( GCW::app()-> gnucash_session().isOpen() )
  {

    Wt::Dbo::Transaction t( GCW::app()-> gnucash_session() );

    auto results =
      GCW::app()-> gnucash_session().find< GCW::Dbo::Splits::Item >()
      .where( "account_guid = ?" )
      .bind( _accountGuid )
      .resultList()
      ;

    for( auto result : results )
      retVal.push_back( result );

    sort( retVal );

  } // endif( GCW::app()-> gnucash_session().isOpen() )

  return retVal;

} // endGCW::Dbo::Splits::Item::Vector GCW::Dbo::Splits::byAccount( const std::string & _accountGuid )

GCW::Dbo::Splits::Item::Vector
GCW::Dbo::Splits::bySplit
(
 const std::string & _splitGuid
)
{
  GCW::Dbo::Splits::Item::Vector retVal;

  auto splitItem = load( _splitGuid );

  /*
  ** If the session isn't open then there's nothing to load.
  **
  */
  if( splitItem )
  {

    Wt::Dbo::Transaction t( GCW::app()-> gnucash_session() );

    auto results =
      GCW::app()-> gnucash_session().find< GCW::Dbo::Splits::Item >()
      .where( "tx_guid = ?" )
      .bind( splitItem-> tx_guid() )
      .resultList()
      ;

    /*
    ** Load the vector, but skip the one that
    **  matches our incoming split guid.
    **
    */
    for( auto result : results )
      if( result-> guid() != _splitGuid )
        retVal.push_back( result );

    /*!
    ** The vector is sorted by transction-date before
    **  returning to the caller.
    **
    */
    sort( retVal );

  } // endif( GCW::app()-> gnucash_session().isOpen() )

  return retVal;

} // endGCW::Dbo::Splits::Item::Vector byTransaction( const std::string & _txGuid )


/*!
* \return Vector of Split Items Sorted by Transaction Date
*/
GCW::Dbo::Splits::Item::Vector
GCW::Dbo::Splits::byTransaction
(
 const std::string & _txGuid
)
{
  GCW::Dbo::Splits::Item::Vector retVal;

  /*
  ** If the session isn't open then there's nothing to load.
  **
  */
  if( GCW::app()-> gnucash_session().isOpen() )
  {

    Wt::Dbo::Transaction t( GCW::app()-> gnucash_session() );

    auto results =
      GCW::app()-> gnucash_session().find< GCW::Dbo::Splits::Item >()
      .where( "tx_guid = ?" )
      .bind( _txGuid )
      .resultList()
      ;

    for( auto result : results )
      retVal.push_back( result );

    /*!
    ** The vector is sorted by transction-date before
    **  returning to the caller.
    **
    */
    sort( retVal );

  } // endif( GCW::app()-> gnucash_session().isOpen() )

  return retVal;

} // endGCW::Dbo::Splits::Item::Vector byTransaction( const std::string & _txGuid )



