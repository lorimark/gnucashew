

#ifndef __BASEITEM_H___
#define __BASEITEM_H___

#include <Wt/Dbo/Dbo.h>

namespace GCW {
  namespace Dbo {

class BaseItem
: public Wt::Dbo::Dbo< BaseItem >
{
  public:

    using Ptr = Wt::Dbo::ptr< Account >;
    using Collection = Wt::Dbo::collection< Ptr >;
    using Vector = std::vector< Ptr >;

    BaseItem() {}

};


  }
}

#endif // end#ifndef __BASEITEM_H___


