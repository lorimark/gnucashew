#line 2 "src/RegisterWidget.cpp"

#include <Wt/WText.h>
#include <Wt/WTableView.h>
#include <Wt/WVBoxLayout.h>

#define DEC_NAMESPACE GCW_DECIMAL
#include "3rd/decimal.h"
using namespace GCW_DECIMAL;

#include "define.h"
#include "App.h"
#include "RegisterWidget.h"
#include "Dbo/Accounts.h"
#include "Dbo/Splits.h"
#include "Dbo/Transactions.h"


GCW::RegisterWidget::
RegisterWidget( const std::string & _accountGuid )
: m_accountGuid( _accountGuid )
{
  addStyleClass( "RegisterWidget" );

  /*
  ** use a layout manager to install the table view into, so
  **  that the widget will fit and scroll properly
  **
  */
  auto lw = setLayout( std::make_unique< Wt::WVBoxLayout >() );
  auto w = std::make_unique< GCW::TableView >();
  m_tableView = w.get();
  lw-> addWidget( std::move( w ), 1 );
  tableView()-> addStyleClass( "TableView" );

  tableView()-> setSortingEnabled       ( false                          );
  tableView()-> setAlternatingRowColors ( true                           );
  tableView()-> setSelectionBehavior    ( Wt::SelectionBehavior::Rows    );
  tableView()-> setSelectionMode        ( Wt::SelectionMode::Single      );
  tableView()-> setEditTriggers         ( Wt::EditTrigger::SingleClicked );

  loadData();

} // endGCW::RegisterWidget::RegisterWidget()


void GCW::RegisterWidget::
loadData()
{
  m_model = std::make_shared< Model >( m_accountGuid );

  tableView()-> setModel( m_model );

  /*
  ** Prefer to set these in gcw.css but having trouble getting the
  **  css to behave
  **
  */
  int col = 0;
  tableView()-> setColumnWidth( col++, "150px" ); // 1 Date
  tableView()-> setColumnWidth( col++,  "50px" ); // 2 Action/Num
  tableView()-> setColumnWidth( col++,   "99%" ); // 3 Memo/Description
  tableView()-> setColumnWidth( col++, "200px" ); // 4 Account
  tableView()-> setColumnWidth( col++,  "25px" ); // 5 Reconciliation
  tableView()-> setColumnWidth( col++, "100px" ); // 6 Deposit
  tableView()-> setColumnWidth( col++, "100px" ); // 7 Withdrawal
  tableView()-> setColumnWidth( col++, "100px" ); // 8 Balance

} // endvoid GCW::RegisterWidget::setModel()

GCW::RegisterWidget::Model::
Model( const std::string & _accountGuid )
: Wt::WStandardItemModel( 0, 8 ),
  m_accountGuid( _accountGuid )
{
  refreshFromDisk();

} // endGCW::RegisterWidget::Model::Model( const std::string & _accountGuid )

void GCW::RegisterWidget::Model::
refreshFromDisk()
{
  /*
  ** Get the account loaded, and all the splits for this account.
  **
  ** NOTE: it is important to load the 'splits' that are associated
  **  with an account, since it is possible to create a single transaction
  **  that is comprised of two splits for the same account!  There's no
  **  practical reason I can think to do this, but it can happen, and the
  **  only way to represent the transactions properly within the view
  **  is to work from the split side of the relationship.
  **
  */
  auto accountItem = GCW::Dbo::Accounts:: byGuid    ( m_accountGuid );
  auto splitItems  = GCW::Dbo::Splits  :: byAccount ( m_accountGuid );

  /*
  ** Set the transaction date on each split, so they can be quickly sorted.
  **  Sorting the vector causes the items to appear in the vector in sequential
  **  (date) order.  They can then be loaded in to the view one after another.
  **
  ** NOTE: this could be done in the sort procedure itself, but I did it
  **  here.
  **
  */
  for( auto splitItem : splitItems )
  {
    auto transactionItem = GCW::Dbo::Transactions::byGuid( splitItem-> tx_guid() );
    splitItem-> set_tx_date( transactionItem-> post_date_as_date() );
  }

  /*
  ** Sort the transactions by transaction date so that they can be loaded
  **  in to the model in proper sequential order.
  **
  */
  std::sort
  (
   splitItems.begin(),
   splitItems.end(),
   []( const GCW::Dbo::Splits::Item::Ptr item1,
       const GCW::Dbo::Splits::Item::Ptr item2
     )
     {
       return item1-> tx_date() < item2-> tx_date();
     }
  );

  /*
  ** Process each split item.  Grab the contents of the split, and
  **  generate a model item row containing all of the column values
  **
  */
  decimal<2> bal( 0 );
  for( auto splitItem : splitItems )
  {
    /*
    ** Need a handle on the transaction.  Fetch the transaction associated
    **  with this split, so we can get things from it, and then also, fetch
    **  the other splits that are on said transaction.
    **
    */
    auto transactionItem   = GCW::Dbo::Transactions::byGuid( splitItem-> tx_guid() );
    auto transactionSplits = GCW::Dbo::Splits::byTransaction( splitItem-> tx_guid(), splitItem-> guid() );

    /*
    ** The columns we are building go here
    **
    */
    std::vector< std::unique_ptr< Wt::WStandardItem > > columns;
    auto _addColumn = [&]( auto _value )
    {
      auto item = std::make_unique< Wt::WStandardItem >( _value );

      item-> setToolTip( _value );
#ifdef NEVER
      auto toolTip =
        TR("gcw.registerwidget.account.tooltip")
        .arg( splitItem-> guid() )
        .arg( splitItem-> tx_date().toString() )
        .arg( transactionItem-> num() )
        .arg( transactionItem-> description () )
        .arg( splitItem-> tx_guid() )
        .toUTF8()
        ;
      item-> setToolTip( toolTip );
#endif

      auto retVal = item.get();
      columns.push_back( std::move( item ) );
      return retVal;
    };

    /*
    ** Assemble each column
    **
    */
    auto post_date = _addColumn( splitItem-> tx_date().toString( "MM/dd/yyyy" ) );
         post_date-> setData( splitItem-> tx_date() );
         post_date-> setFlags( Wt::ItemFlag::Editable );

    auto num = _addColumn( transactionItem-> num() );
         num-> setFlags( Wt::ItemFlag::Editable );

    auto description = _addColumn( transactionItem-> description() );
         description-> setFlags( Wt::ItemFlag::Editable );

    /*
    ** Generate the 'account' text.  The text depends on the
    **  target account defined in the split.  There are three
    **  options here;
    **
    **   - no splits... this shows up as an 'imbalance'
    **   - 1 split...   this just shows the split account on the line
    **   - >1 split...  this is more than one target account, so just indicate 'split'
    **
    */
    Wt::WStandardItem * account = nullptr;
    switch( transactionSplits.size() )
    {
      /*
      ** This is actually a problem... We don't have another split, and
      **  according to 'generally accepted accounting practices' we
      **  should!  So, just  plop an 'imbalance' indicator in the view.
      **
      */
      case 0:
      {
        account = _addColumn( TR("gcw.registerwidget.account.imbalanceUSD") ); // account
        account-> setFlags( Wt::ItemFlag::Editable );
        break;
      }

      /*
      ** This is a straight and simple 1:1 split transaction, so we can pull
      **  the account name from the other side of the split and pop that in
      **  to the model directly.
      **
      */
      case 1:
      {
        auto txSplitItem = *transactionSplits.begin();
        auto splitAccountItem = GCW::Dbo::Accounts::byGuid( txSplitItem-> account_guid() );
        account = _addColumn( GCW::Dbo::Accounts::fullName( splitAccountItem-> guid() ) );
        account-> setFlags( Wt::ItemFlag::Editable );
        break;
      }

      /*
      ** When we have more than one split then we cannot display
      **  all of the split accounts on just one line, so just pop
      **  a message that indicates that we're in a multisplit
      **  transaction.
      **
      */
      default:
      {
        account = _addColumn( TR("gcw.registerwidget.account.multisplit") ); // account
      }

    } // endswitch( transactionSplits.size() )

    auto reconcile = _addColumn( splitItem-> reconcile_state() ); // Reconciled

    /*
    ** The following works out the values for the
    **  'deposit' and 'withdrawal' and 'balance'
    **  columns.  The 'decimal.h' library is used
    **  to perform the arithmetic to prevent the
    **  floating point math problems.
    **
    */
    Wt::WStandardItem * deposit    = nullptr;
    Wt::WStandardItem * withdrawal = nullptr;
    Wt::WStandardItem * balance    = nullptr;

    decimal<2> val( splitItem-> value_num() );
    val /= splitItem-> value_denom();

    bal += val;

    decimal_format f( ',', '.' );

    if( val > 0 )
    {
      deposit    = _addColumn( Wt::WString( "{1}" ).arg(  toString( val, f ) ) );
      withdrawal = _addColumn( "" );
    }

    if( val < 0 )
    {
      deposit    = _addColumn( "" );
      withdrawal = _addColumn( Wt::WString( "{1}" ).arg( toString( val * decimal<2>(-1), f ) ) );
    }

    deposit   -> setFlags( Wt::ItemFlag::Editable );
    withdrawal-> setFlags( Wt::ItemFlag::Editable );

    balance = _addColumn( Wt::WString( "{1}" ).arg( toString( bal, f ) ) );

    if( bal < 0 )
      balance-> setStyleClass( "negval" );

    /*
    ** Add the row to the model
    **
    */
    appendRow( std::move( columns ) );

  } // endfor( auto splitItem : splitItems )

  int col = 0;
  setHeaderData( col, "Date"       ); col++;
  setHeaderData( col, "Num"        ); col++;
  setHeaderData( col, "Memo"       ); col++;
  setHeaderData( col, "Account"    ); col++;
  setHeaderData( col, "R"          ); col++;
  setHeaderData( col, "Deposit"    ); col++;
  setHeaderData( col, "Withdrawal" ); col++;
  setHeaderData( col, "Balance"    ); col++;

} // endvoid GCW::RegisterWidget::Model::refreshFromDisk()


