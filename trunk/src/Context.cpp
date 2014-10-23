/** ***********************************************************************************
 * \file Context.cpp
 * \author Ariane Piel & Jean Bourrely / Onera DCPS
 * \date 2012
 * \brief Context of a chroinicle
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

#include "Utils.h"
#include "Context.h"


// ----------------------------------------------------------------------------
// CLASS METHODS
// ----------------------------------------------------------------------------

namespace CRL 
{

  //! Definition of the class variable
  std::string Context::ANONYMOUS_CODE_NAME = " #ANONYMOUS";


  /** Creates an empty context or a context reduced to \bot.
  *   \param[in] withAnonymous is true if the context is to be reduced to \bot
  */
  Context::Context(bool withAnonymous)
  { 
    if (withAnonymous)
      data.insert(ANONYMOUS_CODE_NAME);
  }


  /** Copies a context, while possibly omitting \bot
  *   \param[in] withAnonymous is true if everything, including \bot, has to be copied
  */
  Context::Context(const Context& c, bool withAnonymous)
    : data(c.data)
  {
    if (withAnonymous == false)
      data.erase(Context::ANONYMOUS_CODE_NAME);
  }


  /** \param[in] s event name to be added to the context
  */
  void Context::add(const std::string& s, bool nothrow) 
  {
    std::pair<std::set<std::string>::iterator, bool> res = data.insert(s);
    if ( (nothrow == false) && (res.second == false) )
      throw(std::string("Impossible to add element ")+s+std::string(" in context"));
  }


  /** \param[in] s event name to be deleted from the context
  */
  void Context::remove(const std::string& s)
  {
    data.erase(s);
  }


  /**  Deletes all the elements, except possibly element ANONYMOUS.
  *   \param[in] exceptAnonymous false indicates that everything has to be deleted
  */
  void Context::clear(bool exceptAnonymous) 
  {
    bool flag = ( (exceptAnonymous)&&(data.count(Context::ANONYMOUS()) == 1) );
    data.clear();
    if (flag)
      data.insert(ANONYMOUS_CODE_NAME);
  }


  /** \result Number of elements (including \bot) of the context
  */
  int Context::size() const
  {
    return data.size();
  }


  /** Raises an exception if the limits [0, size-1] are exceeded
  *   \result constant reference
  */
  const std::string& Context::operator[](int i) const
  {
    if ( (i < 0) || (i >= (int)data.size()) )
      throw("Context::operator[] : outside bounds");

    std::set<std::string>::const_iterator it = data.begin();
    for(int n=0; n<i; n++)
      it++;
    return *it;
  }


  /** Raises an exception if the limits [0, size-1] are exceeded
  *   \result reference allowing a modification of value
  */
  std::string& Context::operator[](int i)
  {
    if ( (i < 0) || (i >= (int)data.size()) )
      throw("Context::operator[] : outside bounds");

    std::set<std::string>::iterator it = data.begin();
    for(int n=0; n<i; n++)
      it++;
    return const_cast<std::string&>(*it);
  }



  /** \param[in] s event name to be sought for in the context
  *   \result true if the element is indeed in the context
  */
  bool Context::contains(const std::string& s)
  {
    return (data.count(s)==1);
  }


  /** \result true if card=1 anf if contains ANONYMOUS
  */
  bool Context::isSingletonAnonymous() const
  {
    return( (data.size() == 1)
         && (data.count(Context::ANONYMOUS()) == 1) );
  }


  /** Class method.
  *   \param[in] c1 first context
  *   \param[in] c2 second context
  *   \param[in] exceptAnonymous true indicates that \bot should not be taken into account
  *   \return true if the intersection is empty
  */
  bool Context::areEquals(const Context& c1, const Context& c2, 
                          bool exceptAnonymous)
  {
    if (exceptAnonymous)
      return Set::equal(c1.data, c2.data, Context::ANONYMOUS_CODE_NAME);
    else
      return Set::equal(c1.data, c2.data);
  }



  /** Class method.
  *   \param[in] c1 first context
  *   \param[in] c2 second context
  *   \param[in] exceptAnonymous true indicates that \bot should not be taken into account
  *   \return true if the intersection is empty
  */
  bool Context::areDisjoint(const Context& c1, const Context& c2, 
                            bool exceptAnonymous)
  {
    if (exceptAnonymous)
      return Set::areDisjoint(c1.data, c2.data, Context::ANONYMOUS_CODE_NAME);
    else
      return Set::areDisjoint(c1.data, c2.data);
  }


  /** Class method.
  *   \param[in] c1 first context
  *   \param[in] c2 second context
  *   \param[out] result context intersection
  *   \param[in] exceptAnonymous true indicates that \bot should not be taken into account
  */
  void Context::contextIntersection(const Context& c1, const Context& c2, 
                                    Context& result, bool exceptAnonymous)
  {
    if (exceptAnonymous)
      return Set::set_intersection(c1.data, c2.data, result.data, 
                                   Context::ANONYMOUS_CODE_NAME);
    else
      return Set::set_intersection(c1.data, c2.data, result.data);
  }


  /** Class method.
  *   \param[in] c1 first context
  *   \param[in] c2 second context
  *   \param[in] exceptAnonymous true indicates that \bot should not be taken into account
  *   \return context intersection
  */
  Context Context::contextIntersection(const Context& c1, const Context& c2, 
                                       bool exceptAnonymous)
  {
    Context result;
    contextIntersection(c1, c2, result, exceptAnonymous);
    return result;
  }


  /** Class method.
  *   \param[in] c1 first context
  *   \param[in] c2 second context
  *   \param[out] result context union
  *   \param[in] exceptAnonymous true indicates that \bot should not be taken into account
  */
  void Context::contextUnion(const Context& c1, const Context& c2, 
                             Context& result, bool exceptAnonymous)
  {
    if (exceptAnonymous)
      return Set::set_union(c1.data, c2.data, result.data, 
                            Context::ANONYMOUS_CODE_NAME);
    else
      return Set::set_union(c1.data, c2.data, result.data);
  }

  /** Class method.
  *   \param[in] c1 first context
  *   \param[in] c2 second context
  *   \param[in] exceptAnonymous true indicates that \bot should not be taken into account
  *   \return context union
  */
  Context Context::contextUnion(const Context& c1, const Context& c2, 
                                bool exceptAnonymous)
  {
    Context result;
    contextUnion(c1, c2, result, exceptAnonymous);
    return result;
  }

} /* namespace CRL */
