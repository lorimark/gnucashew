#line 2 "src/Gui/AccountsTreeView.cpp"

#include <any>

#include <Wt/Json/Array.h>
#include <Wt/Json/Parser.h>
#include <Wt/Json/Serializer.h>
#include <Wt/WText.h>
#include <Wt/WTreeTableNode.h>
#include <Wt/WVBoxLayout.h>

#include "../define.h"
#include "AccountEditor.h"
#include "AccountsTreeView.h"


GCW::Gui::AccountsTreeView::
AccountsTreeView()
{
  addStyleClass( "AccountsTreeView" );

  auto lw = setLayout( std::make_unique< Wt::WVBoxLayout >() );
  lw-> setSpacing( 0 );

  auto w = std::make_unique< Wt::WTreeView >();
  m_view = w.get();
  lw-> addWidget( std::move( w ) );

  view()-> setSelectionBehavior( Wt::SelectionBehavior::Rows );
  view()-> setSelectionMode(     Wt::SelectionMode::Single   );
  view()-> setAlternatingRowColors( true );
  view()-> doubleClicked().connect( this, &AccountsTreeView::on_doubleClicked );

  m_columns.push_back( TR8( "gcw.AccountsTreeView.column.accountcode"       ) );
  m_columns.push_back( TR8( "gcw.AccountsTreeView.column.accountcolor"      ) );
  m_columns.push_back( TR8( "gcw.AccountsTreeView.column.accountname"       ) );
  m_columns.push_back( TR8( "gcw.AccountsTreeView.column.balance"           ) );
  m_columns.push_back( TR8( "gcw.AccountsTreeView.column.balancelimit"      ) );
  m_columns.push_back( TR8( "gcw.AccountsTreeView.column.balanceperiod"     ) );
  m_columns.push_back( TR8( "gcw.AccountsTreeView.column.balanceusd"        ) );
  m_columns.push_back( TR8( "gcw.AccountsTreeView.column.cleared"           ) );
  m_columns.push_back( TR8( "gcw.AccountsTreeView.column.clearedusd"        ) );
  m_columns.push_back( TR8( "gcw.AccountsTreeView.column.commodity"         ) );
  m_columns.push_back( TR8( "gcw.AccountsTreeView.column.description"       ) );
  m_columns.push_back( TR8( "gcw.AccountsTreeView.column.futureminimum"     ) );
  m_columns.push_back( TR8( "gcw.AccountsTreeView.column.futureminimumusd"  ) );
  m_columns.push_back( TR8( "gcw.AccountsTreeView.column.hidden"            ) );
  m_columns.push_back( TR8( "gcw.AccountsTreeView.column.lastnum"           ) );
  m_columns.push_back( TR8( "gcw.AccountsTreeView.column.lastreconciledate" ) );
  m_columns.push_back( TR8( "gcw.AccountsTreeView.column.notes"             ) );
  m_columns.push_back( TR8( "gcw.AccountsTreeView.column.openingbalance"    ) );
  m_columns.push_back( TR8( "gcw.AccountsTreeView.column.placeholder"       ) );
  m_columns.push_back( TR8( "gcw.AccountsTreeView.column.present"           ) );
  m_columns.push_back( TR8( "gcw.AccountsTreeView.column.presentusd"        ) );
  m_columns.push_back( TR8( "gcw.AccountsTreeView.column.reconciled"        ) );
  m_columns.push_back( TR8( "gcw.AccountsTreeView.column.reconciledusd"     ) );
  m_columns.push_back( TR8( "gcw.AccountsTreeView.column.taxinfo"           ) );
  m_columns.push_back( TR8( "gcw.AccountsTreeView.column.total"             ) );
  m_columns.push_back( TR8( "gcw.AccountsTreeView.column.totalperiod"       ) );
  m_columns.push_back( TR8( "gcw.AccountsTreeView.column.totalusd"          ) );
  m_columns.push_back( TR8( "gcw.AccountsTreeView.column.type"              ) );

  setModel();

  loadConfig();

  view()-> collapsed        ().connect( this, &AccountsTreeView::saveConfig    );
  view()-> expanded         ().connect( this, &AccountsTreeView::saveConfig    );
  view()-> selectionChanged ().connect( this, &AccountsTreeView::saveConfig    );

} // endGCW::Gui::AccountsTreeView::AccountsTreeView()

/*!
** \return GUID String
*/
std::string
GCW::Gui::AccountsTreeView::
selectedAccount() const
{
  std::string retVal;

  /*
  ** The tree-view should have only one selection.  Grab
  **  its index and get to the User data that carries the
  **  GUID of the selected item.
  **
  */
  auto selected = view()-> selectedIndexes();
  if( selected.size() == 1 )
    retVal =
      Wt::asString
      (
       model()-> data( *selected.begin(),
       Wt::ItemDataRole::User
      )
    ).toUTF8();

  return retVal;

} // endstd::string GCW::Gui::AccountsTreeView::selectedAccount()

void
GCW::Gui::AccountsTreeView::
editAccount( const std::string & _accountGuid )
{
  if( _accountGuid == "" )
    return;

  GCW::Gui::AccountEditorDialog dialog( "Edit Account" );

  dialog.exec();

} // endvoid GCW::Gui::AccountsTreeView::editAccount( const std::string & _accountGuid )


void
GCW::Gui::AccountsTreeView::
editSelectedAccount()
{
  editAccount( selectedAccount() );

} // endvoid GCW::Gui::AccountsTreeView::editAccount( const std::string & _accountGuid )


void
GCW::Gui::AccountsTreeView::
setModel()
{
  m_model = std::make_shared< Model >();

  m_model-> load();

  view()-> setModel( m_model );

  view()-> sortByColumn( 0, Wt::SortOrder::Ascending );

} // endvoid GCW::Gui::AccountsTreeView::setModel()

GCW::Dbo::Vars::Item::Ptr
GCW::Gui::AccountsTreeView::
configItem()
{
  GCW::Dbo::Vars::Item::Ptr retVal;

  if( GCW::app()-> gnucashew_session().hasGnuCashewExtensions() )
  {
    Wt::Dbo::Transaction t( GCW::app()-> gnucashew_session() );

    retVal = GCW::Dbo::Vars::get( "config", "AccountsTreeView" );
  }

  return retVal;

} // endconfigItem()


void
GCW::Gui::AccountsTreeView::
saveConfig()
{
  if( !GCW::app()-> gnucashew_session().hasGnuCashewExtensions() )
    return;

  Wt::Dbo::Transaction t( GCW::app()-> gnucashew_session() );
  configItem().modify()-> setVarField( Wt::Json::serialize( toJson() ) );

} // endsaveConfig()

void
GCW::Gui::AccountsTreeView::
loadConfig()
{
  if( !GCW::app()-> gnucashew_session().hasGnuCashewExtensions() )
    return;

  Wt::Json::Object jobj;
  try {
    Wt::Json::parse( configItem()-> varField(), jobj );
  }
  catch( std::exception & e )
  {
//    std::cout << __FILE__ << ":" << __LINE__ << " " << e.what() << std::endl;
  }

  fromJson( jobj );

} // endloadConfig()

/*
** This will iterate a single a WTreeView and fill
**  a vector of every node which is the .last-expanded.
**  node of every branch.
**
*/
bool
GCW::Gui::AccountsTreeView::
iterate( Wt::Json::Array & _jary, Wt::WModelIndex _parent ) const
{
  /*
  ** If this _parent node is not expanded, then we're basically done.
  **
  */
  if( !view()-> isExpanded( _parent ) )
    return false;

  /*
  ** This _parent node is expanded, so loop through all the
  **  child nodes checking if any of them are expanded.
  **
  */
  bool expanded = false;
  for( int row=0; row< view()-> model()-> rowCount( _parent ); row++ )
    expanded |= iterate( _jary, view()-> model()-> index( row, 0, _parent ) );

  /*
  ** None of the child nodes are expanded, so record this _parent
  **  node as the 'last' node in the tree
  **
  */
  if( !expanded )
  {
    /*
    ** The true root node is not associated with an actual account,
    **  it is simply the invisibleRoot of the tree itself, and only
    **  contains the set of first-root nodes that actually get
    **  displayed.  So, there is no User data in this one, don't record it.
    **
    */
    auto accountGuid = Wt::asString( _parent.data( Wt::ItemDataRole::User ) );
    if( accountGuid != "" )
      _jary.push_back( accountGuid );

  } // endif( !expanded )

  /*
  ** Something is expanded.  Either we are expanded, or
  **  one of the sub-nodes are expanded, so return that 'someone' is
  **  expanded.
  **
  */
  return true;

} // endvoid iterate( Wt::WModelIndex _index ) const


Wt::Json::Object
GCW::Gui::AccountsTreeView::
toJson() const
{
  Wt::Json::Object jobj;

  jobj["selected"] = Wt::WString( selectedAccount() );

  for( int col=0; col< 7; col++ )
    jobj[ Wt::WString("cw{1}").arg( col ).toUTF8() ] = Wt::WString( view()-> columnWidth( col ).cssText() );

  Wt::Json::Array jary;
  iterate( jary );
  jobj["expanded"] = jary;

  return jobj;

}

bool
GCW::Gui::AccountsTreeView::
expandNode( const std::string & _accountGuid, Wt::WModelIndex _parent )
{
  bool retVal = false;

  /*
  ** Loop through all the children in this node
  **
  */
//  Wt::Dbo::Transaction t( GCW::app()-> gnucashew_session() );
  for( int row=0; row< view()-> model()-> rowCount( _parent ); row++ )
  {
    /*
    ** get the index for this child of this node
    */
    auto child = view()-> model()-> index( row, 0, _parent );

    /*
    ** get the guid of this child
    */
    auto nodeGuid = Wt::asString( child.data( Wt::ItemDataRole::User ) ).toUTF8();

    /*
    ** if this node matches the account, expand it and set the
    **  return to 'found'
    **
    */
    if( nodeGuid == _accountGuid )
    {
      view()-> expand( child );
      retVal |= true;
    }

    /*
    ** remember if any of the sub-nodes get expanded.
    **
    */
    retVal |= expandNode( _accountGuid, child );

  } // endfor( int row=0; row< view()-> model()-> rowCount( _parent ); row++ )

  /*
  ** Either this node was expanded, or any one of
  **  the child nodes was expanded, so therefore we
  **  need to also expand this node.
  **
  */
  if( retVal )
    view()-> expand( _parent );

  /*
  ** None of the nodes here got expanded
  **
  */
  return retVal;

} // endexpandNode( const std::string & _accountGuid, Wt::WModelIndex _parent )

bool
GCW::Gui::AccountsTreeView::
expandTreeNodes( Wt::Json::Object & _jobj )
{
  auto jary = _jobj.get("expanded").orIfNull( Wt::Json::Array() );

  for( auto value : jary )
    expandNode( value.orIfNull( "" ) );

  return true;

} // endexpandNodes()

/*!
** \brief Find Index by AccountGuid
**
** This will loop through the tree and locate a specific
**  index by it's accountGuid value.
**
*/
Wt::WModelIndex
GCW::Gui::AccountsTreeView::
findIndex( const std::string & _accountGuid, Wt::WModelIndex _parentIndex )
{
  /*
  ** If this is the index we are looking for, then just return it.
  **
  */
  if( Wt::asString( _parentIndex.data( Wt::ItemDataRole::User ) ) == _accountGuid )
    return _parentIndex;

  /*
  ** Loop through all the child nodes checking them for
  **  matches
  **
  */
  for( int row=0; row< view()-> model()-> rowCount( _parentIndex ); row++ )
  {
    auto childIndex = findIndex( _accountGuid, view()-> model()-> index( row, 0, _parentIndex ) );

    /*
    ** If we get back a valid index, then we have what we
    **  need and can just return it.
    **
    */
    if( childIndex.isValid() )
      return childIndex;

  } // endfor( int row=0; row< view()-> model()-> rowCount( _parentIndex ); row++ )

  /*
  ** Return an invalid index indicating not-found.
  **
  */
  return Wt::WModelIndex();

} // endfindIndex( const std::string & _accountGuid, Wt::WModelIndex _parentIndex )

bool
GCW::Gui::AccountsTreeView::
setSelected( const std::string & _accountGuid )
{
  auto index = findIndex( _accountGuid );

  view()-> select   ( index                                   );
  view()-> scrollTo ( index, Wt::ScrollHint::PositionAtCenter );

  return true;

} // endexpandNodes()

bool
GCW::Gui::AccountsTreeView::
fromJson( Wt::Json::Object & _jobj )
{
  expandTreeNodes( _jobj );

  setSelected( _jobj.get("selected").orIfNull( std::string() ) );

  return true;

} // endfromJson( const Wt::Json::Object & _jobj )

void
GCW::Gui::AccountsTreeView::
test()
{
  std::cout << __FILE__ << ":" << __LINE__ << " " << std::endl;

  std::cout << __FILE__ << ":" << __LINE__ << " " << Wt::Json::serialize( toJson() ) << std::endl;

} // endvoid GCW::Gui::AccountsTreeView::test()

#ifdef USE_GNUCASHEW_SESSION

void
GCW::Gui::AccountsTreeView::Model::
load()
{
  /*
  ** If the session isn't open then there's nothing to load.
  **
  */
  if( !GCW::app()-> gnucashew_session().isOpen() )
    return;

  /*
  ** load the data in to the model
  **
  */
  auto rootAccount = GCW::Dbo::Accounts::root();
  if( rootAccount-> guid() != "" )
  {
    load( invisibleRootItem(), rootAccount );

    /*
    ** define all the columns
    **
    */
    int col = 0;
    setHeaderData( col++, TR( "gcw.AccountsTreeView.column.accountname"      ) );
    setHeaderData( col++, TR( "gcw.AccountsTreeView.column.accountcode"      ) );
    setHeaderData( col++, TR( "gcw.AccountsTreeView.column.description"      ) );
    setHeaderData( col++, TR( "gcw.AccountsTreeView.column.taxinfo"          ) );
    setHeaderData( col++, TR( "gcw.AccountsTreeView.column.notes"            ) );
    setHeaderData( col++, TR( "gcw.AccountsTreeView.column.futureminimumusd" ) );
    setHeaderData( col++, TR( "gcw.AccountsTreeView.column.total"            ) );

  } // endif( rootAccount-> guid() != "" )

} // endvoid GCW::Gui::AccountsTreeView::Model::load()

void
GCW::Gui::AccountsTreeView::Model::
load( Wt::WStandardItem * _treeItem, GCW::Dbo::Accounts::Item::Ptr _parentAccount )
{
  Wt::Dbo::Transaction t( GCW::app()-> gnucashew_session() );

  auto _append = [=]( Wt::WStandardItem * _item, GCW::Dbo::Accounts::Item::Ptr _accountItem )
  {
    std::vector< std::unique_ptr< Wt::WStandardItem > > columns;

    auto accountName = std::make_unique< Wt::WStandardItem >( _accountItem-> name() );
    accountName-> setToolTip( _accountItem-> guid() );

#ifdef NEVER
    if( _accountItem-> hasColor() )
    {
      auto accountColor = _accountItem-> color();
      std::cout << __FILE__ << ":" << __LINE__ << " " << accountColor << std::endl;
    }
#endif

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
    GCW::app()-> gnucashew_session().find< GCW::Dbo::Accounts::Item >()
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

#endif // #ifdef USE_GNUCASHEW_SESSION

#ifdef USE_GNUCASH_ENGINE

void
GCW::Gui::AccountsTreeView::Model::
load()
{
  /*
  ** If the session isn't open then there's nothing to load.
  **
  */
  if( !GCW::app()-> gnucash_session().isOpen() )
    return;

  auto rootAccount = GCW::app()-> gnucash_session().book().rootAccount();

  /*
  ** load the data in to the model
  **
  */
  if( rootAccount.guid() != "" )
  {
    load( invisibleRootItem(), rootAccount );

    /*
    ** define all the columns
    **
    */
    int col = 0;
    setHeaderData( col++, TR( "gcw.AccountsTreeView.column.accountname"      ) );
    setHeaderData( col++, TR( "gcw.AccountsTreeView.column.accountcode"      ) );
    setHeaderData( col++, TR( "gcw.AccountsTreeView.column.description"      ) );
    setHeaderData( col++, TR( "gcw.AccountsTreeView.column.taxinfo"          ) );
    setHeaderData( col++, TR( "gcw.AccountsTreeView.column.notes"            ) );
    setHeaderData( col++, TR( "gcw.AccountsTreeView.column.futureminimumusd" ) );
    setHeaderData( col++, TR( "gcw.AccountsTreeView.column.total"            ) );

  } // endif( rootAccount-> guid() != "" )

} // endvoid GCW::Gui::AccountsTreeView::Model::load()

void
GCW::Gui::AccountsTreeView::Model::
load( Wt::WStandardItem * _treeItem, GCW::Dbo::GnuCash::Account _parentAccount )
{
  auto _append = [=]( Wt::WStandardItem * _item, GCW::Dbo::GnuCash::Account _accountItem )
  {
    std::vector< std::unique_ptr< Wt::WStandardItem > > columns;

    auto accountName = std::make_unique< Wt::WStandardItem >( _accountItem.name() );
    auto retVal = accountName.get();

    accountName-> setToolTip( _accountItem.guid() );

#ifdef NEVER
    if( _accountItem-> hasColor() )
    {
      auto accountColor = _accountItem-> color();
      std::cout << __FILE__ << ":" << __LINE__ << " " << accountColor << std::endl;
    }
#endif

    /*
    ** set the 'model->data::User' element to contain the guid of the account, so
    **  we can recover it later.
    **
    */
    accountName-> setData( _accountItem.guid(), Wt::ItemDataRole::User );
    columns.push_back( std::move( accountName ) );
    columns.push_back( std::make_unique< Wt::WStandardItem >( _accountItem.code        () ) );
    columns.push_back( std::make_unique< Wt::WStandardItem >( _accountItem.description () ) );
    columns.push_back( std::make_unique< Wt::WStandardItem >(                             ) );
    columns.push_back( std::make_unique< Wt::WStandardItem >( _accountItem.notes       () ) );
    columns.push_back( std::make_unique< Wt::WStandardItem >(                             ) );
    columns.push_back( std::make_unique< Wt::WStandardItem >(                             ) );
    _item-> appendRow( std::move( columns ) );

    return retVal;
  };

  auto accounts = _parentAccount.children();

  for( auto account : accounts )
  {
    auto ti = _append( _treeItem, account );
    load( ti, account );
  }

} // endvoid load( Wt::WStandardItem * _treeItem, Account::Ptr _parentAccount )

#endif // #ifdef USE_GNUCASH_ENGINE


void
GCW::Gui::AccountsTreeView::
on_doubleClicked( const Wt::WModelIndex & index, const Wt::WMouseEvent & event )
{
#ifdef NEVER
  std::cout << std::endl << std::endl << __FILE__ << ":" << __LINE__
    << " " << "on_doubleClicked:"
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

} // endvoid GCW::Gui::AccountsTreeView::on_doubleClicked( const Wt::WModelIndex & index, const Wt::WMouseEvent & event )


