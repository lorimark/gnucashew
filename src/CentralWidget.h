

#ifndef __CENTRALWIDGET_H___
#define __CENTRALWIDGET_H___

#include <Wt/WContainerWidget.h>
#include <Wt/WTabWidget.h>

namespace GCW {

class CentralWidget
: public Wt::WContainerWidget
{
  public:

    CentralWidget();

    const Wt::WTabWidget * tabWidget() const { return m_tabWidget; }
          Wt::WTabWidget * tabWidget()       { return m_tabWidget; }

  private:

    Wt::WTabWidget * m_tabWidget = nullptr;

}; // endclass CentralWidget


} // endnamespace GCW {


#endif // end#ifndef __CENTRALWIDGET_H___


