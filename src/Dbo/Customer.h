

#ifndef __CUSTOMER_H___
#define __CUSTOMER_H___

#include <Wt/WFormModel.h>

namespace GCW {
  namespace Dbo {
    namespace Customer {
      namespace Field {

extern Wt::WFormModel::Field guid           ; // text(32) PRIMARY KEY NOT NULL
extern Wt::WFormModel::Field name           ; // text(2048) NOT NULL
extern Wt::WFormModel::Field id             ; // text(2048) NOT NULL

      }
    }
  }
} // endnamespace GCW {

#endif // end#ifndef __CUSTOMER_H___



