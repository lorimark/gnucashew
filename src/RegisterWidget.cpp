#line 2 "src/RegisterWidget.cpp"

#include <Wt/WItemDelegate.h>
#include <Wt/WText.h>
#include <Wt/WTableView.h>
#include <Wt/WVBoxLayout.h>

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
  /*
  ** Look in gcw.css for styling
  **
  */
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

  tableView()-> setHeaderItemDelegate( std::make_shared< Wt::WItemDelegate >() );

  tableView()-> headerClicked().connect( [=]( int col, Wt::WMouseEvent event )
      {
        std::cout << __FILE__ << ":" << __LINE__ << " " << col << std::endl;


      });

  loadData();

} // endGCW::RegisterWidget::RegisterWidget()


void GCW::RegisterWidget::
loadData()
{
  m_model = std::make_shared< Model >( m_accountGuid );

  tableView()-> setModel( m_model );

  /* Date */
  tableView()-> setColumnWidth     ( 0, "100px"                   );
  tableView()-> setHeaderAlignment ( 0, Wt::AlignmentFlag::Right  );
  tableView()-> setColumnAlignment ( 0, Wt::AlignmentFlag::Right  );

  /* Action/Num */
  tableView()-> setColumnWidth     ( 1,  "50px"                   );
  tableView()-> setHeaderAlignment ( 1, Wt::AlignmentFlag::Center );
  tableView()-> setColumnAlignment ( 1, Wt::AlignmentFlag::Center );

  /* Memo/Description */
  tableView()-> setColumnWidth     ( 2,   "99%"                   );
  tableView()-> setHeaderAlignment ( 2, Wt::AlignmentFlag::Left   );
  tableView()-> setColumnAlignment ( 2, Wt::AlignmentFlag::Left   );

  /* Account/Transfer */
  tableView()-> setColumnWidth     ( 3, "200px"                   );
  tableView()-> setHeaderAlignment ( 3, Wt::AlignmentFlag::Right  );
  tableView()-> setColumnAlignment ( 3, Wt::AlignmentFlag::Right  );

  /* Reconciliation */
  tableView()-> setColumnWidth     ( 4,  "25px"                   );
  tableView()-> setHeaderAlignment ( 4, Wt::AlignmentFlag::Center );
  tableView()-> setColumnAlignment ( 4, Wt::AlignmentFlag::Center );

  /* Deposit */
  tableView()-> setColumnWidth     ( 5, "100px"                   );
  tableView()-> setHeaderAlignment ( 5, Wt::AlignmentFlag::Right  );
  tableView()-> setColumnAlignment ( 5, Wt::AlignmentFlag::Right  );

  /* Withdrawal */
  tableView()-> setColumnWidth     ( 6, "100px"                   );
  tableView()-> setHeaderAlignment ( 6, Wt::AlignmentFlag::Right  );
  tableView()-> setColumnAlignment ( 6, Wt::AlignmentFlag::Right  );

  /* Balance */
  tableView()-> setColumnWidth     ( 7, "100px"                   );
  tableView()-> setHeaderAlignment ( 7, Wt::AlignmentFlag::Right  );
  tableView()-> setColumnAlignment ( 7, Wt::AlignmentFlag::Right  );

} // endvoid GCW::RegisterWidget::setModel()

GCW::RegisterWidget::Model::
Model( const std::string & _accountGuid )
: Wt::WStandardItemModel( 0, 8 ),
  m_accountGuid( _accountGuid )
{
  refreshFromDisk();

} // endGCW::RegisterWidget::Model::Model( const std::string & _accountGuid )

/*!
** \brief Refresh From Disk
**
** This procedure reads from the gnucash storage source
**  (either postgres or sqlite) and loads all of the
**  transactions and their associated splits in to the
**  model suitable for editing within an automatic
**  table view.
**
*/
void GCW::RegisterWidget::Model::
refreshFromDisk()
{
  /*!
  ** Before refreshing from disk, the entire contents of the
  **  model are cleared, so it is important to make sure anything
  **  to be saved from the model should be done first.
  **
  */
  clear();

  /*!
  ** In order to produce a proper 'register' of items, it is important
  **  to load the data from the 'splits' side of the transaction rather
  **  than the transaction itself.  This is due to the fact that it is
  **  possible to have a single transaction that has two splits on the
  **  same account!  While there's no practical reason to enter a
  **  transaction like that, it is still possible, and needs to be 
  **  represented correctly in the view.
  **
  ** Note that when the splits are loaded based on the account ID, they
  **  are returned in a std::vector that is sorted based on the transction
  **  date.
  **
  */
  auto splitItems = GCW::Dbo::Splits::byAccount( m_accountGuid );

  /*!
  ** Each item is processed from the vector in sequential order.
  **  In this process we grab the contents of the split, and
  **  generate a model item row containing all of the column values.
  **  Maintain a running balance as we go along to keep the balance
  **  reflected within the view.  The result is a multi-column row
  **  item that is added to the model.  This allows the model to be
  **  subsequently resorted without affecting the running balances
  **  and so forth.
  **
  */
  GCW_DECIMAL::decimal<2> runningBalance( 0 );
  GCW_DECIMAL::decimal_format format( ',', '.' );
  for( auto splitItem : splitItems )
  {
    /*!
    ** From the initial split item, we get a handle on the transaction,
    **  and then load all of the other splits associated with this
    **  transaction.
    **
    */
    auto transactionItem   = GCW::Dbo::Transactions ::byGuid  ( splitItem-> tx_guid() );
    auto transactionSplits = GCW::Dbo::Splits       ::bySplit ( splitItem-> guid()    );

    /*!
    ** \par Model Columns
    ** \code
    **   col  name                  notes
    **  -----+---------------------+----------------------
    **    0   date
    **    1   num (check number)
    **    2   description
    **    3   account / transfer
    **    4   reconciliation
    **    5   increase column
    **    6   decrease column
    **    7   balance column        r/o (computed)
    ** \endcode
    **
    */
    RowItem columns;
    auto _addColumn = [&]( auto _value )
    {
      auto item = std::make_unique< Wt::WStandardItem >( _value );

      item-> setToolTip( _value );

      auto retVal = item.get();
      columns.push_back( std::move( item ) );
      return retVal;
    };

    /*!
    ** \note The post_date also carries with it the guid of the split item itself, so
    **  that the originating split can be located from the table view.  The guid
    **  can be accessed by;
    **
    ** \code
    ** auto splitRowGuid = Wt::asString( standardItem.data( Wt::ItemRole::User ) )
    ** \endcode
    **
    */
    auto post_date = _addColumn( transactionItem-> post_date_as_date().toString( "MM/dd/yyyy" ) );
         post_date-> setData( splitItem-> guid() );
         post_date-> setFlags( Wt::ItemFlag::Editable );

    auto num = _addColumn( transactionItem-> num() );
         num-> setFlags( Wt::ItemFlag::Editable );

    auto description = _addColumn( transactionItem-> description() );
         description-> setFlags( Wt::ItemFlag::Editable );

    /*!
    ** The 'account' text depends on the
    **  target account defined in the split.  There are three
    **  options here;
    **
    **   -# no splits... this shows up as an <b>'imbalance'</b>
    **   -# 1 split...   this just shows the split account on the line
    **   -# >1 split...  this is more than one target account, so just indicate 'split'
    **
    */
    Wt::WStandardItem * account = nullptr;
    switch( transactionSplits.size() )
    {
      /*!
      ** \par Imbalance
      ** This is actually a problem... We don't have another split, and
      **  according to 'generally accepted accounting practices' we
      **  should!  So, just  plop an 'imbalance' indicator in the view.
      **  A style-class is also applied to the item to allow the rendering
      **  in the view to highlight this problem.
      **
      */
      case 0:
      {
        account = _addColumn( TR("gcw.registerwidget.account.imbalanceUSD") ); // account
        account-> setFlags( Wt::ItemFlag::Editable );
        account-> setStyleClass( "errval" );
        account-> setToolTip( TR("gcw.registerwidget.account.imbalanceUSD.toolTip") );
        break;
      }

      /*!
      ** \par Normal Split
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

      /*!
      ** \par Multi-Split
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

    /*!
    ** Values of the transaction appear either in the (+)Increase column
    **  or in the (-)Decrease column depending on if they are positive
    **  or negative.  The 'decimal.h' library is used to perform the
    **  arithmetic to prevent the floating point math problems.
    **
    ** The balance on the transaction is computed on-the-fly, which
    **  makes clear the importance of having the initial vector of splits
    **  to appear in the correct chronological order.
    **
    */
    Wt::WStandardItem * deposit    = nullptr;
    Wt::WStandardItem * withdrawal = nullptr;
    Wt::WStandardItem * balance    = nullptr;

    /*
    ** Accumulate the running balance
    **
    */
    runningBalance += splitItem-> value();

    if( splitItem-> value() > 0 )
    {
      deposit    = _addColumn( splitItem-> valueAsString() );
      withdrawal = _addColumn( "" );
    }

    else
    if( splitItem-> value() < 0 )
    {
      deposit    = _addColumn( "" );
      withdrawal = _addColumn( splitItem-> valueAsString() );
    }

    else
    if( splitItem-> value() == 0 )
    {
      deposit    = _addColumn( "" );
      withdrawal = _addColumn( "" );
    }

    deposit   -> setFlags( Wt::ItemFlag::Editable );
    withdrawal-> setFlags( Wt::ItemFlag::Editable );

    balance = _addColumn( Wt::WString( "{1}" ).arg( toString( runningBalance, format ) ) );

    if( runningBalance < 0 )
      balance-> setStyleClass( "negval" );

    /*
    ** Add the row to the model
    **
    */
    appendRow( std::move( columns ) );

  } // endfor( auto splitItem : splitItems )

  int col = 0;
  setHeaderData( col++, "Date"       );
  setHeaderData( col++, "Num"        );
  setHeaderData( col++, "Memo"       );
  setHeaderData( col++, "Account"    );
  setHeaderData( col++, "R"          );
  setHeaderData( col++, "Deposit"    );
  setHeaderData( col++, "Withdrawal" );
  setHeaderData( col++, "Balance"    );

} // endvoid GCW::RegisterWidget::Model::refreshFromDisk()

GCW::RegisterWidget::Model::RowItem GCW::RegisterWidget::Model::
makeRow( const std::string & _splitGuid )
{
  RowItem rowItem;

  return rowItem;
}

