
#ifndef __REGISTERWIDGET_H___
#define __REGISTERWIDGET_H___

#include <Wt/WContainerWidget.h>
#include <Wt/WTableView.h>
#include <Wt/WStandardItem.h>
#include <Wt/WStandardItemModel.h>

#include "Dbo/Session.h"
#include "Dbo/Account.h"

namespace GCW {

/*!
** \brief RegisterWidget
**
** The RegisterWidget is a widget that displays a single account transactions
**  within the system.  It displays the account transactions in a table-like format
**  with multiple columns representing other field values such as;
**  date, check-number, description... and so on.
**
** The register view also grants the user editing abilities.
**
*/
class RegisterWidget
: public Wt::WContainerWidget
{
  class Model
  : public Wt::WStandardItemModel
  {
    public:

      void load();

    private:

      void load( Wt::WStandardItem * _treeItem, GCW::Dbo::Account::Item::Ptr _parentAccount );
  };

  public:

    RegisterWidget( const std::string & _accountGuid );

    Wt::WTableView * tableView() { return m_tableView; }

  private:

    void setModel();

    Wt::WTableView * m_tableView = nullptr;
    std::vector< std::string > m_columns;

};  // endclass RegisterWidget

} // endnamespace GCW {

#endif // end __REGISTERWIDGET_H___


