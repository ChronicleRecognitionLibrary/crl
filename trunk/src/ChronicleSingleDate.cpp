/** ***********************************************************************************
 * \file ChronicleSingleDate.cpp
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

// ----------------------------------------------------------------------------
// INCLUDE FILES
// ----------------------------------------------------------------------------

#include "ChronicleSingleDate.h"
#include "RecoTreeSingle.h"


// ----------------------------------------------------------------------------
// CLASS METHODS
// ----------------------------------------------------------------------------

namespace CRL 
{

  //! Default value of the code representing events of type "date"
  std::string ChronicleSingleDate::_singleDateCode = "tcrl";


  /** \param[in] date date of type DateType
  */  
  ChronicleSingleDate::ChronicleSingleDate(const DateType& date) 
    : _date(date) 
  {
    _evaluationContext.add(Context::ANONYMOUS());
    _resultingContext.add(Context::ANONYMOUS());
  }


  /** Display in the form :  \code 4.0 \endcode
  *   \return string
  */
  std::string ChronicleSingleDate::toString() const 
  { 
    if (_name != "") return _name;
    std::stringstream ss;
    ss << _singleDateCode << _date;
    return ss.str();
  }


  /** Updates the recognition set of the chronicle.
  *   \param[in] d date at which the evaluation is undertaken
  *   \param[in] e event to be evaluated
  */
  bool ChronicleSingleDate::process(const DateType& d, CRL::Event* e) 
  {
    // If the chronicle has already been processed this turn
    if (_alreadyProcessed) 
      return _hasNewRecognitions;

    if ( (d == _date) && _recognitionSet.empty() )
    {
      RecoTree* tmp = new RecoTreeSingle(new Event(_date));
      applyActionFunction(tmp);
      _alreadyProcessed = true;
      return _hasNewRecognitions;
    }
    _alreadyProcessed = true;
    return (_hasNewRecognitions = false);
  }


  //! Returns the date in the future, which concerns this chronicle
  DateType ChronicleSingleDate::lookAhead(const DateType& tcurr) const 
  { 
    if ( (tcurr <= _date) && (_recognitionSet.empty()) )
    return _date; 
    else return INFTY_DATE;
  }

} /* namespace CRL */
