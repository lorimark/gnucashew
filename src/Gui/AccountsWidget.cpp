#line 2 "src/AccountsWidget.cpp"

#include <any>

#include <Wt/Json/Serializer.h>
#include <Wt/WText.h>
#include <Wt/WTreeTableNode.h>
#include <Wt/WVBoxLayout.h>

#include "../define.h"
#include "../GnuCashew.h"
#include "AccountEditor.h"
#include "AccountsWidget.h"


GCW::Gui::AccountsWidget::
AccountsWidget()
{
  addStyleClass( "AccountsWidget" );

  auto lw = setLayout( std::make_unique< Wt::WVBoxLayout >() );

  auto w = std::make_unique< Wt::WTreeView >();
  m_treeView = w.get();
  lw-> addWidget( std::move( w ) );

  treeView()-> setSelectionBehavior( Wt::SelectionBehavior::Rows );
  treeView()-> setSelectionMode(     Wt::SelectionMode::Single   );
  treeView()-> setAlternatingRowColors( true );
  treeView()-> doubleClicked().connect( this, &GCW::Gui::AccountsWidget::doubleClicked );

  m_columns.push_back( TR8( "gcw.AccountsWidget.column.accountcode"       ) );
  m_columns.push_back( TR8( "gcw.AccountsWidget.column.accountcolor"      ) );
  m_columns.push_back( TR8( "gcw.AccountsWidget.column.accountname"       ) );
  m_columns.push_back( TR8( "gcw.AccountsWidget.column.balance"           ) );
  m_columns.push_back( TR8( "gcw.AccountsWidget.column.balancelimit"      ) );
  m_columns.push_back( TR8( "gcw.AccountsWidget.column.balanceperiod"     ) );
  m_columns.push_back( TR8( "gcw.AccountsWidget.column.balanceusd"        ) );
  m_columns.push_back( TR8( "gcw.AccountsWidget.column.cleared"           ) );
  m_columns.push_back( TR8( "gcw.AccountsWidget.column.clearedusd"        ) );
  m_columns.push_back( TR8( "gcw.AccountsWidget.column.commodity"         ) );
  m_columns.push_back( TR8( "gcw.AccountsWidget.column.description"       ) );
  m_columns.push_back( TR8( "gcw.AccountsWidget.column.futureminimum"     ) );
  m_columns.push_back( TR8( "gcw.AccountsWidget.column.futureminimumusd"  ) );
  m_columns.push_back( TR8( "gcw.AccountsWidget.column.hidden"            ) );
  m_columns.push_back( TR8( "gcw.AccountsWidget.column.lastnum"           ) );
  m_columns.push_back( TR8( "gcw.AccountsWidget.column.lastreconciledate" ) );
  m_columns.push_back( TR8( "gcw.AccountsWidget.column.notes"             ) );
  m_columns.push_back( TR8( "gcw.AccountsWidget.column.openingbalance"    ) );
  m_columns.push_back( TR8( "gcw.AccountsWidget.column.placeholder"       ) );
  m_columns.push_back( TR8( "gcw.AccountsWidget.column.present"           ) );
  m_columns.push_back( TR8( "gcw.AccountsWidget.column.presentusd"        ) );
  m_columns.push_back( TR8( "gcw.AccountsWidget.column.reconciled"        ) );
  m_columns.push_back( TR8( "gcw.AccountsWidget.column.reconciledusd"     ) );
  m_columns.push_back( TR8( "gcw.AccountsWidget.column.taxinfo"           ) );
  m_columns.push_back( TR8( "gcw.AccountsWidget.column.total"             ) );
  m_columns.push_back( TR8( "gcw.AccountsWidget.column.totalperiod"       ) );
  m_columns.push_back( TR8( "gcw.AccountsWidget.column.totalusd"          ) );
  m_columns.push_back( TR8( "gcw.AccountsWidget.column.type"              ) );

  setModel();

} // endGCW::Gui::AccountsWidget::AccountsWidget()

/*!
** \return GUID String
*/
std::string GCW::Gui::AccountsWidget::
selectedAccount() const
{
  std::string retVal;

  /*
  ** The tree-view should have only one selection.  Grab
  **  its index and get to the User data that carries the
  **  GUID of the selected item.
  **
  */
  auto selected = treeView()-> selectedIndexes();
  if( selected.size() == 1 )
    retVal =
      Wt::asString
      (
       model()-> data( *selected.begin(),
       Wt::ItemDataRole::User
      )
    ).toUTF8();

  return retVal;

} // endstd::string GCW::Gui::AccountsWidget::selectedAccount()

void GCW::Gui::AccountsWidget::
editAccount( const std::string & _accountGuid )
{
  if( _accountGuid == "" )
    return;

  GCW::Gui::AccountEditorDialog dialog( "Edit Account" );

  dialog.exec();

} // endvoid GCW::Gui::AccountsWidget::editAccount( const std::string & _accountGuid )


void GCW::Gui::AccountsWidget::
editSelectedAccount()
{
  editAccount( selectedAccount() );

} // endvoid GCW::Gui::AccountsWidget::editAccount( const std::string & _accountGuid )


void GCW::Gui::AccountsWidget::
setModel()
{
  m_model = std::make_shared< Model >();

  m_model-> load();

  treeView()-> setModel( m_model );

  treeView()-> sortByColumn( 0, Wt::SortOrder::Ascending );

} // endvoid GCW::Gui::AccountsWidget::setModel()

Wt::Json::Object GCW::Gui::AccountsWidget::
toJson() const
{
  Wt::Json::Object jobj;
  jobj["selectedAccount"] = Wt::WString( selectedAccount() );

  for( int col=0; col< 7; col++ )
    jobj[ Wt::WString("columnWidth-{1}").arg( col ).toUTF8() ] = Wt::WString( treeView()-> columnWidth( col ).cssText() );

  return jobj;
}

bool GCW::Gui::AccountsWidget::
fromJson( const Wt::Json::Object & _jobj )
{
  return true;
}

void GCW::Gui::AccountsWidget::
test()
{
  std::cout << __FILE__ << ":" << __LINE__ << " " << std::endl;

  std::cout << __FILE__ << ":" << __LINE__ << " " << Wt::Json::serialize( toJson() ) << std::endl;

} // endvoid GCW::Gui::AccountsWidget::test()

void GCW::Gui::AccountsWidget::Model::
load()
{
  /*
  ** If the session isn't open then there's nothing to load.
  **
  */
  if( !GCW::app()-> gnucash_session().isOpen() )
    return;

  load( invisibleRootItem(), GCW::Dbo::Accounts::root() );

  int col = 0;
  setHeaderData( col++, TR( "gcw.AccountsWidget.column.accountname"      ) );
  setHeaderData( col++, TR( "gcw.AccountsWidget.column.accountcode"      ) );
  setHeaderData( col++, TR( "gcw.AccountsWidget.column.description"      ) );
  setHeaderData( col++, TR( "gcw.AccountsWidget.column.taxinfo"          ) );
  setHeaderData( col++, TR( "gcw.AccountsWidget.column.notes"            ) );
  setHeaderData( col++, TR( "gcw.AccountsWidget.column.futureminimumusd" ) );
  setHeaderData( col++, TR( "gcw.AccountsWidget.column.total"            ) );

} // endvoid GCW::Gui::AccountsWidget::Model::load()

void GCW::Gui::AccountsWidget::Model::
load( Wt::WStandardItem * _treeItem, GCW::Dbo::Accounts::Item::Ptr _parentAccount )
{
  Wt::Dbo::Transaction t( GCW::app()-> gnucash_session() );

  auto _append = [=]( Wt::WStandardItem * _item, GCW::Dbo::Accounts::Item::Ptr _accountItem )
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
    GCW::app()-> gnucash_session().find< GCW::Dbo::Accounts::Item >()
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

void GCW::Gui::AccountsWidget::
doubleClicked( const Wt::WModelIndex & index, const Wt::WMouseEvent & event )
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

} // endvoid GCW::Gui::AccountsWidget::doubleClicked( const Wt::WModelIndex & index, const Wt::WMouseEvent & event )


