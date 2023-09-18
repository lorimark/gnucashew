#line 2 "src/Gui/BillPay/EditWidget.cpp"

#include <Wt/WVBoxLayout.h>

#include "BillPay.h"

#define FN_DUEDAY    "dueDay"
#define FN_MINIMUM   "minimum"
#define FN_BUDGET    "budget"
#define FN_NICKNAME  "nickname"
#define FN_GROUP     "group"
#define FN_LIMIT     "limit"
#define FN_ACTUAL    "actual"
#define FN_AP        "ap"
#define FN_ISACTIVE  "isActive"
#define FN_ISVISIBLE "isVisible"
#define FN_AUTOPAY   "autoPay"
#define FN_PAYNOW    "payNow"
#define FN_LAST4     "last4"
#define FN_NOTE      "note"

namespace {

std::string toString( int _value )
{
  std::string retVal = std::to_string( _value );

  if( retVal.length() == 1 )
    retVal = "0" + retVal;

  return retVal;

} // endstd::string toString( int _value )

} // endnamespace {

GCW::Gui::BillPay::EditWidget::ComboBox::
ComboBox()
{

  /*
  ** Populate the fullName of the account in to the combo box
  **  selection items.
  **
  */
  Wt::Dbo::Transaction t( GCW::app()-> gnucash_session() );
  for( auto accountItem : GCW::Dbo::Accounts::allAccounts() )
  {
    /*
    ** Placehold accounts are not available here
    **
    */
    if( accountItem-> placeHolder() != 0 )
      continue;

    addItem( GCW::Dbo::Accounts::fullName( accountItem-> guid() ) );

  } // endfor( auto accountItem : GCW::Dbo::Accounts::allAccounts() )

} // endComboBox()


GCW::Gui::BillPay::EditWidget::
EditWidget( const std::string & _accountGuid )
: Wt::WContainerWidget(),
  m_accountGuid( _accountGuid )
{
  std::cout << __FILE__ << ":" << __LINE__ << " " << _accountGuid << std::endl;

  addStyleClass( "BillPay_EditWidget" );

  auto lw = setLayout( std::make_unique< Wt::WVBoxLayout >() );

  auto templtMain =
    lw->
    addWidget
    (
     std::make_unique< Wt::WTemplate >( TR("gcw_gui.billpay.editwidget.form.main") )
    );

  m_pbSave    = templtMain-> bindNew< Wt::WPushButton >( "save"      , "save"       );
  m_pbCancel  = templtMain-> bindNew< Wt::WPushButton >( "cancel"    , "cancel"     );
  m_key       = templtMain-> bindNew< ComboBox        >( "key"                      );
  m_dueDay    = templtMain-> bindNew< Wt::WLineEdit   >( "dueDay"                   );
  m_minimum   = templtMain-> bindNew< Wt::WLineEdit   >( "minimum"                  );
  m_budget    = templtMain-> bindNew< Wt::WLineEdit   >( "budget"                   );
  m_nickname  = templtMain-> bindNew< Wt::WLineEdit   >( "nickname"                 );
  m_group     = templtMain-> bindNew< Wt::WLineEdit   >( "group"                    );
  m_limit     = templtMain-> bindNew< Wt::WLineEdit   >( "limit"                    );
  m_actual    = templtMain-> bindNew< Wt::WLineEdit   >( "actual"                   );
  m_ap        = templtMain-> bindNew< Wt::WCheckBox   >( "ap"        , "AP"         );
  m_isActive  = templtMain-> bindNew< Wt::WCheckBox   >( "isActive"  , "is Active"  );
  m_isVisible = templtMain-> bindNew< Wt::WCheckBox   >( "isVisible" , "is Visible" );
  m_autoPay   = templtMain-> bindNew< Wt::WCheckBox   >( "autoPay"   , "Auto Pay"   );
  m_payNow    = templtMain-> bindNew< Wt::WCheckBox   >( "payNow"    , "Pay Now"    );

  m_tabWidget = templtMain-> bindNew< Wt::WTabWidget >( "tabWidget" );

  Wt::WTemplate * templtPayment;
  {
    auto u_ = std::make_unique< Wt::WTemplate >( TR( "gcw_gui.billpayeditor.form.tab1" ) );
    templtPayment = u_.get();
    m_tabWidget-> addTab( std::move( u_ ), "Payment" );

    for( int cb = 0; cb < 12; cb++ )
      m_cbx.push_back( templtPayment-> bindNew< Wt::WCheckBox >( "cb" + toString( cb+1 ), toString( cb+1 ) ) );

    m_last4 = templtPayment-> bindNew< Wt::WLineEdit >( "last4"          );
    m_note  = templtPayment-> bindNew< Wt::WTextArea >( "note"           );
    m_label = templtPayment-> bindNew< Wt::WLabel    >( "image", "image" );
    m_label-> setMinimumSize( "160px", "160px" );
    m_label-> resize( "160px", "160px" );

    auto pbClear = templtPayment-> bindNew< Wt::WPushButton >( "clear", "clear" );
    pbClear-> setToolTip( "Clear all the payment check-boxes" );
    pbClear->
      clicked().connect( [&]()
      {
        for( auto cb : m_cbx )
          cb-> setValueText( "no" );
      });

  } // endWt::WTemplate * templtPayment;

  Wt::WTemplate * templtHistory;
  {
    /*
    ** BUGBUG: this code isn't honoring the layout stuff, to keep
    **  the register table within the boundary of the widget.
    **
    */
    auto cw_ = std::make_unique< Wt::WContainerWidget >();
    cw_-> setMaximumSize( "Auto", "200px" );
    auto lw_ = cw_-> setLayout( std::make_unique< Wt::WVBoxLayout >() );
    auto u_ = std::make_unique< Wt::WTemplate >( TR( "gcw_gui.billpayeditor.form.tab2" ) );
    templtHistory = u_.get();
    lw_-> addWidget( std::move( u_ ) );
    m_tabWidget-> addTab( std::move( cw_ ), "History" );


    templtHistory-> bindNew< GCW::Gui::RegisterWidget >( "accountRegister", _accountGuid );

  } // endWt::WTemplate * templtHistory;

  m_pbSave  -> clicked().connect( [&](){ saveData(); });
  m_pbCancel-> clicked().connect( [&](){ m_cancel.emit(); });

  loadData();

} // endEditWidget( const std::string & _accountGuid )

void
GCW::Gui::BillPay::EditWidget::
loadData()
{
  if( m_accountGuid == "" )
    return;

  Wt::Dbo::Transaction t( GCW::app()-> gnucash_session() );

  auto varItem = GCW::Gui::BillPay::bpItem( m_accountGuid );

  auto fullName = GCW::Dbo::Accounts::fullName( m_accountGuid );

  m_key       -> setValueText( fullName );
  m_dueDay    -> setValueText( varItem-> getVarString( FN_DUEDAY    ) );
  m_minimum   -> setValueText( varItem-> getVarString( FN_MINIMUM   ) );
  m_budget    -> setValueText( varItem-> getVarString( FN_BUDGET    ) );
  m_nickname  -> setValueText( varItem-> getVarString( FN_NICKNAME  ) );
  m_group     -> setValueText( varItem-> getVarString( FN_GROUP     ) );
  m_limit     -> setValueText( varItem-> getVarString( FN_LIMIT     ) );
  m_actual    -> setValueText( varItem-> getVarString( FN_ACTUAL    ) );
  m_ap        -> setValueText( varItem-> getVarString( FN_AP        ) );
  m_isActive  -> setValueText( varItem-> getVarString( FN_ISACTIVE  ) );
  m_isVisible -> setValueText( varItem-> getVarString( FN_ISVISIBLE ) );
  m_autoPay   -> setValueText( varItem-> getVarString( FN_AUTOPAY   ) );
  m_payNow    -> setValueText( varItem-> getVarString( FN_PAYNOW    ) );
  m_last4     -> setValueText( varItem-> getVarString( FN_LAST4     ) );
  m_note      -> setValueText( varItem-> getVarString( FN_NOTE      ) );

  int i = 1;
  for( auto cb : m_cbx )
    cb-> setValueText( varItem-> getVarString( "cb" + toString( i++ ) ) );

} // endloadData( const std::string & _accountGuid )

void
GCW::Gui::BillPay::EditWidget::
saveData()
{
  auto key = m_key-> valueText().toUTF8();

  GCW::Dbo::Accounts::Item::Ptr accountItem;

  Wt::Dbo::Transaction t( GCW::app()-> gnucash_session() );

  /*
  ** If the account guid is not set, then this is a '(new)'
  **  entry.  We have to generate a new item for storage
  **
  */
  if( m_accountGuid == "" )
  {
    /*
    ** If the key is not set, there is nothing to do
    **
    */
    if( key == "" )
      return;

    accountItem = GCW::Dbo::Accounts::byFullName( key );

    m_accountGuid = accountItem-> guid();

  } // endif( m_accountGuid == "" )

  /*
  ** We have an account guid, so just fetch the account
  **
  */
  else
  {
    accountItem = GCW::Dbo::Accounts::byGuid( m_accountGuid );
  }

  auto varItem = GCW::Gui::BillPay::bpItem( m_accountGuid );

  varItem.modify()-> setVar( FN_DUEDAY    , m_dueDay    -> valueText() );
  varItem.modify()-> setVar( FN_MINIMUM   , m_minimum   -> valueText() );
  varItem.modify()-> setVar( FN_BUDGET    , m_budget    -> valueText() );
  varItem.modify()-> setVar( FN_NICKNAME  , m_nickname  -> valueText() );
  varItem.modify()-> setVar( FN_GROUP     , m_group     -> valueText() );
  varItem.modify()-> setVar( FN_LIMIT     , m_limit     -> valueText() );
  varItem.modify()-> setVar( FN_ACTUAL    , m_actual    -> valueText() );
  varItem.modify()-> setVar( FN_AP        , m_ap        -> valueText() );
  varItem.modify()-> setVar( FN_ISACTIVE  , m_isActive  -> valueText() );
  varItem.modify()-> setVar( FN_ISVISIBLE , m_isVisible -> valueText() );
  varItem.modify()-> setVar( FN_AUTOPAY   , m_autoPay   -> valueText() );
  varItem.modify()-> setVar( FN_PAYNOW    , m_payNow    -> valueText() );
  varItem.modify()-> setVar( FN_LAST4     , m_last4     -> valueText() );
  varItem.modify()-> setVar( FN_NOTE      , m_note      -> valueText() );

  int i = 1;
  for( auto cb : m_cbx )
    varItem.modify()-> setVar( "cb" + toString( i++ ), cb-> valueText() );

  m_save.emit();

} // endsaveData()




GCW::Gui::BillPay::EditWidgetDialog::
EditWidgetDialog( const std::string & _accountGuid )
{
  std::cout << __FILE__ << ":" << __LINE__ << " " << _accountGuid << std::endl;

  addStyleClass( "EditWidgetDialog" );
  setWindowTitle( "Bill Pay Account" );

  setClosable( true );
  rejectWhenEscapePressed( true );

  auto editWidget = contents()-> addNew< EditWidget >( _accountGuid );

  editWidget-> save   ().connect( [&](){ accept(); });
  editWidget-> cancel ().connect( [&](){ reject(); });

} // endEditWidgetDialog( const std::string & _accountGuid )



