#line 2 "src/AccountsWidget.cpp"

#include <any>

#include <Wt/WText.h>
#include <Wt/WTreeTableNode.h>

#include "define.h"
#include "GnuCashew.h"
#include "AccountsWidget.h"


GCW::AccountsWidget::AccountsWidget()
: m_treeView( addNew< Wt::WTreeView >() )
{
  treeView()-> setSelectionBehavior( Wt::SelectionBehavior::Rows );
  treeView()-> setSelectionMode(     Wt::SelectionMode::Single   );
  treeView()-> setAlternatingRowColors( true );
  treeView()-> doubleClicked().connect( this, &GCW::AccountsWidget::doubleClicked );

  m_columns.push_back( TR8( "gcw.accountswidget.column.accountcode"       ) );
  m_columns.push_back( TR8( "gcw.accountswidget.column.accountcolor"      ) );
  m_columns.push_back( TR8( "gcw.accountswidget.column.accountname"       ) );
  m_columns.push_back( TR8( "gcw.accountswidget.column.balance"           ) );
  m_columns.push_back( TR8( "gcw.accountswidget.column.balancelimit"      ) );
  m_columns.push_back( TR8( "gcw.accountswidget.column.balanceperiod"     ) );
  m_columns.push_back( TR8( "gcw.accountswidget.column.balanceusd"        ) );
  m_columns.push_back( TR8( "gcw.accountswidget.column.cleared"           ) );
  m_columns.push_back( TR8( "gcw.accountswidget.column.clearedusd"        ) );
  m_columns.push_back( TR8( "gcw.accountswidget.column.commodity"         ) );
  m_columns.push_back( TR8( "gcw.accountswidget.column.description"       ) );
  m_columns.push_back( TR8( "gcw.accountswidget.column.futureminimum"     ) );
  m_columns.push_back( TR8( "gcw.accountswidget.column.futureminimumusd"  ) );
  m_columns.push_back( TR8( "gcw.accountswidget.column.hidden"            ) );
  m_columns.push_back( TR8( "gcw.accountswidget.column.lastnum"           ) );
  m_columns.push_back( TR8( "gcw.accountswidget.column.lastreconciledate" ) );
  m_columns.push_back( TR8( "gcw.accountswidget.column.notes"             ) );
  m_columns.push_back( TR8( "gcw.accountswidget.column.openingbalance"    ) );
  m_columns.push_back( TR8( "gcw.accountswidget.column.placeholder"       ) );
  m_columns.push_back( TR8( "gcw.accountswidget.column.present"           ) );
  m_columns.push_back( TR8( "gcw.accountswidget.column.presentusd"        ) );
  m_columns.push_back( TR8( "gcw.accountswidget.column.reconciled"        ) );
  m_columns.push_back( TR8( "gcw.accountswidget.column.reconciledusd"     ) );
  m_columns.push_back( TR8( "gcw.accountswidget.column.taxinfo"           ) );
  m_columns.push_back( TR8( "gcw.accountswidget.column.total"             ) );
  m_columns.push_back( TR8( "gcw.accountswidget.column.totalperiod"       ) );
  m_columns.push_back( TR8( "gcw.accountswidget.column.totalusd"          ) );
  m_columns.push_back( TR8( "gcw.accountswidget.column.type"              ) );

  setModel();

} // endGCW::AccountsWidget::AccountsWidget()

void GCW::AccountsWidget::setModel()
{
  m_model = std::make_shared< Model >();

  m_model-> load();

  treeView()-> setModel( m_model );

  treeView()-> sortByColumn( 0, Wt::SortOrder::Ascending );

} // endvoid GCW::AccountsWidget::setModel()

void GCW::AccountsWidget::Model::load()
{
  /*
  ** If the session isn't open then there's nothing to load.
  **
  */
  if( !GCW::app()-> gnucash_session().isOpen() )
    return;

  load( invisibleRootItem(), GCW::Dbo::Account::root() );

  int col = 0;
  setHeaderData( col++, "Account Name"         );
  setHeaderData( col++, "Account Code"         );
  setHeaderData( col++, "Description"          );
  setHeaderData( col++, "Tax Info"             );
  setHeaderData( col++, "Notes"                );
  setHeaderData( col++, "Future Minimum (USD)" );
  setHeaderData( col++, "Total"                );

} // endvoid GCW::AccountsWidget::Model::load()

void GCW::AccountsWidget::Model::load( Wt::WStandardItem * _treeItem, GCW::Dbo::Account::Item::Ptr _parentAccount )
{
  Wt::Dbo::Transaction t( GCW::app()-> gnucash_session() );

  auto _append = [=]( Wt::WStandardItem * _item, GCW::Dbo::Account::Item::Ptr _accountItem )
  {
    std::vector< std::unique_ptr< Wt::WStandardItem > > columns;

    auto accountName = std::make_unique< Wt::WStandardItem >( _accountItem-> name() );

    /*
    ** set the 'model->data::User' element to contain the guid of the account, so
    **  we can recover it later.
    **
    */
    accountName-> setData( _accountItem-> guid(), Wt::ItemDataRole::User );
    auto retVal = accountName.get();
    columns.push_back( std::move( accountName ) );
    columns.push_back( std::make_unique< Wt::WStandardItem >( _accountItem-> code        () ) );
    columns.push_back( std::make_unique< Wt::WStandardItem >( _accountItem-> description () ) );
    columns.push_back( std::make_unique< Wt::WStandardItem >() );
    columns.push_back( std::make_unique< Wt::WStandardItem >() );
    columns.push_back( std::make_unique< Wt::WStandardItem >() );
    columns.push_back( std::make_unique< Wt::WStandardItem >() );
    _item-> appendRow( std::move( columns ) );

    return retVal;
  };

  auto accounts =
    GCW::app()-> gnucash_session().find< GCW::Dbo::Account::Item >()
    .where( "parent_guid = ?" )
    .bind( _parentAccount-> guid() )
    .resultList()
    ;

  for( auto account : accounts )
  {
    auto ti = _append( _treeItem, account );
    load( ti, account );
  }

} // endvoid load( Wt::WStandardItem * _treeItem, Account::Ptr _parentAccount )

void GCW::AccountsWidget::doubleClicked( const Wt::WModelIndex & index, const Wt::WMouseEvent & event )
{
#ifdef NEVER
  std::cout << std::endl << std::endl << __FILE__ << ":" << __LINE__
    << " " << "doubleClicked:"
    << " " << index.row()
    << " " << Wt::asString( m_model-> data( index, Wt::ItemDataRole::User ) )
    << std::endl << std::endl
    << std::endl;
#endif

  /*
  ** The 'model->data::User' element should return the guid of the account
  **
  */
  m_doubleClicked.emit( Wt::asString( m_model-> data( index, Wt::ItemDataRole::User ) ).toUTF8() );

} // endvoid GCW::AccountsWidget::doubleClicked( const Wt::WModelIndex & index, const Wt::WMouseEvent & event )


