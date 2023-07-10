
#ifndef __REGISTERWIDGET_H___
#define __REGISTERWIDGET_H___

#include <Wt/WContainerWidget.h>
#include <Wt/WStandardItem.h>
#include <Wt/WStandardItemModel.h>

#include "Dbo/Session.h"
#include "Dbo/Accounts.h"
#include "TableView.h"

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


  public:

  class Model
  : public Wt::WStandardItemModel
  {
    using RowItem = std::vector< std::unique_ptr< Wt::WStandardItem > >;

    public:

      Model( const std::string & _accountGuid );

      void refreshFromDisk();

      RowItem makeRow( const std::string & _splitGuid );
/*
balance
  balance limit
  balance (Period)
  balance usd
  cleared
  cleared usd
  fiture minimum
  future minimum usa
  last num
  last reconcile date
  opening balance
  present
  present usd
  reconciled reconciled usd
  total total period
  total usd
*/

    private:

      std::string m_accountGuid;

  }; // endclass Model

    RegisterWidget( const std::string & _accountGuid );

    GCW::TableView           * tableView () { return m_tableView; }
    std::shared_ptr< Model >   model     () { return m_model;     }

  private:

    void loadData();

    std::string m_accountGuid;
    std::shared_ptr< Model > m_model;
    GCW::TableView * m_tableView = nullptr;

};  // endclass RegisterWidget

} // endnamespace GCW {

#endif // end __REGISTERWIDGET_H___


