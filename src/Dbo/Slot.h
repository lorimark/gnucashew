#line 2 "src/Dbo/Slot.h"

#ifndef __SLOT_H___
#define __SLOT_H___

#include <Wt/WFormModel.h>

namespace GCW {
  namespace Dbo {
    namespace Slot {
      namespace Field {

extern Wt::WFormModel::Field id                ; // integer PRIMARY KEY AUTOINCREMENT NOT NULL,
extern Wt::WFormModel::Field obj_guid          ; // text(32) NOT NULL,
extern Wt::WFormModel::Field name              ; // text(4096) NOT NULL,
extern Wt::WFormModel::Field slot_type         ; // integer NOT NULL,
extern Wt::WFormModel::Field int64_val         ; // bigint,
extern Wt::WFormModel::Field string_val        ; // text(4096),
extern Wt::WFormModel::Field double_val        ; // float8,
extern Wt::WFormModel::Field timespec_val      ; // text(19),
extern Wt::WFormModel::Field guid_val          ; // text(32),
extern Wt::WFormModel::Field numeric_val_num   ; // bigint,
extern Wt::WFormModel::Field numeric_val_denom ; // bigint,
extern Wt::WFormModel::Field gdate_val         ; // text(8)

      }
    }
  }
} // endnamespace GCW {

#endif // end#ifndef __VAR_H___



