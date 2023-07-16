

#include <Wt/WAbstractItemModel.h>

#include "TableView.h"


// Pixels
const int GCW::TableView::kTableHeaderRowHeight = 30;
const int GCW::TableView::kTableBodyRowHeight   = 25;
const int GCW::TableView::kTableCellPadding     = 7;
const int GCW::TableView::kScrollBarWidth       = 20;
const int GCW::TableView::kDefaultDateTimeWidth = 120;

GCW::TableView::TableView()
: Wt::WTableView()
{
  setHeaderHeight( kTableHeaderRowHeight );
  setRowHeight( kTableBodyRowHeight );

  setLayoutSizeAware( true );

} // endGCW::TableView::TableView( Wt::WContainerWidget* parent )

auto GCW::TableView::layoutSizeChanged( int width, int height ) -> void
{
  // Calculate our fixed width columns
  auto nfixed = 0;
  auto nrel = 0;

  // "fixed" number of pixels
  auto fixed = 0.0;

  for( auto col : widths_ )
    if( col.second.unit() == Wt::LengthUnit::Percentage )
      nrel++;

  auto percent_sum=0.0;
  for( auto col : widths_)
  {
    if( col.second.unit() != Wt::LengthUnit::Percentage )
      fixed += col.second.toPixels();
    else
      percent_sum += col.second.value();
  }

  // Check to see if the relative columns are taking up ~100% of the
  // non-fixed width.  "Roughly" because sometimes they're intentionally off
  // by a fraction just to avoid a scroll bar
  const auto epsilon = 0.5;
  if( 100.0 - percent_sum > epsilon )
    Wt::log("debug")
      << "Warning: Relative column widths do not take up 100% of the available width"
      ;

  // Columns who's width wasn't explicitly set are considered "fixed", and Wt
  // will default them to 150px or something.
  nfixed = model()->columnCount() - nrel;

  // Show scroll bar?
  bool show_scroll = visible_rows_ > 0 && model()->rowCount() > visible_rows_;

  auto remainder = width - ( fixed + ( kTableCellPadding * model()->columnCount() ) + ( show_scroll ? kScrollBarWidth : 0.0 ) );

  for( auto col : widths_)
    if( col.second.unit() == Wt::LengthUnit::Percentage )
      setColumnWidth
      (
       col.first,
       Wt::WLength
       (
        col.second.value()/100.0*remainder,
        Wt::LengthUnit::Pixel
       )
      );

  // Pass the call up the chain
  Wt::WTableView::layoutSizeChanged( width, height );

} // endauto GCW::TableView::layoutSizeChanged( int width, int height ) -> void

auto GCW::TableView::setColumnWidth( int column, const Wt::WLength& width ) -> void
{
    // Just save the data and pass the the work up
    widths_.emplace( column, width );

    Wt::WTableView::setColumnWidth( column, width );

} // endauto GCW::TableView::setColumnWidth(int column, const Wt::WLength& width) -> void

void GCW::TableView::handleClick( const Wt::WModelIndex & _index, const Wt::WMouseEvent & _event )
{
#ifdef NEVER
  std::cout << __FILE__ << ":" << __LINE__
    << " handleClick:<start>"
    << " row:" << _index.row()
    << " col:" << _index.column()
    << std::endl
    ;

  if( selectedIndexes().size() > 0 )
  {
    auto selectedIndex = *selectedIndexes().begin();

    std::cout << __FILE__ << ":" << __LINE__
      << " selected:" << selectedIndexes().size()
      << " row:" << selectedIndex.row()
      << " col:" << selectedIndex.column()
       << std::endl
      ;
  }
#endif

  Wt::WTableView::handleClick( _index, _event );

//  std::cout << __FILE__ << ":" << __LINE__ << " handleClick:<end>" << std::endl;

} // endvoid GCW::TableView::handleClick( const Wt::WModelIndex & _index, const Wt::WMouseEvent & _event )




