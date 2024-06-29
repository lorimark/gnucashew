#line 2 "src/Dbo/Slots.h"

#ifndef __DBO_SLOTS_H___
#define __DBO_SLOTS_H___

#include "BaseItem.h"
#include "Slot.h"

/*
** Predefine the class that fixin to come up.
**
*/
namespace GCW {
  namespace Dbo {
    namespace Slots {
      class Item;
    }
  }
}

/*
** Define these dbo_traits to prevent the system from
**  automatically generating an ID field or a VERSION
**  field, and instead substitute the guid field
**  as the primary key.
**
*/
template<> struct Wt::Dbo::dbo_traits< GCW::Dbo::Slots::Item >
: public Wt::Dbo::dbo_default_traits
{
  using IdType = long long;
  static IdType invalidId() { return -1; }
  static const char * surrpgateIdField() { return nullptr; }
  static const char * versionField()     { return nullptr; }
};

template<> struct Wt::Dbo::dbo_traits< const GCW::Dbo::Slots::Item > : Wt::Dbo::dbo_traits< GCW::Dbo::Slots::Item > {};

/*
** Now we can start building our class!
**
*/
namespace GCW {
  namespace Dbo {
    namespace Slots {

/*!
** \brief Variables Item Class
**
**
*/
class Item
: public GCW::Dbo::BaseItem< Item >
{
  public:

    const std::string & obj_guid          () const { return m_obj_guid          ; } // text(32) NOT NULL
    const std::string & name              () const { return m_name              ; } // text(4096) NOT NULL
          int           slot_type         () const { return m_slot_type         ; } // integer NOT NULL
          long long     int64_val         () const { return m_int64_val         ; } // bigint
    const std::string & string_val        () const { return m_string_val        ; } // text(4096),
          double        double_val        () const { return m_double_val        ; } // float8,
    const std::string & timespec_val      () const { return m_timespec_val      ; } // text(19),
    const std::string & guid_val          () const { return m_guid_val          ; } // text(32),
          long long     numeric_val_num   () const { return m_numeric_val_num   ; } // bigint,
          long long     numeric_val_denom () const { return m_numeric_val_denom ; } // bigint,
    const std::string & gdate_val         () const { return m_gdate_val         ; } // text(8)

    template< class Action > void persist( Action & action )
    {
//      Wt::Dbo::field( action, m_id                , GCW::Dbo::Slot::Field:: id                       ); // integer PRIMARY KEY AUTOINCREMENT NOT NULL,
      Wt::Dbo::field( action, m_obj_guid          , GCW::Dbo::Slot::Field:: obj_guid          ,   32 ); // text(32) NOT NULL,
      Wt::Dbo::field( action, m_name              , GCW::Dbo::Slot::Field:: name              , 4096 ); // text(4096) NOT NULL,
      Wt::Dbo::field( action, m_slot_type         , GCW::Dbo::Slot::Field:: slot_type                ); // integer NOT NULL,
      Wt::Dbo::field( action, m_int64_val         , GCW::Dbo::Slot::Field:: int64_val                ); // bigint,
      Wt::Dbo::field( action, m_string_val        , GCW::Dbo::Slot::Field:: string_val        , 4096 ); // text(4096),
      Wt::Dbo::field( action, m_double_val        , GCW::Dbo::Slot::Field:: double_val               ); // float8,
      Wt::Dbo::field( action, m_timespec_val      , GCW::Dbo::Slot::Field:: timespec_val      ,   19 ); // text(19),
      Wt::Dbo::field( action, m_guid_val          , GCW::Dbo::Slot::Field:: guid_val          ,   32 ); // text(32),
      Wt::Dbo::field( action, m_numeric_val_num   , GCW::Dbo::Slot::Field:: numeric_val_num          ); // bigint,
      Wt::Dbo::field( action, m_numeric_val_denom , GCW::Dbo::Slot::Field:: numeric_val_denom        ); // bigint,
      Wt::Dbo::field( action, m_gdate_val         , GCW::Dbo::Slot::Field:: gdate_val         ,   32 ); // text(8)
    }

  private:

//    long long   m_id                ; // integer PRIMARY KEY AUTOINCREMENT NOT NULL,
    std::string m_obj_guid          ; // text(32) NOT NULL
    std::string m_name              ; // text(4096) NOT NULL
    int         m_slot_type         ; // integer NOT NULL
    long long   m_int64_val         ; // bigint
    std::string m_string_val        ; // text(4096),
    double      m_double_val        ; // float8,
    std::string m_timespec_val      ; // text(19),
    std::string m_guid_val          ; // text(32),
    long long   m_numeric_val_num   ; // bigint,
    long long   m_numeric_val_denom ; // bigint,
    std::string m_gdate_val         ; // text(8)

}; // endclass Item

GCW::Dbo::Slots::Item::Vector get();
GCW::Dbo::Slots::Item::Ptr get( const std::string & _guid, const std::string & _name );

extern const char * s_tableName;

    } // endnamespace Slots {
  } // endnamespace Dbo {
} // endnamespace GCW {

#endif // end#ifndef __SLOTS_H___

