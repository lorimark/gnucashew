#line 2 "src/Dbo/AbstractSession.cpp"

#include <iostream>

#include <Wt/Dbo/backend/Sqlite3.h>

#include <gcwglobal.h>

#include "Accounts.h"
#include "Customers.h"
#include "Slots.h"
#include "Splits.h"
#include "Transactions.h"
#include "Vars.h"
#include "AbstractSession.h"

bool
GCW::Dbo::AbstractSession::
open( const std::string & _path )
{
  /*
  ** Clear this so that if we don't get opened, then we don't
  **  accidently indicate that we are open.
  **
  */
  m_isOpen = false;

  /*
  ** Remember the path.
  **
  */
  m_path = _path;

  /*
  ** Dynamicals
  **
  */
  mapClass< GCW::Dbo:: Accounts     ::Item >( GCW::Dbo:: Accounts     ::s_tableName );
  mapClass< GCW::Dbo:: Customers    ::Item >( GCW::Dbo:: Customers    ::s_tableName );
  mapClass< GCW::Dbo:: Transactions ::Item >( GCW::Dbo:: Transactions ::s_tableName );
  mapClass< GCW::Dbo:: Slots        ::Item >( GCW::Dbo:: Slots        ::s_tableName );
  mapClass< GCW::Dbo:: Splits       ::Item >( GCW::Dbo:: Splits       ::s_tableName );
  mapClass< GCW::Dbo:: Vars         ::Item >( GCW::Dbo:: Vars         ::s_tableName );

  /*
  ** This never opens true.
  **
  */
  return false;

} // endbool GCW::Dbo::AbstractSession::open( const std::string & _path )


