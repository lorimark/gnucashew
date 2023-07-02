#line 2 "src/GnuCashew.cpp"

#include <thread>
#include <ctime>
#include <iostream>
#include <fstream>

#include <Wt/Date/tz.h>
#include <Wt/WBootstrapTheme.h>
#include <Wt/WDate.h>
#include <Wt/WLocale.h>
#include <Wt/WMenu.h>
#include <Wt/WPopupMenu.h>
#include <Wt/WPushButton.h>
#include <Wt/WTabWidget.h>
#include <Wt/WText.h>
#include <Wt/WTime.h>
#include <Wt/WToolBar.h>
#include <Wt/WTreeTableNode.h>
#include <Wt/WVBoxLayout.h>
#include <Wt/WHBoxLayout.h>

#include "GnuCashew.h"

#include "AccountsWidget.h"



GCW::CentralWidget::CentralWidget()
{
  auto lw = setLayout( std::make_unique< Wt::WVBoxLayout >() );

  m_tabWidget = lw-> addWidget( std::make_unique< Wt::WTabWidget >() );

  tabWidget()-> addTab( std::make_unique< GCW::AccountsWidget >(), "Accounts" );
  tabWidget()-> addTab( std::make_unique< Wt::WText >( "The Wine Depot" ), "Wine" );

} // endGCW::CentralWidget::CentralWidget()





















GCW::MainWidget::MainWidget()
{
  auto lw = setLayout( std::make_unique< Wt::WVBoxLayout >() );

  m_navBar        = lw-> addWidget( std::make_unique< Wt::WNavigationBar   >()    );
  m_toolBar       = lw-> addWidget( std::make_unique< Wt::WToolBar         >()    );
  m_centralWidget = lw-> addWidget( std::make_unique< GCW::CentralWidget   >(), 1 );
  m_statusBar     = lw-> addWidget( std::make_unique< Wt::WContainerWidget >()    );

  /*
  ** Set up the navbar
  **
  */
  auto link = Wt::WLink("https://www.lorimarksolutions.com/gnucashew");
  link.setTarget( Wt::LinkTarget::NewWindow );
  navBar()-> setResponsive( true );
  navBar()-> addStyleClass( "navbar-light bg-light" );
  navBar()-> setTitle( "GCW", link );

  auto m_menu = navBar()-> addMenu( std::make_unique< Wt::WMenu >() );

  auto m_uFile = m_menu-> addItem( TR("gcw.mu.file") );
  auto popFile = std::make_unique< Wt::WPopupMenu >();
  popFile-> addItem( TR( "gcw.mu.file.new"        ) );
  popFile-> addItem( TR( "gcw.mu.file.open"       ) );
  popFile-> addItem( TR( "gcw.mu.file.import"     ) );
  popFile-> addSeparator();
  popFile-> addItem( TR( "gcw.mu.file.save"       ) );
  popFile-> addItem( TR( "gcw.mu.file.saveas"     ) );
  popFile-> addItem( TR( "gcw.mu.file.revert"     ) );
  popFile-> addSeparator();
  popFile-> addItem( TR( "gcw.mu.file.print"      ) );
  popFile-> addItem( TR( "gcw.mu.file.pagesetup"  ) );
  popFile-> addItem( TR( "gcw.mu.file.export"     ) );
  popFile-> addSeparator();
  popFile-> addItem( TR( "gcw.mu.file.properties" ) );
  popFile-> addSeparator();
  popFile-> addItem( TR( "gcw.mu.file.close"      ) );
  popFile-> addItem( TR( "gcw.mu.file.quit"       ) );
  m_uFile-> setMenu( std::move( popFile ) );

  auto m_uEdit = m_menu-> addItem( TR( "gcw.mu.edit" ) );
  auto popEdit = std::make_unique< Wt::WPopupMenu >();
  popEdit-> addItem( TR( "gcw.mu.edit.cut"             ) );
  popEdit-> addItem( TR( "gcw.mu.edit.copy"            ) );
  popEdit-> addItem( TR( "gcw.mu.edit.paste"           ) );
  popEdit-> addSeparator();
  popEdit-> addItem( TR( "gcw.mu.edit.edit"            ) );
  popEdit-> addItem( TR( "gcw.mu.edit.delete"          ) );
  popEdit-> addItem( TR( "gcw.mu.edit.find"            ) );
  popEdit-> addItem( TR( "gcw.mu.edit.cascade"         ) );
  popEdit-> addItem( TR( "gcw.mu.edit.renumber"        ) );
  popEdit-> addSeparator();
  popEdit-> addItem( TR( "gcw.mu.edit.openaccount"     ) );
  popEdit-> addItem( TR( "gcw.mu.edit.opensubaccounts" ) );
  m_uEdit-> setMenu( std::move( popEdit ) );

  auto m_uView = m_menu-> addItem( TR( "gcw.mu.view" ) );
  auto popView = std::make_unique< Wt::WPopupMenu >();
  popView-> addItem( TR( "gcw.mu.view.toolbar"       ) );
  popView-> addItem( TR( "gcw.mu.view.summarybar"    ) );
  popView-> addItem( TR( "gcw.mu.view.statusbar"     ) );
  popView-> addItem( TR( "gcw.mu.view.tabposition"   ) );
  popView-> addSeparator();
  popView-> addItem( TR( "gcw.mu.view.filterby"      ) );
  popView-> addSeparator();
  popView-> addItem( TR( "gcw.mu.view.refresh"       ) );
  popView-> addItem( TR( "gcw.mu.view.newaccounts"   ) );
  m_uView-> setMenu( std::move( popView ) );


  toolBar()-> addButton( std::make_unique< Wt::WPushButton >("save") );
  toolBar()-> addButton( std::make_unique< Wt::WPushButton >("close") );
  toolBar()-> addButton( std::make_unique< Wt::WPushButton >("new invoice") );
  toolBar()-> addButton( std::make_unique< Wt::WPushButton >("open") );
  toolBar()-> addButton( std::make_unique< Wt::WPushButton >("edit") );
  toolBar()-> addButton( std::make_unique< Wt::WPushButton >("new") );
  toolBar()-> addButton( std::make_unique< Wt::WPushButton >("delete") );

  statusBar()-> addNew< Wt::WText >( "status text" );

} // endGCW::MainWidget::MainWidget()

























GCW::App * GCW::app()
{
  return static_cast< GCW::App* >( Wt::WApplication::instance() );
}

GCW::App::App( const Wt::WEnvironment & env )
: Wt::WApplication( env )
{
//  session().open_gnucash( "/home/serveradmin/gnucash-dev.gnucash" );
  session().open_gnucash( "/home/serveradmin/LorimarkSolutions.gnucash" );

  /*
  ** Utilize the bootstrap theme.
  **
  */
  auto bootstrapTheme = std::make_shared<Wt::WBootstrapTheme>();
  bootstrapTheme-> setVersion( Wt::BootstrapVersion::v3 );
  bootstrapTheme-> setResponsive(true);
  setTheme( bootstrapTheme );
  useStyleSheet("resources/themes/bootstrap/3/bootstrap-theme.min.css");

  /*
  ** GnuCashew english language translations
  **
  */
  messageResourceBundle().use( appRoot() + "gcw_en" );

#ifdef NEVER
  /*
  ** set the date format to regular american.
  **
  */
  auto loc = locale();
  auto tz = date::locate_zone( environment().timeZoneName() );
  loc.setTimeZone( tz );
#endif

  /*
  ** Set a layout manager on the root widget so that everything can
  **  be laid out correctly.
  **
  */
  auto lw = root()-> setLayout( std::make_unique< Wt::WVBoxLayout >() );

  /*
  ** Build and install the main desktop widget.
  **
  */
  m_mainWidget = lw-> addWidget( std::make_unique< GCW::MainWidget >() );

} // endGnuCashew::App::App( const Wt::WEnvironment & env )


