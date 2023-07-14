
#include <Wt/WMenu.h>
#include <Wt/WPopupMenu.h>
#include <Wt/WPushButton.h>
#include <Wt/WVBoxLayout.h>

#include "define.h"
#include "MainWidget.h"

GCW::MainWidget::
MainWidget()
{
  addStyleClass( "MainWidget" );

  auto lw = setLayout( std::make_unique< Wt::WVBoxLayout >() );

  m_navBar        = lw-> addWidget( std::make_unique< Wt::WNavigationBar   >()    );
  m_toolBar       = lw-> addWidget( std::make_unique< Wt::WToolBar         >()    );
  m_centralWidget = lw-> addWidget( std::make_unique< GCW::CentralWidget   >(), 1 );
  m_statusBar     = lw-> addWidget( std::make_unique< Wt::WContainerWidget >()    );

  /*
  ** Set up the navbar
  **
  */
  auto link = Wt::WLink("https://gnucashew.lorimarksolutions.com/dox/html/index.html");
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

  auto m_uHelp = m_menu-> addItem( TR( "gcw.mu.help" ) );
  auto popHelp = std::make_unique< Wt::WPopupMenu >();
  popHelp-> addItem( TR( "gcw.mu.help.tutorial"      ) );
  popHelp-> addItem( TR( "gcw.mu.help.tip"           ) );
  popHelp-> addItem( TR( "gcw.mu.help.contents"      ) );
  popHelp-> addItem( TR( "gcw.mu.help.about"         ) );
  m_uHelp-> setMenu( std::move( popHelp ) );

  toolBar()-> addButton( std::make_unique< Wt::WPushButton >( TR( "gcw.tb.save"       ) ) );
  toolBar()-> addButton( std::make_unique< Wt::WPushButton >( TR( "gcw.tb.close"      ) ) );
  toolBar()-> addButton( std::make_unique< Wt::WPushButton >( TR( "gcw.tb.newInvoice" ) ) );
  toolBar()-> addButton( std::make_unique< Wt::WPushButton >( TR( "gcw.tb.open"       ) ) );

  {
    auto b = std::make_unique< Wt::WPushButton >( TR( "gcw.tb.edit" ) );
    auto e = b.get();
    toolBar()-> addButton( std::move(b) );
    e-> clicked().connect( [=](){ editSelectedAccount(); });
  }

  toolBar()-> addButton( std::make_unique< Wt::WPushButton >( TR( "gcw.tb.new"    ) ) );
  toolBar()-> addButton( std::make_unique< Wt::WPushButton >( TR( "gcw.tb.delete" ) ) );

  statusBar()-> addNew< Wt::WText >( "status bar" );

} // endGCW::MainWidget::MainWidget()

void GCW::MainWidget::
editSelectedAccount()
{
  centralWidget()-> accountsWidget()-> editSelectedAccount();

}

