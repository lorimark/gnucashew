#line 2 "src/Gui/AccountsWidget.h"

#ifndef __GUI_ACCOUNTSWIDGET_H___
#define __GUI_ACCOUNTSWIDGET_H___

#include <Wt/Json/Object.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WTreeView.h>
#include <Wt/WStandardItem.h>
#include <Wt/WStandardItemModel.h>

#include "../Dbo/Session.h"
#include "../Dbo/Accounts.h"

namespace GCW {
  namespace Gui {

/*!
** \brief AccountsWidget
**
** The AccountsWidget is a widget that displays all of the accounts
**  within the system.  It displays the accounts in a tree-like format
**  with multiple columns representing other field values such as;
**  'notes', 'tax info', 'balance remaining' and so on.  The view is
**  sortable and browsable, and facilitates gaining access to the
**  individual account registers.
**
*/
class AccountsWidget
: public Wt::WContainerWidget
{
  class ColDef
  {
    public:

      std::string m_name;

  };

  class Model
  : public Wt::WStandardItemModel
  {
    public:

      void load();

    private:

      void load( Wt::WStandardItem * _treeItem, GCW::Dbo::Accounts::Item::Ptr _parentAccount );
  };

  public:

    AccountsWidget();

    std::shared_ptr< Model >   model () const { return m_model; }
    Wt::WTreeView            * view  () const { return m_view;  }

    Wt::Signal< std::string > & doubleClicked() { return m_doubleClicked; }

    std::string selectedAccount() const;

    void editAccount( const std::string & _accountGuid );
    void editSelectedAccount();

    Wt::Json::Object toJson() const;
    bool fromJson( const Wt::Json::Object & _jobj );

    void test();

  private:

    void setModel();
    void doubleClicked( const Wt::WModelIndex & index, const Wt::WMouseEvent & event );

    Wt::WTreeView            * m_view = nullptr;
    std::shared_ptr< Model >   m_model;
    std::vector< std::string > m_columns;
    Wt::Signal< std::string >  m_doubleClicked;

};  // endclass AccountsWidget

  } // endnamespace Gui {
} // endnamespace GCW {

#endif // end __GUI_ACCOUNTSWIDGET_H___


