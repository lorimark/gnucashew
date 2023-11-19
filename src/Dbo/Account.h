#line 2 "src/Dbo/Account.h"

#ifndef __ACCOUNT_H___
#define __ACCOUNT_H___

#include <Wt/WFormModel.h>

namespace GCW {
  namespace Dbo {
    namespace Account {

/*!
** \brief Account Type Enum
**
** These are used internally to switch the views
**  depending on the account type.  They should
**  not be relied on for the native gnucash account
**  type codes (those are textual)
**
** \sa GCW::Dbo::Accounts::Item
**
*/
enum class DrCr
{
  NONE   = 0x00,
  DEBIT  = 0x01,
  CREDIT = 0x02
};

enum class Type
{
  NONE       = 0x00,
  ROOT       = 0x01,
  ASSET      = 0x02,
  BANK       = 0x03,
  CASH       = 0x04,
  CREDIT     = 0x05,
  EQUITY     = 0x06,
  EXPENSE    = 0x07,
  INCOME     = 0x08,
  LIABILITY  = 0x09,
  PAYABLE    = 0x0a,
  RECEIVABLE = 0x0b
};

typedef struct ACCOUNTDEF_S
{
  DrCr        drcr;
  Type        type;
  std::string name;
  std::string colAccount;
  std::string colDr;
  std::string colCr;

} AccountDef_t;

extern std::vector< AccountDef_t > s_accountDef;

      namespace Field {

extern Wt::WFormModel::Field guid             ; // text(32) PRIMARY KEY NOT NULL
extern Wt::WFormModel::Field name             ; // text(2048) NOT NULL
extern Wt::WFormModel::Field account_typeName ; // text(2048) NOT NULL
extern Wt::WFormModel::Field commodity_guid   ; // text(32)
extern Wt::WFormModel::Field commodity_scu    ; // integer NOT NULL
extern Wt::WFormModel::Field non_std_scu      ; // integer NOT NULL
extern Wt::WFormModel::Field parent_guid      ; // text(32)
extern Wt::WFormModel::Field code             ; // text(2048)
extern Wt::WFormModel::Field description      ; // text(2048)
extern Wt::WFormModel::Field hidden           ; // integer
extern Wt::WFormModel::Field placeHolder      ; // integer

      } // endnamespace Field {
    } // endnamespace Account {
  } // endnamespace Dbo {
} // endnamespace GCW {

#endif // end#ifndef __ACCOUNT_H___



