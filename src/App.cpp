#line 2 "src/App.cpp"

#include <Wt/Date/tz.h>
#include <Wt/WBootstrapTheme.h>
#include <Wt/WDate.h>
#include <Wt/WLocale.h>
#include <Wt/WVBoxLayout.h>
#include <Wt/WHBoxLayout.h>

#include "App.h"

GCW::App * GCW::app()
{
  return static_cast< GCW::App* >( Wt::WApplication::instance() );
}

GCW::App::App( const Wt::WEnvironment & env )
: Wt::WApplication( env )
{
  root()-> addStyleClass( "App" );

  gnucash_session().open( "/home/serveradmin/gnucash-dev.gnucash" );

  /*
  ** Utilize the bootstrap theme.
  **
  */
  auto bootstrapTheme = std::make_shared<Wt::WBootstrapTheme>();
  bootstrapTheme-> setVersion( Wt::BootstrapVersion::v3 );
  bootstrapTheme-> setResponsive(true);
  setTheme( bootstrapTheme );
  useStyleSheet( "resources/themes/bootstrap/3/bootstrap-theme.min.css" );
  useStyleSheet( "gcw.css" );

  /*
  ** GnuCashew english language translations
  **
  */
  messageResourceBundle().use( appRoot() + "gcw_en" );

#ifdef NEVER
  /*
  ** set the date format to regular american.
  **
  */
  auto loc = locale();
  auto tz = date::locate_zone( environment().timeZoneName() );
  loc.setTimeZone( tz );
#endif

  /*
  ** Set a layout manager on the root widget so that everything can
  **  be laid out correctly.
  **
  */
  auto lw = root()-> setLayout( std::make_unique< Wt::WVBoxLayout >() );

  /*
  ** Build and install the main desktop widget.
  **
  */
  m_mainWidget = lw-> addWidget( std::make_unique< GCW::MainWidget >() );

} // endGnuCashew::App::App( const Wt::WEnvironment & env )


