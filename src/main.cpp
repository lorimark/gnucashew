#line 2 "src/main.cpp"

#include <dirent.h>
#include <fstream>
#include <grp.h>
#include <langinfo.h>
#include <locale.h>
#include <pwd.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

#include <Wt/Http/Request.h>
#include <Wt/Http/Response.h>
#include <Wt/WDateTime.h>
#include <Wt/WLayout.h>
#include <Wt/WResource.h>
#include <Wt/WServer.h>

#include "GnuCashew.h"

#define VAULT_ROOT std::string("/")
#define CLIENT_FOLDERS "/"

void show_program_version( const std::string & message )
{
#ifdef __GRAB_DATE_STRING_FROM_PROGRAM_DATE__
  struct stat attr;
  stat( argv[0], &attr );

  auto tm = localtime( &attr.st_mtime );
  char datestring[256];

  /* Get localized date string. */
  strftime( datestring, sizeof(datestring), nl_langinfo(D_T_FMT), tm );
#endif

#ifdef NEVER
  std::string datestring = __GNUCASHEW_VERSION__ + " " + __GNUCASHEW_BUILD__;

  std::cout << __DATETIMEPIDFILELINE__
    << " " << argv[0]
    << " " << datestring
    << " -> " << message
    << std::endl;
#endif

}

class Redirector
: public Wt::WApplication
{
  public:
    Redirector( const Wt::WEnvironment & env )
    : Wt::WApplication(env)
    {
      redirect( "/staff" );
    }
};


/*
** This is a simple static html file server.  It is designed to serve up files
**  in the approot/html folder.  This resource employs a little bit of access
**  control by way of 'valid-ip' address handling.
**
*/
class HtmlResource
: public Wt::WResource
{
  public:

    void handleRequest( const Wt::Http::Request & request, Wt::Http::Response & response );

};

void HtmlResource::handleRequest( const Wt::Http::Request & request, Wt::Http::Response & response )
{
#ifdef NEVER
  std::cout << __FILE__ << ":" << __LINE__
    << "\n headers:" << request.headers().size()
    << "\n path:" << request.path()
    << "\n queryString:" << request.queryString()
    << "\n urlParams:" << request.urlParams().size()
    << "\n pathInfo:" << request.pathInfo()
    << "\n clientAddress:" << request.clientAddress()
    << std::endl;

  for( auto header : request.headers() )
  {
    std::cout << __FILE__ << ":" << __LINE__ << " " << header.name() << " " << header.value() << std::endl;

  }
#endif

  auto _blocked = [&]()
  {
    std::vector< std::string > allowed =
    {
      { "192.168.168.168" }, // router
      { "47.190.7.163"    }, // mark home
      { "67.79.41.154"    }, // artemis office
//      { "70.119.46.4"     }, // thomas
    };

    return
      std::find
      (
       allowed.begin(),
       allowed.end(),
       request.headerValue( "X-Forwarded-For" )
      ) == allowed.end();
  };

  /*
  ** this checks if the IP address is blocked.
  **  if it is blocked, it will insert an entry
  **  in to a local log file so we can back-trace
  **  attempted hacks.
  **
  */
  if( _blocked() )
  {
    std::cout << __FILE__ << ":" << __LINE__
      << " HtmlResource::ipblocked: [" << request.headerValue( "X-Forwarded-For" ) << "]"
      << " '" << request.path() << "'"
      << std::endl;

    response.out() << "<html><body>oh oh, something went wrong.<br />Please contact the site administrator</body></html>";

    std::ofstream file;
    file.open( "HtmlResource.log", std::ios_base::app );
    if( file.is_open() )
    {
      file
        << "["  << Wt::WDateTime::currentDateTime().toString().toUTF8() << "]"
        << " [" << request.headerValue( "x-Forwarded-For" ) << "]"
        << " "  << request.path()
        << std::endl;
    }

    return;
  }

  /*
  ** The user is allowed to view these documents.
  **
  */
  auto fileName = "approot/html" + request.pathInfo();
  std::ifstream file;
  file.open( fileName );

  if( !file.is_open() )
  {
    std::cout << __FILE__ << ":" << __LINE__ << " NO FILE:" << fileName << std::endl;
    response.out() << "no file!";
  }

  std::cout << __FILE__ << ":" << __LINE__
    << " [" << request.headerValue( "X-Forwarded-For" ) << "]"
    << " HtmlResource:" << fileName
    << std::endl;

  response.out() << file.rdbuf();

} // endvoid HtmlResource::handleRequest( const Wt::Http::Request & request, Wt::Http::Response & response )


class MonitResource
: public Wt::WResource
{
  public:

    void handleRequest( const Wt::Http::Request & request, Wt::Http::Response & response );

};

void MonitResource::handleRequest( const Wt::Http::Request & request, Wt::Http::Response & response )
{
#ifdef NEVER
  std::cout << __FILE__ << ":" << __LINE__
    << "\n headers:"       << request. headers       ().size()
    << "\n path:"          << request. path          ()
    << "\n queryString:"   << request. queryString   ()
    << "\n urlParams:"     << request. urlParams     ().size()
    << "\n pathInfo:"      << request. pathInfo      ()
    << "\n clientAddress:" << request. clientAddress ()
    << std::endl;

  for( auto header : request.headers() )
  {
    std::cout << __FILE__ << ":" << __LINE__ << " " << header.name() << " " << header.value() << std::endl;

  }
#endif

#ifdef NEVER
  auto _blocked = [&]()
  {
    std::vector< std::string > allowed =
    {
      { "192.168.168.168" }, // router
      { "47.190.7.163"    }, // mark home
      { "67.79.41.154"    }, // artemis office
      { "70.119.46.4"     }, // thomas
    };

    return
      std::find
      (
       allowed.begin(),
       allowed.end(),
       request.headerValue( "X-Forwarded-For" )
      )
      == allowed.end();
  };

  /*
  ** this checks if the IP address is blocked.
  **  if it is blocked, it will insert an entry
  **  in to a local log file so we can back-trace
  **  attempted hacks.
  **
  */
  if( _blocked() )
  {
    std::cout << __FILE__ << ":" << __LINE__
      << " HtmlResource::ipblocked: [" << request.headerValue( "X-Forwarded-For" ) << "]"
      << " '" << request.path() << "'"
      << std::endl;

    response.out() << "<html><body>oh oh, something went wrong.<br />Please contact the site administrator</body></html>";

    std::ofstream file;
    file.open( "HtmlResource.log", std::ios_base::app );
    if( file.is_open() )
    {
      file
        << "["  << Wt::WDateTime::currentDateTime().toString().toUTF8() << "]"
        << " [" << request.headerValue( "x-Forwarded-For" ) << "]"
        << " "  << request.path()
        << std::endl;
    }

    return;
  }
#endif

  response.out() << "ok\n";

} // endvoid ApiResource::handleRequest( const Wt::Http::Request & request, Wt::Http::Response & response )











/*
** This is a simple static html file server.  It is designed to serve up files
**  in the approot/html folder.  This resource employs a little bit of access
**  control by way of 'valid-ip' address handling.
**
*/
class ApiResource
: public Wt::WResource
{
  public:

    void handleRequest( const Wt::Http::Request & request, Wt::Http::Response & response );

};

void ApiResource::handleRequest( const Wt::Http::Request & request, Wt::Http::Response & response )
{
#ifdef NEVER
  std::cout << __FILE__ << ":" << __LINE__
    << "\n headers:"       << request. headers       ().size()
    << "\n path:"          << request. path          ()
    << "\n queryString:"   << request. queryString   ()
    << "\n urlParams:"     << request. urlParams     ().size()
    << "\n pathInfo:"      << request. pathInfo      ()
    << "\n clientAddress:" << request. clientAddress ()
    << std::endl;

  for( auto header : request.headers() )
  {
    std::cout << __FILE__ << ":" << __LINE__ << " " << header.name() << " " << header.value() << std::endl;

  }
#endif

  auto _blocked = [&]()
  {
    std::vector< std::string > allowed =
    {
      { "192.168.168.168" }, // router
      { "47.190.7.163"    }, // mark home
      { "67.79.41.154"    }, // artemis office
      { "70.119.46.4"     }, // thomas
    };

    return
      std::find
      (
       allowed.begin(),
       allowed.end(),
       request.headerValue( "X-Forwarded-For" )
      )
      == allowed.end();
  };

  /*
  ** this checks if the IP address is blocked.
  **  if it is blocked, it will insert an entry
  **  in to a local log file so we can back-trace
  **  attempted hacks.
  **
  */
  if( _blocked() )
  {
    std::cout << __FILE__ << ":" << __LINE__
      << " HtmlResource::ipblocked: [" << request.headerValue( "X-Forwarded-For" ) << "]"
      << " '" << request.path() << "'"
      << std::endl;

    response.out() << "<html><body>oh oh, something went wrong.<br />Please contact the site administrator</body></html>";

    std::ofstream file;
    file.open( "HtmlResource.log", std::ios_base::app );
    if( file.is_open() )
    {
      file
        << "["  << Wt::WDateTime::currentDateTime().toString().toUTF8() << "]"
        << " [" << request.headerValue( "x-Forwarded-For" ) << "]"
        << " "  << request.path()
        << std::endl;
    }

    return;
  }

  if( request.queryString() == "c=jobphotos" )
  {
    for( int i = 100000; i < 115000; i++ )
    {
      auto _path =
        Wt::WString( VAULT_ROOT + "/jobs/" )
        .arg( i )
        .toUTF8()
        ;

      std::cout << __FILE__ << ":" << __LINE__ << " " << _path << std::endl;



    }
  }

  response.out() << "ok";

} // endvoid ApiResource::handleRequest( const Wt::Http::Request & request, Wt::Http::Response & response )










template <class C>
void addEntryPoint( const std::string & url, Wt::WServer & server )
{
  server.addEntryPoint
  (
   Wt::EntryPointType::Application,
   [](const Wt::WEnvironment &env)
   {
     return std::make_unique<C>(env);
   },
   url
  );
}

int main( int argc, char ** argv )
{
  show_program_version( "start-up" );

  /*
  ** Set the layout to employ the services that
  **  are compatible with embedded table views.  Right now, only
  **  JavaScript layout implementation works properly.
  **
  */
  Wt::WLayout::setDefaultImplementation( Wt::LayoutImplementation::JavaScript );
//  Wt::WLayout::setDefaultImplementation( Wt::LayoutImplementation::Flex );

  try
  {
    Wt::WServer server( argc, argv );

    server.addResource( std::make_shared< HtmlResource  >(), "/html"       );
    server.addResource( std::make_shared< MonitResource >(), "/monittoken" );

    auto apiResource = std::make_shared<ApiResource>();
    server.addResource( apiResource, "/api"  );
    server.addResource( apiResource, "/api2" );

    addEntryPoint< GCW::App >( "gnucashew", server );

#ifdef NEVER
    addEntryPoint< Rtm::EstimateRequest >( "/estimate-request"    , server );
    addEntryPoint< Rtm::AppStaffMobile  >( "/mstaff"              , server );
    addEntryPoint< Rtm::AppStaffMobile  >( "/mstaff2"             , server );
    addEntryPoint< Rtm::AppStaffHandler >( "/handler"             , server ); // handler will handle: bolsubmit, daysheet, photosubmit
    addEntryPoint< Rtm::AppStaffHandler >( "/handler2"            , server );
    addEntryPoint< Rtm::AppStaffHandler >( "/mhandler"            , server );
    addEntryPoint< Rtm::AppStaffHandler >( "/mhandler2"           , server );
    addEntryPoint< Rtm::AppBolSubmit    >( "/bolsubmit"           , server );
    addEntryPoint< Rtm::AppBolSubmit    >( "/bolsubmit2"          , server );

//    addEntryPoint< Rtm::AppDaySheet     >( "/daysheet"            , server ); // a simple app to accept photo uploads of daysheets
//    addEntryPoint< Rtm::AppDaySheet     >( "/daysheet2"           , server );

    addEntryPoint< Rtm::AppStaff        >( "/training"            , server ); // training site (uses same db as staff2)
    addEntryPoint< Rtm::AppStaff        >( "/staff2"              , server ); // primary development site
    addEntryPoint< Rtm::AppStaff        >( "/staff3"              , server ); // secondary development site
    addEntryPoint< Rtm::AppStaff        >( "/beta"                , server ); // beta testing (after staff2,3)
    addEntryPoint< Rtm::AppStaff        >( "/prerelease"          , server ); // live data site, but not released yet
    addEntryPoint< Rtm::AppStaff        >( "/staff"               , server ); // live in-office desktop site
    addEntryPoint< Rtm::AppClient       >( "/client"              , server );
    addEntryPoint< Rtm::AppVault        >( "/vault"               , server );
    addEntryPoint< Rtm::AppTest         >( "/test"                , server );
    addEntryPoint< Rtm::AppFeedback     >( "/feedback"            , server );
//    addEntryPoint< Rtm::AppCalc         >( "/calc",              server );
    addEntryPoint< Rtm::AppStatic       >( "/"                    , server );
#endif

    server.run();
  }
  catch( Wt::WServer::Exception & e )
  {
    std::cerr << e.what() << " TERMINATING TERMINATING TERMINATING" << std::endl;
    return -1;
  }

  return 0;

} // endint main( int argc, char ** argv )


