#line 2 "src/CustomerOverviewWidget.cpp"

#include <Wt/WText.h>
#include <Wt/WVBoxLayout.h>

#include "../define.h"
#include "../GnuCashew.h"
#include "CustomerOverviewWidget.h"

GCW::Gui::CustomerOverviewWidget::
CustomerOverviewWidget()
{
  addStyleClass( "CustomerOverviewWidget" );

  auto lw = setLayout( std::make_unique< Wt::WVBoxLayout >() );

  lw-> addWidget( std::make_unique< Wt::WText >( "these are not the droids you are looking for..." ) );

} // endGCW::Gui::CustomerOverviewWidget::CustomerOverviewWidget()


