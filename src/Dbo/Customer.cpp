#line 2 "src/Dbo/Customer.cpp"

#include "Customer.h"

Wt::WFormModel::Field GCW::Dbo::Customer::Field::guid           = "guid"           ; // text(32) PRIMARY KEY NOT NULL
Wt::WFormModel::Field GCW::Dbo::Customer::Field::name           = "name"           ; // text(2048) NOT NULL
Wt::WFormModel::Field GCW::Dbo::Customer::Field::id             = "id"             ; // text(2048) NOT NULL
Wt::WFormModel::Field GCW::Dbo::Customer::Field::notes          = "notes"          ; // text(2048) NOT NULL,
Wt::WFormModel::Field GCW::Dbo::Customer::Field::active         = "active"         ; // integer NOT NULL,
Wt::WFormModel::Field GCW::Dbo::Customer::Field::discount_num   = "discount_num"   ; // bigint NOT NULL,
Wt::WFormModel::Field GCW::Dbo::Customer::Field::discount_denom = "discount_denom" ; // bigint NOT NULL,
Wt::WFormModel::Field GCW::Dbo::Customer::Field::credit_num     = "credit_num"     ; // bigint NOT NULL,
Wt::WFormModel::Field GCW::Dbo::Customer::Field::credit_denom   = "credit_denom"   ; // bigint NOT NULL,
Wt::WFormModel::Field GCW::Dbo::Customer::Field::currency       = "currency"       ; // text(32) NOT NULL,
Wt::WFormModel::Field GCW::Dbo::Customer::Field::tax_override   = "tax_override"   ; // integer NOT NULL,
Wt::WFormModel::Field GCW::Dbo::Customer::Field::addr_name      = "addr_name"      ; // text(1024),
Wt::WFormModel::Field GCW::Dbo::Customer::Field::addr_addr1     = "addr_addr1"     ; // text(1024),
Wt::WFormModel::Field GCW::Dbo::Customer::Field::addr_addr2     = "addr_addr2"     ; // text(1024),
Wt::WFormModel::Field GCW::Dbo::Customer::Field::addr_addr3     = "addr_addr3"     ; // text(1024),
Wt::WFormModel::Field GCW::Dbo::Customer::Field::addr_addr4     = "addr_addr4"     ; // text(1024),
Wt::WFormModel::Field GCW::Dbo::Customer::Field::addr_phone     = "addr_phone"     ; // text(128),
Wt::WFormModel::Field GCW::Dbo::Customer::Field::addr_fax       = "addr_fax"       ; // text(128),
Wt::WFormModel::Field GCW::Dbo::Customer::Field::addr_email     = "addr_email"     ; // text(256),
Wt::WFormModel::Field GCW::Dbo::Customer::Field::shipaddr_name  = "shipaddr_name"  ; // text(1024),
Wt::WFormModel::Field GCW::Dbo::Customer::Field::shipaddr_addr1 = "shipaddr_addr1" ; // text(1024),
Wt::WFormModel::Field GCW::Dbo::Customer::Field::shipaddr_addr2 = "shipaddr_addr2" ; // text(1024),
Wt::WFormModel::Field GCW::Dbo::Customer::Field::shipaddr_addr3 = "shipaddr_addr3" ; // text(1024),
Wt::WFormModel::Field GCW::Dbo::Customer::Field::shipaddr_addr4 = "shipaddr_addr4" ; // text(1024),
Wt::WFormModel::Field GCW::Dbo::Customer::Field::shipaddr_phone = "shipaddr_phone" ; // text(128),
Wt::WFormModel::Field GCW::Dbo::Customer::Field::shipaddr_fax   = "shipaddr_fax"   ; // text(128),
Wt::WFormModel::Field GCW::Dbo::Customer::Field::shipaddr_email = "shipaddr_email" ; // text(256),
Wt::WFormModel::Field GCW::Dbo::Customer::Field::terms          = "terms"          ; // text(32),
Wt::WFormModel::Field GCW::Dbo::Customer::Field::tax_included   = "tax_included"   ; // integer,
Wt::WFormModel::Field GCW::Dbo::Customer::Field::taxtable       = "taxtable"       ; // text(32)



