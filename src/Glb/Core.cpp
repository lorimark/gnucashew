
#include <sstream>
#include <algorithm>

#include "Core.h"

static std::vector<std::string> & split( const std::string & s, char delim, std::vector<std::string> & elems )
{
  std::stringstream ss( s );
  std::string item;
  while( std::getline( ss, item, delim ) )
  {
    elems.push_back( item );
  }
  return elems;
}


std::vector<std::string> Wtx::Core::split( const std::string & s, char delim )
{
  std::vector<std::string> elems;
  ::split( s, delim, elems );
  return elems;
}

const char* Wtx::Core::trim_ws = " \t\n\r\f\v";

// trim from end of string (right)
std::string & Wtx::Core::rtrim( std::string & s, const char* t )
{
  s.erase( s.find_last_not_of(t) + 1 );
  return s;
}

// trim from beginning of string (left)
std::string & Wtx::Core::ltrim( std::string & s, const char* t )
{
  s.erase( 0, s.find_first_not_of(t) );
  return s;
}

// trim from both ends of string (left & right)
std::string & Wtx::Core::trim( std::string & s, const char* t )
{
  return ltrim( rtrim(s, t), t );
}

std::string Wtx::Core::toupper( const std::string & s )
{
  std::string retVal = s;

  std::transform( retVal.begin(), retVal.end(), retVal.begin(), ::toupper );

  return retVal;
}

std::string Wtx::Core::tolower( const std::string & s )
{
  std::string retVal = s;

  std::transform( retVal.begin(), retVal.end(), retVal.begin(), ::tolower );

  return retVal;
}


