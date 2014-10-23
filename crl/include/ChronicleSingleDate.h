/** ***********************************************************************************
 * \file ChronicleSingleDate.h
 * \author Ariane Piel & Jean Bourrely / Onera DCPS
 * \date 2012
 * \brief Class of single date chronicles (leafs of chronicles tree)
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

#ifndef CHRONICLE_SINGLE_DATE_H_
#define CHRONICLE_SINGLE_DATE_H_

// ----------------------------------------------------------------------------
// INCLUDE FILES
// ----------------------------------------------------------------------------

#include <string>
#include <sstream>

#include "Chronicle.h"

// ----------------------------------------------------------------------------
// CLASS DESCRIPTION
// ----------------------------------------------------------------------------

namespace CRL {

  class ChronicleSingleDate: public CRL::Chronicle 
  {
  private:

    //! String representing this type of events
    static std::string _singleDateCode;

    //! Expected date
    DateType _date;

  public:

    //! Constructor
    ChronicleSingleDate(const DateType& date);

    //! Accessor, links to the recognition engine
    void setMyEngine(RecognitionEngine* e) {
      _myEngine = e;
    }

    //! Display function for unit tests
    std::string toString() const;

    //! Accessor
    static const std::string& getSingleDateCode() { return _singleDateCode; }

    //! Accessor
    static void setSingleDateCode(const std::string& code) { _singleDateCode = code; }

    //! Accessor
    const DateType& getDate() const { return _date; }

    //! Accessor
    void setDate(const DateType& date) { _date = date; }

    //! Main event processing function
    bool process(const DateType& d, CRL::Event* e = NULL);

    //! Returns the date in the future at which the recognitions set of the chronicle must be re-assessed
    DateType lookAhead(const DateType& tcurr) const;


  }; // class ChronicleSingleDate

} /* namespace CRL */

#endif /* CHRONICLE_SINGLE_DATE_H_ */
