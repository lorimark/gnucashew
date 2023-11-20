#line 2 "src/Eng/AccountRegisterModel.cpp"

#include "AccountRegisterModel.h"
#include "../Dbo/Splits.h"
#include "../Dbo/Prefrences.h"
#include "../Dbo/Transactions.h"
#include "../Dbo/Vars.h"

GCW::Eng::AccountRegisterModel::
AccountRegisterModel( const std::string & _accountGuid, bool _editable )
: Wt::WStandardItemModel( 0, 8 ), // 8-columns
  m_accountGuid( _accountGuid ),
  m_editable( _editable )
{
  /*
  ** set the lastDate to match the todays date, so when first
  **  opening the register, the date is automatically set.
  **
  */
  m_lastDate = Wt::WDate::currentDate().toString( GCW::CFG::date_format() ).toUTF8();

  refreshFromDisk();

  dataChanged().connect( [=]( Wt::WModelIndex _index1, Wt::WModelIndex _index2 )
  {
    std::cout << __FILE__ << ":" << __LINE__ << " model.dataChanged()"

      << " r1:" << _index1.row()
      << " c1:" << _index1.column()
      << " v1:" << Wt::asString( _index1.data() )

      << " r2:" << _index2.row()
      << " c2:" << _index2.column()
      << " v2:" << Wt::asString( _index2.data() )

      << std::endl;

  });

  itemChanged().connect( [=]( Wt::WStandardItem * _item )
  {
    std::cout << __FILE__ << ":" << __LINE__ << " model.itemChanged()"
      << Wt::asString( _item-> data() )
      << std::endl;

  });

} // endGCW::Eng::AccountRegisterModel::AccountRegisterModel( const std::string & _accountGuid )


bool
GCW::Eng::AccountRegisterModel::
setData( const Wt::WModelIndex & _index, const Wt::cpp17::any & _value, Wt::ItemDataRole _role )
{
  auto retVal = Wt::WStandardItemModel::setData( _index, _value, _role );

  std::cout << __FILE__ << ":" << __LINE__ << " setData()"
    << "\n rv:" << retVal
    << "\n id:" << Wt::asString( _index.data() )
    << "\n va:" << Wt::asString( _value )
    << std::endl;

  return retVal;

} // endsetData( const Wt::WModelIndex & _index, const Wt::cpp17::any & _value, Wt::ItemDataRole _role )

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
  **    5   debit
  **    6   credit
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

  /*
  ** Get the prefrence item that can inform us about prefrences
  **  to be applied to this model.
  **
  */
  auto prefrenceItem = GCW::Dbo::Prefrences::get();

  /*
  ** Get an account item loaded.  This is the account that _is_ this
  **  register.
  **
  */
  auto accountItem = GCW::Dbo::Accounts::byGuid( m_accountGuid );

#ifdef NEVER
  std::cout << __FILE__ << ":" << __LINE__
    << " guid:" << accountItem-> guid()
    << " name:" << accountItem-> name()
    << " dbcr:" << static_cast<int>( accountItem-> accountDbCr() )
    << " type:" << static_cast<int>( accountItem-> accountType() )
    << " typn:" << accountItem-> accountTypeName()
    << std::endl;
#endif

  /*!
  ** In order to produce a proper 'register' of items, it is important
  **  to load the data from the 'splits' side of the transaction rather
  **  than the transaction itself.
  **
  ** Note that when the splits are loaded based on the account ID, they
  **  are returned in a std::vector that is sorted based on the transction
  **  date.  This chosen sort method insures that the running balance
  **  can be accurately calculated on the fly.
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
  **  subsequently resorted or subset-extracted without affecting
  **  the running balances and so forth.
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
    ** \note The post_date column (col-0) also carries with it the guid of the split
    **  item itself, so that the originating split can be located from the table
    **  view.  The guid can be accessed by;
    **
    ** \code
    ** auto splitRowGuid = Wt::asString( standardItem.data( Wt::ItemRole::User ) )
    ** \endcode
    **
    */
    auto post_date = _addColumn( columns, transactionItem-> post_date_as_date().toString( GCW::CFG::date_format() ) );
         post_date-> setData( splitItem-> guid(), Wt::ItemDataRole::User );
         post_date-> setToolTip( splitItem-> guid() );

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

        // yes account item
        if( splitAccountItem )
          account = _addColumn( columns, GCW::Dbo::Accounts::fullName( splitAccountItem-> guid() ) );

        // no account item
        else
        {
          /*!
          ** \par Another Imbalance
          ** This is another problem... We have another split, but the account
          **  we are split-to doesn't exist.  This is a problem and should not
          **  happen and represents an error in the database.
          **
          */
          account = _addColumn( columns, TR("gcw.RegisterWidget.account.imbalanceUSD") );
          account-> setStyleClass( "errval" );

          auto toolTip =
            Wt::WString("target guid:{1}\n{2}")
            .arg( txSplitItem-> account_guid() )
            .arg( TR("gcw.RegisterWidget.account.invalidTarget.toolTip") )
            .toUTF8()
            ;

          account-> setToolTip( toolTip );

        } // endelse no account item

        break;

      } // endcase 1:

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
    ** Values of the transaction appear either in the Debit column (positive-Left)
    **  or in the Credit column (negative-Right) depending on if they are positive
    **  or negative.  The 'decimal.h' library is used to perform the
    **  arithmetic to prevent the floating point math problems.
    **
    ** The balance on the transaction is computed on-the-fly, which
    **  makes clear the importance of having the initial vector of splits
    **  to appear in the correct chronological order.
    **
    */
    Wt::WStandardItem * debit   = nullptr;
    Wt::WStandardItem * credit  = nullptr;
    Wt::WStandardItem * balance = nullptr;

    /*!
    ** \par Balance Computation Notes
    **
    **  There are two 'types' of accounts; Debit/Credit.  Gnucash
    **   stores split information as a single value that is positive
    **   or negative.  If the value is positive, then it is posted
    **   to the debit (left) column.  If the value is negative, it
    **   is posted to credit (right) column.
    **
    **  Depending on the account type (debit/credit), that value is
    **   then either 'added' or 'subtracted' from the account balance.
    **   If this is a 'credit' account, then the value is subtracted,
    **   and if it is a debit account, the value is added.
    **
    **  Therefore, if this is a credit account, such as a credit card,
    **   then a 'positive' value, posted to the debit column, would
    **   'decrease' the balance in that account.  Therefore, the value,
    **   being positive, is 'subtracted' from the running balance.  If
    **   the value were negative, it would be posted to the credit
    **   column, and again would be 'subtracted' from the running
    **   balance, and a negative value being subtracted from a value
    **   causes the result to 'increase'.
    **
    **  If this is a debit account, such as a bank checking account, a
    **   'positive' value, posted to the debit column (again), would
    **   'increase' the balance in that account.
    **
    **  So, that's the funky GAAP math done here.
    **
    ** What follows is a pretty good explanation of the debit/credit
    **  stuff;
    **
    **  >>>>>>>>>>>>>>>
    **  Debit/Credit is just Left/Right.
    **  Maybe this will help...
    **
    **  The Accounting Equation:
    **  Assets - Liabilities = Equity
    **
    **      (let's make all terms 'positive')
    **  Assets = Liabilities + Equity
    **
    **      (now, we'll split off a subset of Equity)
    **  Assets = Liabilities + Equity + Retained Earnings
    **
    **      (now, we'll substitute temporary accounts for Retained Earnings)
    **  Assets = Liabilities + Equity + (Income - Expenses)
    **
    **      (now, we'll once again, make all terms 'positive')
    **  Assets + Expenses = Liabilities + Equity + Income
    **
    **  And there, you have the full Accounting Equation with the five major account types that GnuCash uses.
    **
    **  In double-entry accounting, ALL transactions are in the form of:
    **  Debit = Credit
    **  Left = Right
    **
    **  The 'Debit' accounts (those that are normally (positive) a Debit balance, and increase with a Debit, decrease with a Credit) are on the left of the equation:
    **  Assets
    **  Expenses
    **
    **  The 'Credit' accounts (those that are normally (positive) a Credit balance, and increase with a Credit, decrease with a Debit) are those on the right of the equation:
    **  Liabilities
    **  Equity
    **  Income
    **
    **  A negative balance in any account would indicate either an entry error or a contra-balance situation. (rare for individuals)
    **
    **  You can move funds from the left to the right, or vice versa, or between any accounts or types on the same side of the equation. (I will use the abbreviations Dr. and Cr. here)
    **  Most texts will write transactions Debit first, then Credit as shown below. The amounts are not shown, because they *must* be equal.
    **
    ** \par Example Left to Right - Asset to Liability (paying down a debt)
    ** \code
    **  Dr. Liabilities:Loan
    **    Cr. Assets:Cash
    **  result: decreased Loan owed, decreased Cash on hand, Assets decreased, Liabilities decreased - equation still in balance
    ** \endcode
    **
    ** \par Example Right to Left - Income to Asset (receipt of income)
    ** \code
    **  Dr. Assets:Cash
    **    Cr. Income:Salary
    **  result: increased Cash on hand, increased Salary earned, Assets increased, Income increased - equation still in balance
    ** \endcode
    **
    ** \par  Example Left to Left(same type) - Asset to Asset (buying land outright)
    ** \code
    **  Dr. Assets:Land
    **    Cr. Assets:Cash
    **  result: increased Land owned, decreased Cash on hand, Assets shifted - equation still in balance
    ** \endcode
    **
    ** \par  Example Left to Left(different type) - Asset to Expense (buying groceries)
    ** \code
    **  Dr. Expenses:Food
    **    Cr. Assets:Cash
    **  result: increased Food expense, decreased Cash on hand, Expenses increased, Assets decreased - equation still in balance
    ** \endcode
    **
    ** \par  Example Right to Right(same type) - Liability to Liability (paying down a loan with a credit card)
    ** \code
    **  Dr. Liabilities:Loan
    **    Cr. Liabilities:Credit Card
    **  result: decreased Loan owed, increased Credit Card owed, Liabilities shifted - equation still in balance
    ** \endcode
    **
    ** \par  Example Right to Right(different type) - Equity to Liability (recognition of dividends to be paid - business transaction)
    ** \code
    **  Dr. Equity:Retained Earnings
    **    Cr. Liabilities:Dividends Payable
    **  result: decreased Retained Earnings, increased Dividends owed to shareholders, Equity decreased, Liability increased - equation remains in balance.
    ** \endcode
    **
    **  *it is rare and unusualy for an individual to shift Equity to Liabilities and vice versa. Forgiveness of Debt may in some jurisdictions be a transfer from Liabilities to Income.
    **
    ** \par Original Post
    ** \ref https://lists.gnucash.org/pipermail/gnucash-user/2023-October/109219.html
    ** \par Accounting Basics
    ** \ref https://www.gnucash.org/docs/v5/C/gnucash-guide/basics-accounting1.html
    **  >>>>>>>>>>>>>>>
    **
    */
    switch( prefrenceItem.reverseBalanceAccounts() )
    {
      case GCW::Dbo::Prefrences::ReverseBalanceAccounts::INCOME_EXPENSE:
      {
        if( accountItem-> accountType() == GCW::Dbo::Account::Type::INCOME
         || accountItem-> accountType() == GCW::Dbo::Account::Type::EXPENSE
          )
          runningBalance -= splitItem-> value(); // math inverted
        else
          runningBalance += splitItem-> value(); // math normal
        break;
      }

      case GCW::Dbo::Prefrences::ReverseBalanceAccounts::CREDIT:
      {

        if( accountItem-> accountDrCr() == GCW::Dbo::Account::DrCr::DEBIT )
          runningBalance -= splitItem-> value(); // math inverted
        else
          runningBalance += splitItem-> value(); // math normal
        break;
      }

      case GCW::Dbo::Prefrences::ReverseBalanceAccounts::NONE:
      default:
      {
        // math normal
        runningBalance += splitItem-> value();
      }

    } // endswitch( prefrenceItem.reverseBalanceAccounts() )

    /*
    ** if the value is positive, we post it to the debit (left) column.
    */
    if( splitItem-> value() > 0 )
    {
      debit  = _addColumn( columns, splitItem-> valueAsString() );
      credit = _addColumn( columns, "" );
    }

    /*
    ** if the value is negative, we post it to the credit (right) column.
    **  however, we invert the value in this column, so that it does not
    **  carry the (-) leading minus sign... all the numbers we enter are
    **  positive... only the 'balance' column can show negative numbers.
    */
    else
    if( splitItem-> value() < 0 )
    {
      debit  = _addColumn( columns, "" );
      credit = _addColumn( columns, splitItem-> valueAsString(true) );
    }

    /*
    ** if the value is zero, we make sure both columns are blank.
    */
    else
    if( splitItem-> value() == 0 )
    {
      debit  = _addColumn( columns, "" );
      credit = _addColumn( columns, "" );
    }

    /*
    ** Poke the balance in
    **
    */
    balance =
      _addColumn
      (
       columns,
       Wt::WString( "{1}" )
       .arg( toString( runningBalance, GCW::CFG::decimal_format() ) )
      );

    /*
    ** If the balance hit negative, highlight the number with a bit
    **  of bad-news-red.
    **
    */
    if( runningBalance < 0 )
      balance-> setStyleClass( "negval" );

    /*
    ** If we can edit at all, then check the reconciliation
    **  state.  If the split has already been reconciled then
    **  we really don't want the user messing around with it.
    **
    */
    if( m_editable )
    {
      if( splitItem-> reconcile_state() == "y" )
        editable = false;
      else
        editable = true;
    }

    /*
    ** If this item can be edited then unlock everything.
    **
    */
    if( editable )
    {
      post_date   -> setFlags( Wt::ItemFlag::Editable );
      num         -> setFlags( Wt::ItemFlag::Editable );
      description -> setFlags( Wt::ItemFlag::Editable );
      account     -> setFlags( Wt::ItemFlag::Editable );
      debit       -> setFlags( Wt::ItemFlag::Editable );
      credit      -> setFlags( Wt::ItemFlag::Editable );
    }

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
  if( m_editable )
  {
    RowItem columns;
    _addColumn( columns, m_lastDate  )-> setFlags( Wt::ItemFlag::Editable ); // Date
    _addColumn( columns, ""          )-> setFlags( Wt::ItemFlag::Editable ); // Num
    _addColumn( columns, ""          )-> setFlags( Wt::ItemFlag::Editable ); // Memo
    _addColumn( columns, ""          )-> setFlags( Wt::ItemFlag::Editable ); // Account
    _addColumn( columns, "n"         )                                     ; // R
    _addColumn( columns, ""          )-> setFlags( Wt::ItemFlag::Editable ); // Deposit
    _addColumn( columns, ""          )-> setFlags( Wt::ItemFlag::Editable ); // Withdrawal
    _addColumn( columns, ""          )                                     ; // Balance
    appendRow( std::move( columns ) );
  }

  /*
  ** poke all the header labels in.  Note that some of the labels change
  **  depending on the account debit/credit type
  **
  */
  auto accountDef = accountItem-> accountDef();
  int col = 0;
  setHeaderData( col++, TR( "gcw.RegisterWidget.column.date"                     ) );
  setHeaderData( col++, TR( "gcw.RegisterWidget.column.num"                      ) );
  setHeaderData( col++, TR( "gcw.RegisterWidget.column.memo"                     ) );
  setHeaderData( col++, TR( "gcw.RegisterWidget.column." + accountDef.colAccount ) );
  setHeaderData( col++, TR( "gcw.RegisterWidget.column.reconcile"                ) );
  setHeaderData( col++, TR( "gcw.RegisterWidget.column." + accountDef.colDr      ) );
  setHeaderData( col++, TR( "gcw.RegisterWidget.column." + accountDef.colCr      ) );
  setHeaderData( col++, TR( "gcw.RegisterWidget.column.balance"                  ) );

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


