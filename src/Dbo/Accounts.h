#line 2 "src/Dbo/Accounts.h"

#ifndef __ACCOUNTS_H___
#define __ACCOUNTS_H___

#include "BaseItem.h"
#include "Account.h"

/*
** Predefine the Account class that fixin to come up.
**
*/
namespace GCW {
  namespace Dbo {
    namespace Accounts {
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
template<> struct Wt::Dbo::dbo_traits< GCW::Dbo::Accounts::Item >
: public Wt::Dbo::dbo_default_traits
{
  using IdType = std::string;
  static IdType invalidId() { return std::string(); }
  static const char * surrogateIdField() { return nullptr; }
  static const char * versionField()     { return nullptr; }
};

template<> struct Wt::Dbo::dbo_traits< const GCW::Dbo::Accounts::Item > : Wt::Dbo::dbo_traits< GCW::Dbo::Accounts::Item > {};

/*
** Now we can start building our class!
**
*/
namespace GCW {
  namespace Dbo {
    namespace Accounts {

/*!
** \brief Account Item Class
**
** This class represents an 'account' within gnucash.  This is a Dbo object
**  so this class is used as the interface between the gnucashew application
**  and the back-end database.
**
** \par Native GnuCash Account Sqlite Schema
** \code
**  CREATE TABLE accounts
**  (
**   guid           text(32) PRIMARY KEY NOT NULL,
**   name           text(2048) NOT NULL,
**   account_type   text(2048) NOT NULL,
**   commodity_guid text(32),
**   commodity_scu  integer NOT NULL,
**   non_std_scu    integer NOT NULL,
**   parent_guid    text(32),
**   code           text(2048),
**   description    text(2048),
**   hidden         integer,
**   placeholder    integer
**  );
**  sqlite> select * from accounts;
**  guid                             name         type commodity                        scu 
**  aa283385e0cf4f57b3360ca5a843bde5|Root Account|ROOT|10b24d11b4b94b8789d1830da2695bbb|100|0||||0|0
**  6e5313b77b4247039f0240ca79e4d871|Assets|ASSET|10b24d11b4b94b8789d1830da2695bbb|100|0|aa283385e0cf4f57b3360ca5a843bde5||Assets|0|1
**  b61b07c024fc463489f5db031135a29e|Current Assets|ASSET|10b24d11b4b94b8789d1830da2695bbb|100|0|6e5313b77b4247039f0240ca79e4d871||Current Assets|0|1
**  822a857c5f484affa5a6a3e62f4b700f|Checking Account|BANK|10b24d11b4b94b8789d1830da2695bbb|100|0|b61b07c024fc463489f5db031135a29e||Checking Account|0|0
**  9e851f524a6a44ef8c93a6b52b004cae|Savings Account|BANK|10b24d11b4b94b8789d1830da2695bbb|100|0|b61b07c024fc463489f5db031135a29e||Savings Account|0|0
** \endcode
**
*/
class Item
: public GCW::Dbo::BaseItem< Item >
{
  public:

    Item(){}
    Item( const std::string & _guid ): m_guid( _guid ){}

    const GCW::Dbo::Account::AccountDef_t & accountDef() const;

    GCW::Dbo::Account::DrCr         accountDrCr() const;
    GCW::Dbo::Account::Type         accountType() const;

    const std::string & guid            () const { return m_guid             ; }
    const std::string & name            () const { return m_name             ; }
    const std::string & accountTypeName () const { return m_account_typeName ; }
    const std::string & commodity_guid  () const { return m_commodity_guid   ; }
    const int           commodity_scu   () const { return m_commodity_scu    ; }
    const std::string & parent_guid     () const { return m_parent_guid      ; }
    const std::string & code            () const { return m_code             ; }
    const std::string & description     () const { return m_description      ; }
    const int           hidden          () const { return m_hidden           ; }
    const int           placeHolder     () const { return m_placeHolder      ; }

    bool hasColor() const;
    std::string color() const;

    template< class Action > void persist( Action & action )
    {
      Wt::Dbo::id   ( action, m_guid             , GCW::Dbo::Account::Field::guid             ,   32 ); // text(32) PRIMARY KEY NOT NULL
      Wt::Dbo::field( action, m_name             , GCW::Dbo::Account::Field::name             , 2048 ); // text(2048) NOT NULL
      Wt::Dbo::field( action, m_account_typeName , GCW::Dbo::Account::Field::account_typeName , 2048 ); // text(2048) NOT NULL
      Wt::Dbo::field( action, m_commodity_guid   , GCW::Dbo::Account::Field::commodity_guid   ,   32 ); // text(32)
      Wt::Dbo::field( action, m_commodity_scu    , GCW::Dbo::Account::Field::commodity_scu           ); // integer NOT NULL
      Wt::Dbo::field( action, m_non_std_scu      , GCW::Dbo::Account::Field::non_std_scu             ); // integer NOT NULL
      Wt::Dbo::field( action, m_parent_guid      , GCW::Dbo::Account::Field::parent_guid      ,   32 ); // text(32)
      Wt::Dbo::field( action, m_code             , GCW::Dbo::Account::Field::code             , 2048 ); // text(2048)
      Wt::Dbo::field( action, m_description      , GCW::Dbo::Account::Field::description      , 2048 ); // text(2048)
      Wt::Dbo::field( action, m_hidden           , GCW::Dbo::Account::Field::hidden                  ); // integer
      Wt::Dbo::field( action, m_placeHolder      , GCW::Dbo::Account::Field::placeHolder             ); // integer
    }

  private:

    std::string m_guid             ;
    std::string m_name             ;
    std::string m_account_typeName ;
    std::string m_commodity_guid   ;
    int         m_commodity_scu    ;
    int         m_non_std_scu      ;
    std::string m_parent_guid      ;
    std::string m_code             ;
    std::string m_description      ;
    int         m_hidden           ;
    int         m_placeHolder      ;

}; // endclass Item

extern const char * s_tableName;

/*!
** \brief Load Root Account
**
**
*/
Item::Ptr root();

/*!
** \brief Load Account by Guid
**
*/
Item::Ptr load( const std::string & _guid );
Item::Ptr byGuid( const std::string & _guid );
Item::Ptr byChildName( const std::string & _parentGuid, const std::string & _childName );
Item::Ptr byFullName( const std::string & _fullName );

std::string fullName( const std::string & _guid );
std::string fullName( Item::Ptr _item );

Item::Vector allAccounts();
Item::Vector activeAccounts();

namespace Children {
  Item::Vector vector( const std::string & _parentGuid );
}

    } // endnamespace Accounts {
  } // endnamespace Dbo {
} // endnamespace GCW {

#endif // end#ifndef __ACCOUNTS_H___

