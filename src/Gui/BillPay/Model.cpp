#line 2 "src/Gui/BillPay/Model.cpp"

#include <Wt/WStandardItem.h>

#include "../../App.h"
#include "../Dbo/Vars.h"
#include "BillPay.h"

namespace {

/*!
** \brief Accounts Sorter
**
**
*/
void sort( std::vector< GCW::Dbo::Vars::Item::Ptr > & _varItems )
{
  /*!
  ** Sort the vector of accounts by accountName
  **
  */
  std::sort
  (
   _varItems.begin(),
   _varItems.end(),
   []( const GCW::Dbo::Vars::Item::Ptr item1,
       const GCW::Dbo::Vars::Item::Ptr item2
     )
     {
//       auto account1 = GCW::Dbo::Accounts::byGuid( item1-> keyField() );
//       auto account2 = GCW::Dbo::Accounts::byGuid( item2-> keyField() );
//       auto name1 = account1-> name();
//       auto name2 = account2-> name();

       auto name1 = item1-> getVarString( "nickname" );
       auto name2 = item2-> getVarString( "nickname" );

       /*
       ** return .bool. of the nickname comparison
       **
       */
       return name1
            < name2;
     }
  );

} // endvoid sort( GCW::Dbo::Splits::Item::Vector & _splitItems )

GCW::Gui::BillPay::ColumnDef_t columns[] =
{
  { "accountKey" , "120px", Wt::AlignmentFlag::Left,   "Primary Account Identifier"        },
  { "last4"      ,  "60px", Wt::AlignmentFlag::Center, "Last 4 digits of account number"   },
  { "Nickname"   , "100px", Wt::AlignmentFlag::Left,   "Friendly Nickname for the account" },
  { "Gp"         ,  "50px", Wt::AlignmentFlag::Center, "Grouping"                          },
  { "Dy"         ,  "50px", Wt::AlignmentFlag::Center, "Day that the payment is due"       },
  { "Min"        ,  "50px", Wt::AlignmentFlag::Right,  "Minimum Payment Due"               },
  { "Bdgt"       ,  "50px", Wt::AlignmentFlag::Right,  "Payment Budget"                    },
  { "Actual"     ,  "50px", Wt::AlignmentFlag::Right,  "Actual payment most recently made" },
  { "Au"         ,  "50px", Wt::AlignmentFlag::Center, "Automatic Payment Indicator"       },
  { "01"         ,  "35px", Wt::AlignmentFlag::Center, "January"                           },
  { "02"         ,  "35px", Wt::AlignmentFlag::Center, "February"                          },
  { "03"         ,  "35px", Wt::AlignmentFlag::Center, "March"                             },
  { "04"         ,  "35px", Wt::AlignmentFlag::Center, "April"                             },
  { "05"         ,  "35px", Wt::AlignmentFlag::Center, "May"                               },
  { "06"         ,  "35px", Wt::AlignmentFlag::Center, "June"                              },
  { "07"         ,  "35px", Wt::AlignmentFlag::Center, "July"                              },
  { "08"         ,  "35px", Wt::AlignmentFlag::Center, "August"                            },
  { "09"         ,  "35px", Wt::AlignmentFlag::Center, "September"                         },
  { "10"         ,  "35px", Wt::AlignmentFlag::Center, "October"                           },
  { "11"         ,  "35px", Wt::AlignmentFlag::Center, "November"                          },
  { "12"         ,  "35px", Wt::AlignmentFlag::Center, "December"                          },
};

#define COLUMN_COUNT sizeof(columns)/sizeof(GCW::Gui::BillPay::ColumnDef_t)

} // endnamespace {

GCW::Gui::BillPay::Model::
Model( int _selectedMonth, const Status _status )
: Wt::WStandardItemModel( 0, COLUMN_COUNT ),
  m_status( _status )
{
  /*
  ** Load the header only on the 'unpaid' view
  **
  */
  if( m_status == GCW::Gui::BillPay::Status::Unpaid )
    for( int col = 0; col< COLUMN_COUNT; col++ )
    {
      setHeaderData( col, Wt::Orientation::Horizontal, columns[col].name    , Wt::ItemDataRole::Display );
      setHeaderData( col, Wt::Orientation::Horizontal, columns[col].toolTip , Wt::ItemDataRole::ToolTip );
    }

  loadData( _selectedMonth );

} // endModel( const Status _status )

void
GCW::Gui::BillPay::Model::
loadData( int _selectedMonth )
{
  while( rowCount() > 0 )
    takeRow( 0 );

  /*
  ** Get all the var items that are for the managed bill pay (mbpi)
  **
  */
  Wt::Dbo::Transaction t( GCW::app()-> gnucash_session() );
  auto items =
    GCW::app()-> gnucash_session().find< GCW::Dbo::Vars::Item >()
      .where( "\"cfyField\" = '" GCW_GUI_BILLPAY_ITEM_CFY "'" )
      .resultList()
      ;

  /*
  ** Calculate our yes/no status for grabbing items
  **
  */
  std::string yesNo = "yes";
  if( m_status == GCW::Gui::BillPay::Status::Unpaid )
    yesNo = "no";

  /*
  ** move the correct items (based on yes/no, disabled/not) in to a vector and sort them by name
  **
  */
  std::vector< GCW::Dbo::Vars::Item::Ptr > varItems;
  for( auto i : items )
  {
    auto isActive  = i-> getVarString( "isActive"  ) == "yes";
    auto isVisible = i-> getVarString( "isVisible" ) == "yes";

    if( m_status != GCW::Gui::BillPay::Status::Disabled )
    {
      if( isActive && isVisible )
        if( i-> getVarString( "cb" + GCW::Gui::BillPay::toString( _selectedMonth ) ) == yesNo )
          varItems.push_back( i );
    }

    else // capture disabled items here
    {
      if(  !isActive
       ||  !isVisible
        )
          varItems.push_back( i );
    }
  }
  ::sort( varItems );

  /*
  ** Walk through all the items and post them to the table
  **
  */
  for( auto varItem : varItems )
  {
    auto accountGuid = varItem-> keyField();
    auto accountItem = GCW::Dbo::Accounts::byGuid( accountGuid );

    std::vector< std::unique_ptr< Wt::WStandardItem > > columns;

    auto name = std::make_unique< Wt::WStandardItem >( accountItem-> name() );
         name-> setToolTip( GCW::Dbo::Accounts::fullName( accountGuid ) );
         name-> setData( accountGuid );
    columns.push_back( std::move( name ) );

    auto last4 = std::make_unique< Wt::WStandardItem >( varItem-> getVarString( "last4" ) );
    columns.push_back( std::move( last4 ) );

    auto nickname = std::make_unique< Wt::WStandardItem >( varItem-> getVarString( "nickname" ) );
    columns.push_back( std::move( nickname ) );

    auto group = std::make_unique< Wt::WStandardItem >( varItem-> getVarString( "group" ) );
    columns.push_back( std::move( group ) );

    auto dueDay = std::make_unique< Wt::WStandardItem >( varItem-> getVarString( "dueDay" ) );
    columns.push_back( std::move( dueDay ) );

    auto minimum = std::make_unique< Wt::WStandardItem >( varItem-> getVarString( "minimum" ) );
    columns.push_back( std::move( minimum ) );

    auto budget = std::make_unique< Wt::WStandardItem >( varItem-> getVarString( "budget" ) );
    columns.push_back( std::move( budget ) );

    auto actual = std::make_unique< Wt::WStandardItem >( varItem-> getVarString( "actual" ) );
    columns.push_back( std::move( actual ) );

    auto autoPay = std::make_unique< Wt::WStandardItem >( varItem-> getVarString( "autoPay" ) );
    columns.push_back( std::move( autoPay ) );

    for( int i=1; i<= 12; i++ )
    {
      auto cb = std::make_unique< Wt::WStandardItem >( varItem-> getVarString( "cb" + toString( i ) ) );
      columns.push_back( std::move( cb ) );
    }

    appendRow( std::move( columns ) );

  } // endfor( auto varItem : varItems )

} // endloadData()

GCW::Gui::BillPay::ColumnDef_t
GCW::Gui::BillPay::Model::
columnDef( int col )
{
  return columns[col];
}



