#line 2 "src/RegisterWidget.cpp"

#include <Wt/WDateEdit.h>
#include <Wt/WItemDelegate.h>
#include <Wt/WSuggestionPopup.h>
#include <Wt/WText.h>
#include <Wt/WTableView.h>
#include <Wt/WTheme.h>
#include <Wt/WVBoxLayout.h>
#include <Wt/WHBoxLayout.h>

#include "define.h"
#include "App.h"
#include "RegisterWidget.h"
#include "Dbo/Accounts.h"
#include "Dbo/Splits.h"
#include "Dbo/Transactions.h"

namespace {

class HeaderDelegate
: public Wt::WItemDelegate
{
  public:


    std::unique_ptr< Wt::WWidget > createEditor
    (
     const Wt::WModelIndex & _index,
     Wt::WFlags< Wt::ViewItemRenderFlag > _flags
    ) const;

    virtual Wt::cpp17::any editState( Wt::WWidget *editor, const Wt::WModelIndex &index ) const override;

};

std::unique_ptr< Wt::WWidget > HeaderDelegate::createEditor
(
  const Wt::WModelIndex & _index,
  Wt::WFlags< Wt::ViewItemRenderFlag > _flags
) const
{
  auto retVal = std::make_unique< Wt::WDateEdit >();

  std::cout << __FILE__ << ":" << __LINE__ << " " << _index.row() << std::endl;

  return std::move( retVal );
}

Wt::cpp17::any HeaderDelegate::editState( Wt::WWidget *editor, const Wt::WModelIndex &index ) const
{
  auto dateEdit = dynamic_cast< Wt::WDateEdit* >( editor );

  std::cout << __FILE__ << ":" << __LINE__ << " " << std::endl;

  return dateEdit-> text();
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * */

class DateDelegate
: public Wt::WItemDelegate
{
  public:


    std::unique_ptr< Wt::WWidget > createEditor
    (
     const Wt::WModelIndex & _index,
     Wt::WFlags< Wt::ViewItemRenderFlag > _flags
    ) const;

    virtual Wt::cpp17::any editState( Wt::WWidget *editor, const Wt::WModelIndex &index ) const override;
    void setEditState( Wt::WWidget * _editor, const Wt::WModelIndex & _index, const Wt::cpp17::any & _value ) const;
    void setModelData ( const Wt::cpp17::any & _editState, Wt::WAbstractItemModel * _model, const Wt::WModelIndex & _index ) const;

    void doCloseEditor( Wt::WDateEdit * _dateEdit, bool save ) const;
    void doTabAction( Wt::WKeyEvent _keyEvent ) const;

    mutable Wt::WDateEdit * m_dateEdit = nullptr;
};

std::unique_ptr< Wt::WWidget > DateDelegate::createEditor
(
  const Wt::WModelIndex & _index,
  Wt::WFlags< Wt::ViewItemRenderFlag > _flags
) const
{
  /*
  ** The editor is placed in to a container for layout
  **  management
  **
  */
  auto retVal = std::make_unique< Wt::WContainerWidget >();
  retVal-> setSelectable( true );

  /*
  ** Get the date from the string value
  **
  */
  auto date =
    Wt::WDate::fromString
    (
     Wt::asString( _index.data( Wt::ItemDataRole::Edit ) ),
     GCW::CFG::date_format()
    );

  /*
  ** Build an editor
  **
  ** Hitting the 'enter' key or the 'esc' key closes the editor
  **
  */
  auto dateEdit = std::make_unique< Wt::WDateEdit >();
  m_dateEdit = dateEdit.get();
  dateEdit-> setFormat( GCW::CFG::date_format() );
  dateEdit-> setDate( date );
  dateEdit-> enterPressed  ().connect( [&](){ doCloseEditor( dateEdit.get(), true  ); });
  dateEdit-> escapePressed ().connect( [&](){ doCloseEditor( dateEdit.get(), false ); });
  dateEdit-> keyWentDown   ().connect( [&]( Wt::WKeyEvent _keyEvent ){ doTabAction( _keyEvent ); });

  /*
  ** Stuff it in to the layout
  **
  */
  retVal-> setLayout( std::make_unique< Wt::WHBoxLayout >() );
  retVal-> layout()-> setContentsMargins( 1,1,1,1 );
  retVal-> layout()-> addWidget( std::move( dateEdit ) );

  return retVal;

} // endstd::unique_ptr< Wt::WWidget > DateDelegate::createEditor

void DateDelegate::doCloseEditor( Wt::WDateEdit * _dateEdit, bool save ) const
{
  closeEditor().emit( _dateEdit, save );

#ifdef NEVER
  m_editorClosed.emit( m_row, m_col );
  m_row = -1;
  m_col = -1;
#endif

} // endvoid DateDelegate::doCloseEditor( Wt::WDateEdit * _dateEdit, bool save ) const

void DateDelegate::doTabAction( Wt::WKeyEvent _keyEvent ) const
{
  std::cout << __FILE__ << ":" << __LINE__ << " " << std::endl;

}



Wt::cpp17::any DateDelegate::editState( Wt::WWidget * _editor, const Wt::WModelIndex & _index ) const
{
  auto cw = dynamic_cast< Wt::WContainerWidget* >( _editor );

  auto de = dynamic_cast< Wt::WDateEdit* >( cw-> layout()-> widget() );

  std::cout << __FILE__ << ":" << __LINE__
    << " " << _index.row()
    << " " << _index.column()
    << " " << de
    << " " << m_dateEdit-> text()
    << std::endl
    ;

//  return "";
  return m_dateEdit-> text();

} // endWt::cpp17::any DateDelegate::editState( Wt::WWidget * _editor, const Wt::WModelIndex & _index ) const

void DateDelegate::setEditState( Wt::WWidget * _editor, const Wt::WModelIndex & _index, const Wt::cpp17::any & _value ) const
{
  std::cout << __FILE__ << ":" << __LINE__ << " " << _editor    << std::endl;
  std::cout << __FILE__ << ":" << __LINE__ << " " << m_dateEdit << std::endl;

} // endvoid DateDelegate::setEditState( Wt::WWidget * _editor, const Wt::WModelIndex & _index, const Wt::cpp17::any & _value ) const

void DateDelegate::setModelData( const Wt::cpp17::any & _editState, Wt::WAbstractItemModel * _model, const Wt::WModelIndex & _index ) const
{
  std::cout << __FILE__ << ":" << __LINE__ << " " << Wt::asString( _editState ) << std::endl;

}




/* * * * * * * * * * * * * * * * * * * * * * * * * * */


class SuggestionDelegate
: public Wt::WItemDelegate
{
  public:


    std::unique_ptr< Wt::WWidget > createEditor
    (
     const Wt::WModelIndex & _index,
     Wt::WFlags< Wt::ViewItemRenderFlag > _flags
    ) const;

};

std::unique_ptr< Wt::WWidget > SuggestionDelegate::createEditor
(
  const Wt::WModelIndex & _index,
  Wt::WFlags< Wt::ViewItemRenderFlag > _flags
) const
{
  auto retVal = Wt::WItemDelegate::createEditor( _index, _flags );
  auto cw = dynamic_cast< Wt::WContainerWidget* >( retVal.get() );
  auto lineEdit = dynamic_cast< Wt::WLineEdit* >( cw-> widget(0) );

  // options for email address suggestions
  Wt::WSuggestionPopup::Options popupOptions =
  {
    "<b>",         // highlightBeginTag
    "</b>",        // highlightEndTag
    ',',           // listSeparator      (for multiple addresses)
    " \n",         // whitespace
    "-., \"@\n;",  // wordSeparators     (within an address)
    ""             // appendReplacedText (prepare next email address)
   };

  auto popup = retVal-> addChild( std::make_unique< Wt::WSuggestionPopup >( popupOptions ) );
  popup-> forEdit( lineEdit );

  auto model = dynamic_cast< const GCW::RegisterWidget::Model* >(_index.model() );

  for( auto item : model-> suggestionsFromColumn( _index.column() ) )
  {
    popup-> addSuggestion( item, item );
  }

  return retVal;

} // endstd::unique_ptr< Wt::WWidget > SuggestionDelegate::createEditor




class AccountDelegate
: public Wt::WItemDelegate
{
  public:


    std::unique_ptr< Wt::WWidget > createEditor
    (
     const Wt::WModelIndex & _index,
     Wt::WFlags< Wt::ViewItemRenderFlag > _flags
    ) const;

};

std::unique_ptr< Wt::WWidget > AccountDelegate::createEditor
(
  const Wt::WModelIndex & _index,
  Wt::WFlags< Wt::ViewItemRenderFlag > _flags
) const
{
  auto retVal = Wt::WItemDelegate::createEditor( _index, _flags );
  auto cw = dynamic_cast< Wt::WContainerWidget* >( retVal.get() );
  auto lineEdit = dynamic_cast< Wt::WLineEdit* >( cw-> widget(0) );

  // options for email address suggestions
  Wt::WSuggestionPopup::Options popupOptions =
  {
    "<b>",         // highlightBeginTag
    "</b>",        // highlightEndTag
    ',',           // listSeparator      (for multiple addresses)
    " \n",         // whitespace
    "-., \"@\n;",  // wordSeparators     (within an address)
    ""             // appendReplacedText (prepare next email address)
   };

  auto popup = retVal-> addChild( std::make_unique< Wt::WSuggestionPopup >( popupOptions ) );
  popup-> forEdit( lineEdit, Wt::PopupTrigger::Editing | Wt::PopupTrigger::DropDownIcon );
  popup-> setAttributeValue( "style", "height:400px;overflow:scroll" );
//  popup-> setJavaScriptMember( "wtNoReparent", "true" );

  auto model = dynamic_cast< const GCW::RegisterWidget::Model* >( _index.model() );

  std::set< std::string > items;
  for( auto accountItem : GCW::Dbo::Accounts::activeAccounts() )
    items.insert( GCW::Dbo::Accounts::fullName( accountItem-> guid() ) );

  for( auto item : items )
  {
    popup-> addSuggestion( item );
  }

  return retVal;

} // endstd::unique_ptr< Wt::WWidget > AccountDelegate::createEditor




} // endnamespace {




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
  tableView()-> addStyleClass( "Gcw-TableView" );

  /*
  ** Configure the table view
  **
  */
  tableView()-> setSortingEnabled       ( false                                                         );
  tableView()-> setAlternatingRowColors ( true                                                          );
  tableView()-> setSelectionBehavior    ( Wt::SelectionBehavior::Rows                                   );
  tableView()-> setSelectionMode        ( Wt::SelectionMode::Single                                     );
  tableView()-> setEditTriggers         ( Wt::EditTrigger::SingleClicked                                );
  tableView()-> setEditOptions          ( Wt::EditOption::SingleEditor | Wt::EditOption::SaveWhenClosed );
  tableView()-> setHeaderItemDelegate   ( std::make_shared< HeaderDelegate >()                          );

  {
    auto delegate = std::make_shared< DateDelegate >();
    tableView()-> setItemDelegateForColumn( 0, delegate  );

    delegate->
      closeEditor().connect( [&]( Wt::WWidget* _widget, bool _save )
      {
        std::cout << __FILE__ << ":" << __LINE__ << " " << std::endl;

      });


#ifdef NEVER
    delegate->
      editorCreated().connect( [&]( int _row, int _column)
      {
        std::cout << __FILE__ << ":" << __LINE__
          << " row:" << _row
          << " col:" << _column
          << std::endl;

        for( int column = 0; column< tableView()-> model()-> columnCount(); column++ )
        {
          std::cout << __FILE__ << ":" << __LINE__
            << " row:" << _row
            << " col:" << column
            << std::endl;

          tableView()-> itemWidget( tableView()-> model()-> index( _row, column ) )-> addStyleClass( "active" );
        }

      });

    delegate->
      editorClosed().connect( [&]( int _row, int _column )
      {
        std::cout << __FILE__ << ":" << __LINE__
          << " row:" << _row
          << " col:" << _column
          << std::endl;

        for( int column = 0; column< tableView()-> model()-> columnCount(); column++ )
        {
          std::cout << __FILE__ << ":" << __LINE__
            << " row:" << _row
            << " col:" << column
            << std::endl;

          tableView()-> itemWidget( tableView()-> model()-> index( _row, column ) )-> removeStyleClass( "active" );
        }

      });
#endif
  }

  tableView()-> setItemDelegateForColumn ( 1, std::make_shared< SuggestionDelegate   >()                 );
  tableView()-> setItemDelegateForColumn ( 2, std::make_shared< SuggestionDelegate   >()                 );
  tableView()-> setItemDelegateForColumn ( 3, std::make_shared< AccountDelegate      >()                 );

  tableView()-> headerClicked().connect( [=]( int col, Wt::WMouseEvent event )
  {
    std::cout << __FILE__ << ":" << __LINE__ << " " << col << std::endl;
  });

  /*
  ** This 'selectionChanged' procedure is 'clunky'.
  **
  ** This procedure is designed to respond to a row-selection
  **  change event.  When a different row is selected, we want
  **  any open editors to be closed, and the row selection to
  **  move to the newly selected row.
  **
  ** Right now the problem is with the 'select' command, where
  **  it calling 'select' cause this 'selectionChanged' event
  **  to fire again.  So, ther is a littl 'selecting' interlock
  **  built around it to try to prevent this weirdness.
  **
  ** The other problem with this routine is when 'selecting'
  **  a cell that is editable, the editor is immediately engaged
  **  but this 'selectionChanged' signal never fires... so we have
  **  to sort that out.
  **
  */
  tableView()-> selectionChanged().connect( [=]()
  {
    std::cout << __FILE__ << ":" << __LINE__ << " selecting:" << std::endl;
    tableView()-> closeEditors( true );
  });

#ifdef NEVER
  tableView()->
    clicked().connect( [=]( Wt::WModelIndex _index, Wt::WMouseEvent _event )
    {
      std::cout << __FILE__ << ":" << __LINE__ << "clicked"
        << " row:" << _index.row()
        << " col:" << _index.column()
        << std::endl
        ;

      if( m_clickedRow != -1
       && m_clickedCol != -1
        )
      {
        std::cout << __FILE__ << ":" << __LINE__ << " unselect:" << m_clickedRow << std::endl;

        for( int column = 0; column< tableView()-> model()-> columnCount(); column++ )
        {
          tableView()->
            itemWidget( tableView()-> model()-> index( m_clickedRow, column ) )->
              removeStyleClass( "active" );
        }

        if( m_clickedRow != _index.row()
          )
          tableView()-> closeEditors( true );
      }

      m_clickedRow = _index.row();
      m_clickedCol = _index.column();

      for( int column = 0; column< tableView()-> model()-> columnCount(); column++ )
      {
        std::cout << __FILE__ << ":" << __LINE__ << " select:" << m_clickedRow << std::endl;

        tableView()-> itemWidget( tableView()-> model()-> index( m_clickedRow, column ) )->
          addStyleClass( "active" );
      }

      std::cout << __FILE__ << ":" << __LINE__ << " " << Wt::WApplication::instance()->theme()->activeClass() << std::endl;

//      tableView()-> clearSelection();

//      tableView()-> closeEditors();
    });

  tableView()-> keyPressed().connect( [=]( Wt::WKeyEvent _event )
  {
    std::cout << __FILE__ << ":" << __LINE__ << " " << _event.charCode() << std::endl;

  });
#endif

  loadData();

} // endGCW::RegisterWidget::RegisterWidget( const std::string & _accountGuid )

void GCW::RegisterWidget::
test()
{
  std::cout << __FILE__ << ":" << __LINE__ << " ::test::" << std::endl;

  std::cout << __FILE__ << ":" << __LINE__ << " " << tableView()-> selectedIndexes().size() << std::endl;

  auto selectedIndex = *tableView()-> selectedIndexes().begin();

  if( selectedIndex.isValid() )
  {

    std::cout << __FILE__ << ":" << __LINE__ << " " << tableView()-> selectedIndexes().size() << std::endl;

  }


} // endvoid GCW::RegisterWidget::test()

Wt::Json::Object GCW::RegisterWidget::
toJson() const
{
  Wt::Json::Object jobj;

  return jobj;
}

bool GCW::RegisterWidget::
fromJson( const Wt::Json::Object & _jobj )
{
  return true;
}


void GCW::RegisterWidget::
loadData()
{
  m_model = std::make_shared< Model >( m_accountGuid );

  tableView()-> setModel( m_model );

  /* Date */
  tableView()-> setColumnWidth     ( 0, "150px"                   );
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

} // endvoid GCW::RegisterWidget::loadData()

GCW::RegisterWidget::Model::
Model( const std::string & _accountGuid )
: Wt::WStandardItemModel( 0, 8 ),
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
        account = _addColumn( columns, GCW::Dbo::Accounts::fullName( splitAccountItem-> guid() ) );
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

} // endvoid GCW::RegisterWidget::Model::refreshFromDisk()

GCW::RegisterWidget::Model::RowItem GCW::RegisterWidget::Model::
makeRow( const std::string & _splitGuid )
{
  RowItem rowItem;

  return rowItem;
}

std::set< std::string > GCW::RegisterWidget::Model::
suggestionsFromColumn( int _column ) const
{
  std::set< std::string > retVal;

  for( int row=0; row< rowCount(); row++ )
    retVal.insert( Wt::asString( item( row, _column )-> text() ).toUTF8() );

  return retVal;
}


GCW::RegisterWidget::AccountSuggestionModel::
AccountSuggestionModel( const std::string & _accountGuid )
: Wt::WStandardItemModel( 0, 1 ),
  m_accountGuid( _accountGuid )
{

} // endGCW::RegisterWidget::Model::Model( const std::string & _accountGuid )


