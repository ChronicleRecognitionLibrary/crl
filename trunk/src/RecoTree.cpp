/** ***********************************************************************************
 * \file RecoTreeCouple.cpp
 * \author Ariane Piel & Jean Bourrely / Onera DCPS
 * \date 2012
 * \brief Class of Recognition Nodes ("complex" chronicles)
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

#include "Chronicle.h"
#include "RecoTree.h"
#include "RecognitionEngine.h"


// ----------------------------------------------------------------------------
// CLASS METHODS
// ----------------------------------------------------------------------------

namespace CRL 
{

  //! Accessor, via the chronicle
  RecognitionEngine* RecoTree::getMyEngine() 
  { 
    return _myChronicle->getMyEngine();
  }


  //! Copy of the orders and dates
  void RecoTree::copyDateAndOrder(const Event& e) {
    _minOrder = e.getOrder();
    _maxOrder = e.getOrder();
    _minDate  = e.getDate();
    _maxDate  = e.getDate();
  }

  //! Copy of the orders and dates
  void RecoTree::copyDateAndOrder(const RecoTree& r) {
    _minOrder = r._minOrder;
    _maxOrder = r._maxOrder;
    _minDate  = r._minDate;
    _maxDate  = r._maxDate;
  }

  //! Copy of the orders and dates
  void RecoTree::copyDateAndOrder(const RecoTree& r1, const RecoTree& r2) {
    _minOrder = (r1._minOrder > r2._minOrder ? r2._minOrder : r1._minOrder);
    _maxOrder = (r1._maxOrder > r2._maxOrder ? r1._maxOrder : r2._maxOrder);
    _minDate  = (r1._minDate > r2._minDate ? r2._minDate : r1._minDate);
    _maxDate  = (r1._maxDate > r2._maxDate ? r1._maxDate : r2._maxDate);
  }

  //! Copy of the orders and dates
  void RecoTree::copyDateAndOrder(const RecoTree& r1, const Event& r2) {
    _minOrder = (r1._minOrder > r2.getOrder() ? r2.getOrder() : r1._minOrder);
    _maxOrder = (r1._maxOrder > r2.getOrder() ? r1._maxOrder : r2.getOrder());
    _minDate  = (r1._minDate > r2.getDate() ? r2.getDate() : r1._minDate);
    _maxDate  = (r1._maxDate > r2.getDate() ? r1._maxDate : r2.getDate());
  }

} /* namespace CRL */

