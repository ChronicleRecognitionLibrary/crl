/** ***********************************************************************************
 * \file PropertyManager.cpp
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

// ----------------------------------------------------------------------------
// INCLUDE FILES
// ----------------------------------------------------------------------------

#include "Context.h"
#include "Property.h"
#include "PropertyManager.h"


// ----------------------------------------------------------------------------
// METHODS
// ----------------------------------------------------------------------------

namespace CRL 
{

  /** Copies all the properties in argument, except those of name \bot, 
  *   if the argument \a exceptAnonymous is true.
  *   \param[in] p copied properties
  *   \param[in] exceptAnonymous true if \bot must not be copied
  */
  PropertyManager::PropertyManager(const PropertyManager& p, bool exceptAnonymous)
  {
    copyProperties(p, exceptAnonymous);
  }

  /** \return number of properties in the manager
  */
  int PropertyManager::countProperties() const
  {
    return (int)properties.size();
  }

  /** The insertion method does not do anything if the name provided for argument already exists.
  *   \param[in] s name of the attribute to be inserted
  *   \param[in] p value of the attribute
  */
  void PropertyManager::insertProperty(const std::string& s, Property* p, bool toDelete)
  { 
    properties.insert(std::pair<std::string, PropertyStored > ( s, std::pair<Property*,bool>(p, toDelete) ) );
  }


  /** Copies all the properties of an argument, except those which name is already
  *   present, and except those of name \bot, if the argument \a exceptAnonymous is true.
  *   \param[in] p properties which are added to those of \a this
  *   \param[in] exceptAnonymous true if \bot must not be copied
  */
  void PropertyManager::copyProperties(const PropertyManager& p, bool exceptAnonymous)
  {
    std::map<std::string, PropertyStored>::const_iterator it;
    for (it=p.properties.begin(); it!=p.properties.end(); it++)
      if ( (exceptAnonymous == false) || ((*it).first != Context::ANONYMOUS()) )
        insertProperty( (*it).first, (*it).second.first, false);
  }


  /** This method creates a new property, containing all those of \a p,
  *   except possibly the one named \bot. It inserts this new property
  *   in the list of properties under name \bot.
  *   The method raised an exception if the set of initial properties is empty.
  *   Is also raises an exception if the final insertion is not possible 
  *   (i.e. name \bot is already used).
  *   \param[in] p properties to be processed
  *   \param[in] exceptAnonymous true if \bot must not be copied
  */
  void PropertyManager::shiftProperties(const PropertyManager& p, bool exceptAnonymous)
  {
    if (  (p.countProperties()==0)
        || ( (exceptAnonymous)&&(p.countProperties()==1)&&(p.findProperty(Context::ANONYMOUS())) )  )
      return;

    if (this->findProperty(Context::ANONYMOUS()) != NULL)
      throw("shiftProperties : anonymous property already exists");

    Property* prop = new Property;
    prop->copyProperties(p, exceptAnonymous);
    insertProperty( Context::ANONYMOUS(), prop, true);
  }


  /** \return NULL or pointer to Property
  */
  Property* PropertyManager::findProperty(const std::string& s) const
  {
    std::map<std::string, PropertyStored>::const_iterator it = properties.find(s);
    if (it != properties.end())
      return (it->second.first);
    else
      return NULL;
  }


  /** \return NULL or pointer to Property
  */
  Property* PropertyManager::findProperty(char const * const s) const
  {
    std::map<std::string, PropertyStored>::const_iterator it = properties.find(s);
    if (it != properties.end())
      return (it->second.first);
    else
      return NULL;
  }


  /** Looks in list #properties for a property named \a s.
  *   It is exists, returns it. Otherwise, creates it.
  *   \param[in] s desired property name
  *   \return found or created property
  */
  Property& PropertyManager::operator[](char const * const s)
  {
    std::map<std::string, PropertyStored>::iterator it = properties.find(s);
    if (it != properties.end())
      return *(it->second.first);
    else
    {
      Property* p = new Property();
      insertProperty( s, p, true );
      return *p;
    }
  }


  /** Looks in list #properties for a property named \a s.
  *   It is exists, returns it. Otherwise, creates it.
  *   \param[in] s desired property name
  *   \return found or created property
  */
  Property& PropertyManager::operator[](const std::string& s)
  {
    return this->operator[](s.c_str());
  }


  /** Looks in list #properties for a property named \a s.
  *   It is exists, returns it. Otherwise, raises an exception.
  *   \param[in] s desired property name
  *   \return found property
  */
  const Property& PropertyManager::operator[](char const * const s) const
  {
    std::map<std::string, PropertyStored>::const_iterator it = properties.find(s);
    if (it != properties.end())
      return *(it->second.first);
    else
      throw(std::string("Property ")+s+std::string(" not found"));
  }


  /** Looks in list #properties for a property named \a s.
  *   It is exists, returns it. Otherwise, raises an exception.
  *   \param[in] s desired property name
  *   \return found property
  */
  const Property& PropertyManager::operator[](const std::string& s) const
  {
    return this->operator[](s.c_str());
  }


} /* namespace CRL */

