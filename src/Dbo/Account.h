#line 2 "src/Dbo/Account.h"

#ifndef __ACCOUNT_H___
#define __ACCOUNT_H___

#include <Wt/WFormModel.h>

namespace GCW {
  namespace Dbo {
    namespace Account {
      namespace Field {

extern Wt::WFormModel::Field guid           ; // text(32) PRIMARY KEY NOT NULL
extern Wt::WFormModel::Field name           ; // text(2048) NOT NULL
extern Wt::WFormModel::Field account_type   ; // text(2048) NOT NULL
extern Wt::WFormModel::Field commodity_guid ; // text(32)
extern Wt::WFormModel::Field commodity_scu  ; // integer NOT NULL
extern Wt::WFormModel::Field non_std_scu    ; // integer NOT NULL
extern Wt::WFormModel::Field parent_guid    ; // text(32)
extern Wt::WFormModel::Field code           ; // text(2048)
extern Wt::WFormModel::Field description    ; // text(2048)
extern Wt::WFormModel::Field hidden         ; // integer
extern Wt::WFormModel::Field placeHolder    ; // integer

      } // endnamespace Field {
    } // endnamespace Account {
  } // endnamespace Dbo {
} // endnamespace GCW {

#endif // end#ifndef __ACCOUNT_H___



