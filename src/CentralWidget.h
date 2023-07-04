

#ifndef __CENTRALWIDGET_H___
#define __CENTRALWIDGET_H___

#include <Wt/WContainerWidget.h>
#include <Wt/WTabWidget.h>

#include "AccountsWidget.h"
#include "RegisterWidget.h"

namespace GCW {

class CentralWidget
: public Wt::WContainerWidget
{
  public:

    CentralWidget();

    Wt::WTabWidget      * tabWidget()       { return m_tabWidget;      }
    GCW::AccountsWidget * accountsWidget()  { return m_accountsWidget; }

    void openAccountRegister( const std::string & _accountGuid );

    int tabIndex( const std::string & _text );

  private:

    Wt::WTabWidget      * m_tabWidget      = nullptr;
    GCW::AccountsWidget * m_accountsWidget = nullptr;

}; // endclass CentralWidget


} // endnamespace GCW {


#endif // end#ifndef __CENTRALWIDGET_H___


