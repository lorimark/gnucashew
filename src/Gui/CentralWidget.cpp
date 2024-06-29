#line 2 "src/Gui/CentralWidget.cpp"

#include <Wt/WMenuItem.h>
#include <Wt/WTabWidget.h>
#include <Wt/WText.h>
#include <Wt/WVBoxLayout.h>

#include "../define.h"
#include "../App.h"
#include "BillPay/MainWidget.h"
#include "CentralWidget.h"
#include "CustomerReportWidget.h"
#include "CustomersWidget.h"
#include "SlotsWidget.h"

/*!
** \brief Central Widget
**
** The CentralWidget is the component on the main user interface that
**  acts as a tab widget for being able to select from one tab to
**  another.
**
** The first tab is usually the 'accounts' tab.  This tab normally cannot
**  be closed.  The other tabs open and close on reports or other views
**  as they are requested.
**
*/
GCW::Gui::CentralWidget::
CentralWidget()
{
  addStyleClass( "CentralWidget" );

  /*
  ** Always use a layout
  **
  */
  auto lw = setLayout( std::make_unique< Wt::WVBoxLayout >() );
  lw-> setSpacing( 0 );

  /*
  ** Build the tab-widget
  **
  */
  m_tabWidget = lw-> addWidget( std::make_unique< Wt::WTabWidget >() );

  /*
  ** This procedure will ~delete~ the tab and its contents when the tab
  **  is closed.  Normally, on 'close' the tab is only 'hidden' and not
  **  actually deleted.  This prevents the widgets from remaining in
  **  memory when the tabs are closed.
  **
  */
  tabWidget()->
    tabClosed().connect( [=]( int tabIndex )
    {
      tabWidget()-> removeTab( tabWidget()-> widget( tabIndex ) );
    });

  /*
  ** Attach the accounts widget tree view as a non-closeable-tab, so that the
  **  user can navigate around the accounts.
  **
  */
  {
    auto widget = std::make_unique< GCW::Gui::AccountsTreeView >();
    m_accountsWidget = widget.get();
    tabWidget()-> addTab( std::move( widget ), TR( "gcw.AccountsTreeView.tabName" ) );

    accountsWidget()-> doubleClicked().connect( this, &GCW::Gui::CentralWidget::open_AccountRegister );
  }

} // endGCW::CentralWidget::CentralWidget()

int GCW::Gui::CentralWidget::
tabIndex( const std::string & _text )
{
  for( int i=0; i< tabWidget()-> count(); i++ )
    if( tabWidget()-> tabText( i ) == _text )
      return i;

  return -1;

} // endint GCW::Gui::CentralWidget::tabIndex( const std::string & _text )

void GCW::Gui::CentralWidget::
open_AccountRegister( const std::string & _accountGuid )
{
  Wt::Dbo::Transaction t( GCW::app()-> gnucash_session() );

  /*
  ** Grab the account so we can fetch things from it.
  **
  */
  auto accountItem = GCW::Dbo::Accounts::byGuid( _accountGuid );

  /*
  ** If we didn't get an account (this shouldn't happen) then
  **  there's nothing for us to do... perhaps pop an error dialog
  **  or something.
  **
  */
  if( !accountItem )
    return;

  /*
  ** See if this tab exists, if not, then add it.
  **
  */
  if( tabIndex( accountItem-> name() ) == -1 )
  {
    /*
    ** Open a new AccountRegister tab that is connected to the account
    **
    */
    auto tab =
      tabWidget()->
        insertTab
        ( 1,
          std::make_unique< GCW::Gui::AccountRegister >( _accountGuid ),
          accountItem-> name()
        );

    tabWidget()-> setTabToolTip( 1, fullName( accountItem ) );

    tab-> setCloseable( true );

  } // endif( tabIndex( _account-> name() ) == -1 )

  /*
  ** Go straight to the tab.
  **
  */
  tabWidget()-> setCurrentIndex( tabIndex( accountItem-> name() ) );

} // endvoid GCW::Gui::CentralWidget::open_AccountRegister( const std::string & _accountGuid )

void GCW::Gui::CentralWidget::
open_CustomerReportWidget( const std::string & _customerGuid )
{
  /*
  ** Grab the account so we can fetch things from it.
  **
  */
  auto customerItem = GCW::Dbo::Customers::byGuid( _customerGuid );

  /*
  ** If we didn't get a customer (this shouldn't happen) then
  **  there's nothing for us to do... perhaps pop an error dialog
  **  or something.
  **
  */
  if( !customerItem )
    return;

  /*
  ** build a tab name
  **
  */
  auto tabName = TR8( "gcw.cw.tabName.Customer" ) + ": " + customerItem-> name();

  /*
  ** See if this tab exists, if not, then add it.
  **
  */
  if( tabIndex( tabName ) == -1 )
  {
    /*
    ** Open a new CustomerReportWidget tab that is connected to the account.
    **  When inserting the tab, insert it immediately after the currently
    **  selected customer.
    **
    */
    auto tab =
      tabWidget()->
        insertTab
        ( tabWidget()-> currentIndex() + 1,
          std::make_unique< GCW::Gui::CustomerReportWidget >( _customerGuid ),
          tabName
        );

    tab-> setCloseable( true );

  } // endif( tabIndex( _account-> name() ) == -1 )

  /*
  ** Go straight to the tab.
  **
  */
  tabWidget()-> setCurrentIndex( tabIndex( tabName ) );

} // endvoid GCW::Gui::CentralWidget::open_CustomerReportWidget( const std::string & _customerGuid )

void
GCW::Gui::CentralWidget::
open_CustomersWidget()
{
  auto tabName = TR8( "gcw.cw.tabName.Customers" );

  /*
  ** See if this tab exists, if not, then add it.
  **
  */
  if( tabIndex( tabName ) == -1 )
  {
    /*
    ** Open a new CustomersWidget tab that is connected to the account
    **
    */
    auto widget = std::make_unique< GCW::Gui::CustomersWidget >();
    auto w = widget.get();

    /*
    ** Double Clicking on a customer causes the customer report
    **  widget to open.
    **
    */
    w->
      doubleClicked().connect( [=]( const std::string & _customerGuid )
      {
        open_CustomerReportWidget( _customerGuid );
      });

    auto tab =
      tabWidget()->
        insertTab
        ( 1,
          std::move( widget ),
          tabName
        );

    tab-> setCloseable( true );

  } // endif( tabIndex( _account-> name() ) == -1 )

  /*
  ** Go straight to the tab.
  **
  */
  tabWidget()-> setCurrentIndex( tabIndex( tabName ) );

} // endopen_CustomersWidget()

void
GCW::Gui::CentralWidget::
open_BillPayWidget()
{
  auto tabName = TR8( "gcw.cw.tabName.BillPay" );

  /*
  ** See if this tab exists, if not, then add it.
  **
  */
  if( tabIndex( tabName ) == -1 )
  {
    /*
    ** Open a new CustomersWidget tab that is connected to the account
    **
    */
    std::cout << __FILE__ << ":" << __LINE__ << " " << std::endl;

    auto widget = std::make_unique< GCW::Gui::BillPay::MainWidget >();
    std::cout << __FILE__ << ":" << __LINE__ << " " << std::endl;

    auto w = widget.get();

    auto tab =
      tabWidget()->
        insertTab
        ( 1,
          std::move( widget ),
          tabName
        );

    tab-> setCloseable( true );

  } // endif( tabIndex( _account-> name() ) == -1 )

  /*
  ** Go straight to the tab.
  **
  */
  tabWidget()-> setCurrentIndex( tabIndex( tabName ) );

} // endopen_BillPayWidget()

void
GCW::Gui::CentralWidget::
open_SlotsWidget()
{
  auto tabName = "slots";

  /*
  ** See if this tab exists, if not, then add it.
  **
  */
  if( tabIndex( tabName ) == -1 )
  {
    /*
    ** Open a new SlotsWidget tab that is connected to the account
    **
    */
    std::cout << __FILE__ << ":" << __LINE__ << " " << std::endl;

    auto widget = std::make_unique< GCW::Gui::SlotsWidget >();
    std::cout << __FILE__ << ":" << __LINE__ << " " << std::endl;

    auto w = widget.get();

    std::cout << __FILE__ << ":" << __LINE__ << " " << std::endl;

    auto tab =
      tabWidget()->
        insertTab
        ( 1,
          std::move( widget ),
          tabName
        );

    std::cout << __FILE__ << ":" << __LINE__ << " " << std::endl;

    tab-> setCloseable( true );

    std::cout << __FILE__ << ":" << __LINE__ << " " << std::endl;

  } // endif( tabIndex( _account-> name() ) == -1 )

  /*
  ** Go straight to the tab.
  **
  */
  tabWidget()-> setCurrentIndex( tabIndex( tabName ) );

} // endopen_SlotsWidget()

void GCW::Gui::CentralWidget::
test()
{
  if( auto registerWidget = dynamic_cast< GCW::Gui::AccountRegister* >( tabWidget()-> widget( tabWidget()-> currentIndex() ) ) )
  {
    registerWidget-> test();
  }

} // endvoid GCW::Gui::CentralWidget::test()



