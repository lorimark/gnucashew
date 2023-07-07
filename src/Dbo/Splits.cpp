
#include "../App.h"

#include "Splits.h"


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

  } // endif( GCW::app()-> gnucash_session().isOpen() )

  return retVal;

} // endGCW::Dbo::Splits::Item::Vector GCW::Dbo::Splits::byAccount( const std::string & _accountGuid )


GCW::Dbo::Splits::Item::Vector
GCW::Dbo::Splits::byTransaction( const std::string & _txGuid, const std::string & _excludeSplitGuid )
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
      if( result-> guid() != _excludeSplitGuid )
        retVal.push_back( result );

  } // endif( GCW::app()-> gnucash_session().isOpen() )

  return retVal;

} // endGCW::Dbo::Splits::Item::Vector byTransaction( const std::string & _txGuid )



