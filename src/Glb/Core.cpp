#line 2 "src/Glb/Core.cpp"

#include <sstream>
#include <algorithm>

#include <Wt/WModelIndex.h>

#include "Core.h"

static
std::vector<std::string> &
split( const std::string & s, char delim, std::vector<std::string> & elems )
{
  std::stringstream ss( s );
  std::string item;
  while( std::getline( ss, item, delim ) )
  {
    elems.push_back( item );
  }
  return elems;
}


std::vector<std::string>
Wtx::Core::
split( const std::string & s, char delim )
{
  std::vector<std::string> elems;
  ::split( s, delim, elems );
  return elems;
}

const char* Wtx::Core::trim_ws = " \t\n\r\f\v";

// trim from end of string (right)
std::string &
Wtx::Core::
rtrim( std::string & s, const char* t )
{
  s.erase( s.find_last_not_of(t) + 1 );
  return s;
}

// trim from beginning of string (left)
std::string &
Wtx::Core::
ltrim( std::string & s, const char* t )
{
  s.erase( 0, s.find_first_not_of(t) );
  return s;
}

// trim from both ends of string (left & right)
std::string &
Wtx::Core::
trim( std::string & s, const char* t )
{
  return ltrim( rtrim(s, t), t );
}

std::string
Wtx::Core::
toupper( const std::string & s )
{
  std::string retVal = s;

  std::transform( retVal.begin(), retVal.end(), retVal.begin(), ::toupper );

  return retVal;
}

std::string
Wtx::Core::
tolower( const std::string & s )
{
  std::string retVal = s;

  std::transform( retVal.begin(), retVal.end(), retVal.begin(), ::tolower );

  return retVal;
}

/*
** This will iterate a single a WTreeView and fill
**  a vector of every node which is the .last-expanded.
**  node of every branch.
**
*/
static
bool
iterate( Wt::Json::Array & _jary, Wt::WModelIndex _parent )
{
#ifdef NEVER
  /*
  ** If this _parent node is not expanded, then we're basically done.
  **
  */
  if( !view()-> isExpanded( _parent ) )
    return false;

  /*
  ** This _parent node is expanded, so loop through all the
  **  child nodes checking if any of them are expanded.
  **
  */
  bool expanded = false;
  for( int row=0; row< view()-> model()-> rowCount( _parent ); row++ )
    expanded |= iterate( _jary, view()-> model()-> index( row, 0, _parent ) );

  /*
  ** None of the child nodes are expanded, so record this _parent
  **  node as the 'last' node in the tree
  **
  */
  if( !expanded )
  {
    /*
    ** The true root node is not associated with an actual account,
    **  it is simply the invisibleRoot of the tree itself, and only
    **  contains the set of first-root nodes that actually get
    **  displayed.  So, there is no User data in this one, don't record it.
    **
    */
    auto accountGuid = Wt::asString( _parent.data( Wt::ItemDataRole::User ) );
    if( accountGuid != "" )
      _jary.push_back( accountGuid );

  } // endif( !expanded )

  /*
  ** Something is expanded.  Either we are expanded, or
  **  one of the sub-nodes are expanded, so return that 'someone' is
  **  expanded.
  **
  */
  return true;
#endif
} // endvoid iterate( Wt::WModelIndex _index ) const

Wt::Json::Object
Wtx::Core::
toJson( Wt::WTreeView * _view )
{
  Wt::Json::Object jobj;

#ifdef NEVER
  jobj["selected"] = Wt::WString( selectedAccount() );

  for( int col=0; col< 7; col++ )
    jobj[ Wt::WString("cw{1}").arg( col ).toUTF8() ] = Wt::WString( view()-> columnWidth( col ).cssText() );

  Wt::Json::Array jary;
  iterate( jary );
  jobj["expanded"] = jary;

#endif

  return jobj;

}

