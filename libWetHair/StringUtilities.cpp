//
// This file is part of the libWetHair open source project
//
// Copyright 2017 Yun (Raymond) Fei, Henrique Teles Maia, Christopher Batty,
// Changxi Zheng, and Eitan Grinspun
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.


#include "StringUtilities.h"

namespace outputmod
{

std::ostream& startred( std::ostream& stream )
{
#ifndef __APPLE__
  stream << "\033[31;1m";
#endif
  return stream;
}

std::ostream& endred( std::ostream& stream )
{
#ifndef __APPLE__
  stream << "\033[m";
#endif
  return stream;
}

std::ostream& startgreen( std::ostream& stream )
{
#ifndef __APPLE__
  stream << "\033[32;1m";
#endif
  return stream;
}

std::ostream& endgreen( std::ostream& stream )
{
#ifndef __APPLE__
  stream << "\033[m";
#endif
  return stream;
}

std::ostream& startpink( std::ostream& stream )
{
#ifndef __APPLE__
  stream << "\033[35;1m";
#endif
  return stream;
}

std::ostream& endpink( std::ostream& stream )
{
#ifndef __APPLE__
  stream << "\033[m";
#endif
  return stream;
}

std::ostream& startblue( std::ostream& stream )
{
#ifndef __APPLE__
  stream << "\033[34;1m";
#endif
  return stream;
}

std::ostream& endblue( std::ostream& stream )
{
#ifndef __APPLE__
  stream << "\033[m";
#endif
  return stream;
}

}

namespace stringutils {

  std::ostream& operator<<(std::ostream &os, const TripletXs &info)
  {
    for(size_t i = 0; i < info.size(); ++i)
      os << "[" << info[i].col() << ", " << info[i].row() << ", " << info[i].value() << "]" << std::endl;
    return os;
  }
  
  std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
      elems.push_back(item);
    }
    return elems;
  }
  
  std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
  }

  void tokenize( const std::string& str, const char chr, std::vector<std::string>& tokens )
  {
    std::string::size_type substring_start = 0;
    std::string::size_type substring_end = str.find_first_of( chr, substring_start );
    while( substring_end != std::string::npos )
    {
      tokens.emplace_back( str.substr( substring_start, substring_end - substring_start ) );
      substring_start = substring_end + 1;
      substring_end = str.find_first_of( chr, substring_start );
    }
    // Grab the trailing substring, if present
    if( substring_start < str.size() )
    {
      tokens.emplace_back( str.substr( substring_start ) );
    }
    // Case of final character the delimiter
    if( str.back() == chr )
    {
      tokens.emplace_back( "" );
    }
  }

  std::vector<std::string> tokenize( const std::string& str, const char delimiter )
  {
    std::vector<std::string> tokens;
    tokenize( str, delimiter, tokens );
    return tokens;
  }  
  
}
