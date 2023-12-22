#line 2 "src/Dbo/Gnucash.cpp"

#include <iostream>

#include <gnucash/gnc-session.h>
#include <gnucash/Account.h>
#include <gnucash/Transaction.h>
#include <gnucash/qofsession.h>

#include "Gnucash.h"
#include "../Glb/Core.h"

#define TESTFILE "gnucash-dev.gnucash"

void
GCW::Dbo::GnuCash::Session::
openGnucash()
{
  std::cout << __FILE__ << ":" << __LINE__ << " " << path() << std::endl;

//  auto userdata_migration_msg = gnc_filepath_init();
  gnc_engine_init( 0, nullptr );
  auto s = gnc_get_current_session(); // qof_session_new( qof_book_new() );

  std::cout << __FILE__ << ":" << __LINE__ << " qof_session_begin" << std::endl;
  std::string testurl = "sqlite3://" + path();
  qof_session_begin( s, testurl.c_str(), SESSION_NORMAL_OPEN );
//  qof_session_begin( s, testurl.c_str(), SESSION_BREAK_LOCK );

  std::cout << __FILE__ << ":" << __LINE__ << " " << qof_session_get_error( s ) << std::endl;

  std::cout << __FILE__ << ":" << __LINE__ << " qof_session_load" << std::endl;
  qof_session_load( s, NULL );

  std::cout << __FILE__ << ":" << __LINE__ << " checking file path" << std::endl;
//  std::cout << __FILE__ << ":" << __LINE__ << " filePath:" << qof_session_get_file_path( s ) << std::endl;

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
    auto guid = guid_to_string( xaccAccountGetGUID( rootAccount ) );

    std::cout << __FILE__ << ":" << __LINE__ << " -------------------------------------- " << std::endl;

    std::cout << __FILE__ << ":" << __LINE__ << " index:" << i << std::endl;
    std::cout << __FILE__ << ":" << __LINE__ << " rootGuid: "    << guid << std::endl;
    std::cout << __FILE__ << ":" << __LINE__ << " rootName: "    << xaccAccountGetName( child ) << std::endl;
    std::cout << __FILE__ << ":" << __LINE__ << " rootCode: "    << xaccAccountGetCode( child ) << std::endl;
    std::cout << __FILE__ << ":" << __LINE__ << " rootDesc: "    << xaccAccountGetDescription( child ) << std::endl;

  }

  std::cout << __FILE__ << ":" << __LINE__ << " qof_session_save" << std::endl;
  qof_session_save( s, NULL );

  std::cout << __FILE__ << ":" << __LINE__ << " qof_session_end" << std::endl;
  qof_session_end( s );

//  AccountList *accountList;
//  gnc_accounts_and_all_descendants( accountList );

//  auto account = gnc_account_lookup_by_name( nullptr, "Assets" );

//  std::cout << __FILE__ << ":" << __LINE__ << " " << std::endl;

//  std::cout << __FILE__ << ":" << __LINE__ << " " << g_list_length( &accountList ) << std::endl;

//  std::cout << __FILE__ << ":" << __LINE__ << " " << std::endl;

//  accountList.next;

  gnc_engine_shutdown();

} // endopenGnucash()



