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

  auto w = std::make_unique< Wt::WTableView >();
  m_tableView = w.get();
  lw-> addWidget( std::move( w ), 1 );

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

  std::cout << __FILE__ << ":" << __LINE__ << " " << accountItem << " " << splitItems.size() << std::endl;

  for( auto splitItem : splitItems )
  {
    auto transactionItem = GCW::Dbo::Transactions::byGuid( splitItem-> tx_guid() );

    std::vector< std::unique_ptr< Wt::WStandardItem > > columns;

    auto _addColumn = [&]( const std::string & _text )
    {
      columns.push_back( std::make_unique< Wt::WStandardItem >( _text ) );
    };

    _addColumn( transactionItem-> post_date   ( "MM/dd/yyyy" ) );
    _addColumn( transactionItem-> num         (                    ) );
    _addColumn( transactionItem-> description (            ) );
    _addColumn( "" );

    appendRow( std::move( columns ) );
  }

} // endvoid GCW::RegisterWidget::Model::refreshFromDisk()


