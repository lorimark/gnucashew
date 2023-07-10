#line 2 "src/GnuCashew.h"

#ifndef __GNUCASHEW_H___
#define __GNUCASHEW_H___

#include "define.h"
#include "App.h"

#define DEC_NAMESPACE DECIMAL
#include "3rd/decimal.h"

namespace GCW {

DECIMAL::decimal_format decimal_format();

std::string date_format();
std::string time_format();

} // endnamespace GCW {

#endif // end#ifndef __GNUCASHEW_H___


