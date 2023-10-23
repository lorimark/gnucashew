#line 2 "src/Gui/BillPay/MainWidget.cpp"

#include <Wt/WVBoxLayout.h>
#include <Wt/WPushButton.h>

#include "BillPay.h"

GCW::Gui::BillPay::MainWidget::
MainWidget()
{
  buildContent();

} // endMainWidget()

void
GCW::Gui::BillPay::MainWidget::
buildContent()
{
  clear();

  addStyleClass( "MainWidget" );

  auto lw = setLayout( std::make_unique< Wt::WVBoxLayout >() );

  // toolbar
  {
    auto u_ = std::make_unique< ToolBar >();
    m_toolBar = u_.get();
    lw-> addWidget( std::move( u_ ) );

    m_toolBar-> addClicked().connect( this, &GCW::Gui::BillPay::MainWidget::addClicked );
    m_toolBar-> buttonGroup()-> checkedChanged().connect( this, &MainWidget::buttonChanged );
    m_toolBar-> disabledButton()-> clicked().connect( this, &MainWidget::disabledClicked );

  } // toolbar

  Wt::WVBoxLayout      * lw2;
  Wt::WContainerWidget * cw;
  {
    auto u_ = std::make_unique< Wt::WContainerWidget >();
    cw = u_.get();
    lw-> addWidget( std::move( u_ ), 1 );

    lw2 = cw-> setLayout( std::make_unique< Wt::WVBoxLayout >() );
  }

  // unpaid items
  {
    auto u_ = std::make_unique< Table >( m_toolBar-> selectedMonth(), Status::Unpaid );
    m_unpaidView = u_.get();
    lw2-> addWidget( std::move( u_ ) );
    m_unpaidView->
      doubleClicked().connect( [&]( Wt::WModelIndex _index, Wt::WMouseEvent _event )
      {
        editClicked( m_unpaidView, _index );
      });
  }

  // paid items
  {
    auto u_ = std::make_unique< Table >( m_toolBar-> selectedMonth(), Status::Paid );
    m_paidView = u_.get();
    lw2-> addWidget( std::move( u_ ), m_toolBar-> showDisabled()? 0:1 );
    m_paidView->
      doubleClicked().connect( [&]( Wt::WModelIndex _index, Wt::WMouseEvent _event )
      {
        editClicked( m_paidView, _index );
      });
  }

  // disabled items
  if( m_toolBar-> showDisabled() )
  {
    auto u_ = std::make_unique< Table >( m_toolBar-> selectedMonth(), Status::Disabled );
    m_disabledView = u_.get();
    lw2-> addWidget( std::move( u_ ), 1 );
    m_disabledView->
      doubleClicked().connect( [&]( Wt::WModelIndex _index, Wt::WMouseEvent _event )
      {
        editClicked( m_disabledView, _index );
      });
  }

}

void
GCW::Gui::BillPay::MainWidget::
openEditor( const std::string & _accountGuid )
{
  m_dialog = std::make_unique< GCW::Gui::BillPay::EditWidgetDialog >( _accountGuid );
  m_dialog-> show();
  m_dialog->
    finished().connect( [&]()
    {
      refreshViews();
      m_dialog.release();
    });

} // endopenEditor( const std::string & _accountGuid )


void
GCW::Gui::BillPay::MainWidget::
addClicked()
{
  Wt::Dbo::Transaction t( GCW::app()-> gnucash_session() );
  openEditor( "" );

} // endaddClicked()

void
GCW::Gui::BillPay::MainWidget::
editClicked( Table * _table, Wt::WModelIndex _index )
{
  Wt::Dbo::Transaction t( GCW::app()-> gnucash_session() );

  /*
  ** Get the 0-col index and use that to get the guid of this
  **  row, and use that to get the Edit dialog open on that
  **  guid.
  **
  */
  auto zcolIndex = _index.model()-> index( _index.row(), 0 );
  auto accountGuid = Wt::asString( _table-> model()-> itemFromIndex( zcolIndex )-> data() ).toUTF8();
  openEditor( accountGuid );

} // endeditClicked( Wt::WModelIndex _index, Wt::WMouseEvent _event )

void
GCW::Gui::BillPay::MainWidget::
buttonChanged( Wt::WRadioButton * _button )
{
  /*
  ** If there is a button (sometimes there is not), then there's
  **  nothing to do.
  **
  ** This function can get called from a selection of one of the
  **  monthly buttons in the tool bar, or it can get called from a
  **  detailForm .save. event.  It shouldn't happen, but it's possible
  **  to add an item without having a month selected.  The button
  **  is going to get defaulted to 1, to help mitigate the issue, but
  **  it's still a problem if we pass a nullptr for the button.  So,
  **  to be safe, just deal with it.
  **
  */
  if( !_button )
    return;

  if( m_paidView     ) m_paidView     -> setMonth( std::stoi( _button-> text().toUTF8() ) );
  if( m_unpaidView   ) m_unpaidView   -> setMonth( std::stoi( _button-> text().toUTF8() ) );
  if( m_disabledView ) m_disabledView -> setMonth( std::stoi( _button-> text().toUTF8() ) );

} // endbuttonChanged( Wt::WRadioButton * _button )

void
GCW::Gui::BillPay::MainWidget::
disabledClicked()
{
  buildContent();
}


void
GCW::Gui::BillPay::MainWidget::
refreshViews()
{
  buttonChanged( m_toolBar-> buttonGroup()-> checkedButton() );
}

