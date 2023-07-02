#line 2 "src/GnuCashew.h"

#ifndef __GNUCASHEW_H___
#define __GNUCASHEW_H___

#include <Wt/WApplication.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WEnvironment.h>
#include <Wt/WNavigationBar.h>
#include <Wt/WStandardItem.h>
#include <Wt/WStandardItemModel.h>
#include <Wt/WTreeView.h>

#include "Dbo/Session.h"

#define TR(X) Wt::WString::tr(X)
#define TR8(X) TR(X).toUTF8()

namespace GCW {

class CentralWidget
: public Wt::WContainerWidget
{
  public:

    CentralWidget();

    const Wt::WTabWidget * tabWidget() const { return m_tabWidget; }
          Wt::WTabWidget * tabWidget()       { return m_tabWidget; }

  private:

    Wt::WTabWidget * m_tabWidget = nullptr;

}; // endclass CentralWidget

/*
** This is the GnuCashew main site container.  It contains
**  a top-line menu, tool bar, bottom-line status bar, and
**  the main central workspace area for the account tree view
**  and other panes
**
*/
class MainWidget
: public Wt::WContainerWidget
{
  public:

    MainWidget();

    const Wt::WNavigationBar   * navBar        () const { return m_navBar;        }
          Wt::WNavigationBar   * navBar        ()       { return m_navBar;        }
    const Wt::WToolBar         * toolBar       () const { return m_toolBar;       }
          Wt::WToolBar         * toolBar       ()       { return m_toolBar;       }
    const Wt::WContainerWidget * statusBar     () const { return m_statusBar;     }
          Wt::WContainerWidget * statusBar     ()       { return m_statusBar;     }
    const GCW::CentralWidget   * centralWidget () const { return m_centralWidget; }
          GCW::CentralWidget   * centralWidget ()       { return m_centralWidget; }

  private:

    Wt::WNavigationBar   * m_navBar        = nullptr;
    Wt::WToolBar         * m_toolBar       = nullptr;
    Wt::WContainerWidget * m_statusBar     = nullptr;
    GCW::CentralWidget   * m_centralWidget = nullptr;

}; // endclass MainWidget

class App
: public Wt::WApplication
{
  public:

    App( const Wt::WEnvironment & env );

    Dbo::Session & session    () { return m_session;    }
    MainWidget   * mainWidget () { return m_mainWidget; }

  private:

    Dbo::Session   m_session;
    MainWidget   * m_mainWidget = nullptr;

}; // endclass App

App * app();

} // endnamespace GCW {

#endif // end#ifndef __GNUCASHEW_H___


