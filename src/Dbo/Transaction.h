

#ifndef __TRANSACTION_H___
#define __TRANSACTION_H___

#include <Wt/Dbo/Dbo.h>

/*
** Predefine the Account class that fixin to come up.
**
*/
namespace GCW {
  namespace Dbo {
    namespace Transaction {
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
template<> struct Wt::Dbo::dbo_traits< GCW::Dbo::Transaction::Item >
: public Wt::Dbo::dbo_default_traits
{
  using IdType = std::string;
  static IdType invalidId() { return std::string(); }
  static const char * surrogateIdField() { return nullptr; }
  static const char * versionField()     { return nullptr; }
};

template<> struct Wt::Dbo::dbo_traits< const GCW::Dbo::Transaction::Item > : Wt::Dbo::dbo_traits< GCW::Dbo::Transaction::Item > {};

/*
** Now we can start building our class!
**
*/
namespace GCW {
  namespace Dbo {
    namespace Transaction {


/*!
** \brief Transaction Item
**
**
** \code
**  CREATE TABLE transactions
**  (
**   guid text(32) PRIMARY KEY NOT NULL,
**   currency_guid text(32) NOT NULL,
**   num text(2048) NOT NULL,
**   post_date text(19),
**   enter_date text(19),
**   description text(2048)
**  );
**  CREATE INDEX tx_post_date_index ON transactions(post_date);
** \endcode
**
*/
class Item
: public Wt::Dbo::Dbo< Item >
{
  public:

    using Ptr = Wt::Dbo::ptr< Item >;
    using Collection = Wt::Dbo::collection< Ptr >;
    using Vector = std::vector< Ptr >;

    Item() {};

    const std::string & guid() const { return m_guid; }

    template< class Action > void persist( Action & action )
    {
      Wt::Dbo::id   ( action, m_guid           , "guid"           ,   32 ); // text(32) PRIMARY KEY NOT NULL,
      Wt::Dbo::field( action, m_currency_guid  , "currency_guid"  ,   32 ); // text(32) NOT NULL,
      Wt::Dbo::field( action, m_num            , "num"            , 2048 ); // text(2048) NOT NULL,
      Wt::Dbo::field( action, m_post_date      , "post_date"      ,   19 ); // text(19),
      Wt::Dbo::field( action, m_enter_date     , "enter_date"     ,   19 ); // text(19),
      Wt::Dbo::field( action, m_description    , "description"    , 2048 ); // text(2048)
    }

    std::string m_guid           ;
    std::string m_currency_guid  ;
    std::string m_num            ;
    std::string m_post_date      ;
    std::string m_enter_date     ;
    std::string m_description    ;

}; // endclass Item

/*!
** \brief Load Transaction by Guid
**
*/
Item::Ptr byGuid( const std::string & _guid );

    } // endnamespace Transaction {
  } // endnamespace Dbo {
} // endnamespace GCW {

#endif // end#ifndef __TRANSACTION_H___


