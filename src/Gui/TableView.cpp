#line 2 "src/Gui/TableView.cpp"

#include <Wt/WStandardItem.h>
#include <Wt/WStandardItemModel.h>

#include "TableView.h"


// Pixels
const int GCW::Gui::TableView::kTableHeaderRowHeight = 30;
const int GCW::Gui::TableView::kTableBodyRowHeight   = 25;
const int GCW::Gui::TableView::kTableCellPadding     = 7;
const int GCW::Gui::TableView::kScrollBarWidth       = 20;
const int GCW::Gui::TableView::kDefaultDateTimeWidth = 120;

GCW::Gui::TableView::
TableView()
: Wt::WTableView()
{
  setHeaderHeight( kTableHeaderRowHeight );
  setRowHeight( kTableBodyRowHeight );

  setLayoutSizeAware( true );

} // endGCW::Gui::TableView::TableView( Wt::WContainerWidget* parent )

auto
GCW::Gui::TableView::
layoutSizeChanged( int width, int height )-> void
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

} // endauto GCW::Gui::TableView::layoutSizeChanged( int width, int height ) -> void

auto GCW::Gui::TableView::
setColumnWidth( int column, const Wt::WLength& width )-> void
{
    // Just save the data and pass the the work up
    widths_.emplace( column, width );

    Wt::WTableView::setColumnWidth( column, width );

} // endauto GCW::Gui::TableView::setColumnWidth(int column, const Wt::WLength& width) -> void

/*!
** \brief Handle Click Event
**
** This handler responds to 'click' events in the view.
**
** The intent here is to provide some 'handling' when the user
**  is mouse-ing around the table view.  He should be able to
**  click around to get editors to open, and select rows and
**  whatnot.
**
** The 'purpose' for the handler is due to the fact that the
**  view responds differently if the view is editable or not.
**  For instance, if an index is read-only, clicking on it
**  causes the view to 'select' the entire row.  But, if the
**  index is 'Editable' then you are placed in to the editor
**  but the view 'selection' is still visible, even if it's on
**  another row.  Further, the editor widget that opened, did
**  not signal to the rest of the row that it should be 'selected'.
**
*/
#ifdef NEVER
void
GCW::Gui::TableView::
handleClick( const Wt::WModelIndex & _index, const Wt::WMouseEvent & _event )
{
#ifndef NEVER
  std::cout << __FILE__ << ":" << __LINE__
    << " handleClick:<start>"
    << " row:" << _index.row()
    << " col:" << _index.column()
    << std::endl
    ;
#endif

#ifdef NEVER
  if( selectedIndexes().size() > 0 )
  {
    auto selectedIndex = *selectedIndexes().begin();

#ifdef NEVER
    std::cout << __FILE__ << ":" << __LINE__
      << " selected:" << selectedIndexes().size()
      << " row:" << selectedIndex.row()
      << " col:" << selectedIndex.column()
       << std::endl
      ;
#endif

    /*
    ** If the selection is different in any way, then clear
    **  the entire selection.  If the selection is the same
    **  as the 'index', then we are handling the same click
    **  on the same index that is already selected - don't
    **  do anything.  But if the _index is different than
    **  the current selection, then we need to clear the
    **  current selection.
    **
    */
    if( selectedIndex.row    () != _index.row    ()
     && selectedIndex.column () != _index.column ()
      )
    {
      clearSelection();
    }

    closeEditors();

  }
#endif

#ifdef NEVER
  {
    auto m = dynamic_cast< Wt::WStandardItemModel* >( model().get() );

    auto row = _index.row();
    for( int column = 0; column< m-> columnCount(); column++ )
    {
      std::cout << __FILE__ << ":" << __LINE__
        << " row:" << row
        << " col:" << column
        << std::endl;

      auto index = m-> index( row, column );
      auto item = m-> itemFromIndex( index );

      item-> setFlags( Wt::ItemFlag::Editable );
//      edit( index );

    } // endfor( int column = 0; column< m-> columnCount(); column++ )
  }
#endif

#ifdef NEVER
  /*
  ** The 'selector' is funky.  If an 'editor' is enabled, then
  **  selecting the cell causes the editor to open, but the previous
  **  selection does not get un-selected, so it looks like a whole row
  **  is selected, and then a cell is open in another row.
  **
  */
  if( selectedIndexes().size() )
  {
    auto oldIndex = *selectedIndexes().begin();

    std::cout << __FILE__ << ":" << __LINE__ << " handleCLick()"
      << " old:" << oldIndex.row()
      << " new:" << _index.row()
      << std::endl;

    if( _index.row() != oldIndex.row() )
    {
      clearSelection();
      closeEditors( true );
    }

  }
#endif

  /*
  ** Call the stock click handler
  **
  */
  Wt::WTableView::handleClick( _index, _event );

#ifndef NEVER
  std::cout << __FILE__ << ":" << __LINE__ << " handleClick:<end>" << std::endl;
#endif

} // endvoid GCW::Gui::TableView::handleClick( const Wt::WModelIndex & _index, const Wt::WMouseEvent & _event )
#endif

#ifdef NEVER
void
GCW::Gui::TableView::
handleDoubleClick( const Wt::WModelIndex & _index, const Wt::WMouseEvent & _event )
{
#ifndef NEVER
  std::cout << __FILE__ << ":" << __LINE__
    << " handleDoubleClick:<start>"
    << " row:" << _index.row()
    << " col:" << _index.column()
    << std::endl
    ;
#endif

  Wt::WTableView::handleDoubleClick( _index, _event );

#ifndef NEVER
  std::cout << __FILE__ << ":" << __LINE__ << " handleDoubleClick:<end>" << std::endl;
#endif

} // endhandleDoubleClick ( const Wt::WModelIndex & _index, const Wt::WMouseEvent & _event )
#endif

#ifndef NEVER
void
GCW::Gui::TableView::
handleMouseDown( const Wt::WModelIndex & _index, const Wt::WMouseEvent & _event )
{
#ifndef NEVER
  std::cout << __FILE__ << ":" << __LINE__
    << " handleMouseDown:<start>"
    << " row:" << _index.row()
    << " col:" << _index.column()
    << std::endl
    ;
#endif

//  clearSelection();

//  if( !isEditing( _index ) )
//    closeEditors( false );

  if( m_lastIndexClick.isValid() )
    if( m_lastIndexClick. row    () != _index. row    ()
     || m_lastIndexClick. column () != _index. column ()
      )
    {
      clearSelection();
      closeEditors();
    }

  Wt::WTableView::handleMouseDown( _index, _event );

#ifndef NEVER
  std::cout << __FILE__ << ":" << __LINE__ << " handleMouseDown:<end>" << std::endl;
#endif

  /*
  ** remember the last index clicked
  **
  */
  m_lastIndexClick = _index;

} // endhandleMouseDown   ( const Wt::WModelIndex & _index, const Wt::WMouseEvent & _event )
#endif

#ifdef NEVER
void
GCW::Gui::TableView::
handleMouseUp( const Wt::WModelIndex & _index, const Wt::WMouseEvent & _event )
{
#ifndef NEVER
  std::cout << __FILE__ << ":" << __LINE__
    << " handleMouseUp:<start>"
    << " row:" << _index.row()
    << " col:" << _index.column()
    << std::endl
    ;
#endif

  Wt::WTableView::handleMouseUp( _index, _event );

#ifndef NEVER
  std::cout << __FILE__ << ":" << __LINE__ << " handleMouseUp:<end>" << std::endl;
#endif

} // endhandleMouseUp( const Wt::WModelIndex & _index, const Wt::WMouseEvent & _event )
#endif



