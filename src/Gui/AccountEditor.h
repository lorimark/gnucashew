#line 2 "src/Gui/AccountEditor.h"

#ifndef __GUI_ACCOUNTEDITOR_H___
#define __GUI_ACCOUNTEDITOR_H___

#include <Wt/WContainerWidget.h>
#include <Wt/WDialog.h>
#include <Wt/WTabWidget.h>
#include <Wt/WTemplateFormView.h>

#include "../GnuCashew.h"

namespace GCW {
  namespace Gui {

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

  } //endnamespace Gui {
} // endnamespace GCW {

#endif // end __GUI_ACCOUNTSWIDGET_H___



