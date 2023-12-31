#line 2 "src/Gui/AccountRegister.h"

#ifndef __GUI_ACCOUNTREGISTER_H___
#define __GUI_ACCOUNTREGISTER_H___

#include <Wt/Json/Object.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WSortFilterProxyModel.h>
#include <Wt/WStandardItem.h>
#include <Wt/WStandardItemModel.h>

#include "../GnuCashew.h"
//#include "../Dbo/Session.h"
#include "../Dbo/Accounts.h"
#include "../Eng/AccountRegisterModel.h"
#include "TableView.h"


namespace GCW {

  // FIXME
  // not sure why I need to forward-declare this class... it's
  //  right up there in the header
  namespace Eng { class AccountRegisterModel; }

  namespace Gui {

/*!
** \brief AccountRegister
**
** The AccountRegister is a widget that displays transactions from a single account
**  within the system.  It displays the account transactions in a table-like format
**  with multiple columns representing other field values such as;
**  date, check-number, description... and so on.
**
** At present this widget does only a quick 'view-only' of an account.  Editable fields
**  are available when a transaction is ~not~ reconciled.  If a transaction is reconciled,
**  then clicking on the row just causes the whole row to highlight.
**
** The goal here is to make this widget behave just as closely as possible to the
**  GnuCash interface, because theirs is very slick and makes navigating the register
**  a mouse-not-necessary affair.  One of the problems with web-based interfaces, however,
**  is trapping keys completely, such as the ~tab~ key.  So... we'll see how this
**  goes!
**
** The register view also grants the user editing abilities.  When the user clicks on
**  fields that are ~not~ reconciled, and appropriate 'editor' will open.  The editor
**  depends on the column selected, and is generated by the column Delegates.
**
*/
class AccountRegister
: public Wt::WContainerWidget
{
  private:

    class StatusBar
    : public Wt::WContainerWidget
    {
      public:
        StatusBar();

        void setPresent   ( GCW_NUMERIC _value );
        void setFuture    ( GCW_NUMERIC _value );
        void setCleared   ( GCW_NUMERIC _value );
        void setReconciled( GCW_NUMERIC _value );
        void setProjected ( GCW_NUMERIC _value );

        Wt::WText * m_present    = nullptr;
        Wt::WText * m_future     = nullptr;
        Wt::WText * m_cleared    = nullptr;
        Wt::WText * m_reconciled = nullptr;
        Wt::WText * m_projected  = nullptr;

    }; // endclass StatusBar

  public:

    /*!
    ** \brief Model
    **
    ** An alias for the Model in case this interface changes.
    **
    */
    using Model = GCW::Eng::AccountRegisterModel;

    /*!
    ** \brief Constructor
    **
    ** A register must be opened to a specific AccountGuid.  If the AccountGuid
    **  is blank or cannot be found (why would that be?) then the register simply
    **  opens, allows registry entries, but has no place to save them (dangerous?).
    **
    */
    AccountRegister( const std::string & _accountGuid );

    /*!
    ** \brief Table View
    **
    **
    **
    */
    GCW::Gui::TableView * tableView () { return m_tableView; }

    std::shared_ptr< Model > model () { return m_model; }

    StatusBar * statusBar() { return m_statusBar; }

    void test();

    /*!
    ** \brief Create View Properties JSON Object
    **
    ** This will pack all of the Widget display variables in to a JSON
    **  object for restoration later.
    **
    ** \sa fromJson()
    **
    */
    Wt::Json::Object toJson() const;

    /*!
    ** \brief Set View Properties from JSON Object
    **
    ** This accepts a JSON object containing all of the view properties
    **  and applys them to the view, essentially restoring it
    **  to its previous layout.
    **
    */
    bool fromJson( const Wt::Json::Object & _jobj );

  private:

    void loadData();

    std::string                m_accountGuid;
    std::shared_ptr< Model >   m_model;
    GCW::Gui::TableView      * m_tableView = nullptr;
    StatusBar                * m_statusBar = nullptr;
    int                        m_clickedRow = -1;
    int                        m_clickedCol = -1;

};  // endclass AccountRegister

  } // endnamespace Gui {
} // endnamespace GCW {

#endif // end __GUI_ACCOUNTREGISTER_H___


