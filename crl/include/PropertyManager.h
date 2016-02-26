/** ***********************************************************************************
 * \file PropertyManager.h
 * \author Ariane Piel & Jean Bourrely / Onera DCPS
 * \date 2012
 * \brief Object able to manage properties (ex : event)
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

#ifndef PROPERTY_MANAGER_H_
#define PROPERTY_MANAGER_H_

// ----------------------------------------------------------------------------
// INCLUDE FILES
// ----------------------------------------------------------------------------

#include <map>
#include <string>


// ----------------------------------------------------------------------------
// CLASS DESCRIPTION
// ----------------------------------------------------------------------------

namespace CRL {

  class Property;

  class PropertyManager
  {
  private:

    //! Pair of property and boolean indicating if the manager should delete the property
    typedef std::pair<Property*,bool> PropertyStored;

    //! Manages a property list indexed by names
    std::map<std::string, PropertyStored> properties;

  public:

    //! Default constructor
    PropertyManager() { }

    //! Copy constructor (except possibly the anonymous property)
    PropertyManager(const PropertyManager& p, bool exceptAnonymous = false);

    //! Returns the number of properties
    int countProperties() const;

    //! Inserts a pair (name - value)
    void insertProperty(const std::string& s, Property* p, bool toDelete);

    //! Copies the properties, except possibly the anonymous property
    void copyProperties(const PropertyManager& p, bool exceptAnonymous = false);
                                
    //! Adds a property which value is a set of properties, of anonymous name
    void shiftProperties(const PropertyManager& p, bool exceptAnonymous = false);

    //! Looks for a property by its name, returns NULL if it does not exist
    Property* findProperty(char const * const s) const;

    //! Looks for a property by its name, returns NULL if it does not exist
    Property* findProperty(const std::string& s) const;

    //! Returns the property named "s", creates it if necessary
    Property& operator[](char const * const s);

    //! Returns the property named "s", creates it if necessary
    Property& operator[](const std::string& s);

    //! Returns the property named "s", error if it does not exist
    const Property& operator[](char const * const s) const;

    //! Returns the property named "s", error if it does not exist
    const Property& operator[](const std::string& s) const;

  }; // class PropertyManager

} /* namespace CRL */

#endif /* PROPERTY_MANAGER_H_ */
