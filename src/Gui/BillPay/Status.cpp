#line 2 "src/Gui/BillPay/Status.cpp"

#include "BillPay.h"

std::string
GCW::Gui::BillPay::
asString( Status _status )
{
  // BUGBUG: it would be better to put this in the gcw_en.xml language
  //          translation files
  //
  std::string values[] =
  {
    "Paid",
    "Unpaid",
    "Disabled"
  };

  return values[ static_cast<int>( _status ) ];
}

