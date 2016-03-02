/** ***********************************************************************************
 * \file ChronicleSingleEvent.h
 * \author Ariane Piel & Jean Bourrely / Onera DCPS
 * \date 2012
 * \brief Class of single event chronicles (leafs of chronicles tree)
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

#ifndef CHRONICLESINGLEEVENT_H_
#define CHRONICLESINGLEEVENT_H_

// ----------------------------------------------------------------------------
// INCLUDE FILES
// ----------------------------------------------------------------------------

#include <string>

#include "Chronicle.h"

// ----------------------------------------------------------------------------
// CLASS DESCRIPTION
// ----------------------------------------------------------------------------

namespace CRL {

  class ChronicleSingleEvent: public CRL::Chronicle 
  {
  protected:

    //! String which has to correspond to event names
    std::string _code;

  public:

    //! Constructor
    ChronicleSingleEvent(const std::string& code);

    //! Accessor, link sto the recognition engine
    void setMyEngine(RecognitionEngine* e) {
      _myEngine = e;
    }

    //! Display function for unit tests
    std::string toString() const { 
      if (_name != "") 
        return _name; 
      return _code; 
    }

    //! Accessor
    const std::string& getCode() const { return _code; }

    //! Accessor
    void setCode(const std::string& code) { _code = code; }

    //! Main event processing function
    virtual bool process(const DateType& d, CRL::Event* e = NULL);

    //! Returns the date in the future at which the recognitions set of the chronicle must be re-assessed
    DateType lookAhead(const DateType& tcurr) const { return INFTY_DATE; }

    //! Implementation of pure virtual
    Chronicle* getChild1() { return NULL; }

    //! Implementation of pure virtual
    Chronicle* getChild2() { return NULL; }

  protected:

    //! Destructor protected (to prevent stack allocation)
    ~ChronicleSingleEvent() { /* empty */ }

  }; // class ChronicleSingleEvent

} /* namespace CRL */

#endif /* CHRONICLESINGLEEVENT_H_ */
