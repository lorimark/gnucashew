
#ifndef __CONFIG_H___
#define __CONFIG_H___

namespace GCW {

/*!
** \brief Application Configuration
**
** The Application Configuration is controlled through this single object.
**  The configuration includes multiples 'areas' followed by specific settings
**  in each area.  The areas included are;
**
**  Global - stored for all applications
**  App    - stored for the application specifically
**  User   - stored for the currently logged in user
**
*/
class Config
{
  public:

    Config();


};

} // endnamespace GCW {

#endif // end#ifndef __CONFIG_H___


