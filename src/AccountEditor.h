
#ifndef __ACCOUNTEDITOR_H___
#define __ACCOUNTEDITOR_H___

#include <Wt/WContainerWidget.h>
#include <Wt/WDialog.h>
#include <Wt/WTabWidget.h>
#include <Wt/WTemplateFormView.h>

#include "GnuCashew.h"

namespace GCW {

/*!
** \brief Account Editor
**
*/
class AccountEditor
: public Wt::WContainerWidget
{
  public:

    AccountEditor( const std::string & _accountGuid );

    Wt::WTabWidget * tabWidget() { return m_tabWidget; }

  private:

    std::string m_accountGuid;
    Wt::WTabWidget        * m_tabWidget = nullptr;
    Wt::WTemplateFormView * m_tab1      = nullptr;
    Wt::WTemplateFormView * m_tab2      = nullptr;

}; // endclass AccountEditor

class AccountEditorDialog
: public Wt::WDialog
{
  public:

    AccountEditorDialog( const std::string & _accountGuid );

  private:

}; // endclass AccountEditorDialog

} // endnamespace GCW {

#endif // end __ACCOUNTSWIDGET_H___



