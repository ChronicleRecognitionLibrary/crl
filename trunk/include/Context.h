/** ***********************************************************************************
 * \file Context.h
 * \author Ariane Piel & Jean Bourrely / Onera DCPS
 * \date 2012
 * \brief Context of property names of a chronicle
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

#ifndef CONTEXT_H_
#define CONTEXT_H_

// ----------------------------------------------------------------------------
// INCLUDE FILES
// ----------------------------------------------------------------------------

#include <string>
#include <set>


// ----------------------------------------------------------------------------
// CLASS DESCRIPTION
// ----------------------------------------------------------------------------

namespace CRL {

  class Context 
  {
  private:

    //! Name-symbol meaning "anonymous"
    static std::string ANONYMOUS_CODE_NAME;

    //! Context of the chronicle
    std::set<std::string> data;

  public:

    //! Accessor to the class attribute
    static const std::string& ANONYMOUS() { return ANONYMOUS_CODE_NAME; }

    //! Default constructor
    Context(bool withAnonymous = false);

    //! Copy constructor with or without ANONYMOUS
    Context(const Context& c, bool withAnonymous = true);

    //! Adds an element to the context (exception raised if the element already exists)
    void add(const std::string& s, bool nothrow = false);

    //! Removes and element from the context
    void remove(const std::string& s);

    //! Empties the context, except possibly \bot
    void clear(bool exceptAnonymous = false);

    //! Returns the context size (cardinal)
    int size() const;

    //! Returns (const) the i-th element of the context
    const std::string& operator[](int i) const;

    //! Retourne (non const) le ième élément du contexte
    std::string& operator[](int i);

    //! Returns true if the element belong sto the context
    bool contains(const std::string& s);

    //! Returns true if the context is reduced to singleton {ANONYMOUS}
    bool isSingletonAnonymous() const;

    //! Returns true if the sets are equal
    static bool areEquals(const Context& c1, const Context& c2, 
                          bool exceptAnonymous = false);

    //! Returns true if the sets are disjoint
    static bool areDisjoint(const Context& c1, const Context& c2, 
                            bool exceptAnonymous = false);

    //! Computes the union of the two contexts
    static void contextUnion(const Context& c1, const Context& c2, Context& result, 
                             bool exceptAnonymous = false);

    //! Returns the union of the two contexts
    static Context contextUnion(const Context& c1, const Context& c2, 
                                bool exceptAnonymous = false);

    //! Computes the intersection of the two contexts
    static void contextIntersection(const Context& c1, const Context& c2, Context& result, 
                                    bool exceptAnonymous = false);

    //! Returns the intersection of the two contexts
    static Context contextIntersection(const Context& c1, const Context& c2, 
                                       bool exceptAnonymous = false);

  }; // class Context

} /* namespace CRL */

#endif /* CONTEXT_H_ */
