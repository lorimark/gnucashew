#line 2 "src/Dbo/Account.cpp"

#include "Account.h"

const std::vector< GCW::Dbo::Account::AccountDef_t > GCW::Dbo::Account::s_accountDef =
{// dbcr                               type                                  name           colAccount   colDr        colCr
  { GCW::Dbo::Account::DrCr::NONE   ,  GCW::Dbo::Account::Type::INVALID    , "INVALID"    , "account"  , "debit"    , "credit"      },
  { GCW::Dbo::Account::DrCr::NONE   ,  GCW::Dbo::Account::Type::NONE       , "NONE"       , "account"  , "debit"    , "credit"      },
  { GCW::Dbo::Account::DrCr::DEBIT  ,  GCW::Dbo::Account::Type::BANK       , "BANK"       , "transfer" , "deposit"  , "withdrawal"  },
  { GCW::Dbo::Account::DrCr::DEBIT  ,  GCW::Dbo::Account::Type::CASH       , "CASH"       , "transfer" , "receive"  , "spend"       },
  { GCW::Dbo::Account::DrCr::CREDIT ,  GCW::Dbo::Account::Type::CREDIT     , "CREDIT"     , "blank"    , "payment"  , "charge"      },
  { GCW::Dbo::Account::DrCr::DEBIT  ,  GCW::Dbo::Account::Type::ASSET      , "ASSET"      , "transfer" , "deposit"  , "withdrawal"  },
  { GCW::Dbo::Account::DrCr::CREDIT ,  GCW::Dbo::Account::Type::LIABILITY  , "LIABILITY"  , "account"  , "payment"  , "charge"      },
  { GCW::Dbo::Account::DrCr::DEBIT  ,  GCW::Dbo::Account::Type::STOCK      , "STOCK"      , "account"  , "debit"    , "credit"      },
  { GCW::Dbo::Account::DrCr::DEBIT  ,  GCW::Dbo::Account::Type::MUTUAL     , "MUTUAL"     , "account"  , "debit"    , "credit"      },
  { GCW::Dbo::Account::DrCr::DEBIT  ,  GCW::Dbo::Account::Type::CURRENCY   , "CURRENCY"   , "account"  , "debit"    , "credit"      },
  { GCW::Dbo::Account::DrCr::CREDIT ,  GCW::Dbo::Account::Type::INCOME     , "INCOME"     , "account"  , "debit"    , "credit"      },
  { GCW::Dbo::Account::DrCr::DEBIT  ,  GCW::Dbo::Account::Type::EXPENSE    , "EXPENSE"    , "transfer" , "expense"  , "rebate"      },
  { GCW::Dbo::Account::DrCr::CREDIT ,  GCW::Dbo::Account::Type::EQUITY     , "EQUITY"     , "transfer" , "decrease" , "increase"    },
  { GCW::Dbo::Account::DrCr::DEBIT  ,  GCW::Dbo::Account::Type::RECEIVABLE , "RECEIVABLE" , "transfer" , "invoice"  , "payment"     },
  { GCW::Dbo::Account::DrCr::CREDIT ,  GCW::Dbo::Account::Type::PAYABLE    , "PAYABLE"    , "account"  , "payment"  , "charge"      },
  { GCW::Dbo::Account::DrCr::NONE   ,  GCW::Dbo::Account::Type::ROOT       , "ROOT"       , "account"  , "debit"    , "credit"      },
  { GCW::Dbo::Account::DrCr::DEBIT  ,  GCW::Dbo::Account::Type::TRADING    , "TRADING"    , "account"  , "debit"    , "credit"      },
  { GCW::Dbo::Account::DrCr::DEBIT  ,  GCW::Dbo::Account::Type::CHECKING   , "CHECKING"   , "account"  , "debit"    , "credit"      },
  { GCW::Dbo::Account::DrCr::DEBIT  ,  GCW::Dbo::Account::Type::SAVINGS    , "SAVINGS"    , "account"  , "debit"    , "credit"      },
  { GCW::Dbo::Account::DrCr::DEBIT  ,  GCW::Dbo::Account::Type::MONEYMRKT  , "MONEYMRKT"  , "account"  , "debit"    , "credit"      },
  { GCW::Dbo::Account::DrCr::CREDIT ,  GCW::Dbo::Account::Type::CREDITLINE , "CREDITLINE" , "account"  , "debit"    , "credit"      },
};

const Wt::WFormModel::Field GCW::Dbo::Account::Field::guid             = "guid"           ; // text(32) PRIMARY KEY NOT NULL
const Wt::WFormModel::Field GCW::Dbo::Account::Field::name             = "name"           ; // text(2048) NOT NULL
const Wt::WFormModel::Field GCW::Dbo::Account::Field::account_typeName = "account_type"   ; // text(2048) NOT NULL
const Wt::WFormModel::Field GCW::Dbo::Account::Field::commodity_guid   = "commodity_guid" ; // text(32)
const Wt::WFormModel::Field GCW::Dbo::Account::Field::commodity_scu    = "commodity_scu"  ; // integer NOT NULL
const Wt::WFormModel::Field GCW::Dbo::Account::Field::non_std_scu      = "non_std_scu"    ; // integer NOT NULL
const Wt::WFormModel::Field GCW::Dbo::Account::Field::parent_guid      = "parent_guid"    ; // text(32)
const Wt::WFormModel::Field GCW::Dbo::Account::Field::code             = "code"           ; // text(2048)
const Wt::WFormModel::Field GCW::Dbo::Account::Field::description      = "description"    ; // text(2048)
const Wt::WFormModel::Field GCW::Dbo::Account::Field::hidden           = "hidden"         ; // integer
const Wt::WFormModel::Field GCW::Dbo::Account::Field::placeHolder      = "placeholder"    ; // integer


