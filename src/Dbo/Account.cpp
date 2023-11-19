#line 2 "src/Dbo/Account.cpp"

#include "Account.h"

std::vector< GCW::Dbo::Account::AccountDef_t > GCW::Dbo::Account::s_accountDef =
{// dbcr                              type                                  name           colAccount   colDr        colCr
  { GCW::Dbo::Account::DrCr::NONE   , GCW::Dbo::Account::Type::NONE       , "NONE"       , "account"  , "debit"    , "credit"     },
  { GCW::Dbo::Account::DrCr::NONE   , GCW::Dbo::Account::Type::ROOT       , "ROOT"       , "account"  , "payment"  , "charge"     },
  { GCW::Dbo::Account::DrCr::DEBIT  , GCW::Dbo::Account::Type::ASSET      , "ASSET"      , "transfer" , "deposit"  , "withdrawal" },
  { GCW::Dbo::Account::DrCr::DEBIT  , GCW::Dbo::Account::Type::BANK       , "BANK"       , "transfer" , "deposit"  , "withdrawal" },
  { GCW::Dbo::Account::DrCr::DEBIT  , GCW::Dbo::Account::Type::CASH       , "CASH"       , "transfer" , "receive"  , "spend"      },
  { GCW::Dbo::Account::DrCr::CREDIT , GCW::Dbo::Account::Type::CREDIT     , "CREDIT"     , "blank"    , "payment"  , "charge"     },
  { GCW::Dbo::Account::DrCr::CREDIT , GCW::Dbo::Account::Type::EQUITY     , "EQUITY"     , "transfer" , "decrease" , "increase"   },
  { GCW::Dbo::Account::DrCr::DEBIT  , GCW::Dbo::Account::Type::EXPENSE    , "EXPENSE"    , "transfer" , "expense"  , "rebate"     },
  { GCW::Dbo::Account::DrCr::CREDIT , GCW::Dbo::Account::Type::INCOME     , "INCOME"     , "account"  , "payment"  , "charge"     },
  { GCW::Dbo::Account::DrCr::CREDIT , GCW::Dbo::Account::Type::LIABILITY  , "LIABILITY"  , "account"  , "payment"  , "charge"     },
  { GCW::Dbo::Account::DrCr::CREDIT , GCW::Dbo::Account::Type::PAYABLE    , "PAYABLE"    , "account"  , "payment"  , "charge"     },
  { GCW::Dbo::Account::DrCr::DEBIT  , GCW::Dbo::Account::Type::RECEIVABLE , "RECEIVABLE" , "transfer" , "invoice"  , "payment"    }
};

Wt::WFormModel::Field GCW::Dbo::Account::Field::guid             = "guid"           ; // text(32) PRIMARY KEY NOT NULL
Wt::WFormModel::Field GCW::Dbo::Account::Field::name             = "name"           ; // text(2048) NOT NULL
Wt::WFormModel::Field GCW::Dbo::Account::Field::account_typeName = "account_type"   ; // text(2048) NOT NULL
Wt::WFormModel::Field GCW::Dbo::Account::Field::commodity_guid   = "commodity_guid" ; // text(32)
Wt::WFormModel::Field GCW::Dbo::Account::Field::commodity_scu    = "commodity_scu"  ; // integer NOT NULL
Wt::WFormModel::Field GCW::Dbo::Account::Field::non_std_scu      = "non_std_scu"    ; // integer NOT NULL
Wt::WFormModel::Field GCW::Dbo::Account::Field::parent_guid      = "parent_guid"    ; // text(32)
Wt::WFormModel::Field GCW::Dbo::Account::Field::code             = "code"           ; // text(2048)
Wt::WFormModel::Field GCW::Dbo::Account::Field::description      = "description"    ; // text(2048)
Wt::WFormModel::Field GCW::Dbo::Account::Field::hidden           = "hidden"         ; // integer
Wt::WFormModel::Field GCW::Dbo::Account::Field::placeHolder      = "placeholder"    ; // integer


