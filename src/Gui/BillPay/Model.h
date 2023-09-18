#line 2 "src/Gui/BillPay/Model.h"

#ifndef __GUI_BILLPAY_MODEL_H___
#define __GUI_BILLPAY_MODEL_H___

#include <Wt/WLength.h>
#include <Wt/WStandardItemModel.h>

#include "Status.h"

namespace GCW {
  namespace Gui {
    namespace BillPay {

typedef struct COLUMNDEF
{
  const char * name;
  Wt::WLength width;
  Wt::AlignmentFlag alignment;
  const char * toolTip;

} ColumnDef_t;

/*
** Data Model: contains paid or unpaid accounts
**
*/
class Model
: public Wt::WStandardItemModel
{
  public:

    Model( int _selectedModel, const Status _status );

    ColumnDef_t columnDef( int col );

    void loadData( int _selectedMonth );

  private:

    Status m_status;
};


    } // endnamespace BillPay {
  } // endnamespace Gui {
} // endnamespace GCW {

#endif // __GUI_BILLPAY_MODEL_H___


