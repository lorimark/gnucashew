#line 2 "src/Dbo/SessionGnucash.h"

#ifndef __SESSIONGNUCASH_H___
#define __SESSIONGNUCASH_H___

#include "AbstractSession.h"

namespace GCW {
  namespace Dbo {
    namespace GnuCash {

/*!
** \brief GnuCash Native Session
**
** This session controller is designed to work through the
**  libgnucash back-end library.
**
*/
class Session
: public GCW::Dbo::AbstractSession
{
  public:

    Session() {}
    ~Session();

    /*!
    ** \brief Open a database
    **
    */
    bool open( const std::string & _path );

  private:

    void closeGnuCash();

    void init();

}; // endclass Session

} // endnamespace GnuCash {


  } // endnamespace Dbo {
} // endnamespace GCW {

#endif // end#ifndef __SESSIONGNUCASH_H___


