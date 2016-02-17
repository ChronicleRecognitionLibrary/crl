/** ***********************************************************************************
 * \file Event.h
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

#ifndef EVENT_H_
#define EVENT_H_

// ----------------------------------------------------------------------------
// INCLUDE FILES
// ----------------------------------------------------------------------------

#include <set>
#include <string>
#include <iostream>

#include "PropertyManager.h"


/* Note : the user may define his own classes to represent dates et durations
 * To do this, use the option in the CMakelists.txt file and define whatever is
 * wanted (name of classes, include files and optional library).
 */

#ifndef DateType
#define DateType double
#endif

#ifdef DateTypeHeader
#include DateTypeHeader
#endif

#ifndef INFTY_DATE
#define INFTY_DATE 1.0e100
#endif

#ifndef DurationType
#define DurationType double
#endif

#ifdef DurationTypeHeader
#include DurationTypeHeader
#endif

#ifndef NO_DATE
#define NO_DATE -1.0e100
#endif

// ----------------------------------------------------------------------------
// CLASS DESCRIPTION
// ----------------------------------------------------------------------------


namespace CRL {

  class Event : public PropertyManager
  {
  private:

    //! Implicit name (code) of events "date t="
    static std::string _timeEventName;

    //! Event name (code)
    std::string _name;

    //! Event date
    DateType _date;

    //! Order (once in the recognition engine buffer)
    long _order;

    //! Set of instances allocated on the heap (dynamic allocation)
    static std::set<Event*> _dynamicInstances;

  public:

    //! Constructor (actual event of the event flow)
    Event(const std::string& name);

    //! Constructor (actual event of the event flow)
    Event(const std::string& name, const DateType& date);

    //! Constructor (event of type "pure date")
    Event(const DateType& date);

    //! Operator new, stores the instance in the dynamic list of instances
    void* operator new(size_t size);

    //! Operator delete, removes the instance from the list
    void operator delete(void* ptr);

    //! Operator new[] is forbidden
    void* operator new[](size_t size);

    //! Operator delete[] is forbidden
    void operator delete[](void* ptr);

    //! Deletes every dynamic instances of Event class
    static void freeAllInstances();

    //! Accessor
    static const std::string& getTimeEventName() { return _timeEventName; }

    //! Accessor
    static void setTimeEventName(const std::string& s) { _timeEventName = s; }

    //! Accessor
    long getOrder() const { return _order; }

    //! Accessor
    void setOrder(long  order) { _order = order; }

    //! Accessor
    DateType getDate() const { return _date; }

    //! Accessor
    const std::string & getName() const { return _name; }

    //! Accessor
    void setDate(const DateType& date) { _date = date; }

    //! Accessor
    void setName(const std::string & name) { _name = name; }

    //! Display for tests
   friend std::ostream& operator<<(std::ostream& os, const Event& e);

   //! Input reception of an event
   friend std::istream& operator>>(std::istream& is, Event& e);

  }; // class Event

} /* namespace CRL */

#endif /* EVENT_H_ */
