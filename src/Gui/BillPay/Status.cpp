#line 2 "src/Gui/BillPay/Status.cpp"

#include "BillPay.h"

std::string
GCW::Gui::BillPay::
asString( Status _status )
{
  std::string values[] =
  {
    TR8( "gcw.tableName.Paid"     ),
    TR8( "gcw.tableName.Unpaid"   ),
    TR8( "gcw.tableName.Disabled" )
  };

  return values[ static_cast<int>( _status ) ];
}

std::string
GCW::Gui::BillPay::
asStyle( Status _status )
{
  std::string values[] =
  {
    TR8( "Paid"     ),
    TR8( "Unpaid"   ),
    TR8( "Disabled" )
  };

  return values[ static_cast<int>( _status ) ];
}

