#line 2 "src/CentralWidget.cpp"

#include <Wt/WMenuItem.h>
#include <Wt/WTabWidget.h>
#include <Wt/WText.h>
#include <Wt/WVBoxLayout.h>

#include "define.h"
#include "App.h"
#include "CentralWidget.h"

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
GCW::CentralWidget::
CentralWidget()
{
  addStyleClass( "CentralWidget" );

  /*
  ** Always use a layout
  **
  */
  auto lw = setLayout( std::make_unique< Wt::WVBoxLayout >() );

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
  ** Attach the accounts widget as a non-closeable-tab, so that the
  **  user can navigate around the accounts.
  **
  */
  {
    auto widget = std::make_unique< GCW::AccountsWidget >();
    m_accountsWidget = widget.get();
    tabWidget()-> addTab( std::move( widget ), TR( "gcw.AccountsWidget.tabName" ) );

    accountsWidget()-> doubleClicked().connect( this, &GCW::CentralWidget::openAccountRegister );
  }

} // endGCW::CentralWidget::CentralWidget()

int GCW::CentralWidget::
tabIndex( const std::string & _text )
{
  for( int i=0; i< tabWidget()-> count(); i++ )
    if( tabWidget()-> tabText( i ) == _text )
      return i;

  return -1;

} // endint GCW::CentralWidget::tabIndex( const std::string & _text )

void GCW::CentralWidget::
openAccountRegister( const std::string & _accountGuid )
{
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
    ** Open a new RegisterWidget tab that is connected to the account
    **
    */
    auto tab =
      tabWidget()->
        insertTab
        ( 1,
          std::make_unique< GCW::RegisterWidget >( _accountGuid ),
          accountItem-> name()
        );

    tab-> setCloseable( true );

  } // endif( tabIndex( _account-> name() ) == -1 )

  /*
  ** Go straight to the tab.
  **
  */
  tabWidget()-> setCurrentIndex( tabIndex( accountItem-> name() ) );

} // endvoid GCW::CentralWidget::openAccountRegister( const std::string & _accountGuid )

void GCW::CentralWidget::
test()
{
  if( auto registerWidget = dynamic_cast< GCW::RegisterWidget* >( tabWidget()-> widget( tabWidget()-> currentIndex() ) ) )
  {
    registerWidget-> test();
  }

} // endvoid GCW::CentralWidget::test()



