

#ifndef __BASEITEM_H___
#define __BASEITEM_H___

#include <Wt/Dbo/Dbo.h>

namespace GCW {
  namespace Dbo {

template <class C> class BaseItem
: public Wt::Dbo::Dbo< C >
{
  public:

    using Ptr = Wt::Dbo::ptr< C >;
    using Collection = Wt::Dbo::collection< Ptr >;
    using Vector = std::vector< Ptr >;

    BaseItem():Wt::Dbo::Dbo<C>() {}

};


  }
}

#endif // end#ifndef __BASEITEM_H___


