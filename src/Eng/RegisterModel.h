
#ifndef __ENG_REGISTERMODEL_H___
#define __ENG_REGISTERMODEL_H___

#include <Wt/WStandardItemModel.h>

namespace GCW {
  namespace Eng {

class RegisterModel
: public Wt::WStandardItemModel
{
  using RowItem = std::vector< std::unique_ptr< Wt::WStandardItem > >;

  public:

    RegisterModel( const std::string & _accountGuid );

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

    std::string m_accountGuid;

}; // endclass RegisterModel

  } //  namespace Eng {
} // endnamespace GCW {


#endif // #ifndef __ENG_REGISTERMODEL_H___



