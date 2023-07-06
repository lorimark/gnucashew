#line 2 "src/RegisterWidget.cpp"

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
{
  addStyleClass( "RegisterWidget" );

  auto lw = setLayout( std::make_unique< Wt::WVBoxLayout >() );

  auto w = std::make_unique< GCW::TableView >();
  m_tableView = w.get();
  lw-> addWidget( std::move( w ), 1 );
  tableView()-> addStyleClass( "TableView" );

#ifdef NEVER
  int col = 0;
  table()-> elementAt( 0, col )-> addWidget( textWidget( 0, col, "Date"       ) ); col++;
  table()-> elementAt( 0, col )-> addWidget( textWidget( 0, col, "Num"        ) ); col++;
  table()-> elementAt( 0, col )-> addWidget( textWidget( 0, col, "Memo"       ) ); col++;
  table()-> elementAt( 0, col )-> addWidget( textWidget( 0, col, "Account"    ) ); col++;
  table()-> elementAt( 0, col )-> addWidget( textWidget( 0, col, "R"          ) ); col++;
  table()-> elementAt( 0, col )-> addWidget( textWidget( 0, col, "Deposit"    ) ); col++;
  table()-> elementAt( 0, col )-> addWidget( textWidget( 0, col, "Withdrawal" ) ); col++;
  table()-> elementAt( 0, col )-> addWidget( textWidget( 0, col, "Balance"    ) ); col++;

  table()-> setHeaderCount( 1 );
#endif

  loadData( _accountGuid );

} // endGCW::RegisterWidget::RegisterWidget()


void GCW::RegisterWidget::
loadData( const std::string & _accountGuid )
{
  m_model = std::make_shared< Model >( _accountGuid );

  tableView()-> setModel( m_model );
  tableView()-> setSortingEnabled( false );

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


#ifdef NEVER
  int row = 1;
  for( auto splitItem : splitItems )
  {
    auto transactionItem = GCW::Dbo::Transactions::byGuid( splitItem-> tx_guid() );

    int col = 0;
    table()-> elementAt( row, col++ )-> addWidget( textWidget( row, col, transactionItem-> post_date( "MM/dd/yyyy" ) ) ); col++;
    table()-> elementAt( row, col++ )-> addWidget( textWidget( row, col, "1" ) );                                                  col++;
    table()-> elementAt( row, col++ )-> addWidget( textWidget( row, col, "1" ) );                                                  col++;
    table()-> elementAt( row, col++ )-> addWidget( textWidget( row, col, "1" ) );                                                  col++ ;

    row++;

  } // endfor( auto splitItem : splits )
#endif

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
  auto accountItem = GCW::Dbo::Accounts::byGuid   ( m_accountGuid );
  auto splitItems  = GCW::Dbo::Splits  ::byAccount( m_accountGuid );

  for( auto splitItem : splitItems )
  {
    auto transactionItem = GCW::Dbo::Transactions::byGuid( splitItem-> tx_guid() );

    std::vector< std::unique_ptr< Wt::WStandardItem > > columns;

    auto _addColumn = [&]( const std::string & _text )
    {
      columns.push_back( std::make_unique< Wt::WStandardItem >( _text ) );
    };

    _addColumn( transactionItem-> post_date   ( "MM/dd/yyyy" ) );
    _addColumn( transactionItem-> num         (              ) );
    _addColumn( transactionItem-> description (              ) );
    _addColumn( "" );

    appendRow( std::move( columns ) );
  }

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


