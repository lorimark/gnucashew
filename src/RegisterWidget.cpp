#line 2 "src/RegisterWidget.cpp"

#include <Wt/WText.h>
#include <Wt/WTableView.h>

#include "define.h"
#include "App.h"
#include "RegisterWidget.h"
#include "Dbo/Split.h"


GCW::RegisterWidget::
RegisterWidget( const std::string & _accountGuid )
: m_tableView( addNew< Wt::WTableView >() )
{
  setModel( _accountGuid );

} // endGCW::RegisterWidget::RegisterWidget()

GCW::RegisterWidget::
~RegisterWidget()
{
  std::cout << __FILE__ << ":" << __LINE__ << " " << std::endl;

}


void GCW::RegisterWidget::
setModel( const std::string & _accountGuid )
{
  auto model = std::make_shared< Model >();

  model-> load( _accountGuid );

  tableView()-> setModel( model );

} // endvoid GCW::RegisterWidget::setModel()

void GCW::RegisterWidget::Model::
load( const std::string & _accountGuid )
{
  /*
  ** If the session isn't open then there's nothing to load.
  **
  */
  if( !GCW::app()-> gnucash_session().isOpen() )
    return;


  auto splits = GCW::Dbo::Split::byAccount( _accountGuid );

  std::cout << __FILE__ << ":" << __LINE__ << " " << splits.size() << std::endl;



#ifdef NEVER
  int col = 0;
  setHeaderData( col++, "Account Name"         );
  setHeaderData( col++, "Account Code"         );
  setHeaderData( col++, "Description"          );
  setHeaderData( col++, "Tax Info"             );
  setHeaderData( col++, "Notes"                );
  setHeaderData( col++, "Future Minimum (USD)" );
  setHeaderData( col++, "Total"                );
#endif

} // endvoid GCW::RegisterWidget::Model::load()


