/** ***********************************************************************************
 * \file Event.cpp
 * \author Ariane Piel & Jean Bourrely / Onera DCPS
 * \date 2012
 * \brief class of Events
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

#include "Event.h"
#include <iostream>


// ----------------------------------------------------------------------------
// CLASS DESCRIPTION
// ----------------------------------------------------------------------------

namespace CRL 
{

  //! Initialisation of the class attribute
  std::string Event::_timeEventName = "t";


  //! Initialisation of the list
  std::set<Event*> Event::_dynamicInstances;


  /** Builds an event of name \a name, not dated.
  *   \param[in] name event name
  */
  Event::Event(const std::string& name)
    : _name(name), _date(NO_DATE), _order(-1)
  {
    if (name == _timeEventName)
      throw(std::string("Forbidden name event : ")+name);
  }


  /** Builds an event of name \a name, at t = \a date.
  *   \param[in] name event name
  *   \param[in] date event date
  */
  Event::Event(const std::string& name, const DateType& date)
    :_name(name), _date(date), _order(-1){
      if (name == _timeEventName)
        throw(std::string("Forbidden name event : ")+name);
  }


  /** Builds an un-named event, at t = \a date.
  *   \param[in] date event date
  */
  Event::Event(const DateType& date)
    :_name(_timeEventName), _date(date), _order(-1){
  }


  /** Returns a newly memory allocation, stores the adress
  *   in the list of instances dynamically created.
  */
  void* Event::operator new(size_t size)
  {
    void *ptr = (void *)malloc(size);
    _dynamicInstances.insert((Event*)ptr);
    return ptr;
  }


  /** Free the dynamically allocated memory, removes the
  *   adress from the set of dynamic instances.
  */
  void Event::operator delete(void* ptr)
  {
    _dynamicInstances.erase((Event*)ptr);
    free(ptr);
  }


  /** Deletes all the instances of list _dynamicInstances
  */
  void Event::freeAllInstances()
  {
    Event* tmp;
    std::set<Event*>::iterator it;
    for(it = _dynamicInstances.begin(); it != _dynamicInstances.end(); )
    {
      tmp = (*it);
      it = _dynamicInstances.erase(it);
      delete tmp;
    }
    //_dynamicInstances.clear();
  }


  std::ostream& operator<<(std::ostream& os, const Event& e) 
  {
    if (e.getDate() == NO_DATE)
      os << "(" << e.getName() << ", " << e.getOrder() << ", NODATE)" ;
    else
      os << "(" << e.getName() << ", " << e.getOrder() << ", " << e.getDate() << ")" ;
    return os;
  }





} /* namespace CRL */

