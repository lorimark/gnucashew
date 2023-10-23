#line 2 "src/Gui/BillPay/ToolBar.cpp"

#include <Wt/WHBoxLayout.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WPushButton.h>

#include "BillPay.h"

GCW::Gui::BillPay::ToolBar::
ToolBar()
{
  auto lw = setLayout( std::make_unique< Wt::WHBoxLayout >() );

  auto pbAdd  = lw-> addWidget( std::make_unique< Wt::WPushButton >( "add"  ) );
  auto pbEdit = lw-> addWidget( std::make_unique< Wt::WPushButton >( "edit" ) );

  auto cw    = std::make_unique< Wt::WContainerWidget >();
  m_group = std::make_shared< Wt::WButtonGroup >();
  m_group-> addButton( cw-> addNew< Wt::WRadioButton >( "01" ),  1 );
  m_group-> addButton( cw-> addNew< Wt::WRadioButton >( "02" ),  2 );
  m_group-> addButton( cw-> addNew< Wt::WRadioButton >( "03" ),  3 );
  m_group-> addButton( cw-> addNew< Wt::WRadioButton >( "04" ),  4 );
  m_group-> addButton( cw-> addNew< Wt::WRadioButton >( "05" ),  5 );
  m_group-> addButton( cw-> addNew< Wt::WRadioButton >( "06" ),  6 );
  m_group-> addButton( cw-> addNew< Wt::WRadioButton >( "07" ),  7 );
  m_group-> addButton( cw-> addNew< Wt::WRadioButton >( "08" ),  8 );
  m_group-> addButton( cw-> addNew< Wt::WRadioButton >( "09" ),  9 );
  m_group-> addButton( cw-> addNew< Wt::WRadioButton >( "10" ), 10 );
  m_group-> addButton( cw-> addNew< Wt::WRadioButton >( "11" ), 11 );
  m_group-> addButton( cw-> addNew< Wt::WRadioButton >( "12" ), 12 );
  lw-> addWidget( std::move( cw ) );

  pbAdd-> clicked().connect( [=](){ m_addClicked.emit(); } );

  /*
  ** Make sure a month is selected.
  **
  */
  auto selectedMonth = configItem()-> getVarInt( "selectedMonth" );
  if( selectedMonth < 1 )
      selectedMonth = 1;

  m_group-> setCheckedButton( m_group-> button( selectedMonth ) );
  m_group->
    checkedChanged().connect( [&]( Wt::WRadioButton * _button )
    {
      configItem().modify()-> setVar( "selectedMonth", m_group-> checkedId() );
    });

  m_disabled = lw-> addWidget( std::make_unique< Wt::WCheckBox >( "disabled" ) );
  m_disabled-> setValueText( configItem()-> getVarString( "showDisabled" ) );
  m_disabled->
    clicked().connect( [&]( Wt::WMouseEvent _event )
    {
      auto item = configItem();
      Wt::Dbo::Transaction t( GCW::app()-> gnucash_session() );
      item.modify()-> setVar( "showDisabled", m_disabled-> valueText() );
      item.flush();

    });

  m_summary = lw-> addWidget( std::make_unique< Wt::WCheckBox >( "summary" ), 1 );
  m_summary-> setValueText( configItem()-> getVarString( "showSummary" ) );
  m_summary->
    clicked().connect( [&]( Wt::WMouseEvent _event )
    {
      auto item = configItem();
      Wt::Dbo::Transaction t( GCW::app()-> gnucash_session() );
      item.modify()-> setVar( "showSummary", m_summary-> valueText() );
      item.flush();

    });

} // endToolBar()



int
GCW::Gui::BillPay::ToolBar::
selectedMonth() const
{
  return m_group-> checkedId();
}

bool
GCW::Gui::BillPay::ToolBar::
showDisabled() const
{
  return disabledButton()-> checkState() == Wt::CheckState::Checked? true:false;
}

