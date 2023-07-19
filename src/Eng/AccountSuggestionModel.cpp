
#include "AccountSuggestionModel.h"

GCW::Eng::AccountSuggestionModel::
AccountSuggestionModel( const std::string & _accountGuid )
: Wt::WStandardItemModel( 0, 1 ),
  m_accountGuid( _accountGuid )
{

} // endGCW::RegisterWidget::Model::Model( const std::string & _accountGuid )


