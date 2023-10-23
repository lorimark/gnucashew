#line 2 "src/Dbo/Account.cpp"

#include "Account.h"

Wt::WFormModel::Field GCW::Dbo::Account::Field::guid           = "guid"           ; // text(32) PRIMARY KEY NOT NULL
Wt::WFormModel::Field GCW::Dbo::Account::Field::name           = "name"           ; // text(2048) NOT NULL
Wt::WFormModel::Field GCW::Dbo::Account::Field::account_type   = "account_type"   ; // text(2048) NOT NULL
Wt::WFormModel::Field GCW::Dbo::Account::Field::commodity_guid = "commodity_guid" ; // text(32)
Wt::WFormModel::Field GCW::Dbo::Account::Field::commodity_scu  = "commodity_scu"  ; // integer NOT NULL
Wt::WFormModel::Field GCW::Dbo::Account::Field::non_std_scu    = "non_std_scu"    ; // integer NOT NULL
Wt::WFormModel::Field GCW::Dbo::Account::Field::parent_guid    = "parent_guid"    ; // text(32)
Wt::WFormModel::Field GCW::Dbo::Account::Field::code           = "code"           ; // text(2048)
Wt::WFormModel::Field GCW::Dbo::Account::Field::description    = "description"    ; // text(2048)
Wt::WFormModel::Field GCW::Dbo::Account::Field::hidden         = "hidden"         ; // integer
Wt::WFormModel::Field GCW::Dbo::Account::Field::placeHolder    = "placeholder"    ; // integer


