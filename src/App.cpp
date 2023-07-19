#line 2 "src/App.cpp"

#include <Wt/Date/tz.h>
#include <Wt/WBootstrapTheme.h>
#include <Wt/WDate.h>
#include <Wt/WDialog.h>
#include <Wt/WEnvironment.h>
#include <Wt/WHBoxLayout.h>
#include <Wt/WLocale.h>
#include <Wt/WText.h>
#include <Wt/WVBoxLayout.h>

#include "App.h"
#include "GnuCashew.h"

extern std::string g_dbName;

namespace {

void showEnvironment()
{
  auto app = GCW::app();

  std::cout << __FILE__ << ":" << __LINE__
    << "\n title:              " << app-> title()
    << "\n appRoot:            " << app-> appRoot()
    << "\n docRoot:            " << app-> docRoot()
    << "\n sessionId:          " << app-> sessionId()
    << "\n metaHeader:         " << app-> metaHeader( Wt::MetaHeaderType::Meta, "*" )
    << "\n bodyClass:          " << app-> bodyClass()
    << "\n htmlClass:          " << app-> htmlClass()
    << "\n url:                " << app-> url()
    << "\n bookmarkUrl:        " << app-> bookmarkUrl()
    << "\n internalPath:       " << app-> internalPath()
    << "\n javaScriptClass:    " << app-> javaScriptClass()
    << "\n resourcesUrl:       " << app-> resourcesUrl()
    << "\n r-resourcesUrl:     " << app-> relativeResourcesUrl()
    << "\n referrer:           " << app-> environment().referer()
    << "\n accept:             " << app-> environment().accept()
    << "\n X-Forwarded-For:    " << app-> environment().headerValue( "X-Forwarded-For" )
    << "\n X-Forwarded-Client: " << app-> environment().headerValue( "X-Forwarded-Client" )
    << "\n X-Forwarded-Xyz:    " << app-> environment().headerValue( "X-Forwarded-Xyz" )
    << "\n clientAddress:      " << app-> environment().clientAddress()
    << std::endl
    ;

  for( const auto & pair : app-> environment().getParameterMap() )
    for( const auto & value : pair.second )
      std::cout << __FILE__ << ":" << __LINE__ << " " << pair.first << "=" << value << std::endl;

} // endvoid showEnvironment()

void showWelcome()
{
  wApp-> processEvents();

  Wt::WDialog dialog( TR( "gcw.welcome.title" )  );
  dialog.rejectWhenEscapePressed( true );
  dialog.setClosable( true );
  dialog.contents()-> addNew< Wt::WText >( TR( "gcw.welcome.body" ) );
  dialog.exec();
}

} // endnamespace {

GCW::App * GCW::app()
{
  return static_cast< GCW::App* >( Wt::WApplication::instance() );
}

GCW::App::App( const Wt::WEnvironment & env )
: Wt::WApplication( env )
{
  root()-> addStyleClass( "GnuCashew" );

  gnucash_session().open( g_dbName );

  showEnvironment();

  /*
  ** Utilize the bootstrap theme.
  **
  */
  auto bootstrapTheme = std::make_shared<Wt::WBootstrapTheme>();
  bootstrapTheme-> setVersion( Wt::BootstrapVersion::v3 );
  bootstrapTheme-> setResponsive( true );
  setTheme( bootstrapTheme );
  useStyleSheet( "resources/themes/bootstrap/3/bootstrap-theme.min.css" );
  useStyleSheet( "styles/gcw.css" );

  /*
  ** GnuCashew english language translations
  **
  */
  messageResourceBundle().use( docRoot() + "/styles/gcw_gui" );  // UI elements
  messageResourceBundle().use( docRoot() + "/styles/gcw_en"  );  // Language Elements

  /*
  ** set the date format to the browser.
  **
  */
#ifdef NEVER
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
  m_mainWidget = lw-> addWidget( std::make_unique< GCW::Gui::MainWidget >() );

  if( bookmarkUrl() == "demo" )
    showWelcome();

} // endGnuCashew::App::App( const Wt::WEnvironment & env )


