#line 2 "src/Eng/AccountRegisterModel.h"

#ifndef __ENG_ACCOUNTREGISTERMODEL_H___
#define __ENG_ACCOUNTREGISTERMODEL_H___

#include <Wt/WStandardItemModel.h>

namespace GCW {
  namespace Eng {

class AccountRegisterModel
: public Wt::WStandardItemModel
{
  using RowItem = std::vector< std::unique_ptr< Wt::WStandardItem > >;

  public:

    AccountRegisterModel( const std::string & _accountGuid, bool _editable = true );

    void refreshFromDisk();

    RowItem makeRow( const std::string & _splitGuid );

    std::set< std::string > suggestionsFromColumn( int _column ) const;

/*
balance
  balance limit
  balance (Period)
  balance usd
  cleared
  cleared usd
  fiture minimum
  future minimum usa
  last num
  last reconcile date
  opening balance
  present
  present usd
  reconciled reconciled usd
  total total period
  total usd
*/

  private:

    bool        m_editable = false;
    std::string m_accountGuid;
    std::string m_lastDate;

}; // endclass AccountRegisterModel

  } //  namespace Eng {
} // endnamespace GCW {


#endif // #ifndef __ENG_REGISTERMODEL_H___



