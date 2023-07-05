#line 2 "src/CentralWidget.h"

#ifndef __CENTRALWIDGET_H___
#define __CENTRALWIDGET_H___

#include <Wt/WContainerWidget.h>
#include <Wt/WTabWidget.h>

#include "AccountsWidget.h"
#include "RegisterWidget.h"

namespace GCW {

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

    Wt::WTabWidget      * tabWidget()       { return m_tabWidget;      }
    GCW::AccountsWidget * accountsWidget()  { return m_accountsWidget; }

    void openAccountRegister( const std::string & _accountGuid );

    /*!
    ** \brief Index of Tab matching 'text'
    **
    ** Return the index of the tab with the
    **  matching text.
    **
    */
    int tabIndex( const std::string & _text );

  private:

    Wt::WTabWidget      * m_tabWidget      = nullptr;
    GCW::AccountsWidget * m_accountsWidget = nullptr;

}; // endclass CentralWidget

} // endnamespace GCW {

#endif // end#ifndef __CENTRALWIDGET_H___


