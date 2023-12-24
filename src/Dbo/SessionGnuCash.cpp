#line 2 "src/Dbo/SessionGnuCash.cpp"

#define TESTFILE "gnucash-dev.gnucash"

#include <iostream>

#include <gnucash/gnc-session.h>
#include <gnucash/Account.h>
#include <gnucash/Transaction.h>
#include <gnucash/qofsession.h>

#include "../Glb/Core.h"

#include <gcwglobal.h>
#include "Accounts.h"
#include "Customers.h"
#include "Slots.h"
#include "Splits.h"
#include "Transactions.h"
#include "Vars.h"
#include "SessionGnuCash.h"

GCW::Dbo::GnuCash::Session::
~Session()
{
  if( isOpen() )
    closeGnuCash();
}

bool
GCW::Dbo::GnuCash::Session::
open( const std::string & _path )
{
  if( isOpen() )
    return true;

  /*
  ** Call the base class.
  **
  */
  GCW::Dbo::AbstractSession::open( _path );

  std::cout << __FILE__ << ":" << __LINE__ << " " << path() << std::endl;

//  auto userdata_migration_msg = gnc_filepath_init();
  gnc_engine_init( 0, nullptr );
  auto s = gnc_get_current_session(); // qof_session_new( qof_book_new() );

  std::cout << __FILE__ << ":" << __LINE__ << " qof_session_begin" << std::endl;
  std::string url = "sqlite3://" + path();

//  qof_session_begin( s, url.c_str(), SESSION_NORMAL_OPEN    );
//  qof_session_begin( s, url.c_str(), SESSION_NEW_STORE      );
//  qof_session_begin( s, url.c_str(), SESSION_NEW_OVERWRITE  );
  qof_session_begin( s, url.c_str(), SESSION_READ_ONLY      );
//  qof_session_begin( s, url.c_str(), SESSION_BREAK_LOCK     );

  std::cout << __FILE__ << ":" << __LINE__ << " error: " << qof_session_get_error( s ) << std::endl;

  if( qof_session_get_error( s ) != 0 )
  {
    std::cout << __FILE__ << ":" << __LINE__ << " error opening qof_session" << std::endl;
    return false;
  }

  std::cout << __FILE__ << ":" << __LINE__ << " qof_session_load" << std::endl;
  qof_session_load( s, NULL );

  std::cout << __FILE__ << ":" << __LINE__ << " error: " << qof_session_get_error( s ) << std::endl;

//  std::cout << __FILE__ << ":" << __LINE__ << " checking file path" << std::endl;
//  std::cout << __FILE__ << ":" << __LINE__ << " filePath:" << qof_session_get_file_path( s ) << std::endl;

#ifdef NEVER
  std::cout << __FILE__ << ":" << __LINE__ << " qof_session_get_book" << std::endl;
  auto book = qof_session_get_book( s );
  std::cout << __FILE__ << ":" << __LINE__ << " " << book << std::endl;

  std::cout << __FILE__ << ":" << __LINE__ << " gnc_book_get_root_account" << std::endl;
  auto rootAccount = gnc_book_get_root_account( book );
  std::cout << __FILE__ << ":" << __LINE__ << " rootAccount: " << rootAccount << std::endl;

  std::cout << __FILE__ << ":" << __LINE__ << " count:" << gnc_book_count_transactions( book ) << std::endl;

  auto guidString = guid_to_string( xaccAccountGetGUID( rootAccount ) );

  std::cout << __FILE__ << ":" << __LINE__ << " rootGuid: "    << guidString << std::endl;
  std::cout << __FILE__ << ":" << __LINE__ << " rootName: "    << xaccAccountGetName( rootAccount ) << std::endl;
  std::cout << __FILE__ << ":" << __LINE__ << " rootCode: "    << xaccAccountGetCode( rootAccount ) << std::endl;
  std::cout << __FILE__ << ":" << __LINE__ << " rootDesc: "    << xaccAccountGetDescription( rootAccount ) << std::endl;

  std::cout << __FILE__ << ":" << __LINE__ << " size: " << gnc_account_n_children( rootAccount ) << std::endl;
  for( int i=0; i< gnc_account_n_children( rootAccount ); i++ )
  {
    auto child = gnc_account_nth_child( rootAccount, i );
    auto guid = guid_to_string( xaccAccountGetGUID( child ) );

    std::cout << __FILE__ << ":" << __LINE__ << " -------------------------------------- " << std::endl;

    std::cout << __FILE__ << ":" << __LINE__ << " index:" << i << " guid: " << guid << " name: "    << xaccAccountGetName( child ) << std::endl;

  }

  std::cout << __FILE__ << ":" << __LINE__ << " qof_session_save" << std::endl;
  qof_session_save( s, NULL );

//  AccountList *accountList;
//  gnc_accounts_and_all_descendants( accountList );

//  auto account = gnc_account_lookup_by_name( nullptr, "Assets" );

//  std::cout << __FILE__ << ":" << __LINE__ << " " << std::endl;

//  std::cout << __FILE__ << ":" << __LINE__ << " " << g_list_length( &accountList ) << std::endl;

//  std::cout << __FILE__ << ":" << __LINE__ << " " << std::endl;

//  accountList.next;

#endif

  return isOpen();

} // endopen( const std::string & _path )

void
GCW::Dbo::GnuCash::Session::
closeGnuCash()
{
  std::cout << __FILE__ << ":" << __LINE__ << " qof_session_end" << std::endl;

  auto s = gnc_get_current_session();
  qof_session_end( s );

  gnc_engine_shutdown();
}


