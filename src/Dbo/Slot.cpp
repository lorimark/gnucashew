#line 2 "src/Dbo/Slot.cpp"

#include "Slot.h"

Wt::WFormModel::Field GCW::Dbo::Slot::Field::id                 = "id"                ; // integer PRIMARY KEY AUTOINCREMENT NOT NULL,
Wt::WFormModel::Field GCW::Dbo::Slot::Field::obj_guid           = "obj_guid"          ; // text(32) NOT NULL,
Wt::WFormModel::Field GCW::Dbo::Slot::Field::name               = "name"              ; // text(4096) NOT NULL,
Wt::WFormModel::Field GCW::Dbo::Slot::Field::slot_type          = "slot_type"         ; // integer NOT NULL,
Wt::WFormModel::Field GCW::Dbo::Slot::Field::int64_val          = "int64_val"         ; // bigint,
Wt::WFormModel::Field GCW::Dbo::Slot::Field::string_val         = "string_val"        ; // text(4096),
Wt::WFormModel::Field GCW::Dbo::Slot::Field::double_val         = "double_val"        ; // float8,
Wt::WFormModel::Field GCW::Dbo::Slot::Field::timespec_val       = "timespec_val"      ; // text(19),
Wt::WFormModel::Field GCW::Dbo::Slot::Field::guid_val           = "guid_val"          ; // text(32),
Wt::WFormModel::Field GCW::Dbo::Slot::Field::numeric_val_num    = "numeric_val_num"   ; // bigint,
Wt::WFormModel::Field GCW::Dbo::Slot::Field::numeric_val_denom  = "numeric_val_denom" ; // bigint,
Wt::WFormModel::Field GCW::Dbo::Slot::Field::gdate_val          = "gdate_val"         ; // text(8)


