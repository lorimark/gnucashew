#line 2 "src/Eng/AccountRegisterModel.h"

#ifndef __ENG_ACCOUNTREGISTERMODEL_H___
#define __ENG_ACCOUNTREGISTERMODEL_H___

#include <Wt/WStandardItemModel.h>

#include "../GnuCashew.h"

namespace GCW {
  namespace Eng {

/*!
** \brief Account Register Model
**
** This model is used to present account details to the
**  'editor register' (the book-like-looking thing that
**  the user posts transactions through.
**
*/
class AccountRegisterModel
: public Wt::WStandardItemModel
{
  using RowItem = std::vector< std::unique_ptr< Wt::WStandardItem > >;

  public:

    AccountRegisterModel( const std::string & _accountGuid, bool _editable = true );

    void refreshFromDisk();
    void saveToDisk();

    std::string splitGuid( const Wt::WModelIndex & _index );

    GCW_NUMERIC present    () const { return m_present;    }
    GCW_NUMERIC future     () const { return m_future;     }
    GCW_NUMERIC cleared    () const { return m_cleared;    }
    GCW_NUMERIC reconciled () const { return m_reconciled; }
    GCW_NUMERIC projected  () const { return m_projected;  }

    RowItem makeRow( const std::string & _splitGuid );

    std::set< std::string > suggestionsFromColumn( int _column ) const;

    bool setData( const Wt::WModelIndex & _index, const Wt::cpp17::any & _value, Wt::ItemDataRole _role );

/*
balance
  balance limit
  balance (Period)
  balance usd
  cleared
  cleared usd
  fiture minimum
  future minimum usa
  last num
  last reconcile date
  opening balance
  present
  present usd
  reconciled reconciled usd
  total total period
  total usd
*/

  private:

    void saveToDisk( const Wt::WModelIndex & _index, const Wt::cpp17::any & _value, Wt::ItemDataRole _role );

#ifdef NEVER
    template <typename T>
    bool matchValue( const Wt::cpp17::any & _any1, const Wt::cpp17::any & _any2 )
    {
      auto v1 = Wt::cpp17::any_cast< T >( _any1 );
      auto v2 = Wt::cpp17::any_cast< T >( _any2 );
      return v1 == v2;
    }
#endif

    /*
    ** Send a signal when a index is being updated
    **  because it was dirty.
    **
    */
    Wt::Signal< Wt::WModelIndex > m_goneDirty;

    /*
    ** keep track of rows as they go dirty.
    **
    */
    std::set< int > m_dirtyRows;

    /*
    ** 
    **
    */
    bool m_editable = false;

    /*
    ** The model is associated with a single account.
    **
    */
    std::string m_accountGuid;

    /*
    ** Keep track of the last date entered for pre-populating
    **  the next row.
    **
    */
    std::string m_lastDate;

    /*
    ** Running balance accumulators.
    **
    */
    GCW_NUMERIC m_present    ;
    GCW_NUMERIC m_future     ;
    GCW_NUMERIC m_cleared    ;
    GCW_NUMERIC m_reconciled ;
    GCW_NUMERIC m_projected  ;

}; // endclass AccountRegisterModel

  } //  namespace Eng {
} // endnamespace GCW {


#endif // #ifndef __ENG_REGISTERMODEL_H___



