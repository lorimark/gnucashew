#line 2 "src/Gui/BillPay/Status.h"

#ifndef __GUI_BILLPAY_STATUS_H___
#define __GUI_BILLPAY_STATUS_H___

#include <string>

namespace GCW {
  namespace Gui {
    namespace BillPay {

/*
** Bill Status: paid or unpaid
**
*/
enum class Status
{
  Paid,
  Unpaid,
  Disabled
};

std::string asString( Status _status );

    } // endnamespace BillPay {
  } // endnamespace Gui {
} // endnamespace GCW {

#endif // __GUI_BILLPAY_STATUS_H___



