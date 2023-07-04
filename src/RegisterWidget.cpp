#line 2 "src/RegisterWidget.cpp"

#include <Wt/WText.h>
#include <Wt/WTableView.h>

#include "define.h"
#include "App.h"
#include "RegisterWidget.h"


GCW::RegisterWidget::RegisterWidget( const std::string & _accountGuid )
: m_tableView( addNew< Wt::WTableView >() )
{
  std::cout << __FILE__ << ":" << __LINE__ << " " << _accountGuid << std::endl;


} // endGCW::RegisterWidget::RegisterWidget()

void GCW::RegisterWidget::setModel()
{
  auto model = std::make_shared< Model >();

  model-> load();

  tableView()-> setModel( model );

} // endvoid GCW::RegisterWidget::setModel()

void GCW::RegisterWidget::Model::load()
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

} // endvoid GCW::RegisterWidget::Model::load()

void GCW::RegisterWidget::Model::load( Wt::WStandardItem * _treeItem, GCW::Dbo::Account::Item::Ptr _parentAccount )
{
  Wt::Dbo::Transaction t( GCW::app()-> gnucash_session() );

  auto _append = [=]( Wt::WStandardItem * _item, GCW::Dbo::Account::Item::Ptr _account )
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
    GCW::app()-> gnucash_session().find< GCW::Dbo::Account::Item >()
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


