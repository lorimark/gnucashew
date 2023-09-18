#line 2 "src/Gui/BillPay/BillPay.h"

#ifndef __GUI_BILLPAY_H___
#define __GUI_BILLPAY_H___

#include "../../GnuCashew.h"
#include "../../Dbo/Vars.h"

#include "EditWidget.h"
#include "MainWidget.h"
#include "Model.h"
#include "Status.h"
#include "Table.h"
#include "ToolBar.h"

namespace GCW {
  namespace Gui {
    namespace BillPay {

/*
** Use these to identify the cfyField type
**  for the varItems for the various functions
**  within this application.
**
*/
#define GCW_GUI_BILLPAY_CONFIG_CFY "mpbc"
#define GCW_GUI_BILLPAY_ITEM_CFY   "mbpi"

GCW::Dbo::Vars::Item::Ptr configItem();
GCW::Dbo::Vars::Item::Ptr bpItem( const std::string & _accountGuid );

std::string toString( int _value );

    } // endnamespace BillPay {
  } // endnamespace Gui {
} // endnamespace GCW {

#endif // __GUI_BILLPAY_H___




