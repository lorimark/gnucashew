#line 2 "src/MainWidget.h"

#ifndef __MAINWIDGET_H___
#define __MAINWIDGET_H___

#include <Wt/WContainerWidget.h>
#include <Wt/WNavigationBar.h>
#include <Wt/WToolBar.h>

#include "CentralWidget.h"

namespace GCW {

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

} // endnamespace GCW {


#endif // #ifndef __MAINWIDGET_H___


