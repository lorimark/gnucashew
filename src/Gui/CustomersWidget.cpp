#line 2 "src/CustomersWidget.cpp"

#include <Wt/WText.h>
#include <Wt/WVBoxLayout.h>

#include "../define.h"
#include "../GnuCashew.h"
#include "CustomersWidget.h"

GCW::Gui::CustomersWidget::
CustomersWidget()
{
  addStyleClass( "CustomersWidget" );

  auto lw = setLayout( std::make_unique< Wt::WVBoxLayout >() );

  auto w = std::make_unique< Wt::WTableView >();
  m_view = w.get();
  lw-> addWidget( std::move( w ), 1 );

  view()-> setSelectionBehavior( Wt::SelectionBehavior::Rows );
  view()-> setSelectionMode(     Wt::SelectionMode::Single   );
  view()-> setAlternatingRowColors( true );
  view()-> doubleClicked().connect( this, &GCW::Gui::CustomersWidget::doubleClicked );

  std::vector< Wt::WFormModel::Field > fields =
  {
    GCW::Dbo::Customer::Field::id,
    GCW::Dbo::Customer::Field::name,
    GCW::Dbo::Customer::Field::addr_name,
    GCW::Dbo::Customer::Field::addr_addr1,
    GCW::Dbo::Customer::Field::addr_phone,
    GCW::Dbo::Customer::Field::addr_email
  };

  m_model = std::make_shared< GCW::Eng::CustomersModel >( fields );
  view()-> setModel( model() );

} // endGCW::Gui::CustomersWidget::CustomersWidget()

void GCW::Gui::CustomersWidget::
doubleClicked( const Wt::WModelIndex & _index, const Wt::WMouseEvent & _event )
{

}



