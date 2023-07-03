

#include <Wt/WTabWidget.h>
#include <Wt/WText.h>
#include <Wt/WVBoxLayout.h>

#include "CentralWidget.h"
#include "AccountsWidget.h"

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
GCW::CentralWidget::CentralWidget()
{
  auto lw = setLayout( std::make_unique< Wt::WVBoxLayout >() );

  m_tabWidget = lw-> addWidget( std::make_unique< Wt::WTabWidget >() );

  tabWidget()-> addTab( std::make_unique< GCW::AccountsWidget >(), "Accounts" );
  tabWidget()-> addTab( std::make_unique< Wt::WText >( "The Wine Depot" ), "Wine" );

} // endGCW::CentralWidget::CentralWidget()



