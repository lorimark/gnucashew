

#ifndef __ACCOUNT_H___
#define __ACCOUNT_H___

#include <Wt/Dbo/Dbo.h>

/*
** Predefine the Account class that fixin to come up.
**
*/
namespace GCW {
  namespace Dbo {
    class Account;
  }
}

/*
** Define these dbo_traits to prevent the system from
**  automatically generating an ID field or a VERSION
**  field, and instead substitute the guid field
**  as the primary key.
**
*/
template<> struct Wt::Dbo::dbo_traits< GCW::Dbo::Account >
: public Wt::Dbo::dbo_default_traits
{
  using IdType = std::string;
  static IdType invalidId() { return std::string(); }
  static const char * surrogateIdField() { return nullptr; }
  static const char * versionField()     { return nullptr; }
};

template<> struct Wt::Dbo::dbo_traits< const GCW::Dbo::Account > : Wt::Dbo::dbo_traits< GCW::Dbo::Account > {};

/*
** Now we can start building our class!
**
*/
namespace GCW {
  namespace Dbo {

class Account
: public Wt::Dbo::Dbo< Account >
{
  public:

    using Ptr = Wt::Dbo::ptr< Account >;
    using Collection = Wt::Dbo::collection< Ptr >;
    using Vector = std::vector< Ptr >;

    Account() {};

    template< class Action > void persist( Action & action )
    {
      Wt::Dbo::id   ( action, m_guid           , "guid"           ,   32 ); // text(32) PRIMARY KEY NOT NULL
      Wt::Dbo::field( action, m_name           , "name"           , 2048 ); // text(2048) NOT NULL
      Wt::Dbo::field( action, m_account_type   , "account_type"   , 2048 ); // text(2048) NOT NULL
      Wt::Dbo::field( action, m_commodity_guid , "commodity_guid" ,   32 ); // text(32)
      Wt::Dbo::field( action, m_commodity_scu  , "commodity_scu"         ); // integer NOT NULL
      Wt::Dbo::field( action, m_non_std_scu    , "non_std_scu"           ); // integer NOT NULL
      Wt::Dbo::field( action, m_parent_guid    , "parent_guid"    ,   32 ); // text(32)
      Wt::Dbo::field( action, m_code           , "code"           , 2048 ); // text(2048)
      Wt::Dbo::field( action, m_description    , "description"    , 2048 ); // text(2048)
      Wt::Dbo::field( action, m_hidden         , "hidden"                ); // integer
      Wt::Dbo::field( action, m_placeholder    , "placeholder"           ); // integer
    }

    std::string m_guid           ;
    std::string m_name           ;
    std::string m_account_type   ;
    std::string m_commodity_guid ;
    int         m_commodity_scu  ;
    int         m_non_std_scu    ;
    std::string m_parent_guid    ;
    std::string m_code           ;
    std::string m_description    ;
    int         m_hidden         ;
    int         m_placeholder    ;

}; // endclass Account

  } // endnamespace Dbo {
} // endnamespace GCW {

#endif // end#ifndef __ACCOUNT_H___

