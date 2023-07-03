

#include <Wt/WText.h>
#include <Wt/WTreeTableNode.h>

#include "define.h"
#include "GnuCashew.h"
#include "AccountsWidget.h"


#ifdef TREETABLE_BASED_WIDGET

GCW::AccountsWidget::AccountsWidget()
: m_treeTable( addNew< Wt::WTreeTable >() )
{
  loadData();

} // endGCW::AccountsWidget::AccountsWidget()

void GCW::AccountsWidget::loadData()
{
  std::cout << __FILE__ << ":" << __LINE__ << " " << std::endl;

  treeTable()-> addColumn( "Account Code"         , 125 );
  treeTable()-> addColumn( "Description"          , 125 );
  treeTable()-> addColumn( "Tax Info"             , 125 );
  treeTable()-> addColumn( "Notes"                , 125 );
  treeTable()-> addColumn( "Future Minimum (USD)" , 125 );
  treeTable()-> addColumn( "Total"                , 125 );

  std::cout << __FILE__ << ":" << __LINE__ << " " << std::endl;

  Wt::WTreeTableNode * rootNode = nullptr;
  {
    auto node = std::make_unique< Wt::WTreeTableNode >( "All" );
    rootNode = node.get();
    treeTable()-> setTreeRoot( std::move( node ), "Account Name" );
  }

  std::cout << __FILE__ << ":" << __LINE__ << " " << std::endl;

  /*
  ** Get a handle on the root account.  The root account is the only
  **  account that has no parent, and has a name == "Root Account".
  **  There should only be one of these.
  **
  */
  GCW::Dbo::Account::Ptr rootAccount;
  {
    Wt::Dbo::Transaction t( GCW::app()-> session() );

    auto results =
      GCW::app()-> session().find< GCW::Dbo::Account >()
      .where( "(parent_guid = '' OR parent_guid IS NULL) AND name = 'Root Account'" )
      .resultList()
      ;

    if( results.size() == 1 )
    {
      rootAccount = *results.begin();
    }

  } // endWt::Dbo::ptr< Account > rootAccount;

  load( rootNode, rootAccount );

} // endvoid GCW::AccountsWidget::load()

void GCW::AccountsWidget::load( Wt::WTreeTableNode * _treeNode, GCW::Dbo::Account::Ptr _parentAccount )
{
  Wt::Dbo::Transaction t( GCW::app()-> session() );

  auto _append = [=]( Wt::WTreeTableNode * _item, GCW::Dbo::Account::Ptr _account )
  {
    auto node = std::make_unique< Wt::WTreeTableNode >( _account-> m_name );
    auto retVal = node.get();

    int col = 1;
    node-> setColumnWidget( col++, std::make_unique< Wt::WText >( _account-> m_code        ) );
    node-> setColumnWidget( col++, std::make_unique< Wt::WText >( _account-> m_description ) );
    node-> setColumnWidget( col++, std::make_unique< Wt::WText >() );
    node-> setColumnWidget( col++, std::make_unique< Wt::WText >() );
    node-> setColumnWidget( col++, std::make_unique< Wt::WText >() );
    node-> setColumnWidget( col++, std::make_unique< Wt::WText >() );
    _item-> addChildNode( std::move( node ) );

    return retVal;
  };

  auto accounts =
    GCW::app()-> session().find< GCW::Dbo::Account >()
    .where( "parent_guid = ?" )
    .bind( _parentAccount-> m_guid )
    .resultList()
    ;

  for( auto account : accounts )
  {
    auto ti = _append( _treeNode, account );
    load( ti, account );
  }

} // endvoid load( Wt::WStandardItem * _treeItem, Account::Ptr _parentAccount )

#endif // TREETABLE_BASED_WIDGET

#ifdef TREEVIEW_BASED_WIDGET

GCW::AccountsWidget::AccountsWidget()
: m_treeView( addNew< Wt::WTreeView >() )
{
  treeView()-> setSelectionBehavior( Wt::SelectionBehavior::Rows );
  treeView()-> setSelectionMode(     Wt::SelectionMode::Single   );
  treeView()-> setAlternatingRowColors( true );

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
  auto model = std::make_shared< Model >();

  model-> load();

  treeView()-> setModel( model );

} // endvoid GCW::AccountsWidget::setModel()

void GCW::AccountsWidget::Model::load()
{
  /*
  ** If the session isn't open then there's nothing to load.
  **
  */
  if( !GCW::app()-> gnucash_session().isOpen() )
    return;

  /*
  ** Get a handle on the root account.  The root account is the only
  **  account that has no parent, and has a name == "Root Account".
  **  There should only be one of these.
  **
  */
  GCW::Dbo::Account::Ptr rootAccount;
  {
    Wt::Dbo::Transaction t( GCW::app()-> gnucash_session() );

    auto results =
      GCW::app()-> gnucash_session().find< GCW::Dbo::Account >()
      .where( "(parent_guid = '' OR parent_guid IS NULL) AND name = 'Root Account'" )
      .resultList()
      ;

    if( results.size() == 1 )
    {
      rootAccount = *results.begin();
    }

  } // endWt::Dbo::ptr< Account > rootAccount;

  load( invisibleRootItem(), rootAccount );

  int col = 0;
  setHeaderData( col++, "Account Name"         );
  setHeaderData( col++, "Account Code"         );
  setHeaderData( col++, "Description"          );
  setHeaderData( col++, "Tax Info"             );
  setHeaderData( col++, "Notes"                );
  setHeaderData( col++, "Future Minimum (USD)" );
  setHeaderData( col++, "Total"                );

} // endvoid GCW::AccountsWidget::Model::load()

void GCW::AccountsWidget::Model::load( Wt::WStandardItem * _treeItem, GCW::Dbo::Account::Ptr _parentAccount )
{
  Wt::Dbo::Transaction t( GCW::app()-> gnucash_session() );

  auto _append = [=]( Wt::WStandardItem * _item, GCW::Dbo::Account::Ptr _account )
  {
    std::vector< std::unique_ptr< Wt::WStandardItem > > columns;

    auto accountName = std::make_unique< Wt::WStandardItem >( _account-> m_name );
    auto retVal = accountName.get();
    columns.push_back( std::move( accountName ) );
    columns.push_back( std::make_unique< Wt::WStandardItem >( _account-> m_code        ) );
    columns.push_back( std::make_unique< Wt::WStandardItem >( _account-> m_description ) );
    columns.push_back( std::make_unique< Wt::WStandardItem >() );
    columns.push_back( std::make_unique< Wt::WStandardItem >() );
    columns.push_back( std::make_unique< Wt::WStandardItem >() );
    columns.push_back( std::make_unique< Wt::WStandardItem >() );
    _item-> appendRow( std::move( columns ) );

    return retVal;
  };

  auto accounts =
    GCW::app()-> gnucash_session().find< GCW::Dbo::Account >()
    .where( "parent_guid = ?" )
    .bind( _parentAccount-> m_guid )
    .resultList()
    ;

  for( auto account : accounts )
  {
    auto ti = _append( _treeItem, account );
    load( ti, account );
  }

} // endvoid load( Wt::WStandardItem * _treeItem, Account::Ptr _parentAccount )

#endif



