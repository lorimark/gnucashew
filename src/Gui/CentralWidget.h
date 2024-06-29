#line 2 "src/Gui/CentralWidget.h"

#ifndef __GUI_CENTRALWIDGET_H___
#define __GUI_CENTRALWIDGET_H___

#include <Wt/WContainerWidget.h>
#include <Wt/WTabWidget.h>

#include "AccountsTreeView.h"
#include "CustomersWidget.h"
#include "AccountRegister.h"

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

    Wt::WTabWidget            * tabWidget()        { return m_tabWidget;       }
    GCW::Gui::AccountsTreeView  * accountsWidget()   { return m_accountsWidget;  }
    GCW::Gui::CustomersWidget * customersWidget()  { return m_customersWidget; }

    void open_AccountRegister( const std::string & _accountGuid );
    void open_CustomersWidget();
    void open_CustomerReportWidget( const std::string & _customerGuid );
    void open_BillPayWidget();
    void open_SlotsWidget();

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

    Wt::WTabWidget            * m_tabWidget       = nullptr;
    GCW::Gui::AccountsTreeView  * m_accountsWidget  = nullptr;
    GCW::Gui::CustomersWidget * m_customersWidget = nullptr;

}; // endclass CentralWidget

  } // endnamespace Gui {
} // endnamespace GCW {

#endif // end#ifndef __CENTRALWIDGET_H___


