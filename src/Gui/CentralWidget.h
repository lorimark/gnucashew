#line 2 "src/CentralWidget.h"

#ifndef __GUI_CENTRALWIDGET_H___
#define __GUI_CENTRALWIDGET_H___

#include <Wt/WContainerWidget.h>
#include <Wt/WTabWidget.h>

#include "AccountsWidget.h"
#include "RegisterWidget.h"

namespace GCW {
  namespace Gui {

/*!
** \brief Main UI Central Widget
**
** This widget acts as a 'tab widget manager' for controlling
**  what views (widgets) are presented to the user in the main
**  central display widget.  The central widget fills up the
**  lions share of the view window.
**
*/
class CentralWidget
: public Wt::WContainerWidget
{
  public:

    CentralWidget();

    Wt::WTabWidget           * tabWidget()       { return m_tabWidget;      }
    GCW::Gui::AccountsWidget * accountsWidget()  { return m_accountsWidget; }

    void openAccountRegister( const std::string & _accountGuid );

    /*!
    ** \brief Index of Tab matching 'text'
    **
    ** Return the index of the tab with the
    **  matching text.
    **
    */
    int tabIndex( const std::string & _text );

    void test();

  private:

    Wt::WTabWidget           * m_tabWidget      = nullptr;
    GCW::Gui::AccountsWidget * m_accountsWidget = nullptr;

}; // endclass CentralWidget

  } // endnamespace Gui {
} // endnamespace GCW {

#endif // end#ifndef __CENTRALWIDGET_H___


