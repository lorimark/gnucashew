#line 2 "src/App.h"

#ifndef __APP_H___
#define __APP_H___

#include <Wt/WApplication.h>
#include <Wt/WEnvironment.h>

#include "Dbo/Session.h"
#include "Config.h"
#include "Gui/MainWidget.h"

namespace GCW {



class App
: public Wt::WApplication
{
  public:

    App( const Wt::WEnvironment & env );

    Dbo::GnuCash::Session   & gnucash_session   () { return m_gnucash_session;    }
    Dbo::GnuCashew::Session & gnucashew_session () { return m_gnucashew_session;  }
    GCW::Gui::MainWidget    * mainWidget        () { return m_mainWidget;         }

  private:

    Dbo::GnuCash::Session      m_gnucash_session;
    Dbo::GnuCashew::Session    m_gnucashew_session;
    GCW::Gui::MainWidget     * m_mainWidget = nullptr;

}; // endclass App

App * app();

} // endnamespace GCW {


#endif // end#ifndef __APP_H___


