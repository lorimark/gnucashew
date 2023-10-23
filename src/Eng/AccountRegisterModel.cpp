#line 2 "src/Eng/AccountRegisterModel.cpp"

#include "AccountRegisterModel.h"
#include "../Dbo/Splits.h"
#include "../Dbo/Transactions.h"

GCW::Eng::AccountRegisterModel::
AccountRegisterModel( const std::string & _accountGuid )
: Wt::WStandardItemModel( 0, 8 ), // 8-columns
  m_accountGuid( _accountGuid )
{
  refreshFromDisk();

  dataChanged().connect( [=]( Wt::WModelIndex _index1, Wt::WModelIndex _index2 )
  {
//    std::cout << __FILE__ << ":" << __LINE__
//    << " r1:" << _index1.row() << " c1:" << _index1.column()
//    << " r2:" << _index2.row() << " c2:" << _index2.column()
//    << std::endl;

  });

  itemChanged().connect( [=]( Wt::WStandardItem * _item )
  {
//    std::cout << __FILE__ << ":" << __LINE__ << " " << Wt::asString( _item-> data() ) << std::endl;

  });

} // endGCW::Eng::AccountRegisterModel::AccountRegisterModel( const std::string & _accountGuid )

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
void
GCW::Eng::AccountRegisterModel::
refreshFromDisk()
{
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
  **    5   increase
  **    6   decrease
  **    7   balance r/o (computed)
  ** \endcode
  **
  */
  auto _addColumn = [&]( RowItem & columns, auto _value )
  {
    auto item = std::make_unique< Wt::WStandardItem >( _value );

    item-> setToolTip( _value );

    auto retVal = item.get();
    columns.push_back( std::move( item ) );
    return retVal;
  };

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
  **  than the transaction itself.
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
  DECIMAL::decimal<2> runningBalance( 0 );
  for( auto splitItem : splitItems )
  {
    RowItem columns;

    /*
    ** Start out read-only.
    **
    */
    bool editable = false;

    /*!
    ** From the initial split item, we get a handle on the transaction,
    **  and then load all of the other splits associated with this
    **  transaction.
    **
    */
    auto transactionItem   = GCW::Dbo::Transactions ::byGuid  ( splitItem-> tx_guid() );
    auto transactionSplits = GCW::Dbo::Splits       ::bySplit ( splitItem-> guid()    );

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
    auto post_date = _addColumn( columns, transactionItem-> post_date_as_date().toString( GCW::CFG::date_format() ) );
         post_date-> setData( splitItem-> guid() );

    auto num = _addColumn( columns, transactionItem-> num() );

    auto description = _addColumn( columns, transactionItem-> description() );

    /*!
    ** The 'account' text depends on the
    **  target account defined in the split.  There are three
    **  options here;
    **
    **   -# no splits... this shows up as an <b>'imbalance'</b>
    **   -# 1 split...   this just shows the split account on the same single line
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
        account = _addColumn( columns, TR("gcw.RegisterWidget.account.imbalanceUSD") ); // account
        account-> setStyleClass( "errval" );
        account-> setToolTip( TR("gcw.RegisterWidget.account.imbalanceUSD.toolTip") );
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

        if( splitAccountItem )
          account = _addColumn( columns, GCW::Dbo::Accounts::fullName( splitAccountItem-> guid() ) );

        else
          account = _addColumn( columns, txSplitItem-> account_guid() );

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
        account = _addColumn( columns, TR("gcw.RegisterWidget.account.multisplit") ); // account
      }

    } // endswitch( transactionSplits.size() )

    auto reconcile = _addColumn( columns, splitItem-> reconcile_state() ); // Reconciled

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
      deposit    = _addColumn( columns, splitItem-> valueAsString() );
      withdrawal = _addColumn( columns, "" );
    }

    else
    if( splitItem-> value() < 0 )
    {
      deposit    = _addColumn( columns, "" );
      withdrawal = _addColumn( columns, splitItem-> valueAsString() );
    }

    else
    if( splitItem-> value() == 0 )
    {
      deposit    = _addColumn( columns, "" );
      withdrawal = _addColumn( columns, "" );
    }

    balance =
      _addColumn
      (
       columns,
       Wt::WString( "{1}" )
       .arg( toString( runningBalance, GCW::CFG::decimal_format() ) )
      );

    if( runningBalance < 0 )
      balance-> setStyleClass( "negval" );

    if( splitItem-> reconcile_state() == "y" )
      editable = false;
    else
      editable = true;

#ifndef NEVER
    if( editable )
    {
      post_date   -> setFlags( Wt::ItemFlag::Editable );
      num         -> setFlags( Wt::ItemFlag::Editable );
      description -> setFlags( Wt::ItemFlag::Editable );
      account     -> setFlags( Wt::ItemFlag::Editable );
      deposit     -> setFlags( Wt::ItemFlag::Editable );
      withdrawal  -> setFlags( Wt::ItemFlag::Editable );
    }
#endif

    /*
    ** Add the row to the model
    **
    */
    appendRow( std::move( columns ) );

  } // endfor( auto splitItem : splitItems )

  /*!
  ** After all the split items are loaded, an additional ~blank~ item
  **  is included at the end of the vector, for coding new entries.
  **
  */
  {
    RowItem columns;
    _addColumn( columns, ""  )-> setFlags( Wt::ItemFlag::Editable ); // Date
    _addColumn( columns, ""  )-> setFlags( Wt::ItemFlag::Editable ); // Num
    _addColumn( columns, ""  )-> setFlags( Wt::ItemFlag::Editable ); // Memo
    _addColumn( columns, ""  )-> setFlags( Wt::ItemFlag::Editable ); // Account
    _addColumn( columns, "n" )                                     ; // R
    _addColumn( columns, ""  )-> setFlags( Wt::ItemFlag::Editable ); // Deposit
    _addColumn( columns, ""  )-> setFlags( Wt::ItemFlag::Editable ); // Withdrawal
    _addColumn( columns, ""  )                                     ; // Balance
    appendRow( std::move( columns ) );
  }

  int col = 0;
  setHeaderData( col++, TR( "gcw.RegisterWidget.column.date"       ) );
  setHeaderData( col++, TR( "gcw.RegisterWidget.column.num"        ) );
  setHeaderData( col++, TR( "gcw.RegisterWidget.column.memo"       ) );
  setHeaderData( col++, TR( "gcw.RegisterWidget.column.account"    ) );
  setHeaderData( col++, TR( "gcw.RegisterWidget.column.reconcile"  ) );
  setHeaderData( col++, TR( "gcw.RegisterWidget.column.deposit"    ) );
  setHeaderData( col++, TR( "gcw.RegisterWidget.column.withdrawal" ) );
  setHeaderData( col++, TR( "gcw.RegisterWidget.column.balance"    ) );

} // endGCW::Eng::AccountRegisterModel::refreshFromDisk()


GCW::Eng::AccountRegisterModel::RowItem
GCW::Eng::AccountRegisterModel::
makeRow( const std::string & _splitGuid )
{
  RowItem rowItem;

  return rowItem;

} // endGCW::Eng::AccountRegisterModel::RowItem GCW::Eng::AccountRegisterModel::makeRow( const std::string & _splitGuid )

std::set< std::string >
GCW::Eng::AccountRegisterModel::
suggestionsFromColumn( int _column ) const
{
  std::set< std::string > retVal;

  for( int row=0; row< rowCount(); row++ )
    retVal.insert( Wt::asString( item( row, _column )-> text() ).toUTF8() );

  return retVal;

} // endstd::set< std::string > GCW::Eng::AccountRegisterModel::suggestionsFromColumn( int _column ) const


