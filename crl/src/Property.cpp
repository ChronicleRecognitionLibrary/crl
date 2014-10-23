/** ***********************************************************************************
 * \file Property.cpp
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

// ----------------------------------------------------------------------------
// INCLUDE FILES
// ----------------------------------------------------------------------------

#include "Property.h"


// ----------------------------------------------------------------------------
// property CLASS METHODS
// ----------------------------------------------------------------------------

namespace CRL
{

  /** Allocation operator
  *   \param[in] x right value of the operator
  *   \result left value, property which value is modified
  */
  Property& Property::operator=(const bool& x)  
  {
    val.b    = x;
    dataType = B;
    return *this;
  }

  /** Allocation operator
  *   \param[in] x right value of the operator
  *   \result left value, property which value is modified
  */
  Property& Property::operator=(const char& x)  
  {
    val.ch   = x;
    dataType = CH;
    return *this;
  }

  /** Allocation operator
  *   \param[in] x right value of the operator
  *   \result left value, property which value is modified
  */
  Property& Property::operator=(const wchar_t& x)  
  {
    val.wch  = x;
    dataType = WCH;
    return *this;
  }

  /** Allocation operator
  *   \param[in] x right value of the operator
  *   \result left value, property which value is modified
  */
  Property& Property::operator=(const int& x)  
  {
    val.i    = x;
    dataType = I;
    return *this;
  }

  /** Allocation operator
  *   \param[in] x right value of the operator
  *   \result left value, property which value is modified
  */
  Property& Property::operator=(const unsigned int& x)  
  {
    val.ui   = x;
    dataType = UI;
    return *this;
  }

  /** Allocation operator
  *   \param[in] x right value of the operator
  *   \result left value, property which value is modified
  */
  Property& Property::operator=(const long& x)  
  {
    val.l    = x;
    dataType = L;
    return *this;
  }

  /** Allocation operator
  *   \param[in] x right value of the operator
  *   \result left value, property which value is modified
  */
  Property& Property::operator=(const unsigned long& x)  
  {
    val.ul   = x;
    dataType = UL;
    return *this;
  }

  /** Allocation operator
  *   \param[in] x right value of the operator
  *   \result left value, property which value is modified
  */
  Property& Property::operator=(const float& x)  
  {
    val.f    = x;
    dataType = F;
    return *this;
  }

  /** Allocation operator
  *   \param[in] x right value of the operator
  *   \result left value, property which value is modified
  */
  Property& Property::operator=(const double& x)  
  {
    val.d    = x;
    dataType = D;
    return *this;
  }

  /** Allocation operator
  *   \param[in] x right value of the operator
  *   \result left value, property which value is modified
  */
  Property& Property::operator=(const std::string& x)  
  {
    str      = x;
    dataType = STR;
    return *this;
  }

  /** Allocation operator
  *   \param[in] x right value of the operator
  *   \result left value, property which value is modified
  */
  Property& Property::operator=(const std::wstring& x)  
  {
    wstr     = x;
    dataType = WSTR;
    return *this;
  }

  /** Allocation operator
  *   \param[in] x right value of the operator
  *   \result left value, property which value is modified
  */
  Property& Property::operator=(char const * const x)  
  {
    str      = x;
    dataType = STR;
    return *this;
  }

  /** Allocation operator
  *   \param[in] x right value of the operator
  *   \result left value, property which value is modified
  */
  Property& Property::operator=(wchar_t const * const x)  
  {
    wstr     = x;
    dataType = WSTR;
    return *this;
  }


  /**  Casting operator to type bool. Raises an exception
  *   if the property is not of this type.
  */
  Property::operator bool() const 
  {
    if (dataType == B)
      return val.b;
    throw("bool type error");
  }

  /**  Casting operator to type char. Raises an exception
  *   if the property is not of this type.
  */
  Property::operator char() const 
  {
    if (dataType == CH)
      return val.ch;
    throw("char type error");
  }

  /**  Casting operator to type wchar_t. Raises an exception
  *   if the property is not of this type.
  */
  Property::operator wchar_t() const 
  {
    if (dataType == WCH)
      return val.wch;
    throw("wchar_t type error");
  }

  /**  Casting operator to type int. Raises an exception
  *   if the property is not of this type.
  */
  Property::operator int() const 
  {
    if (dataType == I)
      return val.i;
    throw("int type error");
  }

  /**  Casting operator to type unsigned int. Raises an exception
  *   if the property is not of this type.
  */
  Property::operator unsigned int() const 
  {
    if (dataType == UI)
      return val.ui;
    throw("unsigned int type error");
  }

   /**  Casting operator to type long. Raises an exception
  *   if the property is not of this type.
  */
  Property::operator long() const 
  {
    if (dataType == L)
      return val.l;
    throw("long type error");
  }

  /**  Casting operator to type unsigned long. Raises an exception
  *   if the property is not of this type.
  */
  Property::operator unsigned long() const 
  {
    if (dataType == UL)
      return val.ul;
    throw("unsigned long type error");
  }

  /**  Casting operator to type float. Raises an exception
  *   if the property is not of this type.
  */
  Property::operator float() const 
  {
    if (dataType == F)
      return val.f;
    throw("float type error");
  }

  /**  Casting operator to type double. Raises an exception
  *   if the property is not of this type.
  */
  Property::operator double() const 
  {
    if (dataType == D)
      return val.d;
    throw("double type error");
  }

  /**  Casting operator to type std::string. Raises an exception
  *   if the property is not of this type.
  */
  Property::operator std::string() const 
  {
    if (dataType == STR)
      return str;
    throw("std::string type error");
  }

  /**  Casting operator to type std::wstring. Raises an exception
  *   if the property is not of this type.
  */
  Property::operator std::wstring() const 
  {
    if (dataType == WSTR)
      return wstr;
    throw("std::wstring type error");
  }

} /* namespace CRL */

