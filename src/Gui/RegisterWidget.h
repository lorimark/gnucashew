
#ifndef __GUI_REGISTERWIDGET_H___
#define __GUI_REGISTERWIDGET_H___

#include <Wt/Json/Object.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WStandardItem.h>
#include <Wt/WStandardItemModel.h>

#include "../Dbo/Session.h"
#include "../Dbo/Accounts.h"
#include "../Eng/RegisterModel.h"
#include "TableView.h"

namespace GCW {
  namespace Gui {

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
    using Model = GCW::Eng::RegisterModel;

    RegisterWidget( const std::string & _accountGuid );

    GCW::Gui::TableView      * tableView () { return m_tableView; }
    std::shared_ptr< Model >   model     () { return m_model;     }

    void test();

    Wt::Json::Object toJson() const;
    bool fromJson( const Wt::Json::Object & _jobj );

  private:

    void loadData();

    std::string m_accountGuid;
    std::shared_ptr< Model > m_model;
    GCW::Gui::TableView * m_tableView = nullptr;
    int m_clickedRow = -1;
    int m_clickedCol = -1;

};  // endclass RegisterWidget

  } // endnamespace Gui {
} // endnamespace GCW {

#endif // end __GUI_REGISTERWIDGET_H___


