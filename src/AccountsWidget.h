
#ifndef __ACCOUNTSWIDGET_H___
#define __ACCOUNTSWIDGET_H___

#include <Wt/WContainerWidget.h>
#include <Wt/WTreeTable.h>
#include <Wt/WStandardItem.h>
#include <Wt/WStandardItemModel.h>

#include "Dbo/Session.h"
#include "Dbo/Account.h"

namespace GCW {

/*
**  this view uses the model based 'WTreeView'.  While the view works,
**   the 'header' in the view will 'scroll away' when scrolling vertically.
**   so, we're dumping this widget and are going to try the 'WTreeTable'
**   widget.
*/

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

      void load( Wt::WStandardItem * _treeItem, GCW::Dbo::Account::Ptr _parentAccount );
  };

  public:

    AccountsWidget();

    Wt::WTreeView * treeView() { return m_treeView; }

  private:

    void setModel();

    Wt::WTreeView * m_treeView = nullptr;
    std::vector< std::string > m_columns;

};  // endclass AccountsWidget

} // endnamespace GCW {

#endif // end __ACCOUNTSWIDGET_H___


