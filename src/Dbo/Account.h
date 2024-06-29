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
  INVALID    = -1,
  NONE       = -1,
  BANK       = 0,
  CASH       = 1,
  CREDIT     = 3,
  ASSET      = 2,
  LIABILITY  = 4,
  STOCK      = 5,
  MUTUAL     = 6,
  CURRENCY   = 7,
  INCOME     = 8,
  EXPENSE    = 9,
  EQUITY     = 10,
  RECEIVABLE = 11,
  PAYABLE    = 12,
  ROOT       = 13,
  TRADING    = 14,
  NUM_ACCOUNT_TYPES    = 15,
  CHECKING   = 15,
  SAVINGS    = 16,
  MONEYMRKT  = 17,
  CREDITLINE = 18,
  LAST

};

/*!
**
**
*/
typedef struct ACCOUNTDEF_S
{
  DrCr        drcr;
  Type        type;
  std::string name;
  std::string colAccount;
  std::string colDr;
  std::string colCr;

} AccountDef_t;

extern const std::vector< AccountDef_t > s_accountDef;

      namespace Field {

extern const Wt::WFormModel::Field guid             ; // text(32) PRIMARY KEY NOT NULL
extern const Wt::WFormModel::Field name             ; // text(2048) NOT NULL
extern const Wt::WFormModel::Field account_typeName ; // text(2048) NOT NULL
extern const Wt::WFormModel::Field commodity_guid   ; // text(32)
extern const Wt::WFormModel::Field commodity_scu    ; // integer NOT NULL
extern const Wt::WFormModel::Field non_std_scu      ; // integer NOT NULL
extern const Wt::WFormModel::Field parent_guid      ; // text(32)
extern const Wt::WFormModel::Field code             ; // text(2048)
extern const Wt::WFormModel::Field description      ; // text(2048)
extern const Wt::WFormModel::Field hidden           ; // integer
extern const Wt::WFormModel::Field placeHolder      ; // integer

      } // endnamespace Field {
    } // endnamespace Account {
  } // endnamespace Dbo {
} // endnamespace GCW {

#endif // end#ifndef __ACCOUNT_H___



