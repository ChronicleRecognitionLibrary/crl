/** ***********************************************************************************
 * \file Property.h
 * \author Ariane Piel & Jean Bourrely / Onera DCPS
 * \date 2012
 * \brief Base virtual class of properties (of events, of recognitions, ...)
 **************************************************************************************/

/*  Copyright (C) 2012, 2013, 2014  ONERA – http://www.onera.fr
    This file is part of CRL : Chronicle Recognition Library.

    CRL is free software: you can redistribute it and/or modify it under
    the terms of the Lesser GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    CRL is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    Lesser GNU General Public License for more details.

    You should have received a copy of the Lesser GNU General Public License
    along with CRL.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef PROPERTY_H_
#define PROPERTY_H_

// ----------------------------------------------------------------------------
// INCLUDE FILES
// ----------------------------------------------------------------------------

#include <list>
#include <string>
#include <iostream>

#include "PropertyManager.h"


// ----------------------------------------------------------------------------
// CLASS DESCRIPTION
// ----------------------------------------------------------------------------

namespace CRL {

  class Property : public PropertyManager
  {
  public:

    //! Internal type : enumeration of the possible types for a property
    enum DataTypeEnum {    B,  //!< bool
                          CH,  //!< char
                         WCH,  //!< whar_t
                           I,  //!< int
                          UI,  //!< unsigned int
                           L,  //!< long
                          UL,  //!< unsigned long
                           F,  //!< float
                           D,  //!< double
                         STR,  //!< std::string
                        WSTR,  //!< std::wstring
                        NONE   //!< None
    };

    //! Internal type : type union for simple types
    union SimpleType
    {
      bool             b;
      char             ch;
      wchar_t          wch;
      int              i;
      unsigned int     ui;
      long             l;
      unsigned long    ul;
      float            f;
      double           d;
    };

    //! Type definition; property list
    //typedef PropertyManager List;

  private:

    //! Contained data type
    DataTypeEnum dataType;

    //! Simple type value (bool, int, long, float, ...)
    SimpleType val;

    //! ASCII string type value
    std::string str;

    //! Extended string type value
    std::wstring wstr;

  public:

    //! Default constructor
    Property() : dataType(NONE) { }

    //! Destructor
    ~Property() { }

    //! Allocation operators from simple types
    Property& operator=(const bool& x);
    Property& operator=(const char& x);
    Property& operator=(const wchar_t& x);
    Property& operator=(const int& x);
    Property& operator=(const unsigned int& x);
    Property& operator=(const long& x);
    Property& operator=(const unsigned long& x);
    Property& operator=(const float& x);
    Property& operator=(const double& x);
    Property& operator=(const std::string& x);
    Property& operator=(const std::wstring& x);
    Property& operator=(char const * const x);
    Property& operator=(wchar_t const * const x);

    //! Casting operators to simple types
    operator bool() const;
    operator char() const;
    operator wchar_t() const;
    operator int() const;
    operator unsigned int() const;
    operator long() const;
    operator unsigned long() const;
    operator float() const;
    operator double() const;
    operator std::string() const;
    operator std::wstring() const;

    //! returns true if the type corresponds
    bool isBool() const { return (dataType == B); }
    bool isChar() const { return (dataType == CH); }
    bool isWchar_t() const { return (dataType == WCH); }
    bool isInt() const { return (dataType == I); }
    bool isUnsignedInt() const { return (dataType == UI); }
    bool isLong() const { return (dataType == L); }
    bool isUnsignedLong() const { return (dataType == UL); }
    bool isFloat() const { return (dataType == F); }
    bool isDouble() const { return (dataType == D); }
    bool isString() const { return (dataType == STR); }
    bool isWstring() const { return (dataType == WSTR); }

  }; // class Property

} /* namespace CRL */

#endif /* PROPERTY_H_ */
